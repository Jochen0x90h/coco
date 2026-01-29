#pragma once

#include "usbdG0H5U3U5.hpp"
#include <coco/debug.hpp>


namespace coco {
namespace usbd {

// interrupt request index
constexpr int irq = USB_FS_IRQn;


/// @brief Initalize the USB device.
/// When HSI48/1 is selected as USB clock source in the RCC->CCIPR1 register, HSI48 and CRS get enabled.
/// When HSE is selected as USB clock source, make sure that HSE is a precise 48MHz clock.
/// @return Instance (wrapper for registers)
inline Instance enableClock() {
    if ((RCC->CCIPR1 & (RCC_CCIPR1_ICLKSEL_Msk | RCC_CCIPR1_USB1SEL)) == 0) {
        // enable HSI48 and wait until ready
        RCC->CR = RCC->CR | RCC_CR_HSI48ON;
        while ((RCC->CR & RCC_CR_HSI48RDY) == 0);

        // enable clock of USB and CRS (clock recovery system)
        RCC->APB1ENR1 = RCC->APB1ENR1 |  RCC_APB1ENR1_CRSEN;
        RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_USB1EN;

        // enable automatic trimming and oscillator clock for the frequency error counter
        CRS->CR = (32 << CRS_CR_TRIM_Pos) | CRS_CR_AUTOTRIMEN | CRS_CR_CEN;

        // use default configuration (sync on USB SOF)
        //CRS->CFGR =
    } else {
        // enable clock of USB
        RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_USB1EN;
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
