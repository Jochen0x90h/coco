#pragma once

#include "dacF0F3F4.hpp"


namespace coco {

/// @brief DAC helpers
/// Documentation: https://www.st.com/en/microcontrollers-microprocessors/stm32f3-series/documentation.html
/// F301x6/8 and F318x8 Reference manual
///   Section 13 (only DAC1)
/// F302xB/C/D/E and F302x6/8 Reference manual
///   Section 16 (only DAC1)
/// F303xB/C/D/E, F303x6/8, F328x8, F358xC, F398xE Reference manual
///   Section 16 (DAC1 and DAC2)
/// F334xx Reference manual
///   Section 14 (only DAC1)
/// F37xxx Reference manual
///   Section 14 (DAC1 and DAC2)
namespace dac {

#ifdef SYSCFG_CFGR1_TIM18DAC2Ch1_DMA_RMP
#define SYSCFG_CFGR1_DAC2Ch1_DMA_RMP SYSCFG_CFGR1_TIM18DAC2Ch1_DMA_RMP
#endif

// remap trigger 1 in SYSCFG->CFGR1
constexpr int DAC1_REMAP_TRIGGER1 = 1 << 16;
constexpr int DAC1_REMAP_TRIGGER1_0 = DAC1_REMAP_TRIGGER1;
constexpr int DAC1_REMAP_TRIGGER1_1 = DAC1_REMAP_TRIGGER1 | (1 << 17);

// remap trigger 3 in SYSCFG->CFGR3
constexpr int DAC1_REMAP_TRIGGER3 = 1 << 18;
constexpr int DAC1_REMAP_TRIGGER3_0 = DAC1_REMAP_TRIGGER3;
constexpr int DAC1_REMAP_TRIGGER3_1 = DAC1_REMAP_TRIGGER3 | (1 << 19);

// remap trigger 5 in SYSCFG->CFGR3
constexpr int DAC1_REMAP_TRIGGER5 = 1 << 20;
constexpr int DAC1_REMAP_TRIGGER5_0 = DAC1_REMAP_TRIGGER5;
constexpr int DAC1_REMAP_TRIGGER5_1 = DAC1_REMAP_TRIGGER5 | (1 << 21);

/// @brief DAC trigger
/// Reference manual section X.5.4
/// Uses EN1 flag as indicator that a remap flag in SYSCFG needs to be set (see reference manual and Instance::configure())
enum class Trigger {
    NONE = 0,

    // DAC triggered by software (via SWTRIGR register)
    SOFTWARE = (7 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,

    // DAC1 triggered by...
    DAC1_TIM2_TRGO = (4 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
    DAC1_TIM3_TRGO = (1 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1 | DAC1_REMAP_TRIGGER1_1, // set SYSCFG_CFGR1_DAC1_TRIG1_RMP
    DAC1_TIM4_TRGO = (5 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1 | DAC1_REMAP_TRIGGER5_0, // clear SYSCFG_CFGR3_DAC1_TRG3_RMP and SYSCFG_CFGR3_DAC1_TRG5_RMP
    DAC1_TIM5_TRGO = (3 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
    DAC1_TIM6_TRGO = (0 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
    DAC1_TIM7_TRGO = (2 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
    DAC1_TIM8_TRGO = (1 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1 | DAC1_REMAP_TRIGGER1_0, // clear SYSCFG_CFGR1_DAC1_TRIG1_RMP
    DAC1_TIM15_TRGO = (3 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1 | DAC1_REMAP_TRIGGER3_0, // clear SYSCFG_CFGR3_DAC1_TRG3_RMP and SYSCFG_CFGR3_DAC1_TRG5_RMP
#ifdef HRTIM1
    DAC1_HRTIM1_DACTRG1 = (3 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1 | DAC1_REMAP_TRIGGER3_1, // set SYSCFG_CFGR3_DAC1_TRG3_RMP
    DAC1_HRTIM1_DACTRG2 = (5 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1 | DAC1_REMAP_TRIGGER5_1, // set SYSCFG_CFGR3_DAC1_TRG5_RMP
#endif
    DAC1_EXTI9 = (6 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,

#ifdef DAC2
    // DAC2 triggered by...
    DAC2_TIM2_TRGO = (4 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
    DAC2_TIM3_TRGO = (1 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
    DAC2_TIM6_TRGO = (0 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
    DAC2_TIM7_TRGO = (2 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
#ifndef TIM18
    DAC2_TIM15_TRGO = (3 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
#else
    DAC2_TIM18_TRGO = (3 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
#endif
#ifdef HRTIM1
    DAC1_HRTIM1_DACTRG3 = 5 << (DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
#endif
    DAC2_EXTI9 = 6 << (DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
#endif
};

inline int updateSYSCFG(Trigger trigger) {
    int t = int(trigger);
#ifdef SYSCFG_CFGR1_DAC1_TRIG1_RMP
    if ((t & DAC1_REMAP_TRIGGER1) != 0) {
        uint32_t cfgr1 = SYSCFG->CFGR1 & ~SYSCFG_CFGR1_DAC1_TRIG1_RMP;
        cfgr1 |= (t >> 17) << SYSCFG_CFGR1_DAC1_TRIG1_RMP_Pos;
        SYSCFG->CFGR1 = cfgr1;
    }
#endif
#ifdef HRTIM1
    if ((t & DAC1_REMAP_TRIGGER3) != 0) {
        uint32_t cfgr3 = SYSCFG->CFGR3 & ~SYSCFG_CFGR3_DAC1_TRG3_RMP;
        cfgr3 |= (t >> 19) << SYSCFG_CFGR3_DAC1_TRG3_RMP_Pos;
        SYSCFG->CFGR3 = cfgr3;
    } else if ((t & DAC1_REMAP_TRIGGER5) != 0) {
        uint32_t cfgr3 = SYSCFG->CFGR3 & ~SYSCFG_CFGR3_DAC1_TRG5_RMP;
        cfgr3 |= (t >> 21) << SYSCFG_CFGR3_DAC1_TRG5_RMP_Pos;
        SYSCFG->CFGR3 = cfgr3;
    }
#endif
    return t & (DAC_CR_TSEL1 | DAC_CR_TEN1);
}

template <dma::Feature F2>
void Info::map(const dma::Info<F2> &dmaInfo, int channelIndex) const {
    uint32_t cfgr1 = SYSCFG->CFGR1 & ~(SYSCFG_CFGR1_TIM6DAC1Ch1_DMA_RMP | SYSCFG_CFGR1_TIM7DAC1Ch2_DMA_RMP
#ifdef DAC2
        | SYSCFG_CFGR1_DAC2Ch1_DMA_RMP
#endif
        );
#ifdef DAC2
    if (dac == DAC2) {
        // DAC2
        if (channelIndex == 0)
            cfgr1 |= SYSCFG_CFGR1_DAC2Ch1_DMA_RMP;
    } else
#endif
    {
        // DAC1
        cfgr1 |= (channelIndex == 0 ? SYSCFG_CFGR1_TIM6DAC1Ch1_DMA_RMP : SYSCFG_CFGR1_TIM7DAC1Ch2_DMA_RMP);
    }
    SYSCFG->CFGR1 = cfgr1;
}


static const Info DAC1_INFO{DAC1, RCC_APB1ENR_DAC1EN, TIM6_DAC1_IRQn};
#ifdef DAC2
static const Info DAC2_INFO{DAC2, RCC_APB1ENR_DAC2EN, TIM7_DAC2_IRQn};
#endif

} // namespace dac
} // namespace coco
