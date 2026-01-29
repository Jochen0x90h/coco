#pragma once

#if defined(__STM32F334x8_H) || defined(__STM32F398xx_H)
#include "adcF0C0G0F3L4G4H5U3U5.hpp"
#else
#include "adcF3F4.hpp"
#endif
#include <coco/bits.hpp>


namespace coco {

/// @brief ADC helpers
/// F3334: Refernece manual: https://www.st.com/resource/en/reference_manual/rm0364-stm32f334xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
///   ADC: Section 13
/// F37 Refernece manual: https://www.st.com/resource/en/reference_manual/rm0313-stm32f37xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
///   ADC: Section 12
namespace adc {

#if defined(__STM32F334x8_H) || defined(__STM32F398xx_H)

/// @brief ADC clock configuration
///
enum class ClockConfig : uint32_t  {
    // ADC clock source is AHB clock
    AHB_DIV_1 = 1 << ADC_CCR_CKMODE_Pos, // AHB / 1
    AHB_DIV_2 = 2 << ADC_CCR_CKMODE_Pos, // AHB / 2
    AHB_DIV_4 = 3 << ADC_CCR_CKMODE_Pos, // AHB / 4 (e.g. 160Mhz / 4 = 40MHz)

    // ADC clock source is SYSCLK or PLLP (configured in RCC e.g. during SystemInit())
    RCC_DIV_1 = 0,
};

/// @brief ADC configuration
///
enum class Config : uint32_t {
    // ADC resolution
    RES_12 = 0 << ADC_CFGR_RES_Pos,
    RES_10 = 1 << ADC_CFGR_RES_Pos,
    RES_8 = 2 << ADC_CFGR_RES_Pos,
    RES_6 = 3 << ADC_CFGR_RES_Pos,
    RES_MASK = ADC_CFGR_RES_Msk,

    // alignment
    ALIGN_RIGHT = 0, // default
    ALIGN_LEFT = ADC_CFGR_ALIGN,
    ALIGN_MASK = ADC_CFGR_ALIGN_Msk,

    // overrun mode
    OVERRUN_KEEP = 0, // default
    OVERRUN_REPLACE = ADC_CFGR_OVRMOD
};

/// @brief ADC trigger
///
enum class Trigger : uint32_t  {
    // ADC triggered by software (set ADSTART bit or call start())
    SOFTWARE = 0,

    // ADC1/ADC2 triggered by...
    ADC12_TIM1_TRGO = ADC_CFGR_EXTEN_0 | (9 << ADC_CFGR_EXTSEL_Pos),
    ADC12_TIM2_TRGO = ADC_CFGR_EXTEN_0 | (11 << ADC_CFGR_EXTSEL_Pos),
    ADC12_TIM3_TRGO = ADC_CFGR_EXTEN_0 | (4 << ADC_CFGR_EXTSEL_Pos),
    ADC12_TIM6_TRGO = ADC_CFGR_EXTEN_0 | (13 << ADC_CFGR_EXTSEL_Pos),
    ADC12_TIM15_TRGO = ADC_CFGR_EXTEN_0 | (14 << ADC_CFGR_EXTSEL_Pos),
    ADC12_EXTI11 = ADC_CFGR_EXTEN_0 | (6 << ADC_CFGR_EXTSEL_Pos),
};

/// @brief DMA mode
///
enum class DmaMode : uint32_t {
    DISABLED = 0,
    ONE_SHOT = ADC_CFGR_DMAEN,
    CIRCULAR = ADC_CFGR_DMAEN | ADC_CFGR_DMACFG,
};
//COCO_ENUM(DmaMode);

/// @brief ADC input, combination of channel index and sampling cycles.
/// Note: See setSequence() for hardware limitations
enum class Input : uint16_t {
    // dummy channel
    CHANNEL0 = 0,

    // fast channels
    CHANNEL_1 = 1,
    CHANNEL_2 = 2,
    CHANNEL_3 = 3,
    CHANNEL_4 = 4,
    CHANNEL_5 = 5,

    // slow channels
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
    CHANNEL_16 = 16,
    CHANNEL_17 = 17,
    CHANNEL_18 = 18,
    CHANNEL_MASK = 0x1f,

    // differential mode
    DIFFERENTIAL = 0x80,

    // convenience for single ended channels
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
    CH16 = 16,
    CH17 = 17,
    CH18 = 18,

    // convenience for differential channels
    CH1D = 1 | DIFFERENTIAL,
    CH2D = 2 | DIFFERENTIAL,
    CH3D = 3 | DIFFERENTIAL,
    CH4D = 4 | DIFFERENTIAL,
    CH5D = 5 | DIFFERENTIAL,
    CH6D = 6 | DIFFERENTIAL,
    CH7D = 7 | DIFFERENTIAL,
    CH8D = 8 | DIFFERENTIAL,
    CH9D = 9 | DIFFERENTIAL,
    CH10D = 10 | DIFFERENTIAL,
    CH11D = 11 | DIFFERENTIAL,
    CH12D = 12 | DIFFERENTIAL,
    CH13D = 13 | DIFFERENTIAL,
    CH14D = 14 | DIFFERENTIAL,
    CH15D = 15 | DIFFERENTIAL,

    // number of sampling cycles
    CYCLES_1_5 = 0x0000,
    CYCLES_2_5 = 0x0100,
    CYCLES_4_5 = 0x0200,
    CYCLES_7_5 = 0x0300,
    CYCLES_19_5 = 0x0400,
    CYCLES_61_5 = 0x0500,
    CYCLES_181_5 = 0x0600,
    CYCLES_601_5 = 0x0700,
    CYCLES_MASK = 0x0700,
};
COCO_ENUM(Input);


/// @brief Set ADC input.
/// @param adc ADC registers
/// @param input Input and sampling cycles
inline void setInput(ADC_TypeDef *adc, Input input) {
    int channel = int(input & Input::CHANNEL_MASK);

    // select input
    adc->SQR1 = channel << ADC_SQR1_SQ1_Pos;

    // set sampling cycles
    uint32_t smpr[2] = {0, 0};
    smpr[channel / 10] = extract(int(input), int(Input::CYCLES_MASK)) << (channel % 10) * 3;
    adc->SMPR1 = smpr[0];
    adc->SMPR2 = smpr[1];

    // select single-ended/differential
    uint32_t difsel = 0;
    if ((input & adc::Input::DIFFERENTIAL) != 0)
        difsel |= 1 << channel;
    adc->DIFSEL = difsel;
}

/// @brief Set ADC input sequence list.
/// Note: When using a channel more than once in a sequence, the sampling cycles must be the same or can be omitted
/// @param adc ADC registers
/// @param sequence Input sequence of up to 16 inputs and sampling cycles for each input
inline void setSequence(ADC_TypeDef *adc, const Array<const Input> &sequence) {
    // pointer to sequence register SQR
    auto SQRn = &adc->SQR1;
    uint32_t sqrValue = sequence.size() - 1;
    int sqrPos = ADC_SQR1_SQ1_Pos;
    const int sqWidth = ADC_SQR1_SQ2_Pos - ADC_SQR1_SQ1_Pos;

    // sampling rate registers SMPR
    uint32_t smpr[2] = {0, 0};

    // single-ended/differential regsiter DIFSEL
    uint32_t difsel = 0;

    for (auto s : sequence) {
        if (sqrPos > 32 - sqWidth) {
            *SQRn = sqrValue;
            ++SQRn;
            sqrValue = 0;
            sqrPos = 0;
        }

        // sequence
        int channel = int(s & Input::CHANNEL_MASK);
        sqrValue |= channel << sqrPos;

        // sampling cycles
        smpr[channel / 10] |= extract(int(s), int(Input::CYCLES_MASK)) << (channel % 10) * 3;

        // differential mode
        if ((s & adc::Input::DIFFERENTIAL) != 0)
            difsel |= 1 << channel;

        sqrPos += sqWidth;
    }
    *SQRn = sqrValue;
    adc->SMPR1 = smpr[0];
    adc->SMPR2 = smpr[1];
    adc->DIFSEL = difsel;
}

// Instance

inline Instance &Instance::configure(Config config, Trigger trigger, DmaMode dmaMode) {
    // set configuration, trigger and DMA mode
    uint32_t cfgr = int(config) | int(trigger) | int(dmaMode);
    adc->CFGR = cfgr;
    return *this;
}

// DualInstance

inline DualInstance &DualInstance::configure(Config config, Trigger trigger, DmaMode dmaMode) {
    // set configuration and trigger
    uint32_t cfgr = int(config) | int(trigger);
    adc[0]->CFGR = cfgr;
    adc[1]->CFGR = cfgr;

    // set DMA mode in common registers
    uint32_t ccr = common->CCR & ~(ADC_CCR_MDMA | ADC_CCR_DMACFG);
    if (dmaMode != DmaMode::DISABLED) {
        uint32_t resolution = extract(config, Config::RES_MASK);
        ccr |= ((2 + (resolution >> 1)) << ADC_CCR_MDMA_Pos) // MDMA mode
            | (extract(uint32_t(dmaMode), ADC_CFGR_DMACFG) << ADC_CCR_DMACFG_Pos); // circular mode flag
    }
    common->CCR = ccr;
    return *this;
}

// Info

inline Instance Info::enableClock(ClockConfig clockConfig) const {
    // enable clock
    rcc.enableClock();

    // enable voltage regulator
    adc::enableVoltageRegulator(adc);

    // set clock config
    common->CCR = int(clockConfig);

    // return Instance
    return {adc};
}

template <dma::Feature F2>
void Info::map(const dma::Info<F2> &dmaInfo) const {
#ifdef ADC2
#ifdef SYSCFG_CFGR3_ADC2_DMA_RMP
    if (adc == ADC2 && dmaInfo.channelIndex == 3)
        SYSCFG->CFGR3 = SYSCFG->CFGR3 | SYSCFG_CFGR3_ADC2_DMA_RMP;
#endif
#endif
}

#ifdef HAVE_ADC_DUAL_MODE

// DualInfo

inline DualInstance DualInfo::enableClock(ClockConfig clockConfig) const {
    // enable clock
    rcc.enableClock();

    // enable voltage regulator
    adc::enableVoltageRegulator(adc[0], adc[1]);

    // set clock config
    common->CCR = int(clockConfig) | ADC_CCR_DUAL_2 | ADC_CCR_DUAL_1; // regular simultaneous mode;

    // return DualInstance
    return {common, {adc[0], adc[1]}};
}

template <dma::Feature F2>
void DualInfo::map(const dma::Info<F2> &dmaInfo, int channelIndex) const {
#ifdef ADC2
#ifdef SYSCFG_CFGR3_ADC2_DMA_RMP
    if (channelIndex == 1 && adc[1] == ADC2 && dmaInfo.channelIndex == 3)
        SYSCFG->CFGR3 = SYSCFG->CFGR3 | SYSCFG_CFGR3_ADC2_DMA_RMP;
#endif
#endif
}

#endif // HAVE_ADC_DUAL_MODE

#else // defined(__STM32F334x8_H)

/// @brief ADC clock configuration
///
enum class ClockConfig : uint32_t {
    // ADC clock source is APB2 clock
    APB2 = 0,
};

/// @brief ADC configuration
///
enum class Config : uint32_t {
    // ADC resolution
    RES_12 = 0,
};

/// @brief ADC Trigger
///
enum class Trigger : uint32_t {
    // ADC triggered by software (set ADSTART bit or call start())
    SOFTWARE = 7 << ADC_CR2_EXTSEL_Pos,

    // ADC triggered by...
    ADC_TIM19_TRGO = 0 << ADC_CR2_EXTSEL_Pos,
    ADC_TIM19_CC3 = 1 << ADC_CR2_EXTSEL_Pos,
    ADC_TIM19_CC4 = 2 << ADC_CR2_EXTSEL_Pos,
    ADC_TIM2_CC2 = 3 << ADC_CR2_EXTSEL_Pos,
    ADC_TIM3_TRGO = 4 << ADC_CR2_EXTSEL_Pos,
    ADC_TIM4_CC4 = 5 << ADC_CR2_EXTSEL_Pos,
    ADC_EXTI11 = 6 << ADC_CR2_EXTSEL_Pos,
};

/// @brief DMA mode
///
enum class DmaMode : uint32_t {
    DISABLED = 0,
    ONE_SHOT = ADC_CR2_DMA,
    CIRCULAR = ADC_CR2_DMA // | ADC_CR2_DDS,
};

template <dma::Feature F2>
void Info::map(const dma::Info<F2> &dmaInfo) const {
    // todo
}

#ifdef HAVE_ADC_DUAL_MODE
template <dma::Feature F2>
void DualInfo::map(const dma::Info<F2> &dmaInfo, int channelIndex) const {
    // todo
}
#endif // HAVE_ADC_DUAL_MODE

#endif // defined(__STM32F334x8_H)

// single channel infos
#ifdef RCC_APB2ENR_ADC1EN
static const Info ADC1_INFO{ADC1_COMMON, ADC1, RCC_APB2ENR_ADC1EN, ADC1_IRQn};
#endif
#ifdef RCC_AHBENR_ADC1EN
static const Info ADC1_INFO{ADC1_COMMON, ADC1, RCC_AHBENR_ADC1EN, ADC1_IRQn};
#endif
#ifdef RCC_AHBENR_ADC12EN
static const Info ADC1_INFO{ADC12_COMMON, ADC1, RCC_AHBENR_ADC12EN, ADC1_2_IRQn};
static const Info ADC2_INFO{ADC12_COMMON, ADC2, RCC_AHBENR_ADC12EN, ADC1_2_IRQn};
#endif
#ifdef RCC_AHBENR_ADC34EN
static const Info ADC3_INFO{ADC34_COMMON, ADC3, RCC_AHBENR_ADC34EN, ADC3_IRQn};
#endif
#ifdef RCC_AHBENR_ADC34EN
static const Info ADC4_INFO{ADC34_COMMON, ADC4, RCC_AHBENR_ADC34EN, ADC4_IRQn};
#endif

// dual channel infos
#ifdef RCC_AHBENR_ADC12EN
static const DualInfo ADC12_INFO{ADC12_COMMON, ADC1, ADC2, RCC_AHBENR_ADC12EN, ADC1_2_IRQn};
#endif
#ifdef RCC_AHBENR_ADC34EN
static const DualInfo ADC34_INFO{ADC34_COMMON, ADC3, ADC4, RCC_AHBENR_ADC34EN, ADC3_IRQn};
#endif

} // namespace adc
} // namespace coco
