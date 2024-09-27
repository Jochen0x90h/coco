#pragma once

#include "platform.hpp"


namespace coco {

/**
 * EXTI helpers
 * Refernece manual: https://www.st.com/resource/en/reference_manual/dm00031936-stm32f0x1stm32f0x2stm32f0x8-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 * EXTI: Section 15.2
 */
namespace exti {

static constexpr uint8_t irqs [16] = {
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

} // namespace exti
} // namespace coco
