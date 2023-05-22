#include <stdio.h>   
#include "cdh-eps.h"
#include "HAL.h"
#include <stdint.h>

uint32_t read(uint8_t reg_addr)
{
    HAL_I2C_sendData(eps, reg_addr, 1)
    //CDH-EPS ICD says "The full set of response bytes is the data in the virtual register." 
    //since we are queueing the data we receive, need to somehow know which register it's from, so the above might have to be changed
    return 0;
}

void write(uint8_t reg_addr, char[] data, int dataSize)
{
    char* to_send = malloc(dataSize+1)
    memcpy(to_send, reg_addr, 1);
    memcpy(to_send+1, data, dataSize)
    HAL_I2C_sendData(eps, to_send, dataSize+1);
}

void writeFSWRAP()
{
    char[] data = [0xfe, 1]
    HAL_I2c_sendData(eps, data, 2)
}

uint32_t conductFullScan()
{
    //write/read to all registers
    writeFSWRAP();
    return 0;
}

void signalSafeModeStart()
{
    return;
}

void signalSafeModeEnd();
{
    return;
}

void notifyFaultDetected()
{
    return;
}