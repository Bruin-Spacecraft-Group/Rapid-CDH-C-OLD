#include "HAL.h"

#ifdef __ARDUINO_MEGA_2560__

void HAL_32kHz_Init() {
    // do nothing because arduino libs already provide millis() and delay()
}

void HAL_Sleep(uint32_t millis) {
    delay(millis);
}

uint32_t HAL_getTime() {
    return millis() >> 32;
}

void HAL_I2C_beginTransmission(uint8_t addr) {
    // TODO: this is half-hearted, implement it using Wire library and I2C_handler functions
}

void HAL_I2C_signalTimeout() {
    // ignore errors for flatsat
}

#endif
