#pragma once

#include "usbdG0H5U3U5.hpp"
#include <coco/debug.hpp>


namespace coco {
namespace usbd {

// interrupt request index
constexpr int irq = USB_DRD_FS_IRQn;


/// @brief Initalize the USB device.
/// When no clock is selected as USB clock source in the RCC->CCIPR4 register, HSI48 and CRS get enabled.
/// When HSI48 is selected as USB clock source, make sure HSI48 and CRS are enabled.
/// When PLL1Q/PLL2Q is selected as USB clock source, make sure that PLL1Q/PLL2Q generates 48MHz from a precise clock.
/// @return Instance (wrapper for registers)
inline Instance enableClock() {
    if ((RCC->CCIPR4 & RCC_CCIPR4_USBSEL_Msk) == 0) {
        // enable HSI48 and wait until ready
        RCC->CR = RCC->CR | RCC_CR_HSI48ON;
        while ((RCC->CR & RCC_CR_HSI48RDY) == 0);

        // use HSI48 as USB clock source
        RCC->CCIPR4 = RCC->CCIPR4 | (3 << RCC_CCIPR4_USBSEL_Pos);

        // enable clock of USB and CRS (clock recovery system)
        RCC->APB1LENR = RCC->APB1LENR | RCC_APB1LENR_CRSEN;
        RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_USBEN;

        // enable automatic trimming and oscillator clock for the frequency error counter
        CRS->CR = (32 << CRS_CR_TRIM_Pos) | CRS_CR_AUTOTRIMEN | CRS_CR_CEN;

        // use default configuration (sync on USB SOF)
        //CRS->CFGR =
    } else {
        // enable clock of USB
        RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_USBEN;
        __NOP();
        __NOP();
    }

    // switch on usb transceiver (clear PWDN bit), but keep reset (reference manual: USB functional description -> System and power-on reset)
    USB_DRD_FS->CNTR = USB_CNTR_USBRST;

    // wait for at least 1us (see data sheet: t_STARTUP)
    debug::sleep(1us);

    // enable pull-up resistor to start enumeration
    USB_DRD_FS->BCDR = USB_BCDR_DPPU;

    // exit reset of usb
    USB_DRD_FS->CNTR = 0;

    return {USB_DRD_FS, (volatile uint32_t *)USB_DRD_PMAADDR};
}

/// @brief Get the instance
/// @return Instance (wrapper for registers)
inline Instance instance() {
    return {USB_DRD_FS, (volatile uint32_t *)USB_DRD_PMAADDR};
}

} // namespace usbd
} // namespace coco
