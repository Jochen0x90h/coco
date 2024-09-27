#pragma once

#include "platform.hpp"
#include <coco/platform/rccInfoC0G4.hpp>
#include <coco/platform/timerInfo.hpp>


namespace coco {
namespace timer {

/**
 * Timer trigger
 * Reference manual section 15.4.3
 */
enum class Trigger {
    // TIM1 triggered by...
    TIM1_TIM3_TRGO = TIM_SMCR_TS_1, // ITR2
    TIM1_TIM17_OC1 = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // ITR3

    // TIM3 triggered by...
    TIM3_TIM1_TRGO = 0, // ITR0
    TIM3_TIM14_OC1 = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // ITR3

    // TIM1, TIM3 triggered by...
    TI1F_ED = TIM_SMCR_TS_2, // Timer Input 1 Edge Detector (TI1F_ED)
    TI1FP1 = TIM_SMCR_TS_2 | TIM_SMCR_TS_0, // Filtered Timer Input 1 (TI1FP1)
    TI1FP2 = TIM_SMCR_TS_2 | TIM_SMCR_TS_1, // Filtered Timer Input 2 (TI2FP2)
    ETRF = TIM_SMCR_TS_2 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0 // External Trigger Input (ETRF)
};

// general purpose timers
static const Info1 TIM3_INFO{TIM3, {&RCC->APBENR1, RCC_APBENR1_TIM3EN}, TIM3_IRQn};
static const Info1 TIM14_INFO{TIM14, {&RCC->APBENR2, RCC_APBENR2_TIM14EN}, TIM14_IRQn};
static const Info1 TIM16_INFO{TIM16, {&RCC->APBENR2, RCC_APBENR2_TIM16EN}, TIM16_IRQn};
static const Info1 TIM17_INFO{TIM17, {&RCC->APBENR2, RCC_APBENR2_TIM17EN}, TIM17_IRQn};

// advanced control timers
static const Info2 TIM1_INFO{TIM1, {&RCC->APBENR2, RCC_APBENR2_TIM1EN}, TIM1_BRK_UP_TRG_COM_IRQn, TIM1_BRK_UP_TRG_COM_IRQn, TIM1_BRK_UP_TRG_COM_IRQn, TIM1_CC_IRQn};

} // namespace timer
} // namespace coco
