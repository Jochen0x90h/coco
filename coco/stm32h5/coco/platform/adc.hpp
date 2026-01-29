#pragma once

#include "adcG4H5.hpp"


namespace coco {
namespace adc {

/// @brief ADC trigger (CFGR register)
/// H5: Reference manual section 26.4.2
/// H503: Reference manual section 19.4.2
enum class Trigger : uint32_t {
    // ADC triggered by software (set ADSTART bit or call start())
    SOFTWARE = 0,

    // ADC1/ADC2 triggered by...
    ADC12_TIM1_TRGO = ADC_CFGR_EXTEN_0 | (9 << ADC_CFGR_EXTSEL_Pos),
    ADC12_TIM1_TRGO2 = ADC_CFGR_EXTEN_0 | (10 << ADC_CFGR_EXTSEL_Pos),
    ADC12_TIM2_TRGO = ADC_CFGR_EXTEN_0 | (11 << ADC_CFGR_EXTSEL_Pos),
    ADC12_TIM3_TRGO = ADC_CFGR_EXTEN_0 | (4 << ADC_CFGR_EXTSEL_Pos),
#ifdef TIM4
    ADC12_TIM4_TRGO = ADC_CFGR_EXTEN_0 | (12 << ADC_CFGR_EXTSEL_Pos),
#endif
    ADC12_TIM6_TRGO = ADC_CFGR_EXTEN_0 | (13 << ADC_CFGR_EXTSEL_Pos),
#ifdef STM32H503xx_H
    ADC12_TIM7_TRGO = ADC_CFGR_EXTEN_0 | (17 << ADC_CFGR_EXTSEL_Pos),
#endif
#ifdef TIM8
    ADC12_TIM8_TRGO = ADC_CFGR_EXTEN_0 | (7 << ADC_CFGR_EXTSEL_Pos),
    ADC12_TIM8_TRGO2 = ADC_CFGR_EXTEN_0 | (8 << ADC_CFGR_EXTSEL_Pos),
#endif
#ifdef TIM15
    ADC12_TIM15_TRGO = ADC_CFGR_EXTEN_0 | (14 << ADC_CFGR_EXTSEL_Pos),
#endif
    ADC12_LPTIM1_CH1 = ADC_CFGR_EXTEN_0 | (18 << ADC_CFGR_EXTSEL_Pos),
    ADC12_LPTIM2_CH1 = ADC_CFGR_EXTEN_0 | (19 << ADC_CFGR_EXTSEL_Pos),
    ADC12_EXTI11 = ADC_CFGR_EXTEN_0 | (6 << ADC_CFGR_EXTSEL_Pos),
    ADC12_EXTI15 = ADC_CFGR_EXTEN_0 | (16 << ADC_CFGR_EXTSEL_Pos),
};


template <dma::Feature F2>
void Info::map(const dma::Info<F2> &dmaInfo) const {
    dmaInfo.setRxRequest(drq);
}

#ifdef HAVE_ADC_DUAL_MODE
template <dma::Feature F2>
void DualInfo::map(const dma::Info<F2> &dmaInfo, int channelIndex) const {
    dmaInfo.setRxRequest(drq + channelIndex);
}
#endif // HAVE_ADC_DUAL_MODE


// single channel ADC infos
static const Info ADC1_INFO{ADC12_COMMON, ADC1, RCC_AHB2ENR_ADCEN, ADC1_IRQn, 0};
#ifdef ADC2
static const Info ADC2_INFO{ADC12_COMMON, ADC2, RCC_AHB2ENR_ADCEN, ADC2_IRQn, 1};
#endif

// dual channel ADC infos
#ifdef ADC2
static const DualInfo ADC12_INFO{ADC12_COMMON, ADC1, ADC2, RCC_AHB2ENR_ADCEN, ADC1_IRQn, 0};
#endif

} // namespace adc
} // namespace coco
