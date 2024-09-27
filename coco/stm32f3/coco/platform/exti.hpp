#pragma once

#include "platform.hpp"


namespace coco {

/**
 * EXTI helpers
 * Refernece manual: https://www.st.com/resource/en/reference_manual/rm0364-stm32f334xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 * EXTI: Section 12.2
 */
namespace exti {

static constexpr uint8_t irqs [16] = {
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

} // namespace exti
} // namespace coco
