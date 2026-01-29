#pragma once

#include "extiF0F3F4.hpp"


namespace coco {

/// @brief EXTI helpers
/// Refernece manual: https://www.st.com/resource/en/reference_manual/rm0364-stm32f334xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
/// EXTI: Section 12.2
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

    USB_WAKEUP = 18,

    COPMP1_OUT = 21,
    COPMP2_OUT = 22,
};

static constexpr uint8_t irqs[16] = {
    EXTI0_IRQn,
    EXTI1_IRQn,
    EXTI2_TSC_IRQn,
    EXTI3_IRQn,
    EXTI4_IRQn,
    EXTI9_5_IRQn,
    EXTI9_5_IRQn,
    EXTI9_5_IRQn,
    EXTI9_5_IRQn,
    EXTI9_5_IRQn,
    EXTI15_10_IRQn,
    EXTI15_10_IRQn,
    EXTI15_10_IRQn,
    EXTI15_10_IRQn,
    EXTI15_10_IRQn,
    EXTI15_10_IRQn,
};

/// @brief Get the IRQ number for a given EXTI channel
/// @param channel EXTI channel number 0 - 15
/// @return Interrupt request index (may be shared with other channels)
inline uint8_t irq(int channel) {
    return irqs[channel];
}

} // namespace exti
} // namespace coco
