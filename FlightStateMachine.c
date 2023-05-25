#include "FlightStateMachine.h"
#include "state_machine_required_functions.h"

#include "HAL.h"
#include "util.h"

/* PERSISTENT */ FLIGHT_STATE currentFlightState = LAUNCH_MODE;
/* PERSISTENT */ uint32_t deploymentTime;
uint8_t commsBuffer[COMMS_BLOCK_SIZE];
Command* nextAction = NULL;

void doPeriodicOps() {
    static uint32_t epsCheckTime = 1 << 31;
    static uint32_t adcsCheckTime = 1 << 31;
    adjustTemperatures();
    kickWatchdog();
    if ((HAL_getTime() - epsCheckTime) < (EPS_CHECKIN_TIME << 5)) {
        monitorEPS();
        epsCheckTime = HAL_getTime();
    }
    if ((HAL_getTime() - adcsCheckTime) < (ADCS_CHECKIN_TIME << 5)) {
        monitorADCS();
        adcsCheckTime = HAL_getTime();
    }
}

void doListen() {
    COMMS_attemptToConnect();
    if (COMMS_hasConnection()) {
        processReceivedBytes(commsBuffer, COMMS_receive(commsBuffer, COMMS_BLOCK_SIZE));
    }
}

void runStateEvents() {
    static uint32_t adcsTargetTime = -1;
    size_t readyBytes;
    switch (currentFlightState) {
        case LAUNCH_MODE:
            HAL_32kHz_Init();
            deploymentTime = HAL_getTime();
            setupI2C();
            setupGPIO();
            setupTemperatureSensors();
            currentFlightState = STARTUP_MODE;
            break;
        case STARTUP_MODE:
            monitorEPS();
            adjustTemperatures();
            kickWatchdog();
            HAL_Sleep(EPS_CHECKIN_TIME);
            if (((HAL_getTime() - deploymentTime) >> 5) > 45 * 60) {
                setAdcsOn(1);
                currentFlightState = DETUMBLE_MODE;
            }
            break;
        case DETUMBLE_MODE:
            if (ADCS_isUnrecoverable()) {
                currentFlightState = END_OF_LIFE_MODE;
            } else if (ADCS_attitudeStable()) {
                deploySolarArray();
                deployAntenna();
                setCommsOn(1);
                currentFlightState = STANDBY_MODE;
            } else {
                ADCS_Detumble();
            }
            doPeriodicOps();
            break;
        case STANDBY_MODE:
            ADCS_setGroundStationPointing();
            doListen();
            if (nextAction || numRcvdCmds()) {
                if (nextAction == NULL) {
                    if (!getRcvdCmd(nextAction)) {
#if defined(__DEBUG) && __DEBUG
                        // somehow report the error to the programmer
                        while (1) {
                            SIDE_EFFECT_FOR_DEBUGGING++;
                        }
#else
                        // somehow, our queue with non-zero length was empty, skip
                        // running of the command
                        break;
#endif
                    }
                }
                popRcvdCmd();
                if (runCommand(nextAction)) {
                    nextAction = NULL;
                }
            } else if (!ADCS_hasControl()) {
                currentFlightState = DETUMBLE_MODE;
            } else if (EPS_lowBattery()) {
                currentFlightState = CHARGE_MODE;
            } else if (impendingRadiationLevel() > RADIATION_SAFE_MODE_ENTRY_THRESHOLD) {
                setCommsOn(0);
                currentFlightState = RADIATION_SAFE_MODE;
            } else if (numBytesToSend() && COMMS_hasConnection()) {
                currentFlightState = DOWNLINK_MODE;
            }
            doPeriodicOps();
            break;
        case DOWNLINK_MODE:
            COMMS_attemptToConnect();
            if (COMMS_hasConnection()) {
                processReceivedBytes(commsBuffer, COMMS_receive(commsBuffer, COMMS_BLOCK_SIZE));
                readyBytes = getBytesToSend(commsBuffer, COMMS_BLOCK_SIZE);
                if (readyBytes == COMMS_transmit(commsBuffer, readyBytes)) {
                    popBytesToSend(readyBytes);
                }
                if (!numBytesToSend()) {
                    currentFlightState = STANDBY_MODE;
                }
            } else {
                currentFlightState = STANDBY_MODE;
            }
            doPeriodicOps();
            break;
        case ATTITUDE_HOLD_MODE:
            doListen();
            doPeriodicOps();
            ADCS_holdAttitude(getAttitudeTarget());
            if (adcsTargetTime == -1) adcsTargetTime = HAL_getTime();
            if (((HAL_getTime() - adcsTargetTime) << 5) > getAttitudeHoldTime()) {
                adcsTargetTime = -1;
                currentFlightState = STANDBY_MODE;
            } else if (!ADCS_hasControl()) {
                currentFlightState = DETUMBLE_MODE;
            }
            break;
        case CHARGE_MODE:
            doPeriodicOps();
            ADCS_setSunPointing();
            if (EPS_lowBattery() && EPS_chargingAnomaly()) {
                currentFlightState = POWER_SAFE_MODE;
            } else if (EPS_fullyCharged()) {
                currentFlightState = STANDBY_MODE;
            }
        case RADIATION_SAFE_MODE:
            doPeriodicOps();
            ADCS_releaseAttitude();
            HAL_Sleep(min(EPS_CHECKIN_TIME, ADCS_CHECKIN_TIME));
            if (impendingRadiationLevel() > RADIATION_SAFE_MODE_ENTRY_THRESHOLD) {
                setAdcsOn(1);
                currentFlightState = DETUMBLE_MODE;
            }
            break;
        case POWER_SAFE_MODE:
            monitorEPS();
            adjustTemperatures();
            kickWatchdog();
            HAL_Sleep(450);
            if (!EPS_chargingAnomaly()) {
                currentFlightState = CHARGE_MODE;
            }
            break;
        case PRE_THRUST_MODE:
            doListen();
            doPeriodicOps();
            ADCS_holdAttitude(getDvGoalNormal());
            if (!ADCS_hasControl() || EPS_lowBattery() || (impendingRadiationLevel() > RADIATION_SAFE_MODE_ENTRY_THRESHOLD)) {
                ADCS_resetDv();
                currentFlightState = POST_THRUST_MODE;
            }
            PROP_prepareToThrust();
            if (PROP_canThrust()) {
                currentFlightState = THRUST_MODE;
            }
        case THRUST_MODE:
            doListen();
            doPeriodicOps();
            ADCS_holdAttitude(getDvGoalNormal());
            if (!ADCS_hasControl() || EPS_lowBattery() || (impendingRadiationLevel() > RADIATION_SAFE_MODE_ENTRY_THRESHOLD)) {
                currentFlightState = POST_THRUST_MODE;
            }
            PROP_thrust();
            if (PROP_mustStopThrust() || (ADCS_getDv() > getTargetDv())) {
                currentFlightState = POST_THRUST_MODE;
            }
        case POST_THRUST_MODE:
            doListen();
            doPeriodicOps();
            PROP_cleanup();
            if (PROP_cleanedUp()) {
                setPropOn(0);
                currentFlightState = STANDBY_MODE;
            }
        case END_OF_LIFE_MODE:
            monitorEPS();
            kickWatchdog();
            HAL_Sleep(450);
        // add other cases
        default:
#if defined(__DEBUG) && __DEBUG
            // somehow report the error to the programmer
            while (1) {
                SIDE_EFFECT_FOR_DEBUGGING++;
            }
#else
            // somehow, our state has been corrupted: return to normal mode
            currentFlightState = STANDBY_MODE;
#endif
            break;
    }
}

void dataRecieved(char data[], int dataSize) {
    // do stuff with the data
}

void performI2cDemo() {
    // send data via I2C
    char data[] = {1, 2};
    HAL_I2C_sendData(eps, data, 2);

    // recieve data via I2C
    HAL_I2C_registerDataRecievedCallback(eps, dataRecieved);
}

void reinitializeCDH() {
    performI2cDemo();   // NOTE: this is for demonstration for the i2c
    switch (currentFlightState) {
        case LAUNCH_MODE:
            // no initialization to be re-completed, as this is the first mode for
            // the CDH FC on power-up
            break;
        case POWER_SAFE_MODE:
            HAL_32kHz_Init();
            setupI2C();
            setupGPIO();
            setAdcsOn(0);
            setPropOn(0);
            break;
        case RADIATION_SAFE_MODE:
            HAL_32kHz_Init();
            setupI2C();
            setupGPIO();
            setAdcsOn(0);
            setCommsOn(0);
            setPropOn(0);
            break;
        case STARTUP_MODE:
            HAL_32kHz_Init();
            setupI2C();
            setupGPIO();
            break;
        case DETUMBLE_MODE:
            HAL_32kHz_Init();
            setupI2C();
            setupGPIO();
            setAdcsOn(1);
            break;
        case STANDBY_MODE:
        case DOWNLINK_MODE:
        case ATTITUDE_HOLD_MODE:
        case CHARGE_MODE:
            HAL_32kHz_Init();
            setupI2C();
            setupGPIO();
            setAdcsOn(1);
            setCommsOn(1);
            break;
        case PRE_THRUST_MODE:
        case THRUST_MODE:
        case POST_THRUST_MODE:
            HAL_32kHz_Init();
            setupI2C();
            setupGPIO();
            setAdcsOn(1);
            setCommsOn(1);
            setPropOn(1);
            break;
        case END_OF_LIFE_MODE:
            HAL_32kHz_Init();
            setupI2C();
            setupGPIO();
            setAdcsOn(0);
            setCommsOn(0);
            setPropOn(0);
        // add other cases
        default:
#if defined(__DEBUG) && __DEBUG
            // somehow report the error to the programmer
            while (1) {
                SIDE_EFFECT_FOR_DEBUGGING++;
            }
#else
            // somehow, our state has been corrupted: return to normal mode
            currentFlightState = STANDBY_MODE;
#endif
            break;
    }
}
