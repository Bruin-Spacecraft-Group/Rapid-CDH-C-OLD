/* 
 * File:   FlightStateMachine.h
 * Author: Randall
 *
 * Created on March 20, 2023, 11:02 AM
 */

#ifndef FLIGHTSTATEMACHINE_H
#define	FLIGHTSTATEMACHINE_H

#include "stdint.h"

typedef enum {
    INVALID_MODE,
    LAUNCH_MODE,
    STARTUP_MODE,
    DETUMBLE_MODE,
    STANDBY_MODE,
    END_OF_LIFE_MODE,
    POWER_SAFE_MODE,
    RADIATION_SAFE_MODE,
    CHARGE_MODE,
    DOWNLINK_MODE,
    ATTITUDE_HOLD_MODE,
    THRUST_MODE,
    PRE_THRUST_MODE,
    POST_THRUST_MODE
} FLIGHT_STATE;

extern FLIGHT_STATE currentFlightState;

/**
 * Runs events associated specifically with the current state of the system, then
 * updates the state machine to have the appropriate next state.
 */
void runStateEvents();

/**
 * Runs the appropriate functions to set up the flight computer from fully-stopped
 * so that it can operate in the current state as though it has never stopped since
 * the spacecraft was launched.
 */
void reinitializeCDH();

#endif	/* FLIGHTSTATEMACHINE_H */
