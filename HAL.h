// Hardware Abstraction Layer
// See https://www.st.com/resource/en/user_manual/um1725-description-of-stm32f4-hal-and-lowlayer-drivers-stmicroelectronics.pdf
// for description of HAL interface used on STM32 microcontrollers, which serves as an example architecture

#ifndef HAL_H
#define	HAL_H

#include "xc.h"
#define SAMD51 1
#define SAMV71 2
#ifdef __SAMD51J19A__
    #define MCU SAMD51
#elif __SAMV71Q21B__
    #define MCU SAMV71
#else
    #error Unrecognized configuration, cannot create HAL
#endif



extern void* const PORT_A;
extern void* const PORT_B;

void HAL_GPIO_Init(void* port, uint32_t pins);
void HAL_GPIO_TogglePin(void* port, uint32_t pins);

#endif	/* HAL_H */

