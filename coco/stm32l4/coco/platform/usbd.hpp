#pragma once

#include "usbdF0C0F3G4L4.hpp"
#include <coco/debug.hpp>


#ifdef HAVE_USBD

namespace coco {

/// @brief USB device helpers
///
namespace usbd {

// interrupt request index
constexpr int irq = USB_IRQn;


/// @brief Initalize the USB device.
/// When HSI48 is selected as USB clock source in the RCC->CCIPR register, HSI48 and CRS get enabled.
/// When PLLQ or PLLSAI1Q is selected as USB clock source, make sure that PLLQ generates 48MHz from a precise clock.
/// @return Instance (wrapper for registers)
inline Instance enableClock() {
    uint32_t apb1enr1 = RCC->APB1ENR1;
    if ((RCC->CCIPR & RCC_CCIPR_CLK48SEL_Msk) == 0) {
        // enable HSI48 and wait until ready
        RCC->CRRCR = RCC_CRRCR_HSI48ON;
        while ((RCC->CRRCR & RCC_CRRCR_HSI48RDY) == 0);

        // enable clock of USB and CRS (clock recovery system)
        RCC->APB1ENR1 = apb1enr1 | (RCC_APB1ENR1_CRSEN | RCC_APB1ENR1_USBFSEN);
        __NOP();
        __NOP();

        // enable automatic trimming and oscillator clock for the frequency error counter
        CRS->CR = (32 << CRS_CR_TRIM_Pos) | CRS_CR_AUTOTRIMEN | CRS_CR_CEN;

        // use default configuration (sync on USB SOF)
        //CRS->CFGR =
    } else {
        // enable clock of USB
        RCC->APB1ENR1 = apb1enr1 | RCC_APB1ENR1_USBFSEN;
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

#endif //HAVE_USBD
