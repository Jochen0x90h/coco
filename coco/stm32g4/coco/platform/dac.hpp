#pragma once

#include "platform.hpp"
#include "dma.hpp"


// defines
#define HAVE_DAC
#define HAVE_DAC_CLOCK_CONFIG


namespace coco {

/**
 * DAC helpers
 * Refernece manual: https://www.st.com/resource/en/reference_manual/rm0440-stm32g4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 * DAC: Section 22
 */
namespace dac {

/**
 * DAC clock configuration
 */
enum class ClockConfig {
    // AHB up to 80MHz
    AHB_MAX_80 = 0,

    // AHB up to 16MHz
    AHB_MAX_160 = DAC_MCR_HFSEL_0,

    // AHB greater than 16MHz
    AHB_GREATER_160 = DAC_MCR_HFSEL_1,
};

constexpr int MCR_SHIFT = 1;

/**
 * DAC configuration
 */
enum class Config : uint32_t {
    // channel 1 enable
    CH1 = DAC_CR_EN1,

    // channel 1 mode
    CH1_UNSIGNED = 0, // default
    CH1_SIGNED = DAC_MCR_SINFORMAT1 << MCR_SHIFT,

    // channel 1 sample and hold
    CH1_SAMPLE_AND_HOLD = DAC_MCR_MODE1_2 << MCR_SHIFT,

    // channel 1 connection to external pin or on-chip peripherals with optional buffer
    CH1_BUFFERED_EXTERNAL = 0,
    CH1_BUFFERED_EXTERNAL_INTERNAL = DAC_MCR_MODE1_0 << MCR_SHIFT,
    CH1_EXTERNAL = DAC_MCR_MODE1_1 << MCR_SHIFT,
    CH1_INTERNAL = (DAC_MCR_MODE1_1 | DAC_MCR_MODE1_0) << MCR_SHIFT,


    // channel 2 enable
    CH2 = DAC_CR_EN2,

    // channel 2 mode
    CH2_UNSIGNED = 0, // default
    CH2_SIGNED = DAC_MCR_SINFORMAT2 << MCR_SHIFT,

    // channel 2 sample and hold
    CH2_SAMPLE_AND_HOLD = DAC_MCR_MODE2_2 << MCR_SHIFT,

    // channel 2 connection to external pin or on-chip peripherals with optional buffer
    CH2_BUFFERED_EXTERNAL = 0,
    CH2_BUFFERED_EXTERNAL_INTERNAL = DAC_MCR_MODE2_0 << MCR_SHIFT,
    CH2_EXTERNAL = DAC_MCR_MODE2_1 << MCR_SHIFT,
    CH2_INTERNAL = (DAC_MCR_MODE2_1 | DAC_MCR_MODE2_0) << MCR_SHIFT,


    // dual enable
    DUAL = CH1 | CH2,

    // dual channel mode
    UNSIGNED = 0, // default
    SIGNED = CH1_SIGNED | CH2_SIGNED,

    // dual channel sample and hold
    SAMPLE_AND_HOLD = CH1_SAMPLE_AND_HOLD | CH2_SAMPLE_AND_HOLD,

    // dual channel connection to external pin or on-chip peripherals with optional buffer
    BUFFERED_EXTERNAL = 0,
    BUFFERED_EXTERNAL_INTERNAL = CH1_BUFFERED_EXTERNAL_INTERNAL | CH2_BUFFERED_EXTERNAL_INTERNAL,
    EXTERNAL = CH1_EXTERNAL | CH2_EXTERNAL,
    INTERNAL = CH1_INTERNAL | CH2_INTERNAL,


    // masks
    CR_MASK = DAC_CR_EN1 | DAC_CR_EN2,
    MCR_MASK = (DAC_MCR_SINFORMAT1 | DAC_MCR_SINFORMAT2 | DAC_MCR_MODE1 | DAC_MCR_MODE2) << MCR_SHIFT
};
COCO_ENUM(Config)

/**
 * Supported formats (can be used to index the data registers)
 */
enum Format {
    RES_12_RIGHT = 0,
    RES_12_LEFT = 1,
    RES_8 = 2,
};

/**
 * DAC trigger
 * Reference manual section 22.4.2
 */
enum class Trigger {
    // DAC triggered by software (via SWTRIGR register)
    SOFTWARE = 0,

    // DAC1 triggered by...
    DAC1_TIM2_TRGO = 4 << DAC_CR_TSEL1_Pos,
    DAC1_TIM3_TRGO = 8 << DAC_CR_TSEL1_Pos,
    DAC1_TIM4_TRGO = 5 << DAC_CR_TSEL1_Pos,
    DAC1_TIM6_TRGO = 7 << DAC_CR_TSEL1_Pos,
    DAC1_TIM7_TRGO = 2 << DAC_CR_TSEL1_Pos,
    DAC1_TIM8_TRGO = 1 << DAC_CR_TSEL1_Pos,
    DAC1_TIM15_TRGO = 3 << DAC_CR_TSEL1_Pos,
    DAC1_EXTI9 = 6 << DAC_CR_TSEL1_Pos,

    // DAC2 triggered by...
    DAC2_TIM2_TRGO = 4 << DAC_CR_TSEL1_Pos,
    DAC2_TIM3_TRGO = 8 << DAC_CR_TSEL1_Pos,
    DAC2_TIM4_TRGO = 5 << DAC_CR_TSEL1_Pos,
    DAC2_TIM6_TRGO = 7 << DAC_CR_TSEL1_Pos,
    DAC2_TIM7_TRGO = 2 << DAC_CR_TSEL1_Pos,
    DAC2_TIM8_TRGO = 1 << DAC_CR_TSEL1_Pos,
    DAC2_TIM15_TRGO = 3 << DAC_CR_TSEL1_Pos,
    DAC2_EXTI9 = 6 << DAC_CR_TSEL1_Pos,

    // DAC3 triggered by...
    DAC3_TIM1_TRGO = 1 << DAC_CR_TSEL1_Pos,
    DAC3_TIM2_TRGO = 4 << DAC_CR_TSEL1_Pos,
    DAC3_TIM3_TRGO = 8 << DAC_CR_TSEL1_Pos,
    DAC3_TIM4_TRGO = 5 << DAC_CR_TSEL1_Pos,
    DAC3_TIM6_TRGO = 7 << DAC_CR_TSEL1_Pos,
    DAC3_TIM7_TRGO = 2 << DAC_CR_TSEL1_Pos,
    DAC3_TIM15_TRGO = 3 << DAC_CR_TSEL1_Pos,
    DAC3_EXTI9 = 6 << DAC_CR_TSEL1_Pos,

    // DAC4 triggered by...
    DAC4_TIM2_TRGO = 4 << DAC_CR_TSEL1_Pos,
    DAC4_TIM3_TRGO = 8 << DAC_CR_TSEL1_Pos,
    DAC4_TIM4_TRGO = 5 << DAC_CR_TSEL1_Pos,
    DAC4_TIM6_TRGO = 7 << DAC_CR_TSEL1_Pos,
    DAC4_TIM7_TRGO = 2 << DAC_CR_TSEL1_Pos,
    DAC4_TIM8_TRGO = 1 << DAC_CR_TSEL1_Pos,
    DAC4_TIM15_TRGO = 3 << DAC_CR_TSEL1_Pos,
    DAC4_EXTI9 = 6 << DAC_CR_TSEL1_Pos,
};

/**
 * Internal configuration which is typically not exposed to the user of a "driver"
 */
enum class InternalConfig : uint32_t {
    NONE = 0,
    ENABLE_DMA = DAC_CR_DMAEN1,
};


/**
 * DAC info
 */
struct Info {
    // registers
    DAC_TypeDef *dac;

    // reset and clock control
    rcc::Info2<offsetof(RCC_TypeDef, AHB2ENR)> rcc;

    // interrupt
    uint8_t irq;

    // DMA mux index
    uint8_t mux;


    /**
     * Configure DAC for use in single or dual mode (enable clock, configure and enable DAC)
     * @param clockConfig clock configuration
     * @param config configuration
     * @param internalConfig additional config such as trigger and DMA enable, gets applied to both channels (which has effect only for enabled channels)
     */
    void configure(dac::ClockConfig clockConfig, dac::Config config, InternalConfig internalConfig = InternalConfig::NONE) const {
        // enable clock
        this->rcc.enableClock();

        auto dac = this->dac;

        // configure clock, mode and output
        dac->MCR = int(clockConfig) | (int(config & dac::Config::MCR_MASK) >> dac::MCR_SHIFT);

        // apply internal config (trigger, DMA) only to first enabled channel
        uint32_t cr = int(config & dac::Config::CR_MASK);
        dac->CR = cr | ((cr & DAC_CR_EN1) != 0 ? int(internalConfig) : (int(internalConfig) << 16));
    }

    /**
     * Configure DAC with trigger for use in single or dual mode (enable clock, configure and enable DAC)
     * @param clockConfig clock configuration
     * @param config configuration
     * @param trigger trigger
     * @param internalConfig additional config such as and DMA enable, gets applied to both channels (which has effect only for enabled channels)
     */
    void configure(dac::ClockConfig clockConfig, dac::Config config, dac::Trigger trigger,
        InternalConfig internalConfig = InternalConfig::NONE) const
    {
        configure(clockConfig, config, InternalConfig(int(trigger) | DAC_CR_TEN1 | int(internalConfig)));
    }

    /**
     * Map a DMA channel to the first channel of the DAC
     * @param dmaInfo DMA info
     * @param channelIndex channel index 0 or 1
     */
    void map(const dma::Info &dmaInfo, int channelIndex) const {
        dmaInfo.setMux(this->mux + channelIndex);
    }
};


static const Info DAC1_INFO{DAC1, RCC_AHB2ENR_DAC1EN, TIM6_DAC_IRQn, 6};
#ifdef DAC2
static const Info DAC2_INFO{DAC2, RCC_AHB2ENR_DAC2EN, TIM7_DAC_IRQn, 41};
#endif
static const Info DAC3_INFO{DAC3, RCC_AHB2ENR_DAC3EN, TIM6_DAC_IRQn, 102};
#ifdef DAC4
static const Info DAC4_INFO{DAC4, RCC_AHB2ENR_DAC4EN, TIM7_DAC_IRQn, 104};
#endif

} // namespace dac
} // namespace coco
