#pragma once

#include "platform.hpp"
#include <coco/platform/rccInfoC0G4.hpp>
#include <coco/platform/timerInfo.hpp>


namespace coco {
namespace timer {

/**
 * Timer trigger
 * Reference manual section 28.3.2
 */
enum class Trigger {
    // TIM1 triggered by...
    TIM1_TIM2_TRGO = TIM_SMCR_TS_0, // tim_itr1
    TIM1_TIM3_TRGO = TIM_SMCR_TS_1, // tim_itr2
    TIM1_TIM4_TRGO = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr3
    TIM1_TIM5_TRGO = TIM_SMCR_TS_3, // tim_itr4
    TIM1_TIM8_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_0, // tim_itr5
    TIM1_TIM15_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_1, // tim_itr6
    TIM1_TIM16_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr7
    TIM1_TIM17_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2, // tim_itr8
    TIM1_TIM20_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_0, // tim_itr9
    TIM1_HRTIM_OUT_SYNC2 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_1, // tim_itr10

    // TIM2 triggered by...
    TIM2_TIM1_TRGO = 0, // tim_itr0
    TIM2_TIM3_TRGO = TIM_SMCR_TS_1, // tim_itr2
    TIM2_TIM4_TRGO = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr3
    TIM2_TIM5_TRGO = TIM_SMCR_TS_3, // tim_itr4
    TIM2_TIM8_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_0, // tim_itr5
    TIM2_TIM15_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_1, // tim_itr6
    TIM2_TIM16_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr7
    TIM2_TIM17_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2, // tim_itr8
    TIM2_TIM20_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_0, // tim_itr9
    TIM2_HRTIM_OUT_SYNC2 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_1, // tim_itr10
    TIM2_USB_SOF_SYNC = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr11

    // TIM3 triggered by...
    TIM3_TIM1_TRGO = 0, // tim_itr0
    TIM3_TIM2_TRGO = TIM_SMCR_TS_0, // tim_itr1
    TIM3_TIM4_TRGO = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr3
    TIM3_TIM5_TRGO = TIM_SMCR_TS_3, // tim_itr4
    TIM3_TIM8_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_0, // tim_itr5
    TIM3_TIM15_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_1, // tim_itr6
    TIM3_TIM16_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr7
    TIM3_TIM17_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2, // tim_itr8
    TIM3_TIM20_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_0, // tim_itr9
    TIM3_HRTIM_OUT_SYNC2 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_1, // tim_itr10

    // TIM4 triggered by...
    TIM4_TIM1_TRGO = 0, // tim_itr0
    TIM4_TIM2_TRGO = TIM_SMCR_TS_0, // tim_itr1
    TIM4_TIM3_TRGO = TIM_SMCR_TS_1, // tim_itr2
    TIM4_TIM5_TRGO = TIM_SMCR_TS_3, // tim_itr4
    TIM4_TIM8_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_0, // tim_itr5
    TIM4_TIM15_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_1, // tim_itr6
    TIM4_TIM16_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr7
    TIM4_TIM17_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2, // tim_itr8
    TIM4_TIM20_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_0, // tim_itr9
    TIM4_HRTIM_OUT_SYNC2 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_1, // tim_itr10

    // TIM5 triggered by...
    TIM5_TIM1_TRGO = 0, // tim_itr0
    TIM5_TIM2_TRGO = TIM_SMCR_TS_0, // tim_itr1
    TIM5_TIM3_TRGO = TIM_SMCR_TS_1, // tim_itr2
    TIM5_TIM4_TRGO = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr3
    TIM5_TIM8_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_0, // tim_itr5
    TIM5_TIM15_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_1, // tim_itr6
    TIM5_TIM16_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr7
    TIM5_TIM17_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2, // tim_itr8
    TIM5_TIM20_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_0, // tim_itr9
    TIM5_HRTIM_OUT_SYNC2 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_1, // tim_itr10

    // TIM8 triggered by...
    TIM8_TIM1_TRGO = 0, // tim_itr0
    TIM8_TIM2_TRGO = TIM_SMCR_TS_0, // tim_itr1
    TIM8_TIM3_TRGO = TIM_SMCR_TS_1, // tim_itr2
    TIM8_TIM4_TRGO = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr3
    TIM8_TIM5_TRGO = TIM_SMCR_TS_3, // tim_itr4
    TIM8_TIM15_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_1, // tim_itr6
    TIM8_TIM16_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr7
    TIM8_TIM17_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2, // tim_itr8
    TIM8_TIM20_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_0, // tim_itr9
    TIM8_HRTIM_OUT_SYNC2 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_1, // tim_itr10

    // TIM20 triggered by...
    TIM20_TIM1_TRGO = 0, // tim_itr0
    TIM20_TIM2_TRGO = TIM_SMCR_TS_0, // tim_itr1
    TIM20_TIM3_TRGO = TIM_SMCR_TS_1, // tim_itr2
    TIM20_TIM4_TRGO = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr3
    TIM20_TIM5_TRGO = TIM_SMCR_TS_3, // tim_itr4
    TIM20_TIM8_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_0, // tim_itr5
    TIM20_TIM15_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_1, // tim_itr6
    TIM20_TIM16_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr7
    TIM20_TIM17_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2, // tim_itr8
    TIM20_HRTIM_OUT_SYNC2 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_1, // tim_itr10

    // TIM1, TIM2, TIM3, TIM4, TIM5, TIM8, TIM20 triggered by...
    TI1F_ED = TIM_SMCR_TS_2, // Timer Input 1 Edge Detector (tim_ti1f_ed)
    TI1FP1 = TIM_SMCR_TS_2 | TIM_SMCR_TS_0, // Filtered Timer Input 1 (tim_ti1fp1)
    TI1FP2 = TIM_SMCR_TS_2 | TIM_SMCR_TS_1, // Filtered Timer Input 2 (tim_ti2fp2)
    ETRF = TIM_SMCR_TS_2 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0 // External Trigger Input (tim_etrf)
};


// general purpose timers
static const Info1 TIM2_INFO{TIM2, {&RCC->APB1ENR1, RCC_APB1ENR1_TIM2EN}, TIM2_IRQn}; // 32 bit
static const Info1 TIM3_INFO{TIM3, {&RCC->APB1ENR1, RCC_APB1ENR1_TIM3EN}, TIM3_IRQn};
static const Info1 TIM4_INFO{TIM4, {&RCC->APB1ENR1, RCC_APB1ENR1_TIM4EN}, TIM4_IRQn};
#ifdef TIM5
static const Info1 TIM5_INFO{TIM5, {&RCC->APB1ENR1, RCC_APB1ENR1_TIM5EN}, TIM5_IRQn}; // 32 bit
#endif
static const Info1 TIM6_INFO{TIM6, {&RCC->APB1ENR1, RCC_APB1ENR1_TIM6EN}, TIM6_DAC_IRQn};
static const Info1 TIM7_INFO{TIM7, {&RCC->APB1ENR1, RCC_APB1ENR1_TIM7EN}, TIM7_DAC_IRQn};
static const Info1 TIM15_INFO{TIM15, {&RCC->APB2ENR, RCC_APB2ENR_TIM15EN}, TIM1_BRK_TIM15_IRQn};
static const Info1 TIM16_INFO{TIM16, {&RCC->APB2ENR, RCC_APB2ENR_TIM16EN}, TIM1_UP_TIM16_IRQn};
static const Info1 TIM17_INFO{TIM17, {&RCC->APB2ENR, RCC_APB2ENR_TIM17EN}, TIM1_TRG_COM_TIM17_IRQn};

// advanced control timers
static const Info2 TIM1_INFO{TIM1, {&RCC->APB2ENR, RCC_APB2ENR_TIM1EN}, TIM1_BRK_TIM15_IRQn, TIM1_UP_TIM16_IRQn, TIM1_TRG_COM_TIM17_IRQn, TIM1_CC_IRQn};
static const Info2 TIM8_INFO{TIM8, {&RCC->APB2ENR, RCC_APB2ENR_TIM8EN}, TIM8_BRK_IRQn, TIM8_UP_IRQn, TIM8_TRG_COM_IRQn, TIM8_CC_IRQn};
#ifdef TIM20
static const Info2 TIM20_INFO{TIM20, {&RCC->APB2ENR, RCC_APB2ENR_TIM20EN}, TIM20_BRK_IRQn, TIM20_UP_IRQn, TIM20_TRG_COM_IRQn, TIM20_CC_IRQn};
#endif

} // namespace timer
} // namespace coco
