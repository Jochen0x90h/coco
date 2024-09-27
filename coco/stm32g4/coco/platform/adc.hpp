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
 * Refernece manual: https://www.st.com/resource/en/reference_manual/rm0440-stm32g4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 * ADC: Section 21
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

    // ADC clock source SYSCLK or PLLP (configured in RCC e.g. during SystemInit())
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
    RES_12 = 0 << ADC_CFGR_RES_Pos, // default
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
COCO_ENUM(Config)

/**
 * ADC trigger
 * Reference manual section 21.4.18
 */
enum class Trigger {
    // ADC triggered by software (set ADSTART bit or call start())
    SOFTWARE = 0,

    // ADC1/ADC2 triggered by...
    ADC12_TIM1_TRGO = ADC_CFGR_EXTEN_0 | (9 << ADC_CFGR_EXTSEL_Pos),
    ADC12_TIM1_TRGO2 = ADC_CFGR_EXTEN_0 | (10 << ADC_CFGR_EXTSEL_Pos),
    ADC12_TIM2_TRGO = ADC_CFGR_EXTEN_0 | (11 << ADC_CFGR_EXTSEL_Pos),
    ADC12_TIM3_TRGO = ADC_CFGR_EXTEN_0 | (4 << ADC_CFGR_EXTSEL_Pos),
    ADC12_TIM4_TRGO = ADC_CFGR_EXTEN_0 | (12 << ADC_CFGR_EXTSEL_Pos),
    ADC12_TIM6_TRGO = ADC_CFGR_EXTEN_0 | (13 << ADC_CFGR_EXTSEL_Pos),
    ADC12_TIM7_TRGO = ADC_CFGR_EXTEN_0 | (30 << ADC_CFGR_EXTSEL_Pos),
    ADC12_TIM8_TRGO = ADC_CFGR_EXTEN_0 | (7 << ADC_CFGR_EXTSEL_Pos),
    ADC12_TIM8_TRGO2 = ADC_CFGR_EXTEN_0 | (8 << ADC_CFGR_EXTSEL_Pos),
    ADC12_TIM15_TRGO = ADC_CFGR_EXTEN_0 | (14 << ADC_CFGR_EXTSEL_Pos),
    ADC12_TIM20_TRGO = ADC_CFGR_EXTEN_0 | (16 << ADC_CFGR_EXTSEL_Pos),
    ADC12_TIM20_TRGO2 = ADC_CFGR_EXTEN_0 | (17 << ADC_CFGR_EXTSEL_Pos),
    ADC12_LPTIMOUT = ADC_CFGR_EXTEN_0 | (29 << ADC_CFGR_EXTSEL_Pos),
    ADC12_EXTI11 = ADC_CFGR_EXTEN_0 | (6 << ADC_CFGR_EXTSEL_Pos),

    // ADC3/ADC4/ADC5 triggered by...
    ADC345_TIM1_TRGO = ADC_CFGR_EXTEN_0 | (9 << ADC_CFGR_EXTSEL_Pos),
    ADC345_TIM1_TRGO2 = ADC_CFGR_EXTEN_0 | (10 << ADC_CFGR_EXTSEL_Pos),
    ADC345_TIM2_TRGO = ADC_CFGR_EXTEN_0 | (11 << ADC_CFGR_EXTSEL_Pos),
    ADC345_TIM3_TRGO = ADC_CFGR_EXTEN_0 | (4 << ADC_CFGR_EXTSEL_Pos),
    ADC345_TIM4_TRGO = ADC_CFGR_EXTEN_0 | (12 << ADC_CFGR_EXTSEL_Pos),
    ADC345_TIM6_TRGO = ADC_CFGR_EXTEN_0 | (13 << ADC_CFGR_EXTSEL_Pos),
    ADC345_TIM7_TRGO = ADC_CFGR_EXTEN_0 | (30 << ADC_CFGR_EXTSEL_Pos),
    ADC345_TIM8_TRGO = ADC_CFGR_EXTEN_0 | (7 << ADC_CFGR_EXTSEL_Pos),
    ADC345_TIM8_TRGO2 = ADC_CFGR_EXTEN_0 | (8 << ADC_CFGR_EXTSEL_Pos),
    ADC345_TIM15_TRGO = ADC_CFGR_EXTEN_0 | (14 << ADC_CFGR_EXTSEL_Pos),
    ADC345_TIM20_TRGO = ADC_CFGR_EXTEN_0 | (16 << ADC_CFGR_EXTSEL_Pos),
    ADC345_TIM20_TRGO2 = ADC_CFGR_EXTEN_0 | (17 << ADC_CFGR_EXTSEL_Pos),
    ADC345_LPTIMOUT = ADC_CFGR_EXTEN_0 | (29 << ADC_CFGR_EXTSEL_Pos),
    ADC345_EXTI2 = ADC_CFGR_EXTEN_0 | (5 << ADC_CFGR_EXTSEL_Pos),
};

/**
 * ADC input, combination of channel index, differential flag and sampling cycles
 * Note: When using a channel more than once in a sequence, the sampling cycles must be the same or can be omitted
 */
enum class Input : uint16_t {
    // dummy channel
    CHANNEL_0 = 0,

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
    CYCLES_2_5 = 0x0000,
    CYCLES_3_5 = 0x1000, // excludes the use of CYCLES_2_5
    CYCLES_6_5 = 0x0100,
    CYCLES_12_5 = 0x0200,
    CYCLES_24_5 = 0x0300,
    CYCLES_47_5 = 0x0400,
    CYCLES_92_5 = 0x0500,
    CYCLES_247_5 = 0x0600,
    CYCLES_640_5 = 0x0700,
    CYCLES_MASK = 0x0700, // does not include CYCLES_3_5 which is a flag
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


/**
 * Enable voltage regulator
 */
inline void enableVoltageRegulator(ADC_TypeDef *adc) {
    adc->CR = ADC_CR_ADVREGEN;

    // wait tADCVREG_STUP
    debug::sleep(20us);
}

/**
 * Enable dual voltage regulator
 */
inline void enableVoltageRegulator(ADC_TypeDef *adc1, ADC_TypeDef *adc2) {
    adc1->CR = ADC_CR_ADVREGEN;
    adc2->CR = ADC_CR_ADVREGEN;

    // wait tADCVREG_STUP
    debug::sleep(20us);
}

/**
 * Calibrate single ended
 */
inline void calibrate(ADC_TypeDef *adc)  {
    adc->CR = ADC_CR_ADVREGEN | ADC_CR_ADCAL;
    while ((adc->CR & ADC_CR_ADCAL) != 0) {}
    for (int i = 0; i < 8; ++i)
        __NOP();
}

/**
 * Calibrate differential
 */
inline void calibrateDifferential(ADC_TypeDef *adc)  {
    adc->CR = ADC_CR_ADVREGEN | ADC_CR_ADCAL | ADC_CR_ADCALDIF;
    while ((adc->CR & ADC_CR_ADCAL) != 0) {}
    for (int i = 0; i < 8; ++i)
        __NOP();
}

/**
 * Enable ADC
 */
inline void enable(ADC_TypeDef *adc) {
    adc->CR = ADC_CR_ADVREGEN | ADC_CR_ADEN;
    while ((adc->ISR & ADC_ISR_ADRDY) == 0);
}

///
/// Enable dual ADC
///
inline void enable(ADC_TypeDef *adc1, ADC_TypeDef *adc2) {
    adc1->CR = ADC_CR_ADVREGEN | ADC_CR_ADEN;
    adc2->CR = ADC_CR_ADVREGEN | ADC_CR_ADEN;
    while ((adc1->ISR & adc2->ISR & ADC_ISR_ADRDY) == 0);
}

/// @brief Set ADC input
/// @param adc ADC registers
/// @param input input and sampling cycles
inline void setInput(ADC_TypeDef *adc, Input input) {
    int channel = int(input & Input::CHANNEL_MASK);

    // select input
    adc->SQR1 = channel << ADC_SQR1_SQ1_Pos;

    // set sampling cycles
    uint32_t smpr[2] = {0, 0};
    smpr[channel / 10] = extract(int(input), int(Input::CYCLES_MASK)) << (channel % 10) * 3;
    if ((input & Input::CYCLES_3_5) != 0)
        smpr[0] |= ADC_SMPR1_SMPPLUS;
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

    for (auto input : sequence) {
        if (sqrPos > 32 - sqWidth) {
            *SQRn = sqrValue;
            ++SQRn;
            sqrValue = 0;
            sqrPos = 0;
        }

        // sequence
        int channel = int(input & Input::CHANNEL_MASK);
        sqrValue |= channel << sqrPos;

        // sampling cycles
        smpr[channel / 10] |= extract(int(input), int(Input::CYCLES_MASK)) << (channel % 10) * 3;
        if ((input & Input::CYCLES_3_5) != 0)
            smpr[0] |= ADC_SMPR1_SMPPLUS;

        // differential mode
        if ((input & adc::Input::DIFFERENTIAL) != 0)
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
    ADC_Common_TypeDef *common;
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
    ADC_Common_TypeDef *common;
    ADC_TypeDef *adc;

    // reset and clock control
    rcc::Info2<offsetof(RCC_TypeDef, AHB2ENR)> rcc;

    // interrupt
    uint8_t irq;

    // DMA mux index
    uint8_t mux;


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
        adc::calibrateDifferential(adc);

        // set clock config
        common->CCR = int(clockConfig);

        // set config (resolution, alignment, trigger)
        adc->CFGR = int(config) | int(trigger) | int(dmaMode);

        // enable ADC
        adc::enable(adc);

        return {adc};
    }

    /**
     * Map a DMA channel to the the ADC
     */
    void map(const dma::Info &dmaInfo) const {
        dmaInfo.setMux(this->mux);
    }
};


/// @brief Info for two ADCs in dual mode
///
struct Info2 {
    ADC_Common_TypeDef *common;
    ADC_TypeDef *adc1;
    ADC_TypeDef *adc2;

    // reset and clock control
    rcc::Info2<offsetof(RCC_TypeDef, AHB2ENR)> rcc;

    // interrupt of first channel
    uint8_t irq;

    // DMA mux index of first channel
    uint8_t mux;


    /// @brief Configure ADC for use (enable clock, enable voltage regulator, calibrate, configure and enable ADC).
    /// Only need to set the input or sequence of both ADCs (.setInput1() and .setInput2() or adc::setSequence2() and adc::setSequence2())
    Registers2 configure(ClockConfig clockConfig, Config config, Trigger trigger,
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
        adc::calibrateDifferential(adc1);
        adc::calibrateDifferential(adc2);

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
     * Map a DMA channel to the first channel of the ADC
     * @param dmaInfo DMA info
     * @param channelIndex channel index 0 or 1
     */
    void map(const dma::Info &dmaInfo, int channelIndex = 0) const {
        dmaInfo.setMux(this->mux + channelIndex);
    }
};


// single channel ADC infos
static const Info ADC1_INFO{ADC12_COMMON, ADC1, RCC_AHB2ENR_ADC12EN, ADC1_2_IRQn, 5};
static const Info ADC2_INFO{ADC12_COMMON, ADC2, RCC_AHB2ENR_ADC12EN, ADC1_2_IRQn, 36};
#ifdef ADC3
static const Info ADC3_INFO{ADC345_COMMON, ADC3, RCC_AHB2ENR_ADC345EN, ADC3_IRQn, 37};
#endif
#ifdef ADC4
static const Info ADC4_INFO{ADC345_COMMON, ADC4, RCC_AHB2ENR_ADC345EN, ADC4_IRQn, 38};
#endif
#ifdef ADC5
static const Info ADC5_INFO{ADC345_COMMON, ADC5, RCC_AHB2ENR_ADC345EN, ADC5_IRQn, 39};
#endif

// dual channel ADC infos
static const Info2 ADC12_INFO{ADC12_COMMON, ADC1, ADC2, RCC_AHB2ENR_ADC12EN, ADC1_2_IRQn, 5};
#ifdef ADC4
static const Info2 ADC34_INFO{ADC345_COMMON, ADC3, ADC4, RCC_AHB2ENR_ADC345EN, ADC3_IRQn, 37};
#endif

} // namespace adc
} // namespace coco
