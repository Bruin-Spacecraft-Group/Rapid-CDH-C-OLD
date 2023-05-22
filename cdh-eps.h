/* 
 * File:   cdh-eps.h
 * Author: userZ
 *
 * Created on May 17, 2023, 7:37 PM
 */

#ifndef CDH_EPS_H
#define	CDH_EPS_H
#include <stdint.h>

uint32_t read(uint8_t reg_addr);
/*
    Returns the data of the virtual register with address reg_addr
*/

void write(uint8_t reg_addr, char[] data, int dataSize);
/*
    Writes data to the virtual register with address reg_addr
    dataSize should be number of bytes(?)
*/

void writeFSWRAP();
/*
    A write to the FSWRAP register, which is the last step to conduct a full scan
*/

uint32_t conductFullScan();
/*
    Conducts a full scan, which requires reading/writing to every virtual register
*/

void signalSafeModeStart();
/*
    Signal to the EPS MCU that safe mode should be entered
*/

void signalSafeModeEnd();
/*
    Signal to the EPS MCU that safe mode can be exited
*/

void notifyFaultDetected();
/*
    Notify to the relevant systems that a fault was detected while reading/writing to registers
*/




#endif	/* CDH_EPS_H */

