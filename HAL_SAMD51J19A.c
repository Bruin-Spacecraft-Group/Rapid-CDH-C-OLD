#include "HAL.h"

#ifdef __SAMD51J19A__

#if defined(__DEBUG) && __DEBUG
volatile int SIDE_EFFECT_FOR_DEBUGGING = 0x12345678;
#endif

#include "samd51j19a.h"
#include <string.h>

void* const PORT_A = PORT_REGS->GROUP;
void* const PORT_B = PORT_REGS->GROUP + 1;

void HAL_GPIO_Init(void* port, uint32_t pins) {
    ((port_group_registers_t*) port)->PORT_DIRSET = pins;
}
void HAL_GPIO_TogglePin(void* port, uint32_t pins) {
    ((port_group_registers_t*) port)->PORT_OUTTGL = pins;
}

void TC0_Handler() {
    // turn off the sleep timer, since we're now awake
    PM_REGS->PM_INTENCLR = 1 << 4;
}

void HAL_32kHz_Init() {
    // set GCLK0 to use the internal ultra low power 332kHz oscillator
    OSC32KCTRL_REGS->OSC32KCTRL_OSCULP32K = (OSC32KCTRL_REGS->OSC32KCTRL_OSCULP32K & ~OSC32KCTRL_OSCULP32K_EN32K_Msk) | OSC32KCTRL_OSCULP32K_EN32K(1);
    GCLK_REGS->GCLK_GENCTRL[2] = (GCLK_REGS->GCLK_GENCTRL[2] & 
            ~GCLK_GENCTRL_GENEN_Msk & ~GCLK_GENCTRL_DIVSEL_Msk & ~GCLK_GENCTRL_DIV_Msk & ~GCLK_GENCTRL_SRC_Msk)
            | (GCLK_GENCTRL_GENEN(1) | GCLK_GENCTRL_DIVSEL(0) | GCLK_GENCTRL_DIV(1) | GCLK_GENCTRL_SRC_OSCULP32K);
    // set TC0/TC1 to use the 32kHz clock given by GCLK0
    GCLK_REGS->GCLK_PCHCTRL[9] = (GCLK_REGS->GCLK_PCHCTRL[9] & ~GCLK_PCHCTRL_CHEN_Msk) | GCLK_PCHCTRL_CHEN(0);
    while (GCLK_REGS->GCLK_SYNCBUSY);
    while ((GCLK_REGS->GCLK_PCHCTRL[9] & GCLK_PCHCTRL_CHEN_Msk) != GCLK_PCHCTRL_CHEN(0));
    GCLK_REGS->GCLK_PCHCTRL[9] = (GCLK_REGS->GCLK_PCHCTRL[9] & ~GCLK_PCHCTRL_GEN_Msk) | GCLK_PCHCTRL_GEN_GCLK0;
    GCLK_REGS->GCLK_PCHCTRL[9] = (GCLK_REGS->GCLK_PCHCTRL[9] & ~GCLK_PCHCTRL_CHEN_Msk) | GCLK_PCHCTRL_CHEN(1);
    while (GCLK_REGS->GCLK_SYNCBUSY);
    while ((GCLK_REGS->GCLK_PCHCTRL[9] & GCLK_PCHCTRL_CHEN_Msk) != GCLK_PCHCTRL_CHEN(1));
}

void HAL_Sleep(uint32_t millis) {
    // start the timer for however long we're running
    TC0_REGS->COUNT32.TC_CC[0] = TC0_REGS->COUNT32.TC_COUNT + (millis << 5);
    // save port configurations for seamless low-frequency interactions (ports still
    // turn off while the device is asleep, so they don't draw significant power)
    // do nothing with serial or other similar output configurations because there's
    // no way to prevent high-frequency interactions from being interrupted while
    // sleeping, that's the whole idea of sleeping
    PM_REGS->PM_CTRLA = (PM_REGS->PM_CTRLA & ~PM_CTRLA_IORET_Msk) | PM_CTRLA_IORET(1);
    uint32_t saved_ports[PORT_GROUP_NUMBER][4 + sizeof(PORT_REGS->GROUP[0].PORT_PINCFG) + sizeof(PORT_REGS->GROUP[0].PORT_PMUX)];
    for (unsigned i = 0U; i < PORT_GROUP_NUMBER; i++) {
        saved_ports[i][0] = PORT_REGS->GROUP[i].PORT_DIR;
        saved_ports[i][1] = PORT_REGS->GROUP[i].PORT_OUT;
        saved_ports[i][2] = PORT_REGS->GROUP[i].PORT_CTRL;
        saved_ports[i][3] = PORT_REGS->GROUP[i].PORT_EVCTRL;
        memcpy(saved_ports[i] + 4,
                // casting away volatility is safe here because if we are going to sleep,
                // we don't expect to be interrupted, and this function doesn't change
                // PINCFG values with third-party IO registers like WRCONFIG or side effects
                (uint8_t*) PORT_REGS->GROUP[i].PORT_PINCFG,
                sizeof(PORT_REGS->GROUP[i].PORT_PINCFG)
        );
        memcpy(saved_ports[i] + 4 + sizeof(PORT_REGS->GROUP[i].PORT_PINCFG),
                // casting away volatility is safe here because if we are going to sleep,
                // we don't expect to be interrupted, and this function doesn't change
                // PMUX values with third-party IO registers like WRCONFIG or side effects
                (uint8_t*) PORT_REGS->GROUP[i].PORT_PMUX,
                sizeof(PORT_REGS->GROUP[i].PORT_PMUX)
        );
    }
    // set to use HIBERNATE sleep mode with full RAM retention
    PM_REGS->PM_HIBCFG = (PM_REGS->PM_HIBCFG & ~PM_HIBCFG_BRAMCFG_Msk & ~PM_HIBCFG_RAMCFG_Msk) | PM_HIBCFG_BRAMCFG_RET | PM_HIBCFG_RAMCFG_RET;
    PM_REGS->PM_SLEEPCFG = (PM_REGS->PM_SLEEPCFG & ~PM_SLEEPCFG_Msk) | PM_SLEEPCFG_SLEEPMODE(PM_SLEEPCFG_SLEEPMODE_HIBERNATE);
    while ((PM_REGS->PM_SLEEPCFG & PM_SLEEPCFG_Msk) != PM_SLEEPCFG_SLEEPMODE(PM_SLEEPCFG_SLEEPMODE_HIBERNATE));
    // wait for interrupt
    PM_REGS->PM_INTENSET = 1 << 4;
    __WFI();
    // restore saved port configuration
    for (unsigned i = 0U; i < PORT_GROUP_NUMBER; i++) {
        PORT_REGS->GROUP[i].PORT_DIR = saved_ports[i][0];
        PORT_REGS->GROUP[i].PORT_OUT = saved_ports[i][1];
        PORT_REGS->GROUP[i].PORT_CTRL = saved_ports[i][2];
        PORT_REGS->GROUP[i].PORT_EVCTRL = saved_ports[i][3];
        memcpy(
                // casting away volatility is safe here because if we just woke up from sleep,
                // we don't expect to be interrupted, and this function doesn't change
                // PINCFG values with third-party IO registers like WRCONFIG or side effects
                (uint8_t*) PORT_REGS->GROUP[i].PORT_PINCFG,
                saved_ports[i] + 4,
                sizeof(PORT_REGS->GROUP[i].PORT_PINCFG)
        );
        memcpy(
                // casting away volatility is safe here because if we just woke up from sleep,
                // we don't expect to be interrupted, and this function doesn't change
                // PMUX values with third-party IO registers like WRCONFIG or side effects
                (uint8_t*) PORT_REGS->GROUP[i].PORT_PMUX,
                saved_ports[i] + 4 + sizeof(PORT_REGS->GROUP[i].PORT_PINCFG),
                sizeof(PORT_REGS->GROUP[i].PORT_PMUX)
        );
    }
    PM_REGS->PM_CTRLA = (PM_REGS->PM_CTRLA & ~PM_CTRLA_IORET_Msk) | PM_CTRLA_IORET(0);
}

uint32_t HAL_getTime() {
    return TC0_REGS->COUNT32.TC_COUNT;
}

#endif
