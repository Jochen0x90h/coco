#pragma once

#include "extiC0G0H5U3U5.hpp"


namespace coco {
namespace exti {

enum class Line : uint8_t {
    GPIO_0 = 0,
    GPIO_1 = 1,
    GPIO_2 = 2,
    GPIO_3 = 3,
    GPIO_4 = 4,
    GPIO_5 = 5,
    GPIO_6 = 6,
    GPIO_7 = 7,
    GPIO_8 = 8,
    GPIO_9 = 9,
    GPIO_10 = 10,
    GPIO_11 = 11,
    GPIO_12 = 12,
    GPIO_13 = 13,
    GPIO_14 = 14,
    GPIO_15 = 15,
    PVD_OUT = 16,

    USART1_WAKEUP = 25,
    USART2_WAKEUP = 26,
    USART3_WAKEUP = 27,

    USB_WAKEUP = 47,
    USBPD1_WAKEUP = 48,
};

/// @brief Get the IRQ number for a given EXTI channel
/// @param channel EXTI channel number 0 - 15
/// @return Interrupt request index (may be shared with other channels)
inline uint8_t irq(int channel) {
    return EXTI0_IRQn + channel;
}

} // namespace exti
} // namespace coco
