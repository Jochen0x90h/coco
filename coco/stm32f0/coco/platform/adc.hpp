#pragma once

#include "adcF0C0G0F3L4G4H5U3U5.hpp"
#include <coco/bits.hpp>


namespace coco {

/// @brief ADC helpers
/// Refernece manual: https://www.st.com/resource/en/reference_manual/dm00031936-stm32f0x1stm32f0x2stm32f0x8-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
///   ADC: Section 13
namespace adc {

/// @brief ADC clock configuration
///
enum class ClockConfig : uint32_t {
    // ADC clock source is AHB clock
    AHB_DIV_1 = 1u << ADC_CFGR2_CKMODE_Pos, // AHB / 1
    AHB_DIV_2 = 2u << ADC_CFGR2_CKMODE_Pos, // AHB / 2
    AHB_DIV_4 = 3u << ADC_CFGR2_CKMODE_Pos, // AHB / 4 (e.g. 160Mhz / 4 = 40MHz)

    // ADC clock source is SYSCLK or PLLP (configured in RCC e.g. during SystemInit())
    RCC_DIV_1 = 0,
};

/// @brief ADC configuration
///
enum class Config : uint32_t {
    // ADC resolution
    RES_12 = 0 << ADC_CFGR1_RES_Pos,
    RES_10 = 1 << ADC_CFGR1_RES_Pos,
    RES_8 = 2 << ADC_CFGR1_RES_Pos,
    RES_6 = 3 << ADC_CFGR1_RES_Pos,
    RES_MASK = ADC_CFGR1_RES_Msk,

    // alignment
    ALIGN_RIGHT = 0, // default
    ALIGN_LEFT = ADC_CFGR1_ALIGN,
    ALIGN_MASK = ADC_CFGR1_ALIGN_Msk,

    // overrun mode
    OVERRUN_KEEP = 0, // default
    OVERRUN_REPLACE = ADC_CFGR1_OVRMOD
};

/// @brief DMA mode
///
enum class DmaMode : uint32_t {
    DISABLED = 0,
    ONE_SHOT = ADC_CFGR1_DMAEN,
    CIRCULAR = ADC_CFGR1_DMAEN | ADC_CFGR1_DMACFG,
};

/// @brief ADC trigger
///
enum class Trigger : uint32_t {
    // ADC triggered by software (set ADSTART bit)
    SOFTWARE = 0,

    // ADC1 triggered by...
    ADC1_TIM1_TRGO = ADC_CFGR1_EXTEN_0 | (0 << ADC_CFGR1_EXTSEL_Pos),
    ADC1_TIM1_CC4 = ADC_CFGR1_EXTEN_0 | (1 << ADC_CFGR1_EXTSEL_Pos),
#ifdef TIM2
    ADC1_TIM2_TRGO = ADC_CFGR1_EXTEN_0 | (2 << ADC_CFGR1_EXTSEL_Pos),
#endif
#ifdef TIM3
    ADC1_TIM3_TRGO = ADC_CFGR1_EXTEN_0 | (3 << ADC_CFGR1_EXTSEL_Pos),
#endif
#ifdef TIM15
    ADC1_TIM15_TRGO = ADC_CFGR1_EXTEN_0 | (4 << ADC_CFGR1_EXTSEL_Pos),
#endif
};

/// @brief ADC input, combination of channel index and sampling cycles.
/// Note: See setSequence() for hardware limitations
enum class Input : uint16_t {
    CHANNEL_0 = 0,
    CHANNEL_1 = 1,
    CHANNEL_2 = 2,
    CHANNEL_3 = 3,
    CHANNEL_4 = 4,
    CHANNEL_5 = 5,
    CHANNEL_6 = 6,
    CHANNEL_7 = 7,
    CHANNEL_8 = 8,
    CHANNEL_9 = 9,
    CHANNEL_10 = 10,
    CHANNEL_11 = 11,
    CHANNEL_12 = 12,
    CHANNEL_13 = 13,
    CHANNEL_14 = 14,
    CHANNEL_15 = 15,
    CHANNEL_MASK = 0x1f,

    // convenience for single ended channels
    CH0 = 0,
    CH1 = 1,
    CH2 = 2,
    CH3 = 3,
    CH4 = 4,
    CH5 = 5,
    CH6 = 6,
    CH7 = 7,
    CH8 = 8,
    CH9 = 9,
    CH10 = 10,
    CH11 = 11,
    CH12 = 12,
    CH13 = 13,
    CH14 = 14,
    CH15 = 15,

    // number of sampling cycles
    CYCLES_1_5 = 0x0000,
    CYCLES_7_5 = 0x0100,
    CYCLES_13_5 = 0x0200,
    CYCLES_28_5 = 0x0300,
    CYCLES_41_5 = 0x0400,
    CYCLES_55_5 = 0x0500,
    CYCLES_71_5 = 0x0600,
    CYCLES_239_5 = 0x0700,
    CYCLES_MASK = 0x0700,
};
COCO_ENUM(Input);


/// @brief Set ADC input.
/// @param adc ADC registers
/// @param input Input and sampling cycles
inline void setInput(ADC_TypeDef *adc, Input input) {
    // select channel
    adc->CHSELR = 1 << int(input & Input::CHANNEL_MASK);

    // sampling cycles
    adc->SMPR = extract(int(input), int(Input::CYCLES_MASK)) << ADC_SMPR_SMP_Pos;
}

/// @brief Set ADC input sequence list.
/// Note: Hardware only supports flags which means the order of the list gets lost. Same sampling cycles for all channels.
/// @param adc ADC registers
/// @param sequence Input sequence and sampling cycles taken from first channel
inline void setSequence(ADC_TypeDef *adc, const Array<const Input> &sequence) {
    // sequence
    int chselr = 0;
    for (auto input : sequence) {
        chselr |= 1 << int(input & Input::CHANNEL_MASK);
    }
    adc->CHSELR = chselr;

    // sampling cycles
    adc->SMPR = extract(int(sequence[0]), int(Input::CYCLES_MASK)) << ADC_SMPR_SMP_Pos;
}

inline Instance &Instance::configure(Config config, Trigger trigger, DmaMode dmaMode) {
    // set configuration, trigger and DMA mode
    uint32_t cfgr = int(config) | int(trigger) | int(dmaMode);
    adc->CFGR1 = cfgr;
    return *this;
}

inline Instance Info::enableClock(ClockConfig clockConfig) const {
    // enable clock
    rcc.enableClock();

    // set clock config
    adc->CFGR2 = int(clockConfig);

    // return Instance
    return {adc};
}

template <dma::Feature F2>
void Info::map(const dma::Info<F2> &dmaInfo) const {
#ifdef SYSCFG_CFGR1_ADC_DMA_RMP
    if (dmaInfo.channelIndex == 1)
        SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_ADC_DMA_RMP;
#endif
}


// single channel infos
static const Info ADC1_INFO{ADC1_COMMON, ADC1, RCC_APB2ENR_ADCEN, ADC1_IRQn};

} // namespace adc
} // namespace coco
