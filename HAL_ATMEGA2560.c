#include "HAL.h"

#ifdef __ARDUINO_MEGA_2560__

void HAL_I2C_sendData(
    enum HAL_Device device,  // device number
    uint8_t data[],        // array of data to be sent
    int dataSize        // the size of data[]
) 
{
    //TODO
    //actual i2c things
    //implement isr (the callback) as tail call to isr
    //i2c shenanigans
}

#endif
