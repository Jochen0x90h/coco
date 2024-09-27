#pragma once

#include "platform.hpp"
#include <coco/platform/rccInfoF0F3.hpp>
#include <coco/platform/timerInfo.hpp>


namespace coco {
namespace timer {

/**
 * Timer trigger
 * Reference manual section 17.4.3
 */
enum class Trigger {
    // TIM1 triggered by...
    TIM1_TIM15_TRGO = 0, // ITR0
    TIM1_TIM2_TRGO = TIM_SMCR_TS_0, // ITR1
    TIM1_TIM3_TRGO = TIM_SMCR_TS_1, // ITR2
    TIM1_TIM17_TRGO = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // ITR3

    // TIM2 triggered by...
    TIM2_TIM1_TRGO = 0, // ITR0
    TIM2_TIM15_TRGO = TIM_SMCR_TS_0, // ITR1
    TIM2_TIM3_TRGO = TIM_SMCR_TS_1, // ITR2
    TIM2_TIM14_TRGO = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // ITR3

    // TIM3 triggered by...
    TIM3_TIM1_TRGO = 0, // ITR0
    TIM3_TIM2_TRGO = TIM_SMCR_TS_0, // ITR1
    TIM3_TIM15_TRGO = TIM_SMCR_TS_1, // ITR2
    TIM3_TIM14_TRGO = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // ITR3

    // TIM1, TIM2, TIM3 triggered by...
    TI1F_ED = TIM_SMCR_TS_2, // TI1 Edge Detector
    TI1FP1 = TIM_SMCR_TS_2 | TIM_SMCR_TS_0, // Filtered Timer Input 1
    TI1FP2 = TIM_SMCR_TS_2 | TIM_SMCR_TS_1, // Filtered Timer Input 2
    ETRF = TIM_SMCR_TS_2 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0 // External Trigger Input
};

// general purpose timers
constexpr Info1 TIM2_INFO{TIM2, {&RCC->APB1ENR, RCC_APB1ENR_TIM2EN}, TIM2_IRQn}; // 32 bit
#ifdef TIM3
constexpr Info1 TIM3_INFO{TIM3, {&RCC->APB1ENR, RCC_APB1ENR_TIM3EN}, TIM3_IRQn};
#endif
#ifdef TIM6
constexpr Info1 TIM6_INFO{TIM6, {&RCC->APB1ENR, RCC_APB1ENR_TIM6EN}, TIM6_DAC_IRQn};
#endif
#ifdef TIM7
constexpr Info1 TIM7_INFO{TIM7, {&RCC->APB1ENR, RCC_APB1ENR_TIM7EN}, TIM7_IRQn};
#endif
static const Info1 TIM14_INFO{TIM14, {&RCC->APB2ENR, RCC_APB1ENR_TIM14EN}, TIM14_IRQn};
#ifdef TIM15
static const Info1 TIM15_INFO{TIM15, {&RCC->APB2ENR, RCC_APB2ENR_TIM15EN}, TIM15_IRQn};
#endif
static const Info1 TIM16_INFO{TIM16, {&RCC->APB2ENR, RCC_APB2ENR_TIM16EN}, TIM16_IRQn};
static const Info1 TIM17_INFO{TIM17, {&RCC->APB2ENR, RCC_APB2ENR_TIM17EN}, TIM17_IRQn};

// advanced control timers
static const Info2 TIM1_INFO{TIM1, {&RCC->APB2ENR, RCC_APB2ENR_TIM1EN}, TIM1_BRK_UP_TRG_COM_IRQn, TIM1_BRK_UP_TRG_COM_IRQn, TIM1_BRK_UP_TRG_COM_IRQn, TIM1_CC_IRQn};

} // namespace timer
} // namespace coco
