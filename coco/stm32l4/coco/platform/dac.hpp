#pragma once

#include "dacG0L4G4H5U3U5.hpp"


#ifdef HAVE_DAC

namespace coco {

/// @brief DAC helpers
///   Section 17
///   Section 19
namespace dac {

/// @brief DAC trigger
/// Reference manual section 17.4.6, 19.4.6
enum class Trigger {
    NONE = 0,

    // DAC triggered by software (via SWTRIGR register)
    SOFTWARE = (7 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,

    // DAC1 triggered by...
    DAC1_TIM2_TRGO = (4 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
    DAC1_TIM4_TRGO = (5 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
    DAC1_TIM5_TRGO = (3 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
    DAC1_TIM6_TRGO = (0) | DAC_CR_TEN1,
    DAC1_TIM7_TRGO = (2 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1, // not on STM32L45xxx and STM32L46xxx
    DAC1_EXTI9 = (6 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
};


template <dma::Feature F2>
void Info::map(const dma::Info<F2> &dmaInfo, int channelIndex) const {
    // channels
    dmaInfo.setRequest(dmaInfo.index() == 0 ? 6 - channelIndex : 3);
}


static const Info DAC1_INFO{DAC1, RCC_APB1ENR1_DAC1EN, TIM6_DAC_IRQn};

} // namespace dac
} // namespace coco

#endif // HAVE_DAC
