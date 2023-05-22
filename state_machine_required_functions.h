#ifndef STATE_MACHINE_REQUIRED_FUNCTIONS_H
#define	STATE_MACHINE_REQUIRED_FUNCTIONS_H

#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"

// <editor-fold defaultstate="collapsed" desc="CDH MCU Setup Functions">
// prevent us from getting restarted
void kickWatchdog() {}
// connect to the inter-subsystem i2c bus
void setupI2C() {}
// configure heaters, deployers, subsystem control and interrupt pins, and other
// digital logic
void setupGPIO() {}
// configure ADCs, DACs, and PWM channels related to temperature control
void setupTemperatureSensors() {}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Functions to Fulfill ICDs">
// do the thing we have to do once a second according to EPS ICD
void monitorEPS() {}
// actual period with which we do that thing, in milliseconds
#define EPS_CHECKIN_TIME 450
// do the thing we have to do periodically after xxx seconds according to ADCS ICD
void monitorADCS() {}
// actual period with which we do that thing, in milliseconds
#define ADCS_CHECKIN_TIME 450
// do the stuff we have to do according to thermal's instructions
void adjustTemperatures() {}
// activate the mechanism that structures is using to control the solar array position
void deploySolarArray() {}
// activate the mechanism that structures is using to control the antenna position
void deployAntenna() {}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Functions to Control Subsystems' Power">
// turn subsystems completely on or off to isolate failures or reduce power consumption
void setAdcsOn(bool state) {}
void setCommsOn(bool state) {}
void setPropOn(bool state) {}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Command Queue Interface">
// the data sent from the ground to CDH that represents one doable task, a command,
// to be done at some point in the future
typedef struct {} Command;
// number of commands from the command queue that we have remaining (to do)
uint32_t numRcvdCmds() {}
// gets the oldest command from the command queue, returning true if successful
// and false if failure (due to the queue being empty)
bool getRcvdCmd(Command* cmd) {}
// removes the oldest command from the command queue
void popRcvdCmd() {}
// runs the supplied command, and if successful, deallocates the command, because
// a command that has been run may not be idempotent (although in practice, commands
// should be) and may therefore not still meaningfully exist in a form representable
// by the same bytes as before
bool runCommand(Command* cmd) {}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="ADCS Control Functions">
// true if the some valid attitude (fulfilling the last command) has been reached
// and the spacecraft is not rotating: a moving spacecraft is not stable, and a
// spacecraft yet to reach its target will move and therefore cannot be stable either
bool ADCS_attitudeStable() {}
// true if ADCS has a stable attitude or is executing a recent command and the attitude
// change is being executed nominally; false if a set of commands or an anomaly has
// caused the spacecraft to being tumbling
bool ADCS_hasControl() {}
// true only if the spacecraft is tumbling, cannot be stabilized, and is approaching
// rotational speeds at which it poses a risk of rapid unscheduled disassembly
bool ADCS_isUnrecoverable() {}
// an angular orientation that the spacecraft can be in or be placed in by ADCS
typedef struct {} Orientation;
// sets the target angular velocity for ADCS to be zero without specifying a target
// angular orientation
void ADCS_Detumble() {}
// sets the target for adcs to be "ground station"
void ADCS_setGroundStationPointing() {}
// sets the target for adcs to be "sun"
void ADCS_setSunPointing() {}
// sets the target for adcs to an arbitrary orientation
void ADCS_holdAttitude(Orientation target) {}
// tells adcs to stop doing the control part of adcs and just do the determination
// part of adcs; no angular velocity or orientation target should be used
void ADCS_releaseAttitude() {}
// resets the non-gravitational velocity change measurement to be relative to the
// current velocity
void ADCS_resetDv() {}
// gets the total non-gravitational velocity change since the last reset, in cm/s
float ADCS_getDv() {}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Comms Control Functions">
// Comms transfers data from its SDR to a buffer in CDH via I2C messages in chunks,
// each of which has the following number of bytes
#define COMMS_BLOCK_SIZE 1024
// try to establish communication with the ground, determining or redetermining the
// data rates and integrity available with the current physical spacecraft and
// ground station setup
void COMMS_attemptToConnect() {}
// true if and only if the current communication channel with the ground station has
// a positive bandwidth
bool COMMS_hasConnection() {}
// transfers as much data as possible from comms into the buffer provided (which has
// the size specified: the implementation must not overrun this buffer). return value:
// number of bytes actually transferred (found at the start of the buffer, the invalid
// data is placed at the end)
size_t COMMS_receive(uint8_t* buffer, size_t buffer_size) {}
// transfers as much data as possible from the buffer provided (which has the size specified:
// the implementation must not overrun this buffer) to comms. return value: number
// of bytes actually transferred to comms, from the start of the buffer
size_t COMMS_transmit(uint8_t* buffer, size_t buffer_size) {}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Communication Data Processing Functions">
// processes some bytes received from the ground, which are stored in the buffer supplied
void processReceivedBytes(uint8_t* buffer, size_t buffer_size) {}
// the number of bytes that CDH has queued to send to the ground when it next is able
// to make a connection
size_t numBytesToSend() {}
// takes bytes from the front of the queue of bytes that should be sent to the ground,
// up to the length of the buffer supplied, and store them into the buffer supplied,
// returning the number of bytes actually stored into the buffer
size_t getBytesToSend(uint8_t* buffer, size_t buffer_size) {}
// removes bytes from the queue of bytes that should be sent to the ground
void popBytesToSend(size_t numBytes) {}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="EPS Control Functions">
// true if and only if the spacecraft has low battery (such that it should go into
// charge mode)
bool EPS_lowBattery() {}
// true if and only if the spacecraft is at the highest state of charge it intends
// to obtain during charging, or higher
bool EPS_fullyCharged() {}
// true if and only if something is preventing the spacecraft from charging normally
bool EPS_chargingAnomaly() {}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Prop Control Functions">
// ramp up voltages, pressurize, or whatever prop has to do before shooting ions
void PROP_prepareToThrust() {}
// true if and only if prop can start shooting ions now
bool PROP_canThrust() {}
// shoot ions really fast so we go the other way really slow
void PROP_thrust() {}
// true if something is preventing the ion thruster from continuing to thrust
// (overdischarge of an energy source, heat issues, loss of pressure, etc)
bool PROP_mustStopThrust() {}
// depressurize tanks, discharge stored energy, or whatever must be done after thrusting
// in order to put the thruster into a safe and stable state where it can be turned
// off
void PROP_cleanup() {}
// true if and only if it is safe to turn off the propulsion system now (because
// everything that needs to be is depressurized or sealed or discharged)
bool PROP_cleanedUp() {}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Miscellaneous Functions">
// when the expected amount of radiation in the near future exceeds this threshold,
// we go to radiation safe mode
#define RADIATION_SAFE_MODE_ENTRY_THRESHOLD 1
// when the expected amount of radiation in the near future exceeds this threshold,
// we go to radiation safe mode
#define RADIATION_SAFE_MODE_EXIT_THRESHOLD 1
// gives the normalized expected amount of radiation in the near future
float impendingRadiationLevel() {}
// determines the target orientation given by the last "attitude hold" command from
// the ground station, or some default if no appropriate command exists
Orientation getAttitudeTarget() {}
// determines the amount of time to stay in the target orientation given by the
// last "attitude hold" command from the ground station, or some default if no
// appropriate command exists
uint32_t getAttitudeHoldTime() {}
// returns the orientation such that thrust will maximally contribute to the current
// orbit-changing maneuver desired
Orientation getDvGoalNormal() {}
// returns the amount of delta-V, in meters per second, that the current thruster
// firing is hoping to achieve, based on commands or defaults as is appropriate
float getTargetDv() {}
// </editor-fold>


#endif	/* STATE_MACHINE_REQUIRED_FUNCTIONS_H */

