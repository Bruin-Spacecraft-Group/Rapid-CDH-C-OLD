// hey you
// include all the project C files in order
// yes all the C files
// yes they have to be in a topological-sort-by-dependency order
// it's cuz Arduino IDE is bad
// shut up
// i said shut up

#include "util.c"
#include "HAL_ATMEGA2560.c"
#include "HAL_SAMD51J19A.c"
#include "HAL_SAMV71Q21B.c"
#include "HAL_simulation.c"
#include "checksum.c"
#include "I2C_handler.c"
#include "ADCS.c"
#include "FlightStateMachine.c"
#include "FSM_flatSat.c"
#include "main.c"
