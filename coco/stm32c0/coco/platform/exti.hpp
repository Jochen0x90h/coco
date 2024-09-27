#pragma once

#include "platform.hpp"


namespace coco {

/**
 * EXTI helpers
 * Refernece manual: https://www.st.com/resource/en/reference_manual/rm0440-stm32g4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 * EXTI: Section 15
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