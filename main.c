void setOutput_PA0();
void toggle_PA0();

#include "xc.h"
#ifdef __SAMD51J19A__
void setOutput_PA0() {
    PORT_REGS->GROUP[0].PORT_DIRSET = 1;
}
void toggle_PA0() {
    PORT_REGS->GROUP[0].PORT_OUTTGL = 1;
}
#endif
#ifdef __SAMV71Q21B__
#include "samv71q21b.h"
void setOutput_PA0() {
    PIOA_REGS->PIO_PER = 1;
    PIOA_REGS->PIO_OER = 1;
}
void toggle_PA0() {
    if (PIOA_REGS->PIO_ODSR & 1) {
        PIOA_REGS->PIO_CODR = 1;
    } else {
        PIOA_REGS->PIO_SODR = 1;
    }
}
#endif

int main(void) {
    /* Replace with your application code */
    setOutput_PA0();
    while (1) {
        toggle_PA0();
    }
}
