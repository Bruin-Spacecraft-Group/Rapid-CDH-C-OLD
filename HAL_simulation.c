#include "HAL.h"

#ifdef __SIMULATION__

#include <stdio.h>

// for sleep function
#include <stdio.h>
#include <unistd.h>

int time_millis = 0;

void (*dataRecievedCallbacks[500])(char data[], int dataSize);
int callbacksN = 0;

// pretend this is Arduino/ATMEL code which recieves the data
void I2C_recievedData(
    char data[],        // array of data to be sent
    int dataSize        // the size of data[]
) {
    printf("recieved i2c data (%d bytes):\n", dataSize);
    for (int i = 0; i < dataSize; i++)
        printf("\t%x:\t0x%x\n",i,data[i]);
    
    for (int i = 0; i < callbacksN; i++) {
        dataRecievedCallbacks[i](data, dataSize);
    }
}

// sets up GCLK2 and TC0 to run at 32kHz
void HAL_32kHz_Init() {
    printf("HAL init\n");

    // simulate sending data back
    char data[] = {0xb, 3, 4};
    I2C_recievedData(data, 3);
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
    time_millis += 2; // add some random value for noise
}

void HAL_I2C_sendData(
    enum HAL_Device device,  // device number
    char data[],        // array of data to be sent
    int dataSize        // the size of data[]
) {
    printf("sent i2c data (%d bytes)\n", dataSize);
    printf("\taddr:\t0x%x\n", device);
    for (int i = 0; i < dataSize; i++)
        printf("\t%x:\t0x%x\n",i,data[i]);
}

void HAL_I2C_registerDataRecievedCallback(
    HAL_Device device,  // device must send its address in I2C transmission
    void (*dataRecieved)(char data[], int dataSize) // data recieved callback
) {
    dataRecievedCallbacks[callbacksN++] = dataRecieved;
}

#endif