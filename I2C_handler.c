#include <stdint.h>
#include "I2C_handler.h"
#include "HAL.h"

/*
int const MAX_DATA_RECEIVED_CALLBACKS = 500;
void (*dataReceivedCallbacks[MAX_DATA_RECEIVED_CALLBACKS])(char data[], int dataSize);
int callbacksN = 0;

void HAL_I2C_sendData(
    enum HAL_Device device,  // device number
    uint8_t data[],        // array of data to be sent
    int dataSize        // the size of data[]
) {
    printf("sent i2c data (%d bytes)\n", dataSize);
    printf("\taddr:\t0x%x\n", device);
    for (int i = 0; i < dataSize; i++)
        printf("\t%x:\t0x%x\n",i,data[i]);
}

void HAL_I2C_registerDataRecievedCallback(
    enum HAL_Device device,  // device must send its address in I2C transmission
    void (*dataReceived)(char data[], int dataSize) // data recieved callback
) {
    dataReceivedCallbacks[callbacksN++] = dataReceived;
}

int transmitting_handler(uint8_t* next_byte) {
    return 0;
}

int receiving_handler(uint8_t last_byte){
    return 0;
}

void signal_try_again(){

}*/