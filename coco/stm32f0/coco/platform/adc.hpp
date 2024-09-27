#pragma once

#include "platform.hpp"
#include "dma.hpp"
#include <coco/Array.hpp>
#include <coco/bits.hpp>


namespace coco {

/**
 * ADC helpers
 * Refernece manual: https://www.st.com/resource/en/reference_manual/dm00031936-stm32f0x1stm32f0x2stm32f0x8-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 * ADC: Section 13
 */
namespace adc {

/**
 * ADC clock configuration
 */
enum class ClockConfig {
    // ADC clock source is AHB clock
    AHB_DIV1 = 1 << ADC_CFGR2_CKMODE_Pos, // AHB / 1
    AHB_DIV2 = 2 << ADC_CFGR2_CKMODE_Pos, // AHB / 2
    AHB_DIV4 = 3 << ADC_CFGR2_CKMODE_Pos, // AHB / 4 (e.g. 160Mhz / 4 = 40MHz)

    // ADC clock source is SYSCLK or PLLP (configured in RCC e.g. during SystemInit())
    RCC_DIV1 = 0,
};

/**
 * ADC configuration
 */
enum class Config {
    // ADC resolution
    RES12 = 0 << ADC_CFGR1_RES_Pos,
    RES10 = 1 << ADC_CFGR1_RES_Pos,
    RES8 = 2 << ADC_CFGR1_RES_Pos,
    RES6 = 3 << ADC_CFGR1_RES_Pos,
    RES_MASK = ADC_CFGR1_RES_Msk,

    // alignment
    ALIGN_RIGHT = 0, // default
    ALIGN_LEFT = ADC_CFGR1_ALIGN,
    ALIGN_MASK = ADC_CFGR1_ALIGN_Msk,

    // overrun mode
    OVERRUN_KEEP = 0, // default
    OVERRUN_REPLACE = ADC_CFGR1_OVRMOD
};

/**
 * ADC Trigger
 */
enum class Trigger {
    // ADC triggered by software (set ADSTART bit)
    SOFTWARE = 0,

    // ADC1 triggered by...
    ADC1_TIM1_TRGO = ADC_CFGR1_EXTEN_0 | (0 << ADC_CFGR1_EXTSEL_Pos),
    ADC1_TIM1_CC4 = ADC_CFGR1_EXTEN_0 | (1 << ADC_CFGR1_EXTSEL_Pos),
    ADC1_TIM2_TRGO = ADC_CFGR1_EXTEN_0 | (2 << ADC_CFGR1_EXTSEL_Pos),
    ADC1_TIM3_TRGO = ADC_CFGR1_EXTEN_0 | (3 << ADC_CFGR1_EXTSEL_Pos),
    ADC1_TIM15_TRGO = ADC_CFGR1_EXTEN_0 | (4 << ADC_CFGR1_EXTSEL_Pos),
};

/**
 * ADC input, combination of channel index and sampling cycles
 * Note: Order of channels in a sequence is ignored. The sampling cycles of the first channel are used for all channels
 */
enum class Input : uint16_t {
    CHANNEL0 = 0,
    CHANNEL1 = 1,
    CHANNEL2 = 2,
    CHANNEL3 = 3,
    CHANNEL4 = 4,
    CHANNEL5 = 5,
    CHANNEL6 = 6,
    CHANNEL7 = 7,
    CHANNEL8 = 8,
    CHANNEL9 = 9,
    CHANNEL10 = 10,
    CHANNEL11 = 11,
    CHANNEL12 = 12,
    CHANNEL13 = 13,
    CHANNEL14 = 14,
    CHANNEL15 = 15,
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

/**
 * Internal configuration which is typically not exposed to the user of a "driver"
 */
enum class InternalConfig {
    NONE = 0,
    ENABLE_DMA = ADC_CFGR1_DMAEN,
    CIRCULAR_DMA = ADC_CFGR1_DMACFG,
};
COCO_ENUM(InternalConfig);

/// @brief DMA mode
///
enum class DmaMode : uint32_t {
    DISABLED = 0,
    ONE_SHOT = ADC_CFGR1_DMAEN,
    CIRCULAR = ADC_CFGR1_DMAEN | ADC_CFGR1_DMACFG,
};
COCO_ENUM(DmaMode);


// calibrate single ended
inline void calibrate(ADC_TypeDef *adc)  {
    adc->CR = ADC_CR_ADCAL;
    while ((adc->CR & ADC_CR_ADCAL) != 0) {}
    for (int i = 0; i < 8; ++i)
        __NOP();
}

// enable ADC
inline void enable(ADC_TypeDef *adc) {
    adc->CR = ADC_CR_ADEN;
    while ((adc->ISR & ADC_ISR_ADRDY) == 0);
}

/// Set ADC input
/// @param adc ADC registers
/// @param input input and sampling cycles
inline void setInput(ADC_TypeDef *adc, Input input) {
    // select channel
    adc->CHSELR = 1 << int(input & Input::CHANNEL_MASK);

    // sampling cycles
    adc->SMPR = extract(int(input), int(Input::CYCLES_MASK)) << ADC_SMPR_SMP_Pos;
}

/// @brief Set ADC input sequence list (hardware only supports flags which means the order of the list gets lost)
/// @param adc ADC registers
/// @param sequence input sequence and sampling cycles for each input
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

/**
 * Start conversion or wait for trigger if a trigger is enabled
 */
inline void start(ADC_TypeDef *adc) {
    adc->CR = ADC_CR_ADEN | ADC_CR_ADSTART;
}

/**
 * Stop conversion
 */
inline void stop(ADC_TypeDef *adc) {
    adc->CR = ADC_CR_ADEN | ADC_CR_ADSTP;
}

/**
 * Wait for end of conversion
 */
inline void wait(ADC_TypeDef *adc) {
    while ((adc->ISR & ADC_ISR_EOC) == 0);
}


/// @brief Wrapper for ADC registers
///
struct Registers {
    ADC_TypeDef *adc;


    //Registers(ADC_TypeDef *adc) : adc(adc) {}
    //Registers &operator =(ADC_TypeDef *adc) {this->adc = adc; return *this;}
    ADC_TypeDef *operator ->() {return this->adc;}

    /// @brief Set input of ADC
    /// @param input to sample from
    /// @return this
    auto &setInput(Input input) {
        adc::setInput(this->adc, input);
        return *this;
    }

    /// @brief Set input sequence list
    /// @param sequence input sequence to sample from
    /// @return this
    auto &setSequence(const Array<const Input> &sequence) {
        adc::setSequence(this->adc, sequence);
        return *this;
    }

    /// @brief Start conversion or wait for trigger if a trigger is enabled.
    /// @return this
    auto &start() {
        adc::start(this->adc);
        return *this;
    }

    /// @brief Stop conversion.
    /// @return this
    auto &stop() {
        adc::stop(this->adc);
        return *this;
    }

    /// @brief Wait for end of conversion.
    /// @return this
    auto &wait() {
        adc::wait(this->adc);
        return *this;
    }

    /// @brief Get conversion result
    /// @return
    uint32_t data() {return this->adc->DR;}
};


/// @brief Info for an ADC in independent mode
///
struct Info {
    // registers
    ADC_Common_TypeDef *common;
    ADC_TypeDef *adc;

    // reset and clock control
    rcc::Info2<offsetof(RCC_TypeDef, APB2ENR)> rcc;

    // interrupt
    uint8_t irq;


    /// @brief Configure ADC for use (enable clock, enable voltage regulator, calibrate, configure and enable ADC).
    /// Only need to set the input or sequence (.setInput() or .setSequence())
    Registers configure(ClockConfig clockConfig, Config config, Trigger trigger,
        DmaMode dmaMode = DmaMode::DISABLED) const
    {
        // enable clock
        this->rcc.enableClock();

        //auto common = this->common;
        auto adc = this->adc;

        adc::calibrate(adc);

        // set clock config
        adc->CFGR2 = int(clockConfig);

        // set config (resolution, alignment, trigger)
        adc->CFGR1 = int(config) | int(trigger) | int(dmaMode);

        // enable ADC
        adc::enable(adc);

        return {adc};
    }

    /**
     * Map a DMA channel to the the DAC
     * Check reference manual which mappings are possible
     */
    void map(const dma::Info &dmaInfo) const {
        if (dmaInfo.channelIndex == 1)
            SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_ADC_DMA_RMP;
    }
};


// single channel infos
static const Info ADC1_INFO{ADC1_COMMON, ADC1, RCC_APB2ENR_ADCEN, ADC1_IRQn};

} // namespace adc
} // namespace coco
