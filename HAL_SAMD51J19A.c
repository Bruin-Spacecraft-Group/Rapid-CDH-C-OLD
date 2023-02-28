#include "HAL.h"

#ifdef __SAMD51J19A__

#include "samd51j19a.h"

void* const PORT_A = PORT_REGS->GROUP;
void* const PORT_B = PORT_REGS->GROUP + 1;

void HAL_GPIO_Init(void* port, uint32_t pins) {
    ((port_group_registers_t*) port)->PORT_DIRSET = pins;
}
void HAL_GPIO_TogglePin(void* port, uint32_t pins) {
    ((port_group_registers_t*) port)->PORT_OUTTGL = pins;
}
#endif
