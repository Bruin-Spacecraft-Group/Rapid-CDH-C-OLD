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
    HAL_GPIO_AssignPins(PORT_B, adcs_data.flatsat_data3 << 12);
    
    // Display the least significant byte of the checksum (aka last transmitted byte) using 8 LEDs
    // TODO: LEDS


    // Expect to receive checksum 0x26 (00100110)


    // Ping Prop (send 0x00 to address 0x0d, exact message: 0x00003d)
    // Confirm received message from Prop (make Prop send exactly 0x00003d [includes checksum] back)
    // Display the least significant byte of the checksum (aka last transmitted byte) using 8 LEDs
    // Expect to receive checksum 0x3d (00111101)


}

void reinitializeCDH() {
    //initialize I2C
    //initialize GPIO
    //initialize timers

    HAL_GPIO_Init(PORT_B, (1 << 12) | (1 << 13) | (1 << 14) |(1 << 15) |(1 << 16) |(1 << 17));

}



#endif
