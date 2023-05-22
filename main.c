#include "HAL.h"
#include "FlightStateMachine.h"

int main(void) {
    reinitializeCDH();
    while (1) {
        runStateEvents();
    }
}
