#ifndef I2C_HANDLER_H
#define I2C_HANDLER_H

#include <stdint.h>

uint8_t transmitting_handler(bool& continue_transmitting);
bool receiving_handler(uint8_t next_byte);
void signal_try_again();

#endif