#include "HAL.h"
#include "I2C_handler.h"

#ifdef __SIMULATION__

// print
#include <stdio.h>
// sleep
#include <unistd.h>

int time_millis = 0;

// port names
void* const PORT_A = (void*)0;
void* const PORT_B = (void*)1;

void (*dataReceivedCallbacks[500])(uint8_t data[], int dataSize);
int callbacksN = 0;

// pretend this is Arduino/ATMEL code which receives the data
void I2C_receivedData(
    uint8_t data[],        // array of data to be sent
    int dataSize        // the size of data[]
) {
    printf("received i2c data (%d bytes):\n", dataSize);
    for (int i = 0; i < dataSize; i++)
        printf("\t%x:\t0x%x\n",i,data[i]);
    
    for (int i = 0; i < callbacksN; i++) {
        dataReceivedCallbacks[i](data, dataSize);
    }
}

// sets up GCLK2 and TC0 to run at 32kHz
void HAL_32kHz_Init() {
    printf("HAL init\n");

    // simulate sending data back
    /*
    uint8_t data[] = {0xb, 3, 4};
    I2C_receivedData(data, 3);
    */
}
// turns off CPU and peripherals; keeps RAM, stack, and GP registers. interrupts and
// returns using TC0 after the time expires - requires 32kHz to be initialized
void HAL_Sleep(uint32_t millis) {
    //printf("sleeping for %dms\n", millis);
    float seconds = (float)millis / 1000.0;
    sleep(seconds);
    time_millis += millis;
}
// gives the elapsed time according to the ultra low-power 32kHz clock, in ticks
// overflows each 1.5 days: this function is appropriate only for measuring small
// elapsed durations and requires 32kHz to be initialized
uint32_t HAL_getTime() {
    return time_millis;
}

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

void HAL_I2C_registerDataReceivedCallback(
    enum HAL_Device device,  // device must send its address in I2C transmission
    void (*dataReceived)(uint8_t data[], int dataSize) // data received callback
) {
    dataReceivedCallbacks[callbacksN++] = dataReceived;
}

// sets up the pins chosen by the bit mask from the selected port as general purpose outputs
void HAL_GPIO_Init(void* port, uint32_t pins) {
    printf("initializing port %d pins %x\n", port, pins);
}

// sets the value of all output pins on the selected port according to the bit mask
void HAL_GPIO_AssignPins(void* port, uint32_t pins) {
    printf("assigning port %d pins %x\n", port, pins);
}

#endif