#pragma once

#include "platform.hpp"
#include <coco/platform/rccInfoF0F3.hpp>
#include <coco/platform/timerInfo.hpp>


namespace coco {
namespace timer {

/**
 * Timer trigger
 * Reference manual section 18.4.3
 */
enum class Trigger {
    // TIM1 triggered by...
    TIM1_TIM15_TRGO = 0, // ITR0
    TIM1_TIM2_TRGO = TIM_SMCR_TS_0, // ITR1
    TIM1_TIM3_TRGO = TIM_SMCR_TS_1, // ITR2
    TIM1_TIM17_OC1 = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // ITR3

    // TIM2 triggered by...
    TIM2_TIM1_TRGO = 0, // ITR0
    TIM2_TIM3_TRGO = TIM_SMCR_TS_1, // ITR2

    // TIM3 triggered by...
    TIM3_TIM1_TRGO = 0, // ITR0
    TIM3_TIM2_TRGO = TIM_SMCR_TS_0, // ITR1

    // TIM1, TIM2, TIM3 triggered by...
    TI1F_ED = TIM_SMCR_TS_2, // TI1 Edge Detector
    TI1FP1 = TIM_SMCR_TS_2 | TIM_SMCR_TS_0, // Filtered Timer Input 1
    TI1FP2 = TIM_SMCR_TS_2 | TIM_SMCR_TS_1, // Filtered Timer Input 2
    ETRF = TIM_SMCR_TS_2 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0 // External Trigger Input
};

// general purpose timers
static const Info1 TIM2_INFO{TIM2, {&RCC->APB1ENR, RCC_APB1ENR_TIM2EN}, TIM2_IRQn}; // 32 bit
#ifdef TIM3
static const Info1 TIM3_INFO{TIM3, {&RCC->APB1ENR, RCC_APB1ENR_TIM3EN}, TIM3_IRQn};
#endif
#ifdef TIM4
static const Info1 TIM4_INFO{TIM4, {&RCC->APB1ENR, RCC_APB1ENR_TIM4EN}, TIM4_IRQn};
#endif
#ifdef TIM5
static const Info1 TIM5_INFO{TIM5, {&RCC->APB1ENR, RCC_APB1ENR_TIM5EN}, TIM5_IRQn}; // 32 bit
#endif
static const Info1 TIM6_INFO{TIM6, {&RCC->APB1ENR, RCC_APB1ENR_TIM6EN}, TIM6_DAC1_IRQn};
#ifdef TIM7
static const Info1 TIM7_INFO{TIM7, {&RCC->APB1ENR, RCC_APB1ENR_TIM7EN}, TIM7_DAC2_IRQn};
#endif
static const Info1 TIM15_INFO{TIM15, {&RCC->APB2ENR, RCC_APB2ENR_TIM15EN}, TIM1_BRK_TIM15_IRQn};
static const Info1 TIM16_INFO{TIM16, {&RCC->APB2ENR, RCC_APB2ENR_TIM16EN}, TIM1_UP_TIM16_IRQn};
static const Info1 TIM17_INFO{TIM17, {&RCC->APB2ENR, RCC_APB2ENR_TIM17EN}, TIM1_TRG_COM_TIM17_IRQn};

// advanced control timers
static const Info2 TIM1_INFO{TIM1, {&RCC->APB2ENR, RCC_APB2ENR_TIM1EN}, TIM1_BRK_TIM15_IRQn, TIM1_UP_TIM16_IRQn, TIM1_TRG_COM_TIM17_IRQn, TIM1_CC_IRQn};
#ifdef TIM8
static const Info2 TIM8_INFO{TIM8, {&RCC->APB2ENR, RCC_APB2ENR_TIM8EN}, TIM8_BRK_IRQn, TIM8_UP_IRQn, TIM8_TRG_COM_IRQn, TIM8_CC_IRQn};
#endif

} // namespace timer
} // namespace coco
