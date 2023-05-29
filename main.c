#include "HAL.h"
#include "FlightStateMachine.h"

int main(void) {
    while (1) {
    // Ping ADCS (send 0x00 to address 0x0c, exact message: 0x000026)
    // Confirm received message from ADCS (make ADCS send exactly 0x000026 back [includes checksum])
    // Display the least significant byte of the checksum (aka last transmitted byte) using 8 LEDs
    // Expect to receive checksum 0x26 (00100110)
    // Ping Prop (send 0x00 to address 0x0d, exact message: 0x00003d)
    // Confirm received message from Prop (make Prop send exactly 0x00003d [includes checksum] back)
    // Display the least significant byte of the checksum (aka last transmitted byte) using 8 LEDs
    // Expect to receive checksum 0x3d (00111101)

    }
}
