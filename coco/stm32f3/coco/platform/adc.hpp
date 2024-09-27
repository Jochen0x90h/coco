#pragma once

#include "platform.hpp"
#include "dma.hpp"
#include <coco/Array.hpp>
#include <coco/bits.hpp>
#include <coco/debug.hpp>


// dual mode is supported
#define HAVE_ADC_DUAL_MODE


namespace coco {

/**
 * ADC helpers
 * Refernece manual: https://www.st.com/resource/en/reference_manual/rm0364-stm32f334xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 * ADC: Section 13
 */
namespace adc {

/**
 * ADC clock configuration
 */
enum class ClockConfig {
    // ADC clock source is AHB clock
    AHB_DIV1 = 1 << ADC_CCR_CKMODE_Pos, // AHB / 1
    AHB_DIV2 = 2 << ADC_CCR_CKMODE_Pos, // AHB / 2
    AHB_DIV4 = 3 << ADC_CCR_CKMODE_Pos, // AHB / 4 (e.g. 160Mhz / 4 = 40MHz)

    // ADC clock source is SYSCLK or PLLP (configured in RCC e.g. during SystemInit())
    RCC_DIV1 = 0,
};

/**
 * ADC configuration
 */
enum class Config {
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

/**
 * ADC Trigger
 */
enum class Trigger {
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

/**
 * ADC input, combination of channel index and sampling cycles
 * Note: When using a channel more than once in a sequence, the sampling cycles must be the same or can be omitted
 */
enum class Input : uint16_t {
    // dummy channel
    CHANNEL0 = 0,

    // fast channels
    CHANNEL1 = 1,
    CHANNEL2 = 2,
    CHANNEL3 = 3,
    CHANNEL4 = 4,
    CHANNEL5 = 5,

    // slow channels
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
    CHANNEL16 = 16,
    CHANNEL17 = 17,
    CHANNEL18 = 18,
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

/// @brief DMA mode
///
enum class DmaMode : uint32_t {
    DISABLED = 0,
    ONE_SHOT = ADC_CFGR_DMAEN,
    CIRCULAR = ADC_CFGR_DMAEN | ADC_CFGR_DMACFG,
};
COCO_ENUM(DmaMode);


// enable voltage regulator
inline void enableVoltageRegulator(ADC_TypeDef *adc) {
    adc->CR = ADC_CR_ADVREGEN;

    // wait tADCVREG_STUP
    debug::sleep(10us);
}

// enable dual voltage regulator
inline void enableVoltageRegulator(ADC_TypeDef *adc1, ADC_TypeDef *adc2) {
    adc1->CR = ADC_CR_ADVREGEN;
    adc2->CR = ADC_CR_ADVREGEN;

    // wait tADCVREG_STUP
    debug::sleep(10us);
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

// enable dual ADC
inline void enable(ADC_TypeDef *adc1, ADC_TypeDef *adc2) {
    adc1->CR = ADC_CR_ADVREGEN | ADC_CR_ADEN;
    adc2->CR = ADC_CR_ADVREGEN | ADC_CR_ADEN;
    while ((adc1->ISR & adc2->ISR & ADC_ISR_ADRDY) == 0);
}

/// Set ADC input
/// @param adc ADC registers
/// @param input input and sampling cycles
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

/// @brief Set ADC input sequence list
/// @param adc ADC registers
/// @param sequence input sequence of up to 16 inputs and sampling cycles for each input
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

/// @brief Wait for end of dual conversion
/// @param common common registers
inline void wait(ADC_Common_TypeDef *common) {
    while ((common->CSR & (ADC_CSR_EOC_MST | ADC_CSR_EOC_SLV)) != (ADC_CSR_EOC_MST | ADC_CSR_EOC_SLV));
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


/// @brief Wrapper for ADC regsiters in dual mode
///
struct Registers2 {
    ADC_Common_TypeDef *common; // needed for common data register
    ADC_TypeDef *adc1;
    ADC_TypeDef *adc2;


    /// @brief Get master ADC
    /// @return Registers of master ADC
    Registers master() {return {this->adc1};}

    /// @brief Set input of first ADC
    /// @param input to sample from
    /// @return this
    auto &setInput1(Input input) {
        adc::setInput(this->adc1, input);
        return *this;
    }

    /// @brief Set input of second ADC
    /// @param input to sample from
    /// @return this
    auto &setInput2(Input input) {
        adc::setInput(this->adc2, input);
        return *this;
    }

    /// @brief Set input sequence list of first ADC
    /// @param sequence input sequence to sample from
    /// @return this
    auto &setSequence1(const Array<const Input> &sequence) {
        adc::setSequence(this->adc1, sequence);
        return *this;
    }

    /// @brief Set input sequence list of second ADC
    /// @param sequence input sequence to sample from
    /// @return this
    auto &setSequence2(const Array<const Input> &sequence) {
        adc::setSequence(this->adc2, sequence);
        return *this;
    }

    /// @brief Start dual conversion or wait for trigger if a trigger is enabled.
    /// @return this
    auto &start() {
        adc::start(this->adc1);
        return *this;
    }

    /// @brief Stop dual conversion.
    /// @return this
    auto &stop() {
        adc::stop(this->adc1);
        return *this;
    }

    /// @brief Wait for end of dual conversion.
    /// @return this
    auto &wait() {
        adc::wait(this->common);
        return *this;
    }

    /// @brief Get first conversion result
    /// @return
    uint32_t data1() {return this->adc1->DR;}

    /// @brief Get second conversion result
    /// @return
    uint32_t data2() {return this->adc2->DR;}

    /// @brief Get combined conversion result
    /// @return
    uint32_t data() {return this->common->CDR;}
};


/// @brief Info for an ADC in independent mode
///
struct Info {
    // registers
    ADC_Common_TypeDef *common;
    ADC_TypeDef *adc;

    // reset and clock control
#ifdef RCC_APB2ENR_ADC1EN
    rcc::Info2<offsetof(RCC_TypeDef, APB2ENR)> rcc; // some MCUs have a single ADC on APB2
#else
    rcc::Info2<offsetof(RCC_TypeDef, AHBENR)> rcc;
#endif

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

        // set clock config
        common->CCR = int(clockConfig);

        // set config (resolution, alignment, trigger)
        adc->CFGR = int(config) | int(trigger) | int(dmaMode);

        // enable ADC
        adc::enable(adc);

        return {adc};
    }

    /**
     * Map a DMA channel to the the DAC
     * Check reference manual which mappings are possible
     */
    void map(const dma::Info &dmaInfo) const {
#ifdef ADC2
        if (this->adc == ADC2 && dmaInfo.channelIndex == 3)
            SYSCFG->CFGR3 = SYSCFG->CFGR3 | SYSCFG_CFGR3_ADC2_DMA_RMP;
#endif
    }
};


/// @brief Info for two ADCs in dual mode
///
struct Info2 {
    // registers
    ADC_Common_TypeDef *common;
    ADC_TypeDef *adc1;
    ADC_TypeDef *adc2;

    // reset and clock control
    rcc::Info2<offsetof(RCC_TypeDef, AHBENR)> rcc;

    // interrupt of first channel
    uint8_t irq;


    /// @brief Configure ADC for use (enable clock, enable voltage regulator, calibrate, configure and enable ADC).
    /// Only need to set the input or sequence of both ADCs (.setInput1() and .setInput2() or adc::setSequence2() and adc::setSequence2())
    Registers2 configure(ClockConfig clockConfig, Config config, adc::Trigger trigger,
        DmaMode dmaMode = DmaMode::DISABLED) const
    {
        // enable clock
        this->rcc.enableClock();

        auto common = this->common;
        auto adc1 = this->adc1;
        auto adc2 = this->adc2;

        adc::enableVoltageRegulator(adc1, adc2);
        adc::calibrate(adc1);
        adc::calibrate(adc2);

        // set clock config, common mode and DMA mode
        uint32_t ccr = int(clockConfig)
            | ADC_CCR_DUAL_2 | ADC_CCR_DUAL_1; // regular simultaneous mode
        if (dmaMode != DmaMode::DISABLED) {
            int resolution = extract(config, Config::RES_MASK);
            ccr |= ((2 + (int(resolution) >> 1)) << ADC_CCR_MDMA_Pos) // MDMA mode
                | (extract(uint32_t(dmaMode), ADC_CFGR_DMACFG) << ADC_CCR_DMACFG_Pos); // circular mode flag
        }
        common->CCR = ccr;

        // set config (resolution, alignment, trigger)
        uint32_t cfgr =  int(config) | int(trigger);
        adc1->CFGR = cfgr;
        adc2->CFGR = cfgr;

        // enable ADC
        adc::enable(adc1, adc2);

        return {common, adc1, adc2};
    }

    /**
     * Map a DMA channel to the second channel of the ADC
     * Check reference manual which mappings are possible
     * @param dmaInfo DMA info
     * @param channelIndex channel index 0 or 1
     */
    void map(const dma::Info &dmaInfo, int channelIndex = 0) const {
#ifdef ADC2
        if (channelIndex == 1 && this->adc2 == ADC2 && dmaInfo.channelIndex == 3)
            SYSCFG->CFGR3 = SYSCFG->CFGR3 | SYSCFG_CFGR3_ADC2_DMA_RMP;
#endif
    }
};


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
static const Info2 ADC12_INFO{ADC12_COMMON, ADC1, ADC2, RCC_AHBENR_ADC12EN, ADC1_2_IRQn};
#endif
#ifdef RCC_AHBENR_ADC34EN
static const Info2 ADC34_INFO{ADC34_COMMON, ADC3, ADC4, RCC_AHBENR_ADC34EN, ADC3_IRQn};
#endif

} // namespace adc
} // namespace coco
