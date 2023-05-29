#include "adcs.h"

#include "HAL.h"
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
        case CHARGE_MODE:
            // send code 0x03
            uint8_t data[1] = {0x03};
            HAL_I2C_sendData(adcs, data, 1);
            break;
        case DOWNLINK_MODE:
            // send code 0x04
            uint8_t data[1] = {0x04};
            HAL_I2C_sendData(adcs, data, 1);
            break;
        case RADIATION_SAFE_MODE:
            // send code 0x05
            uint8_t data[1] = {0x05};
            HAL_I2C_sendData(adcs, data, 1);
            break;
        case POWER_SAFE_MODE:
            // send code 0x06
            uint8_t data[1] = {0x06};
            HAL_I2C_sendData(adcs, data, 1);
            break;
        case END_OF_LIFE_MODE:
            // send code 0x07
            uint8_t data[1] = {0x07};
            HAL_I2C_sendData(adcs, data, 1);
            break;
        case STARTUP_MODE:
            // send code 0x08
            uint8_t data[1] = {0x08};
            HAL_I2C_sendData(adcs, data, 1);
            break;
        case DETUMBLE_MODE:
            // send code 0x09
            uint8_t data[1] = {0x09};
            HAL_I2C_sendData(adcs, data, 1);
            break;
    }

}

void ADCS_setAttitude(uint16_t w, uint16_t x, uint16_t y, uint16_t z){
    // send code 0x0A
    uint8_t data[9] = {0x0A, (uint8_t) (w>>8), (uint8_t) w, (uint8_t) (x>>8), (uint8_t) x, (uint8_t) (y>>8), (uint8_t) y, (uint8_t) (z>>8), (uint8_t) z};
    HAL_I2C_sendData(adcs, data, 9);
    // also sets to ATTITUDE_HOLD mode
}

void ADCS_getData(AdcsData* data){
    // writes the data into data and returns status code
    // TODO: format
}