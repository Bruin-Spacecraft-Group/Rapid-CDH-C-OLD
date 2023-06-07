#include "HAL.h"
#include "I2C_handler.h"

#ifdef __ARDUINO_MEGA_2560__
#include <math.h>
#include <Wire.h>

const int MAX_DATA_RESPONSE_SIZE = 256;
void (*dataReceivedCallbacks[500])(uint8_t data[], int dataSize);
int callbacksN = 0;

// pretend this is Arduino/ATMEL code which receives the data
void I2C_receivedData(int bytes)
{
    uint8_t data[MAX_DATA_RESPONSE_SIZE];

    if (bytes > MAX_DATA_RESPONSE_SIZE) {
        // TODO: bad state
    }

    for (int i = 0; i < bytes; i++) {
        data[i] = Wire.read();    // read one character from the I2C
    }
    for (int i = 0; i < callbacksN; i++) {
        dataReceivedCallbacks[i](data, bytes);
    }
}

void HAL_32kHz_Init() {
    // Start the I2C Bus as Master
    Wire.begin(); 
    Wire.onReceive(I2C_receivedData);
}

void HAL_Sleep(uint32_t millis) {
    delay(millis);
}

uint32_t HAL_getTime() {
    return millis() >> 32;
}

void HAL_I2C_sendData(
    enum HAL_Device device,     // device number
    uint8_t data[],             // array of data to be sent
    int dataSize                // the size of data[]
) {
    Wire.beginTransmission(device);
    // sends data over I2C
    for (int i = 0; i < dataSize; i++) {
        Wire.write(data[i]);              // sends x 
    }

    // the following will be implemented in the ISR (interrupt service routine) in actual cubesat
    uint8_t recievedData[3];
    for (int i = 0; i < 3; i++){
        recievedData[i] = Wire.read();
    }

    for (int i = 0; i < callbacksN; i++){ // for each subsystem's callback (not associating data for each subsystem yet)
        dataReceivedCallbacks[i]((uint8_t*) recievedData, 3);
    }

    Wire.endTransmission();    // stop transmitting
}

void HAL_I2C_registerDataReceivedCallback(
    enum HAL_Device device,  // device must send its address in I2C transmission
    void (*dataReceived)(uint8_t data[], int dataSize) // data received callback
) {
    dataReceivedCallbacks[callbacksN++] = dataReceived;
    // table of callback functions per subsystem, need to associate each callback with each subsystem
}

// sets up the pins chosen by the bit mask from the selected port as general purpose outputs
void HAL_GPIO_Init(void* port, uint32_t pins) {
    uint32_t mask = 1;
    int i = 0;
    while(i < 32) {
        int pinNum = pow(2, i) - 1;
        if (mask & pins) {
            pinMode (pinNum, OUTPUT);
        }
        mask << 1;
        i++;
    }
}
// sets the value of all output pins on the selected port according to the bit mask
void HAL_GPIO_AssignPins(void* port, uint32_t pins) {
    uint32_t mask = 1;
    int i = 0;
    while(i < 32) {
        int pinNum = pow(2, i) - 1;
        if (mask & pins) {
            digitalWrite(pinNum, HIGH);
        } else {
            digitalWrite(pinNum, LOW);
        }
        mask << 1;
        i++;
    }
}
#endif
