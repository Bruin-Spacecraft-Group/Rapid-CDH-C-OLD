#ifndef I2C_HANDLER_H
#define I2C_HANDLER_H

#include <stdint.h>

// return status code:
/// 0 indicates to continue transmitting
/// 1 indicates to send Sr and become a receiver
// if status code is 0, next_byte must be set properly
int transmitting_handler(uint8_t& next_byte);

// return status code:
/// 0 indicates to ACK and continue receiving
/// 1 indicates to NACK and close the connection
/// 2 indicates to NACK, close the connection, and record that an error occurred
/// 3 indicates to ACK, close the connection, and record that an error occurred
// last_byte will be the most recent data byte received
int receiving_handler(uint8_t last_byte);

void signal_try_again();

#endif