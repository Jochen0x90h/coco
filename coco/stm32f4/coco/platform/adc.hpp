#pragma once

#include "platform.hpp"
#include "dma.hpp"
#include <coco/Array.hpp>
#include <coco/bits.hpp>


// dual mode is supported
#ifdef ADC2
#define HAVE_ADC_DUAL_MODE
#endif


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
    APB_DIV2 = 0 << ADC_CCR_ADCPRE_Pos, // APB2 / 2
    APB_DIV4 = 1 << ADC_CCR_ADCPRE_Pos, // APB2 / 4
    APB_DIV6 = 2 << ADC_CCR_ADCPRE_Pos, // APB2 / 6
    APB_DIV8 = 3 << ADC_CCR_ADCPRE_Pos, // APB2 / 8
};

/**
 * ADC common configuration
 */
enum class CommonConfig {
    // mode
    INDEPENDENT = 0,
    REGULAR_SIMULTANEOUS = ADC_CCR_MULTI_2 | ADC_CCR_MULTI_1,

    // DMA
    MDMA_12_10 = ADC_CCR_DMA_1,
    MDMA_8_6 = ADC_CCR_DMA_0 | ADC_CCR_DMA_1,
    CIRCULAR_DMA = ADC_CCR_DDS,
};

/**
 * ADC configuration
 */
enum class Config {
    // ADC resolution
    RES_12 = 0 << ADC_CR1_RES_Pos,
    RES_10 = 1 << ADC_CR1_RES_Pos,
    RES_8 = 2 << ADC_CR1_RES_Pos,
    RES_6 = 3 << ADC_CR1_RES_Pos,
    RES_MASK = ADC_CR1_RES_Msk,
};

/**
 * ADC Trigger
 * Reference manual section 13.13.3
 */
enum class Trigger {
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

/**
 * ADC input, combination of channel index and sampling cycles
 * Note: When using a channel more than once in a sequence, the sampling cycles must be the same or can be omitted
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

    // number of sampling cycles
    CYCLES_3 = 0x0000,
    CYCLES_15 = 0x0100,
    CYCLES_28 = 0x0200,
    CYCLES_56 = 0x0300,
    CYCLES_84 = 0x0400,
    CYCLES_112 = 0x0500,
    CYCLES_144 = 0x0600,
    CYCLES_480 = 0x0700,
    CYCLES_MASK = 0x0700,
};
COCO_ENUM(Input);

/// @brief DMA mode
///
enum class DmaMode : uint32_t {
    DISABLED = 0,
    ONE_SHOT = ADC_CR2_DMA,
    CIRCULAR = ADC_CR2_DMA | ADC_CR2_DDS,
};
COCO_ENUM(DmaMode);



// enable ADC
inline void enable(ADC_TypeDef *adc) {
    adc->CR2 = ADC_CR2_ADON;
}

// enable dual ADC
inline void enable(ADC_TypeDef *adc1, ADC_TypeDef *adc2) {
    adc1->CR2 = ADC_CR2_ADON;
    adc2->CR2 = ADC_CR2_ADON;
}

/// @brief Set input of ADC
///
inline void setInput(ADC_TypeDef *adc, Input input) {
    int channel = int(input & Input::CHANNEL_MASK);

    // select input
    adc->SQR1 = 0;
    adc->SQR3 = channel;

    // set sampling cycles
    uint32_t smpr[2] = {0, 0};
    smpr[channel / 10] = extract(int(input), int(Input::CYCLES_MASK)) << (channel % 10) * 3;
    adc->SMPR2 = smpr[0]; // note SMPR2 contains channels 0-9
    adc->SMPR1 = smpr[1];
}

/// @brief Set input sequence list
///
inline void setSequence(ADC_TypeDef *adc, const Array<const Input> &sequence) {
    // sequence register SQR
    auto SQRn = &adc->SQR3;
    uint32_t sqrValue = 0;
    int sqrPos = ADC_SQR3_SQ1_Pos;
    const int sqWidth = ADC_SQR3_SQ2_Pos - ADC_SQR3_SQ1_Pos;

    // sampling rate registers SMPR
    uint32_t smpr[2] = {0, 0};

    for (auto input : sequence) {
        if (sqrPos > 32 - sqWidth) {
            *SQRn = sqrValue;
            --SQRn; // reverse order of SQR registers
            sqrValue = 0;
            sqrPos = 0;
        }

        // sequence
        int channel = int(input & Input::CHANNEL_MASK);
        sqrValue |= channel << sqrPos;

        // sampling cycles
        smpr[channel / 10] |= extract(int(input), int(Input::CYCLES_MASK)) << channel % 10;

        sqrPos += sqWidth;
    }
    *SQRn = sqrValue;
    adc->SQR1 = adc->SQR1 | ((sequence.size() - 1) << ADC_SQR1_L_Pos);
    adc->SMPR2 = smpr[0]; // note SMPR2 contains channels 0-9
    adc->SMPR1 = smpr[1];
}

/**
 * Start conversion or wait for trigger if a trigger is enabled
 */
inline void start(ADC_TypeDef *adc) {
    adc->CR2 = adc->CR2 | ADC_CR2_ADON | ADC_CR2_SWSTART;
}

/**
 * Stop conversion
 */
inline void stop(ADC_TypeDef *adc) {
}

/**
 * Wait for end of conversion
 */
inline void wait(ADC_TypeDef *adc) {
    while ((adc->SR & ADC_SR_EOC) == 0);
}

/// @brief Wait for end of dual conversion
/// @param common common registers
inline void wait(ADC_Common_TypeDef *common) {
    while ((common->CSR & (ADC_CSR_EOC1 | ADC_CSR_EOC2)) != (ADC_CSR_EOC1 | ADC_CSR_EOC2));
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


#ifdef HAVE_ADC_DUAL_MODE
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
#endif


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

    // DMA channel selection
    uint8_t sel;


    /// @brief Configure ADC for use (enable clock, configure and enable ADC).
    /// Only need to set the input or sequence (.setInput() or .setSequence())
    Registers configure(ClockConfig clockConfig, Config config, Trigger trigger,
        DmaMode dmaMode = DmaMode::DISABLED) const
    {
        // enable clock
        this->rcc.enableClock();

        auto common = this->common;
        auto adc = this->adc;

        // set clock config
        common->CCR = int(clockConfig);

        // set config (resolution, alignment, trigger)
        adc->CR1 = int(config);
        adc->CR2 = int(trigger) | int(dmaMode);

        // enable ADC
        adc::enable(adc);

        return {adc};
    }

    /**
     * Map a DMA channel to the the DAC
     * Check reference manual which mappings are possible
     */
    void map(const dma::Info &dmaInfo) const {
        dmaInfo.setSel(this->sel);
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
    rcc::Info2<offsetof(RCC_TypeDef, APB2ENR)> rcc;

    // interrupt of first channel
    uint8_t irq;

    // DMA channel selection
    uint8_t sel;


    /// @brief Configure ADC for use (enable clock, configure and enable ADC).
    /// Only need to set the input or sequence of both ADCs (.setInput1() and .setInput2() or adc::setSequence2() and adc::setSequence2())
    Registers2 configure(ClockConfig clockConfig, Config config, Trigger trigger,
        DmaMode dmaMode = DmaMode::DISABLED) const
    {
        // enable clock
        this->rcc.enableClock();

        auto common = this->common;
        auto adc1 = this->adc1;
        auto adc2 = this->adc2;

        // set clock config, common mode and DMA mode
        uint32_t ccr = int(clockConfig)
            | ADC_CCR_MULTI_2 | ADC_CCR_MULTI_1; // regular simultaneous mode
        if (dmaMode != DmaMode::DISABLED) {
            int resolution = extract(config, Config::RES_MASK);
            ccr |= ((2 + (int(resolution) >> 1)) << ADC_CCR_DMA_Pos) // DMA mode
                | (extract(uint32_t(dmaMode), ADC_CR2_DDS) << ADC_CCR_DDS_Pos); // circular mode flag
        }
        common->CCR = ccr;

        // set config (resolution, alignment, trigger)
        adc1->CR1 = int(config);
        adc1->CR1 = int(config);
        adc2->CR2 = int(trigger);
        adc2->CR2 = int(trigger);

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
        dmaInfo.setSel(this->sel);
    }
};


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
static const Info2 ADC12_INFO{ADC123_COMMON, ADC1, ADC2, RCC_APB2ENR_ADC1EN | RCC_APB2ENR_ADC2EN, ADC_IRQn, 0};
#endif

} // namespace adc
} // namespace coco
