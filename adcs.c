#include "adcs.h"

#include "HAL.h"
#include "util.h"

uint32_t ping(){
    // send code 0x00
    return 0;
}

uint32_t get_data_regular(){
    // send code 0x01
    return 0;
}

uint32_t get_data_detailed(){
    // send code 0x02
    return 0;
}

uint32_t set_mode(FLIGHT_STATE mode){

switch(mode){
    case CHARGE_MODE:
        // send code 0x03
        break;
    case DOWNLINK_MODE:
        // send code 0x04
        break;
    case RADIATION_SAFE_MODE:
        // send code 0x05
        break;
    case POWER_SAFE_MODE:
        // send code 0x06
        break;
    case END_OF_LIFE_MODE:
        // send code 0x07
        break;
    case STARTUP_MODE:
        // send code 0x08
        break;
    case DETUMBLE_MODE:
        // send code 0x09
        break;
}

}

uint32_t set_attitude(uint32_t x, uint32_t y, uint32_t z){
    // send code 0x0A
    return 0;
}

AdcsData* data_for_comms(){
    
}