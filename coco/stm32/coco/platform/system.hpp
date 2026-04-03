#pragma once

#include "backup.hpp"
#include "nvic.hpp"
#include "rcc.hpp"
#include <coco/convert.hpp>
#include <coco/StringBuffer.hpp>

namespace coco {

/// @brief System functions.
/// System functions such as reset, intent, jump
namespace system {

/// @brief Get system id.
/// @return System id in the format stm32-device-revision
inline StringBuffer<16> id() {
    StringBuffer<16> result;
#ifdef DBGMCU
    uint32_t idcode = DBGMCU->IDCODE;
    int device = (idcode & DBGMCU_IDCODE_DEV_ID_Msk) >> DBGMCU_IDCODE_DEV_ID_Pos;
    int revision = (idcode & DBGMCU_IDCODE_REV_ID_Msk) >> DBGMCU_IDCODE_REV_ID_Pos;
#endif
#ifdef DBG
    uint32_t idcode = DBG->IDCODE;
    int device = (idcode & DBG_IDCODE_DEV_ID_Msk) >> DBG_IDCODE_DEV_ID_Pos;
    int revision = (idcode & DBG_IDCODE_REV_ID_Msk) >> DBG_IDCODE_REV_ID_Pos;
#endif
    result << "stm32-" << hex(device, 1) << '-' << hex(revision, 1);
    return result;
}

/// @brief Reset the microcontroller.
///
__NO_RETURN __STATIC_FORCEINLINE void reset() {
    NVIC_SystemReset();
}

/// @brief Reset the microcontroller with a specific intent.
/// The intent can be used to instruct a bootloader to stay in the bootloader and wait for commands.
/// The intent value is stored in the backup register BKP0R.
/// The function does not return.
/// @param intent Intent value
__NO_RETURN __STATIC_FORCEINLINE void reset(int intent) {
    __disable_irq();

#ifdef HAVE_BACKUP
    backup::unlock();
    backup::set(0, intent);
#endif

    NVIC_SystemReset();
}

/// @brief Get intent.
/// The intent indicates what to do at startup, e.g. stay in bootloader or run the application.
/// Returns 0 when backup registers are not supported (HAVE_BACKUP is not defined).
/// @return Reset intent
__STATIC_FORCEINLINE int intent() {
#ifdef HAVE_BACKUP
    return backup::get(0);
#else
    return 0;
#endif
}

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

} // namespace system
} // namespace coco
