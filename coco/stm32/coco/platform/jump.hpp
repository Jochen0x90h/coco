#pragma once

#include "nvic.hpp"
#include "rcc.hpp"


namespace coco {

/// @brief Jump to an address, e.g. bootloader or application.
/// Function is inline so that it is placed into the section of the containing function,
/// e.g. __attribute__((section(".ccmram"))) void foo() {jumpInline(BOOT_ADDRESS);}
/// @param address
/// @return Does not return
__attribute__((noreturn)) __STATIC_FORCEINLINE void jumpInline(uint32_t address) {
    __disable_irq();

    rcc::resetAll(); // reset all peripherals
    nvic::disableAll(); // disable all interrupts
    nvic::clearAll(); // clear all pending interrupts

    // disable SysTick and clear interrupt flag
    SysTick->CTRL = 0;
    SCB->ICSR |= SCB_ICSR_PENDSTCLR_Msk;

    __enable_irq();

    // set vector table offset to the application address
    // note that Cortex M0 lacks VTOR register and the table needs to be copied to a specific ram location
#ifdef SCB_VTOR_TBLOFF_Pos
    SCB->VTOR = address;
    __DSB();
    __ISB();
#endif

    // load stack pointer und reset handler from vector table of the application
    uint32_t sp = *(volatile uint32_t *)(address + 0x00);
    uint32_t pc = *(volatile uint32_t *)(address + 0x04);

    // set stack pointer and jump to reset handler of application
    __asm volatile (
        "msr msp, %[sp]\n"
        "bx %[pc]\n"
        :
        : [sp] "r" (sp), [pc] "r" (pc)
        :
    );
    __builtin_unreachable();
}


/// @brief Jump to an address, e.g. bootloader or application.
/// For this function, the compiler decides if it gets inlined or not.
/// @param address
/// @return Does not return
__attribute__((noreturn)) inline void jump(uint32_t address) {
    jumpInline(address);
}

} // namespace coco
