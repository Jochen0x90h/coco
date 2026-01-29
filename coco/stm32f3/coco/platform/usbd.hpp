#pragma once

#include "usbdF0C0F3G4L4.hpp"
#include <coco/debug.hpp>


#ifdef HAVE_USBD

namespace coco {
namespace usbd {

// interrupt request index
constexpr int irq = USB_LP_IRQn;


/// @brief Initalize the USB device.
/// Note that for F3 series an external pull-up resistor is required to start enumeration.
/// Make sure that HSE generates a precise 48MHz clock for USB.
/// @return Instance (wrapper for registers)
inline Instance enableClock() {
    // enable clock of USB
    RCC->APB1ENR = RCC->APB1ENR | RCC_APB1ENR_USBEN;
    __NOP();
    __NOP();

    // switch on usb transceiver (clear PWDN bit), but keep reset (reference manual: USB functional description -> System and power-on reset)
    USB->CNTR = USB_CNTR_FRES;

    // wait for at least 1us (see data sheet: t_STARTUP)
    debug::sleep(1us);

    // enable pull-up resistor to start enumeration: needs external resistor
    //USB->BCDR = USB_BCDR_DPPU;

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
