#include "HAL.h"

int main(void) {
    HAL_GPIO_Init(PORT_B, MCU == SAMD51 ? 0x0007 : 0x000b);
    while (1) {
        HAL_GPIO_TogglePin(PORT_B, MCU == SAMV71 ? 0x0001 : 0x0002);
    }
}
