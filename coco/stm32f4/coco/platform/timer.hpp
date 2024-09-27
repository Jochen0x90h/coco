#pragma once

#include "platform.hpp"
#include <coco/platform/rccInfoF4.hpp>
#include <coco/platform/timerInfo.hpp>


namespace coco {
namespace timer {

/**
 * Timer trigger
 * Reference manual section 17.4.3
 */
enum class Trigger {
    // TIM1 triggered by...
    TIM1_TIM5_TRGO = 0, // ITR0
    TIM1_TIM2_TRGO = TIM_SMCR_TS_0, // ITR1
    TIM1_TIM3_TRGO = TIM_SMCR_TS_1, // ITR2
    TIM1_TIM4_TRGO = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // ITR3

    // TIM8 triggered by...
    TIM8_TIM1_TRGO = 0, // ITR0
    TIM8_TIM2_TRGO = TIM_SMCR_TS_0, // ITR1
    TIM8_TIM4_TRGO = TIM_SMCR_TS_1, // ITR2
    TIM8_TIM5_TRGO = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // ITR3

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
static const Info1 TIM4_INFO{TIM4, {&RCC->APB1ENR, RCC_APB1ENR_TIM4EN}, TIM4_IRQn};
static const Info1 TIM5_INFO{TIM5, {&RCC->APB1ENR, RCC_APB1ENR_TIM5EN}, TIM5_IRQn}; // 32 bit
#ifdef TIM6
static const Info1 TIM6_INFO{TIM6, {&RCC->APB1ENR, RCC_APB1ENR_TIM6EN}, TIM6_DAC_IRQn};
#endif
#ifdef TIM7
static const Info1 TIM7_INFO{TIM7, {&RCC->APB1ENR, RCC_APB1ENR_TIM7EN}, TIM7_IRQn};
#endif
static const Info1 TIM9_INFO{TIM9, {&RCC->APB1ENR, RCC_APB2ENR_TIM9EN}, TIM1_BRK_TIM9_IRQn};
#ifdef TIM10
static const Info1 TIM10_INFO{TIM10, {&RCC->APB1ENR, RCC_APB2ENR_TIM10EN}, TIM1_UP_TIM10_IRQn};
#endif
static const Info1 TIM11_INFO{TIM11, {&RCC->APB1ENR, RCC_APB2ENR_TIM11EN}, TIM1_TRG_COM_TIM11_IRQn};
#ifdef TIM12
static const Info1 TIM12_INFO{TIM12, {&RCC->APB1ENR, RCC_APB1ENR_TIM12EN}, TIM8_BRK_TIM12_IRQn};
#endif
#ifdef TIM13
static const Info1 TIM13_INFO{TIM13, {&RCC->APB1ENR, RCC_APB1ENR_TIM13EN}, TIM8_UP_TIM13_IRQn};
#endif
#ifdef TIM14
static const Info1 TIM14_INFO{TIM14, {&RCC->APB1ENR, RCC_APB1ENR_TIM14EN}, TIM8_TRG_COM_TIM14_IRQn};
#endif

// advanced control timers
static const Info2 TIM1_INFO{TIM1, {&RCC->APB2ENR, RCC_APB2ENR_TIM1EN}, TIM1_BRK_TIM9_IRQn, TIM1_UP_TIM10_IRQn, TIM1_TRG_COM_TIM11_IRQn, TIM1_CC_IRQn};
#ifdef TIM8
static const Info2 TIM8_INFO{TIM8, {&RCC->APB2ENR, RCC_APB2ENR_TIM8EN}, TIM8_BRK_TIM12_IRQn, TIM8_UP_TIM13_IRQn, TIM8_TRG_COM_TIM14_IRQn, TIM8_CC_IRQn};
#endif

} // namespace timer
} // namespace coco
