// Hardware Abstraction Layer
// See https://www.st.com/resource/en/user_manual/um1725-description-of-stm32f4-hal-and-lowlayer-drivers-stmicroelectronics.pdf
// for description of HAL interface used on STM32 microcontrollers, which serves as an example architecture

#ifndef HAL_H
#define	HAL_H

#include "xc.h"

extern void* const PORT_A;
extern void* const PORT_B;

void HAL_GPIO_Init(void* port, uint32_t pins);
void HAL_GPIO_TogglePin(void* port, uint32_t pins);

#endif	/* HAL_H */

