#include "HAL.h"

int main(void) {
    HAL_GPIO_Init(PORT_B, 0x0001);
    while (1) {
        HAL_GPIO_TogglePin(PORT_B, 0x0001);
    }
}
