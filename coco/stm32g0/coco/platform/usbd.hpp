#pragma once

#include "usbdG0H5U3U5.hpp"
#include <coco/debug.hpp>


#ifdef HAVE_USBD

namespace coco {
namespace usbd {

// interrupt request index
constexpr int irq = USB_UCPD1_2_IRQn;


/// @brief Initalize the USB device
/// When HSI48 is selected as USB clock source in the RCC->CCIPR2 register, HSI48 and CRS get enabled.
/// When PLLQ is selected as USB clock source, make sure that PLLQ generates 48MHz from a precise clock.
/// When HSE is selected as USB clock source, make sure that HSE is a precise 48MHz oscillator.
/// @return Instance (wrapper for registers)
inline Instance enableClock() {
    uint32_t apbenr1 = RCC->APBENR1;
    if ((RCC->CCIPR2 & RCC_CCIPR2_USBSEL_Msk) == 0) {
        // enable HSI48 and wait until ready
        RCC->CR = RCC->CR | RCC_CR_HSI48ON;
        while ((RCC->CR & RCC_CR_HSI48RDY) == 0);

        // enable clock of USB and CRS (clock recovery system)
        RCC->APBENR1 = apbenr1 | (RCC_APBENR1_CRSEN | RCC_APBENR1_USBEN);
        __NOP();
        __NOP();

        // enable automatic trimming and oscillator clock for the frequency error counter
        CRS->CR = (32 << CRS_CR_TRIM_Pos) | CRS_CR_AUTOTRIMEN | CRS_CR_CEN;

        // use default configuration (sync on USB SOF)
        //CRS->CFGR =
    } else {
        // enable clock of USB
        RCC->APBENR1 = apbenr1 | RCC_APBENR1_USBEN;
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

#endif // HAVE_USBD
