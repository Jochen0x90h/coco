#pragma once

#include "adcF3F4.hpp"
#include <coco/bits.hpp>


namespace coco {

/// @brief ADC helpers.
/// Refernece manual:
///   Section 13
namespace adc {

/// @brief ADC clock configuration
///
enum class ClockConfig : uint32_t {
    // ADC clock source is APB2 clock
    APB2_DIV_2 = 0 << ADC_CCR_ADCPRE_Pos, // APB2 / 2
    APB2_DIV_4 = 1 << ADC_CCR_ADCPRE_Pos, // APB2 / 4
    APB2_DIV_6 = 2 << ADC_CCR_ADCPRE_Pos, // APB2 / 6
    APB2_DIV_8 = 3 << ADC_CCR_ADCPRE_Pos, // APB2 / 8
};

/// @brief ADC configuration
///
enum class Config : uint32_t {
    // ADC resolution
    RES_12 = 0 << ADC_CR1_RES_Pos,
    RES_10 = 1 << ADC_CR1_RES_Pos,
    RES_8 = 2 << ADC_CR1_RES_Pos,
    RES_6 = 3 << ADC_CR1_RES_Pos,
    RES_MASK = ADC_CR1_RES_Msk,
};

/// @brief ADC Trigger
/// Reference manual section 13.13.3
enum class Trigger : uint32_t {
    // ADC triggered by software (set ADSTART bit or call start())
    SOFTWARE = 0,

    // ADC triggered by...
    ADC_TIM1_CC1 = ADC_CR2_EXTEN_0 | (0 << ADC_CR2_EXTSEL_Pos),
    ADC_TIM1_CC2 = ADC_CR2_EXTEN_0 | (1 << ADC_CR2_EXTSEL_Pos),
    ADC_TIM1_CC3 = ADC_CR2_EXTEN_0 | (2 << ADC_CR2_EXTSEL_Pos),
    ADC_TIM2_CC2 = ADC_CR2_EXTEN_0 | (3 << ADC_CR2_EXTSEL_Pos),
    ADC_TIM2_CC3 = ADC_CR2_EXTEN_0 | (4 << ADC_CR2_EXTSEL_Pos),
    ADC_TIM2_CC4 = ADC_CR2_EXTEN_0 | (5 << ADC_CR2_EXTSEL_Pos),
    ADC_TIM2_TRGO = ADC_CR2_EXTEN_0 | (6 << ADC_CR2_EXTSEL_Pos),
    ADC_TIM3_CC1 = ADC_CR2_EXTEN_0 | (7 << ADC_CR2_EXTSEL_Pos),
    ADC_TIM1_TRGO = ADC_CR2_EXTEN_0 | (8 << ADC_CR2_EXTSEL_Pos),
    ADC_TIM4_CC4 = ADC_CR2_EXTEN_0 | (9 << ADC_CR2_EXTSEL_Pos),
    ADC_TIM5_CC1 = ADC_CR2_EXTEN_0 | (10 << ADC_CR2_EXTSEL_Pos),
    ADC_TIM5_CC2 = ADC_CR2_EXTEN_0 | (11 << ADC_CR2_EXTSEL_Pos),
    ADC_TIM5_CC3 = ADC_CR2_EXTEN_0 | (12 << ADC_CR2_EXTSEL_Pos),
    ADC_TIM8_CC1 = ADC_CR2_EXTEN_0 | (13 << ADC_CR2_EXTSEL_Pos),
    ADC_TIM8_TRGO = ADC_CR2_EXTEN_0 | (14 << ADC_CR2_EXTSEL_Pos),
    ADC_EXTI11 = ADC_CR2_EXTEN_0 | (15 << ADC_CR2_EXTSEL_Pos),
};

/// @brief DMA mode
///
enum class DmaMode : uint32_t {
    DISABLED = 0,
    ONE_SHOT = ADC_CR2_DMA,
    CIRCULAR = ADC_CR2_DMA | ADC_CR2_DDS,
};


template <dma::Feature F2>
void Info::map(const dma::Info<F2> &dmaInfo) const {
    dmaInfo.setRequest(drq);
}

#ifdef HAVE_ADC_DUAL_MODE
template <dma::Feature F2>
void DualInfo::map(const dma::Info<F2> &dmaInfo, int channelIndex) const {
    dmaInfo.setRequest(drq + channelIndex);
}
#endif


// single channel infos
#ifdef ADC1_COMMON
static const Info ADC1_INFO{ADC1_COMMON, ADC1, RCC_APB2ENR_ADC1EN, ADC_IRQn, 0};
#endif
#ifdef ADC123_COMMON
static const Info ADC1_INFO{ADC123_COMMON, ADC1, RCC_APB2ENR_ADC1EN, ADC_IRQn, 0};
static const Info ADC2_INFO{ADC123_COMMON, ADC2, RCC_APB2ENR_ADC2EN, ADC_IRQn, 1};
static const Info ADC3_INFO{ADC123_COMMON, ADC3, RCC_APB2ENR_ADC3EN, ADC_IRQn, 2};
#endif

// dual channel infos
#ifdef ADC123_COMMON
static const DualInfo ADC12_INFO{ADC123_COMMON, ADC1, ADC2, RCC_APB2ENR_ADC1EN | RCC_APB2ENR_ADC2EN, ADC_IRQn, 0};
#endif

} // namespace adc
} // namespace coco
