#pragma once

#include "dacG0L4G4H5U3U5.hpp"


namespace coco {
namespace dac {

/// @brief DAC trigger
/// Reference manual section 23.4.2
enum class Trigger {
    NONE = 0,

    // DAC triggered by software (via SWTRIGR register)
    SOFTWARE = DAC_CR_TEN1,

    // DAC1 triggered by...
    DAC1_TIM1_TRGO = (1 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
    DAC1_TIM2_TRGO = (2 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
    DAC1_TIM4_TRGO = (3 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
    DAC1_TIM6_TRGO = (5 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
    DAC1_TIM7_TRGO = (6 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
    DAC1_TIM15_TRGO = (8 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
    DAC1_LPTIM1_CH1 = (11 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
    DAC1_LPTIM3_CH1 = (12 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
    DAC1_EXTI9 = (13 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
};


template <dma::Feature F2>
void Info::map(const dma::Info<F2> &dmaInfo, int channelIndex) const {
    dmaInfo.setTxRequest(drq + channelIndex);
}


static const Info DAC1_INFO{DAC1, RCC_AHB2ENR1_DAC1EN, DAC1_IRQn, 2};

} // namespace dac
} // namespace coco
