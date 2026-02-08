#pragma once

#include "usbdF0C0F3G4L4.hpp"
#include <coco/debug.hpp>


#ifdef HAVE_USBD

namespace coco {
namespace usbd {

// interrupt request index
constexpr int irq = USB_IRQn;


/// @brief Initalize the USB device.
/// When HSI48 is selected as USB clock source by USBSW in the RCC->CFGR3 register, HSI48 and CRS get enabled.
/// When HSI48 does not exist (F07x), make sure that the PLL derives a precise 48MHz clock from HSE.
/// @return Instance (wrapper for registers)
inline Instance enableClock() {
    uint32_t apb1enr = RCC->APB1ENR;
#ifdef RCC_CR2_HSI48ON
    if ((RCC->CFGR3 & RCC_CFGR3_USBSW) == 0) {
        // enable HSI and wait until ready
        RCC->CR2 = RCC->CR2 | RCC_CR2_HSI48ON;
        while ((RCC->CR2 & RCC_CR2_HSI48RDY) == 0);

        // enable clock of USB and CRS (clock recovery system)
        RCC->APB1ENR = apb1enr | (RCC_APB1ENR_CRSEN | RCC_APB1ENR_USBEN);
        __NOP();
        __NOP();

        // enable automatic trimming and oscillator clock for the frequency error counter
        CRS->CR = (32 << CRS_CR_TRIM_Pos) | CRS_CR_AUTOTRIMEN | CRS_CR_CEN;

        // use default configuration (sync on USB SOF)
        //CRS->CFGR =
    } else
#endif
    {
        // enable clock of USB
        RCC->APB1ENR = apb1enr | RCC_APB1ENR_USBEN;
        __NOP();
        __NOP();
    }

    // switch on usb transceiver (clear PWDN bit), but keep reset (reference manual: USB functional description -> System and power-on reset)
    USB->CNTR = USB_CNTR_FRES;

    // wait for at least 1us (see data sheet: t_STARTUP)
    debug::sleep(1us);

    // enable pull-up resistor to start enumeration
    USB->BCDR = USB_BCDR_DPPU;

    // exit reset of usb
    USB->CNTR = 0;

    return {USB, (volatile uint16_t *)USB_PMAADDR};
}

/// @brief Get the instance
/// @return Instance (wrapper for registers)
inline Instance instance() {
    return {USB, (volatile uint16_t *)USB_PMAADDR};
}

} // namespace usbd
} // namespace coco

#endif // HAVE_USBD
