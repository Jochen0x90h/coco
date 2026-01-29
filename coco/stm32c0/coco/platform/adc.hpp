#pragma once

#include "adcF0C0G0F3L4G4H5U3U5.hpp"
#include <coco/bits.hpp>


namespace coco {

/// @brief ADC helpers
/// Refernece manual: https://www.st.com/resource/en/reference_manual/rm0490-stm32c0x1-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
///   ADC: Section 14
namespace adc {

/// @brief ADC clock configuration
/// Contains fields from CCR and CFGR2 registers which do not overlap
enum class ClockConfig : uint32_t {
    // ADC clock source is AHB clock
    AHB_DIV_1 = 1u << ADC_CFGR2_CKMODE_Pos, // AHB / 1
    AHB_DIV_2 = 2u << ADC_CFGR2_CKMODE_Pos, // AHB / 2
    AHB_DIV_4 = 3u << ADC_CFGR2_CKMODE_Pos, // AHB / 4 (e.g. 160Mhz / 4 = 40MHz)

    // ADC clock source is SYSCLK or PLLP (configured in RCC e.g. during SystemInit())
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
/// Reference manual section 14.4.1, Table 53
enum class Trigger : uint32_t {
    // ADC triggered by software (set ADSTART bit or call start())
    SOFTWARE = 0,

    // ADC1 triggered by...
    ADC1_TIM1_TRGO2 = ADC_CFGR1_EXTEN_0 | (0 << ADC_CFGR1_EXTSEL_Pos),
    ADC1_TIM1_CC4 = ADC_CFGR1_EXTEN_0 | (1 << ADC_CFGR1_EXTSEL_Pos),
    ADC1_TIM3_TRGO = ADC_CFGR1_EXTEN_0 | (3 << ADC_CFGR1_EXTSEL_Pos),
    ADC1_EXTI11 = ADC_CFGR1_EXTEN_0 | (7 << ADC_CFGR1_EXTSEL_Pos),
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

    // Vsense
    CHANNEL_9 = 9,

    // Vrefint
    CHANNEL_10 = 10,

    CHANNEL_11 = 11,
    CHANNEL_12 = 12,
    CHANNEL_13 = 13,
    CHANNEL_14 = 14,

    // Vdda
    CHANNEL_15 = 15,

    // Vssa
    CHANNEL_16 = 16,

    CHANNEL_17 = 17,
    CHANNEL_18 = 18,
    CHANNEL_19 = 19,
    CHANNEL_20 = 20,
    CHANNEL_21 = 21,
    CHANNEL_22 = 22,
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
    CH19 = 19,
    CH20 = 20,
    CH21 = 21,
    CH22 = 22,

    // number of sampling cycles
    CYCLES_1_5 = 0x000,
    CYCLES_3_5 = 0x100,
    CYCLES_7_5 = 0x200,
    CYCLES_12_5 = 0x300,
    CYCLES_19_5 = 0x400,
    CYCLES_39_5 = 0x500,
    CYCLES_79_5 = 0x600,
    CYCLES_160_5 = 0x700,
    CYCLES_MASK = 0x700,
};
COCO_ENUM(Input);


/// @brief Set ADC input
/// @param adc ADC registers
/// @param input Input and sampling cycles
inline void setInput(ADC_TypeDef *adc, Input input) {
    // select channel
    adc->CHSELR = int(input & Input::CHANNEL_MASK) | 0xf0;

    // sampling cycles
    adc->SMPR = extract(int(input), int(Input::CYCLES_MASK)) << ADC_SMPR_SMP1_Pos;
}

/// @brief Set ADC input sequence list.
/// Note: Only two different sampling cycles are supportd.
/// @param adc ADC registers
/// @param sequence Input sequence of up to 8 inputs and up to two different sampling cycles
inline void setSequence(ADC_TypeDef *adc, const Array<const Input> &sequence) {
    int count = sequence.size();

    // initialize CHSELR with end-of-sequence marker
    uint32_t chselr = 0xf0 << (count - 1) * 4;

    // initialize SMPR with sampling cycles from first channel
    int smp1 = extract(int(sequence[0]), int(Input::CYCLES_MASK));
    int smp2 = 0;
    uint32_t smpr = smp1 << ADC_SMPR_SMP1_Pos;

    // iterate over channels
    for (int i = 0; i < count; ++i) {
        auto input = sequence[i];
        int channel = int(input & Input::CHANNEL_MASK);
        chselr |= channel << i * 4;

        // check for second sampling time
        int smp = extract(int(input), int(Input::CYCLES_MASK));
        if (smp != smp1) {
            smpr |= ADC_SMPR_SMPSEL0 << channel;
            smp2 = smp;
        }
    }

    // set registers
    adc->CHSELR = chselr;
    adc->SMPR = smpr | (smp2 << ADC_SMPR_SMP2_Pos);

/*
    // sequence
    int chsel = 0;
    for (auto s : sequence) {
        chsel |= 1 << int(s & Input::CHANNEL_MASK);
    }
    adc->CHSELR = chsel;

    // sampling cycles
    // todo: use ADC_SMPR_SMPSEL
    adc->SMPR = extract(int(sequence[0]), int(Input::CYCLES_MASK)) << ADC_SMPR_SMP1_Pos;
*/
}

inline Instance &Instance::configure(Config config, Trigger trigger, DmaMode dmaMode) {
    // set configuration, trigger and DMA mode
    uint32_t cfgr = int(config) | int(trigger) | int(dmaMode);
    cfgr |= ADC_CFGR1_CHSELRMOD; // use CHSELR as sequence
    adc->CFGR1 = cfgr;
    return *this;
}

inline Instance Info::enableClock(ClockConfig clockConfig) const {
    // enable clock
    rcc.enableClock();

    // enable voltage regulator
    adc::enableVoltageRegulator(adc);

    // set clock config
    common->CCR = int(clockConfig) & ADC_CCR_PRESC;
    adc->CFGR2 = int(clockConfig) & ADC_CFGR2_CKMODE;

    // return Instance
    return {adc};
}

template <dma::Feature F2>
void Info::map(const dma::Info<F2> &dmaInfo) const {
    dmaInfo.setRequest(5);
}


// single channel ADC infos
static const Info ADC1_INFO{ADC1_COMMON, ADC1, RCC_APBENR2_ADCEN, ADC1_IRQn, 5};

} // namespace adc
} // namespace coco
