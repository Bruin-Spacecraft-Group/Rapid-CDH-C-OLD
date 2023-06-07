#ifndef I2C_HANDLER_H
#define I2C_HANDLER_H

#include <stdint.h>
#include "HAL.h"

enum HAL_Device {
    adcs = 0x0c,
    prop = 0x0d,
    eps = 0x0b // as specified in EPS ICD
};

// CDH -> external device
void HAL_I2C_sendData(
    enum HAL_Device device,  // device number
    uint8_t data[],        // array of data to be sent
    int dataSize        // the size of data[]
);

// external device -> CDH
void HAL_I2C_registerDataReceivedCallback(
    enum HAL_Device device,  // device must send its address in I2C transmission
    void (*dataReceived)(uint8_t data[], int dataSize) // data recieved callback
);

// return status code:
/// 0 indicates to continue transmitting
/// 1 indicates to send Sr and become a receiver
// if status code is 0, next_byte must be set properly
int transmitting_handler(uint8_t* next_byte);

// return status code:
/// 0 indicates to ACK and continue receiving
/// 1 indicates to NACK and close the connection
/// 2 indicates to NACK, close the connection, and record that an error occurred
/// 3 indicates to ACK, close the connection, and record that an error occurred
// last_byte will be the most recent data byte received
int receiving_handler(uint8_t last_byte);

void signal_try_again();

#endif