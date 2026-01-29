#pragma once

#include "extiC0G0H5U3U5.hpp"


namespace coco {

/// @brief EXTI helpers
/// Refernece manual: https://www.st.com/resource/en/reference_manual/rm0440-stm32g4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
/// EXTI: Section 15
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

    USART1_WAKEUP = 25,
};

static constexpr uint8_t irqs[16] = {
    EXTI0_1_IRQn,
    EXTI0_1_IRQn,
    EXTI2_3_IRQn,
    EXTI2_3_IRQn,
    EXTI4_15_IRQn,
    EXTI4_15_IRQn,
    EXTI4_15_IRQn,
    EXTI4_15_IRQn,
    EXTI4_15_IRQn,
    EXTI4_15_IRQn,
    EXTI4_15_IRQn,
    EXTI4_15_IRQn,
    EXTI4_15_IRQn,
    EXTI4_15_IRQn,
    EXTI4_15_IRQn,
    EXTI4_15_IRQn,
};

/// @brief Get the IRQ number for a given EXTI channel
/// @param channel EXTI channel number 0 - 15
/// @return Interrupt request index (may be shared with other channels)
inline uint8_t irq(int channel) {
    return irqs[channel];
}

} // namespace exti
} // namespace coco
