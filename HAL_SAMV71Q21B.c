#include "HAL.h"

#if MCU==SAMV71

#include "samv71q21b.h"

void* const PORT_A = PIOA_REGS;
void* const PORT_B = PIOB_REGS;

void HAL_GPIO_Init(void* port, uint32_t pins) {
    ((pio_registers_t*) port)->PIO_PER = pins;
    ((pio_registers_t*) port)->PIO_OER = pins;
}
void HAL_GPIO_TogglePin(void* port, uint32_t pins) {
    uint32_t status = ((pio_registers_t*) port)->PIO_ODSR;
    ((pio_registers_t*) port)->PIO_CODR = pins & status;
    ((pio_registers_t*) port)->PIO_SODR = pins & ~status;
}
#endif
