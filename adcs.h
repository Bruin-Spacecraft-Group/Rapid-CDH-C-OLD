/* 
 * File:   adcs.h
 * Author: Annie Xiang
 *
 * Created on May 12, 2023, 5:02 PM
 */

#ifndef ADCS_H
#define	ADCS_H

#include "stdint.h"
#include "FlightStateMachine.h"

// incomplete struct of data that would be sent to comms subsystem
typedef struct {
    uint8_t mode;
    uint8_t attitude_x;
    uint8_t attitude_y;
    uint8_t attitude_z;
} AdcsData;

typedef enum { //not sure if we need this
    0x00,
    0x01,
    0x02,
    0x03,
    0x04,
    0x05,
    0x06,
    0x07,
    0x08,
    0x09,
    0x0A
} COMMAND_CODE;

/**
 * Sends a message to ADCS.
    * @return 0 if unsuccessful, 1 if successful
 */
uint32_t ping();

/**
 * Asks ADCS to send data.
    * @return 0 if unsuccessful, 1 if successful
 */
uint32_t get_data_regular();

/**
 * Asks ADCS to send more detailed data.
    * @return 0 if unsuccessful, 1 if successful
 */
uint32_t get_data_detailed();

/**
 * Tells ADCS to set the mode.
    * @return 0 if unsuccessful, 1 if successful
    *Charge mode (0x03)
    *Downlink mode (0x04)
    *Radiation safe mode (0x05)
    *Power safe mode (0x06)
    *End of life mode (0x07)
    *Startup mode (0x08)
    *Detumble mode (0x09)
 */
uint32_t set_mode(FLIGHT_STATE mode);

uint32_t set_attitude(uint32_t x, uint32_t y, uint32_t z);

AdcsData* data_for_comms();

#endif	/* ADCS_H */