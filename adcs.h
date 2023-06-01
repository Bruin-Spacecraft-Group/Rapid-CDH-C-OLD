/* 
 * File:   adcs.h
 * Author: Annie Xiang
 *
 * Created on May 12, 2023, 5:02 PM
 */

#ifndef ADCS_H
#define	ADCS_H

#include "stdint.h"

// incomplete struct of data that would be sent to comms subsystem
// typedef struct {
//     uint8_t mode;
//     uint8_t attitude_x;
//     uint8_t attitude_y;
//     uint8_t attitude_z;
// } AdcsData;

typedef struct {
   uint8_t flatsat_data1;
   uint8_t flatsat_data2;
   uint8_t flatsat_data3;
} AdcsData;

typedef enum {
   ADCS_PING = 0x00,
   ADCS_GET_DATA_REGULAR = 0x01,
   ADCS_GET_DATA_DETAILED = 0x02,
   ADCS_CHARGE_MODE = 0x03,
   ADCS_DOWNLINK_MODE = 0x04,
   ADCS_RADIATION_SAFE_MODE = 0x05,
   ADCS_POWER_SAFE_MODE = 0x06,
   ADCS_END_OF_LIFE_MODE = 0x07,
   ADCS_STARTUP_MODE = 0x08,
   ADCS_DETUMBLE_MODE = 0x09,
   ADCS_SET_ATTITUDE = 0x0A
} COMMAND_CODE;

/**
 * Sends a message to ADCS.
 */
void ADCS_ping();

/**
 * Asks ADCS to send data.
 */
void ADCS_getDataDegular();

/**
 * Asks ADCS to send more detailed data.
 */
void ADCS_getDataDetailed();

/**
 * Tells ADCS to set the mode.
    *Charge mode (0x03)
    *Downlink mode (0x04)
    *Radiation safe mode (0x05)
    *Power safe mode (0x06)
    *End of life mode (0x07)
    *Startup mode (0x08)
    *Detumble mode (0x09)
 */
void ADCS_setMode(COMMAND_CODE mode);

/**
 * Tells ADCS to set the attitude according to the coordinates passed in. Also sets the mode to ATTITUDE_HOLD mode.
 */
void ADCS_setAttitude(uint16_t w, uint16_t x, uint16_t y, uint16_t z);

/**
 * Puts the data from ADCS into an AdcsData struct.
 */
void ADCS_getData(AdcsData* data);

#endif	/* ADCS_H */