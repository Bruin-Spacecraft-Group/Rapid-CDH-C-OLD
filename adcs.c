
#include "adcs.h"

#include "HAL.h"
#include "I2C_handler.h"
#include "util.h"


void ADCS_ping(){
    // send code 0x00
    uint8_t data[1] = {0x00};
    HAL_I2C_sendData(adcs, data, 1);
}

void ADCS_getDataRegular(){
    // send code 0x01
    uint8_t data[1] = {0x01};
    HAL_I2C_sendData(adcs, data, 1);
}

void ADCS_getDataDetailed(){
    // send code 0x02
    uint8_t data[1] = {0x02};
    HAL_I2C_sendData(adcs, data, 1);
}

void ADCS_setMode(COMMAND_CODE mode){

    switch(mode){
        case ADCS_CHARGE_MODE:
        {
            // send code 0x03
            uint8_t data[1] = {0x03};
            HAL_I2C_sendData(adcs, data, 1);
            break;
        }
        case ADCS_DOWNLINK_MODE:
        {
            // send code 0x04
            uint8_t data[1] = {0x04};
            HAL_I2C_sendData(adcs, data, 1);
            break;
        }
        case ADCS_RADIATION_SAFE_MODE:
        {
            // send code 0x05
            uint8_t data[1] = {0x05};
            HAL_I2C_sendData(adcs, data, 1);
            break;
        }
        case ADCS_POWER_SAFE_MODE:
        {
            // send code 0x06
            uint8_t data[1] = {0x06};
            HAL_I2C_sendData(adcs, data, 1);
            break;
        }
        case ADCS_END_OF_LIFE_MODE:
        {
            // send code 0x07
            uint8_t data[1] = {0x07};
            HAL_I2C_sendData(adcs, data, 1);
            break;
        }
        case ADCS_STARTUP_MODE:
        {
            // send code 0x08
            uint8_t data[1] = {0x08};
            HAL_I2C_sendData(adcs, data, 1);
            break;
        }
        case ADCS_DETUMBLE_MODE:
        {
            // send code 0x09
            uint8_t data[1] = {0x09};
            HAL_I2C_sendData(adcs, data, 1);
            break;
        }
    }

}

uint8_t buffer[256];
void ADCS_dataReceivedCallback(uint8_t data[], int dataSize) {
    for (int i = 0; i < dataSize; i++) {
        buffer[i] = data[i];
    }
}

void ADCS_init() {
    HAL_I2C_registerDataReceivedCallback(adcs, ADCS_dataReceivedCallback);
}

void ADCS_setAttitude(uint16_t w, uint16_t x, uint16_t y, uint16_t z){
    // send code 0x0A
    uint8_t data[9] = {0x0A, (uint8_t) (w>>8), (uint8_t) w, (uint8_t) (x>>8), (uint8_t) x, (uint8_t) (y>>8), (uint8_t) y, (uint8_t) (z>>8), (uint8_t) z};
    HAL_I2C_sendData(adcs, data, 9);
    // also sets to ATTITUDE_HOLD mode
}

void ADCS_getData(AdcsData* data){
    data = (AdcsData*) buffer;
    // refer to a cache of the more recent ADCS data
    // for flatsat, just most recent transmitted byte
    // TODO: format

    //I2C_transact

}
