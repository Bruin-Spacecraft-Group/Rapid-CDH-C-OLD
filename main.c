#include "HAL.h"
#include "FlightStateMachine.h"

void setup() {
    //Serial.begin(115200);
    //DDRK = 0xff;
    //HAL_GPIO_Init(PORT_K, 0xff);

    Serial.begin(115200);
    /*
    Serial.println("BEGIN ADDRESSES");
    Serial.println(String((int) &DDRK, HEX));
    Serial.println(String((int) &DDRF, HEX));
    Serial.println(String((int) &PORTK, HEX));
    Serial.println(String((int) &PORTF, HEX));
    Serial.println("END ADDRESSES");
    */
    reinitializeCDH();
    runStateEvents();
}

void loop() {
    //PORTK = 0xa5;
    //HAL_GPIO_AssignPins(PORT_K, 0xa5);

    delay(100);
    //Serial.println("loop");
    //runStateEvents();
}

// int main(void) {
//     Serial.begin(115200);
//     Serial.println("BEGIN ADDRESSES");
//     //Serial.println(String((int) &DDRK, HEX));
//     //Serial.println(String((int) &DDRF, HEX));
//     //Serial.println(String((int) &PORTK, HEX));
//     //Serial.println(String((int) &PORTF, HEX));
//     Serial.println("END ADDRESSES");
//     reinitializeCDH();
//     while (1) {
//         delay(100);
//         runStateEvents();
//     }
// }
