#pragma once

#include "dacF0F3F4.hpp"


#ifdef HAVE_DAC

namespace coco {

/// @brief DAC helpers
///
namespace dac {

/// @brief DAC trigger
/// Reference manual section 14.3.6
enum class Trigger {
    NONE = 0,

    // DAC triggered by software (via SWTRIGR register)
    SOFTWARE = (7 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,

    // DAC1 triggered by...
    DAC1_TIM2_TRGO = (4 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
    DAC1_TIM4_TRGO = (5 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
    DAC1_TIM5_TRGO = (3 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
    DAC1_TIM6_TRGO = (0) | DAC_CR_TEN1,
    DAC1_TIM7_TRGO = (2 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
    DAC1_EXTI9 = (6 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
};

inline int updateSYSCFG(Trigger trigger) {
    return int(trigger);
}


template <dma::Feature F2>
void Info::map(const dma::Info<F2> &dmaInfo, int channelIndex) const {
    dmaInfo.setRequest(7);
}


static const Info DAC1_INFO{DAC1, RCC_APB1ENR_DACEN, TIM6_DAC_IRQn, 7};

} // namespace dac
} // namespace coco

#endif // HAVE_DAC
