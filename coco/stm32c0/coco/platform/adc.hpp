#pragma once

#include "platform.hpp"
#include "dma.hpp"
#include <coco/Array.hpp>
#include <coco/bits.hpp>
#include <coco/debug.hpp>


namespace coco {

/**
 * ADC helpers
 * Refernece manual: https://www.st.com/resource/en/reference_manual/rm0490-stm32c0x1-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 * ADC: Section 14
 */
namespace adc {

/**
 * ADC clock configuration
 * Contains fiels from CCR and CFGR2 registers which do not overlap
 */
enum class ClockConfig {
    // ADC clock source is AHB clock
    AHB_DIV1 = 1 << ADC_CFGR2_CKMODE_Pos, // AHB / 1
    AHB_DIV2 = 2 << ADC_CFGR2_CKMODE_Pos, // AHB / 2
    AHB_DIV4 = 3 << ADC_CFGR2_CKMODE_Pos, // AHB / 4 (e.g. 160Mhz / 4 = 40MHz)

    // ADC clock source is SYSCLK or PLLP (configured in RCC e.g. during SystemInit())
    RCC_DIV1 = 0,
    RCC_DIV2 = 1 << ADC_CCR_PRESC_Pos,
    RCC_DIV4 = 2 << ADC_CCR_PRESC_Pos,
    RCC_DIV6 = 3 << ADC_CCR_PRESC_Pos,
    RCC_DIV8 = 4 << ADC_CCR_PRESC_Pos,
    RCC_DIV10 = 5 << ADC_CCR_PRESC_Pos,
    RCC_DIV12 = 6 << ADC_CCR_PRESC_Pos,
    RCC_DIV16 = 7 << ADC_CCR_PRESC_Pos,
    RCC_DIV32 = 8 << ADC_CCR_PRESC_Pos,
    RCC_DIV64 = 9 << ADC_CCR_PRESC_Pos,
    RCC_DIV128 = 10 << ADC_CCR_PRESC_Pos,
    RCC_DIV256 = 11 << ADC_CCR_PRESC_Pos,
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
 * ADC trigger
 * Reference manual section 21.4.18
 */
enum class Trigger {
    // ADC triggered by software (set ADSTART bit or call start())
    SOFTWARE = 0,

    // ADC1 triggered by...
    ADC1_TIM1_TRGO2 = ADC_CFGR1_EXTEN_0 | (0 << ADC_CFGR1_EXTSEL_Pos),
    ADC1_TIM1_CC4 = ADC_CFGR1_EXTEN_0 | (1 << ADC_CFGR1_EXTSEL_Pos),
    ADC1_TIM3_TRGO = ADC_CFGR1_EXTEN_0 | (3 << ADC_CFGR1_EXTSEL_Pos),
    ADC1_EXTI11 = ADC_CFGR1_EXTEN_0 | (7 << ADC_CFGR1_EXTSEL_Pos),
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

    // Vsense
    CHANNEL9 = 9,

    // Vrefint
    CHANNEL10 = 10,

    CHANNEL11 = 11,
    CHANNEL12 = 12,
    CHANNEL13 = 13,
    CHANNEL14 = 14,

    // Vdda
    CHANNEL15 = 15,

    // Vssa
    CHANNEL16 = 16,

    CHANNEL17 = 17,
    CHANNEL18 = 18,
    CHANNEL19 = 19,
    CHANNEL20 = 20,
    CHANNEL21 = 21,
    CHANNEL22 = 22,
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

/// @brief DMA mode
///
enum class DmaMode : uint32_t {
    DISABLED = 0,
    ONE_SHOT = ADC_CFGR1_DMAEN,
    CIRCULAR = ADC_CFGR1_DMAEN | ADC_CFGR1_DMACFG,
};
COCO_ENUM(DmaMode);


// enable voltage regulator
inline void enableVoltageRegulator(ADC_TypeDef *adc) {
    adc->CR = ADC_CR_ADVREGEN;

    // wait tADCVREG_STUP
    debug::sleep(20us);
}

// calibrate single ended
inline void calibrate(ADC_TypeDef *adc)  {
    adc->CR = ADC_CR_ADVREGEN | ADC_CR_ADCAL;
    while ((adc->CR & ADC_CR_ADCAL) != 0) {}
    for (int i = 0; i < 8; ++i)
        __NOP();
}

// enable ADC
inline void enable(ADC_TypeDef *adc) {
    adc->CR = ADC_CR_ADVREGEN | ADC_CR_ADEN;
    while ((adc->ISR & ADC_ISR_ADRDY) == 0);
}

/// Set ADC input
/// @param adc ADC registers
/// @param input input and sampling cycles
inline void setInput(ADC_TypeDef *adc, Input input) {
    // select channel
    adc->CHSELR = int(input & Input::CHANNEL_MASK) | 0xf0;

    // sampling cycles
    adc->SMPR = extract(int(input), int(Input::CYCLES_MASK)) << ADC_SMPR_SMP1_Pos;
}

/// @brief Set ADC input sequence list
/// @param adc ADC registers
/// @param sequence input sequence of up to 8 inputs and up to two different sampling cycles
inline void setSequence(ADC_TypeDef *adc, const Array<const Input> &sequence) {
    int count = sequence.size();

    // initialize CHSELR with end-of-sequence marker
    uint32_t chselr = 0xf0 << (count - 1) * 4;

    // initialize SMPR with first sampling cycles
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

/**
 * Start conversion or wait for trigger if a trigger is enabled
 */
inline void start(ADC_TypeDef *adc) {
    adc->CR = ADC_CR_ADVREGEN | ADC_CR_ADEN | ADC_CR_ADSTART;
}

/**
 * Stop conversion
 */
inline void stop(ADC_TypeDef *adc) {
    adc->CR = ADC_CR_ADVREGEN | ADC_CR_ADEN | ADC_CR_ADSTP;
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
    rcc::Info2<offsetof(RCC_TypeDef, APBENR2)> rcc;

    // interrupt
    uint8_t irq;


    /// @brief Configure ADC for use (enable clock, enable voltage regulator, calibrate, configure and enable ADC).
    /// Only need to set the input or sequence (.setInput() or .setSequence())
    Registers configure(ClockConfig clockConfig, Config config, Trigger trigger,
        DmaMode dmaMode = DmaMode::DISABLED) const
    {
        // enable clock
        this->rcc.enableClock();

        auto common = this->common;
        auto adc = this->adc;

        adc::enableVoltageRegulator(adc);
        adc::calibrate(adc);

        // set clock config and common mode
        common->CCR = int(clockConfig) & ADC_CCR_PRESC;

        // set config (resolution, alignment, trigger)
        adc->CFGR1 = int(config) | int(trigger) | int(dmaMode)
            | ADC_CFGR1_CHSELRMOD; // use CHSELR as sequence
        adc->CFGR2 = int(clockConfig) & ADC_CFGR2_CKMODE;

        // enable ADC
        adc::enable(adc);

        return {adc};
    }

    /**
     * Map a DMA channel to the the ADC
     */
    void map(const dma::Info &dmaInfo) const {
        dmaInfo.setMux(5);
    }
};


// single channel ADC infos
static const Info ADC1_INFO{ADC1_COMMON, ADC1, RCC_APBENR2_ADCEN, ADC1_IRQn};

} // namespace adc
} // namespace coco
