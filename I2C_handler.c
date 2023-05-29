#include <stdint.h>
#include "I2C_handler.h"
#include "HAL.h"

void HAL_I2C_sendData(
    enum HAL_Device device,  // device number
    uint8_t data[],        // array of data to be sent
    int dataSize        // the size of data[]
) {
    // call actual i2c stuff
}

void HAL_I2C_registerDataRecievedCallback(
    enum HAL_Device device,  // device must send its address in I2C transmission
    void (*dataRecieved)(char data[], int dataSize) // data recieved callback
) {
    // do stuff yes idk what is going on
    // populates cache with data received through I2C
}

int transmitting_handler(uint8_t* next_byte) {
    return 0;
}

int receiving_handler(uint8_t last_byte){
    return 0;
}

void signal_try_again(){

}