#include "config.h"
#if FSM_CHOICE == FSM_FLATSAT

#include "HAL.h"
#include "util.h"
#include "adcs.h"

void runStateEvents() {


    // Ping ADCS (send 0x00 to address 0x0c, exact message: 0x000026)
    ADCS_ping();

    // Confirm received message from ADCS (make ADCS send exactly 0x000026 back [includes checksum])
    AdcsData adcs_data;
    ADCS_getData(&adcs_data);

    // HAL_GPIO_AssignPins(PORT_B, adcs_data.flatsat_data3 << 12);
    
    // Display the least significant byte of the checksum (aka last transmitted byte) using 8 LEDs
    // TODO: LEDS (this is specific for arduino)

    //HAL_GPIO_AssignPins(PORT_K, adcs_data.flatsat_data3);
    HAL_GPIO_AssignPins(PORT_K, 0x10101010);
    HAL_GPIO_AssignPins(PORT_F, 0x10101010);

    // Expect to receive checksum 0x26 (00100110)


    // Ping Prop (send 0x00 to address 0x0d, exact message: 0x00003d)
    // Confirm received message from Prop (make Prop send exactly 0x00003d [includes checksum] back)

    // Display the least significant byte of the checksum (aka last transmitted byte) using 8 LEDs
    //HAL_GPIO_AssignPins(PORT_F, prop data);

    // Expect to receive checksum 0x3d (00111101)

    Serial.println("ran state events");
}

void reinitializeCDH() {
    //initialize I2C
    //initialize GPIO
    //initialize timers
    HAL_32kHz_Init();

    HAL_GPIO_Init(PORT_K, 0xFF);
    HAL_GPIO_Init(PORT_F, 0xFF);
    Serial.println("CDH initialized");
}



#endif
