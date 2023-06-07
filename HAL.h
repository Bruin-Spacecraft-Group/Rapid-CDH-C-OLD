// Hardware Abstraction Layer
// See https://www.st.com/resource/en/user_manual/um1725-description-of-stm32f4-hal-and-lowlayer-drivers-stmicroelectronics.pdf
// for description of HAL interface used on STM32 microcontrollers, which serves as an example architecture

#ifndef HAL_H
#define	HAL_H

#ifndef __ARDUINO_MEGA_2560__ 
#ifndef __SAMV71Q21B__
#ifndef __SIMULATION__
#error must define device to build to! (see config.h)
#endif
#endif
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

#if defined(__DEBUG) && __DEBUG
// In order to force a branch or loop to be compiled as-is in debug mode, increment
// this variable from somewhere in the branch or loop, and it will be forced to be
// compiled into the code. This variable should not be used for release builds because
// it serves no purpose and is not truly volatile, so doing so is a compile error.
extern volatile int SIDE_EFFECT_FOR_DEBUGGING;
#endif

#ifdef __XC32
    #include "xc.h"
#endif

#include <stdint.h>

extern void* const PORT_A;
extern void* const PORT_B;
extern void* const PORT_C;
extern void* const PORT_D;
extern void* const PORT_E;
extern void* const PORT_F;
extern void* const PORT_G;
extern void* const PORT_H;
extern void* const PORT_J;
extern void* const PORT_K;

// sets up the pins chosen by the bit mask from the selected port as general purpose outputs
void HAL_GPIO_Init(void* port, uint32_t pins);
// toggles any output pins chosen by the bit mask from the selected port
void HAL_GPIO_TogglePin(void* port, uint32_t pins);
// sets the value of all output pins on the selected port according to the bit mask
void HAL_GPIO_AssignPins(void* port, uint32_t pins);
// sets any output pins chosen by the bit mask from the selected port
void HAL_GPIO_SetPins(void* port, uint32_t pins);
// clears any output pins chosen by the bit mask from the selected port
void HAL_GPIO_ClearPins(void* port, uint32_t pins);


// sets up GCLK2 and TC0 to run at 32kHz
void HAL_32kHz_Init();
// turns off CPU and peripherals; keeps RAM, stack, and GP registers. interrupts and
// returns using TC0 after the time expires - requires 32kHz to be initialized
void HAL_Sleep(uint32_t millis);
// gives the elapsed time according to the ultra low-power 32kHz clock, in ticks
// overflows each 1.5 days: this function is appropriate only for measuring small
// elapsed durations and requires 32kHz to be initialized
uint32_t HAL_getTime();

void HAL_I2C_beginTransmission(uint8_t addr);
void HAL_I2C_signalTimeout();

#endif	/* HAL_H */

