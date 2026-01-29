#pragma once

#include "adcF0C0G0F3L4G4H5U3U5.hpp"
#include <coco/bits.hpp>


namespace coco {
namespace adc {

/// @brief ADC clock configuration (CCR register)
///
enum class ClockConfig : uint32_t {
    // ADC clock source is AHB clock
    //AHB_DIV1 = 1 << ADC_CCR_CKMODE_Pos, // AHB / 1
    //AHB_DIV2 = 2 << ADC_CCR_CKMODE_Pos, // AHB / 2
    //AHB_DIV4 = 3 << ADC_CCR_CKMODE_Pos, // AHB / 4 (e.g. 160Mhz / 4 = 40MHz)

    // ADC clock source SYSCLK or PLLP (configured in RCC e.g. during SystemInit())
    RCC_DIV_1 = 0,
    RCC_DIV_2 = 1 << ADC_CCR_PRESC_Pos,
    RCC_DIV_4 = 2 << ADC_CCR_PRESC_Pos,
    RCC_DIV_6 = 3 << ADC_CCR_PRESC_Pos,
    RCC_DIV_8 = 4 << ADC_CCR_PRESC_Pos,
    RCC_DIV_10 = 5 << ADC_CCR_PRESC_Pos,
    RCC_DIV_12 = 6 << ADC_CCR_PRESC_Pos,
    RCC_DIV_16 = 7 << ADC_CCR_PRESC_Pos,
    RCC_DIV_32 = 8 << ADC_CCR_PRESC_Pos,
    RCC_DIV_64 = 9 << ADC_CCR_PRESC_Pos,
    RCC_DIV_128 = 10 << ADC_CCR_PRESC_Pos,
    RCC_DIV_256 = 11 << ADC_CCR_PRESC_Pos,
};

/// @brief ADC configuration (CFGR register)
///
enum class Config : uint32_t {
    // ADC resolution
    RES_12 = 0 << ADC_CFGR1_RES_Pos, // default
    RES_10 = 1 << ADC_CFGR1_RES_Pos,
    RES_8 = 2 << ADC_CFGR1_RES_Pos,
    RES_6 = 3 << ADC_CFGR1_RES_Pos,
    RES_MASK = ADC_CFGR1_RES_Msk,

    // alignment
    //ALIGN_RIGHT = 0, // default
    //ALIGN_LEFT = ADC_CFGR_ALIGN,
    //ALIGN_MASK = ADC_CFGR_ALIGN_Msk,

    // overrun mode
    OVERRUN_KEEP = 0, // default
    OVERRUN_REPLACE = ADC_CFGR1_OVRMOD,

    // discontinuous mode
    DISC_1 = ADC_CFGR1_DISCEN,
    DISC_2 = ADC_CFGR1_DISCEN | (1 << ADC_CFGR1_DISCNUM_Pos),
    DISC_3 = ADC_CFGR1_DISCEN | (2 << ADC_CFGR1_DISCNUM_Pos),
    DISC_4 = ADC_CFGR1_DISCEN | (3 << ADC_CFGR1_DISCNUM_Pos),
    DISC_5 = ADC_CFGR1_DISCEN | (4 << ADC_CFGR1_DISCNUM_Pos),
    DISC_6 = ADC_CFGR1_DISCEN | (5 << ADC_CFGR1_DISCNUM_Pos),
    DISC_7 = ADC_CFGR1_DISCEN | (6 << ADC_CFGR1_DISCNUM_Pos),
    DISC_8 = ADC_CFGR1_DISCEN | (7 << ADC_CFGR1_DISCNUM_Pos),
};
COCO_ENUM(Config)

/// @brief DMA mode
///
enum class DmaMode : uint32_t {
    DISABLED = 0,
    ONE_SHOT = 1 << ADC_CFGR1_DMNGT_Pos,
    CIRCULAR = 3 << ADC_CFGR1_DMNGT_Pos,
};
//COCO_ENUM(DmaMode);

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
    CHANNEL_16 = 16,
    CHANNEL_17 = 17,
    CHANNEL_18 = 18,
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
    CH16 = 16,
    CH17 = 17,
    CH18 = 18,

    // number of sampling cycles
    CYCLES_1_5 = 0x0000,
    CYCLES_2_5 = 0x0100,
    CYCLES_6_5 = 0x0200,
    CYCLES_11_5 = 0x0300,
    CYCLES_23_5 = 0x0400,
    CYCLES_46_5 = 0x0500,
    CYCLES_246_5 = 0x0600,
    CYCLES_1499_5 = 0x0700,
    CYCLES_MASK = 0x0700,
};
COCO_ENUM(Input);

/// @brief ADC trigger (CFGR register)
/// Reference manual section 22.4.2
enum class Trigger : uint32_t {
    // ADC triggered by software (set ADSTART bit or call start())
    SOFTWARE = 0,

    // ADC1/ADC2 triggered by...
    ADC12_TIM1_TRGO = ADC_CFGR1_EXTEN_0 | (9 << ADC_CFGR1_EXTSEL_Pos),
    ADC12_TIM1_TRGO2 = ADC_CFGR1_EXTEN_0 | (10 << ADC_CFGR1_EXTSEL_Pos),
    ADC12_TIM2_TRGO = ADC_CFGR1_EXTEN_0 | (11 << ADC_CFGR1_EXTSEL_Pos),
    ADC12_TIM3_TRGO = ADC_CFGR1_EXTEN_0 | (4 << ADC_CFGR1_EXTSEL_Pos),
    ADC12_TIM4_TRGO = ADC_CFGR1_EXTEN_0 | (12 << ADC_CFGR1_EXTSEL_Pos),
    ADC12_TIM6_TRGO = ADC_CFGR1_EXTEN_0 | (13 << ADC_CFGR1_EXTSEL_Pos),
    ADC12_TIM15_TRGO = ADC_CFGR1_EXTEN_0 | (14 << ADC_CFGR1_EXTSEL_Pos),
    ADC12_LPTIM1_CH1 = ADC_CFGR1_EXTEN_0 | (18 << ADC_CFGR1_EXTSEL_Pos),
    ADC12_LPTIM2_CH1 = ADC_CFGR1_EXTEN_0 | (19 << ADC_CFGR1_EXTSEL_Pos),
    ADC12_LPTIM3_CH1 = ADC_CFGR1_EXTEN_0 | (20 << ADC_CFGR1_EXTSEL_Pos),
    ADC12_LPTIM4_CH1 = ADC_CFGR1_EXTEN_0 | (21 << ADC_CFGR1_EXTSEL_Pos),
    ADC12_EXTI11 = ADC_CFGR1_EXTEN_0 | (6 << ADC_CFGR1_EXTSEL_Pos),
    ADC12_EXTI15 = ADC_CFGR1_EXTEN_0 | (16 << ADC_CFGR1_EXTSEL_Pos),
};


/// @brief Set ADC input.
/// @param adc ADC registers
/// @param input Input and sampling cycles
inline void setInput(ADC_TypeDef *adc, Input input) {
    int channel = int(input & Input::CHANNEL_MASK);

    // select channel
    adc->SQR1 = channel << ADC_SQR1_SQ1_Pos;

    // set sampling cycles
    uint32_t smpr[2] = {0, 0};
    smpr[channel / 10] = extract(int(input), int(Input::CYCLES_MASK)) << (channel % 10) * 3;
    adc->SMPR1 = smpr[0];
    adc->SMPR2 = smpr[1];
}

/// @brief Set ADC input sequence list.
/// Note: When using a channel more than once in a sequence, the sampling cycles must be the same or can be omitted.
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

    for (auto input : sequence) {
        if (sqrPos > 32 - sqWidth) {
            *SQRn = sqrValue;
            ++SQRn;
            sqrValue = 0;
            sqrPos = 0;
        }

        // select channel
        int channel = int(input & Input::CHANNEL_MASK);
        sqrValue |= channel << sqrPos;

        // sampling cycles
        smpr[channel / 10] |= extract(int(input), int(Input::CYCLES_MASK)) << (channel % 10) * 3;

        sqrPos += sqWidth;
    }
    *SQRn = sqrValue;
    adc->SMPR1 = smpr[0];
    adc->SMPR2 = smpr[1];
}

inline Instance &Instance::configure(Config config, Trigger trigger, DmaMode dmaMode) {
    // set configuration (e.g. resolution), trigger and DMA mode
    adc->CFGR1 = int(config) | int(trigger) | int(dmaMode);

    return *this;
}

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
    dmaInfo.setRxRequest(drq);
}


inline DualInstance &DualInstance::configure(Config config, Trigger trigger, DmaMode dmaMode) {
    // set configuration (e.g. resolution) and trigger
    uint32_t cfgr = int(config) | int(trigger);
    adc[0]->CFGR1 = cfgr;
    adc[1]->CFGR1 = cfgr;
// TODO
    // set DMA mode in common registers
/*uint32_t ccr = common->CCR & ~(ADC_CCR_MDMA | ADC_CCR_DMACFG);
    if (dmaMode != DmaMode::DISABLED) {
        int resolution = extract(config, Config::RES_MASK);
        ccr |= ((2 + (int(resolution) >> 1)) << ADC_CCR_MDMA_Pos) // MDMA mode
            | (extract(uint32_t(dmaMode), ADC_CFGR_DMACFG) << ADC_CCR_DMACFG_Pos); // circular mode flag
    }
    common->CCR = ccr;*/
    return *this;
}


inline DualInstance DualInfo::enableClock(ClockConfig clockConfig) const {
    // enable clock
    rcc.enableClock();

    // enable voltage regulator
    adc::enableVoltageRegulator(adc[0], adc[1]);

    // set clock config
    common->CCR = int(clockConfig) | ADC_CCR_DUAL_2 | ADC_CCR_DUAL_1; // regular simultaneous mode

    // return DualInstance
    return {common, {adc[0], adc[1]}};
}

template <dma::Feature F2>
void DualInfo::map(const dma::Info<F2> &dmaInfo, int channelIndex) const {
    dmaInfo.setRequest(drq + channelIndex);
}


// single channel ADC infos
static const Info ADC1_INFO{ADC12_COMMON, ADC1, RCC_AHB2ENR1_ADC12EN, ADC1_IRQn, 0};
static const Info ADC2_INFO{ADC12_COMMON, ADC2, RCC_AHB2ENR1_ADC12EN, ADC2_IRQn, 1};

// dual channel ADC infos
static const DualInfo ADC12_INFO{ADC12_COMMON, ADC1, ADC2, RCC_AHB2ENR1_ADC12EN, ADC1_IRQn, 0};

} // namespace adc
} // namespace coco
