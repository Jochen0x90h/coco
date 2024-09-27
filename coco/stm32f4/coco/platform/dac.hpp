#pragma once

#include "platform.hpp"
#include "dma.hpp"


#ifdef DAC1

// defines
#define HAVE_DAC

namespace coco {

/**
 * DAC helpers
 * Refernece manual: https://www.st.com/resource/en/reference_manual/rm0364-stm32f334xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 * DAC: Section 14
 */
namespace dac {


// dac configuration
enum class Config : uint32_t {
    // channel 1 enable
    CH1 = DAC_CR_EN1,

    // channel 1 mode
    CH1_UNSIGNED = 0, // default


    // channel 2 enable
    CH2 = DAC_CR_EN2,

    // channel 2 mode
    CH2_UNSIGNED = 0, // default


    // dual enable
    DUAL = CH1 | CH2,

    // dual channel mode
    UNSIGNED = 0, // default


    // masks
    CR_MASK = DAC_CR_EN1 | DAC_CR_EN2,
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
 * Reference manual section 14.3.6
 */
enum class Trigger {
    // DAC1 triggered by...
    DAC1_SOFTWARE = 7 << DAC_CR_TSEL1_Pos,
    DAC1_TIM2_TRGO = 4 << DAC_CR_TSEL1_Pos,
    DAC1_TIM4_TRGO = 5 << DAC_CR_TSEL1_Pos,
    DAC1_TIM5_TRGO = 3 << DAC_CR_TSEL1_Pos,
    DAC1_TIM6_TRGO = 0,
    DAC1_TIM7_TRGO = 2 << DAC_CR_TSEL1_Pos,
    DAC1_EXTI9 = 6 << DAC_CR_TSEL1_Pos,
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
    rcc::Info2<offsetof(RCC_TypeDef, APB1ENR)> rcc;

    // interrupt
    uint8_t irq;


    /**
     * Configure DAC for use in single or dual mode (enable clock, configure and enable DAC)
     * @param config configuration
     * @param internalConfig additional config such as trigger and DMA enable, gets applied to both channels (which has effect only for enabled channels)
     */
    void configure(dac::Config config, InternalConfig internalConfig = InternalConfig::NONE) const {
        // enable clock
        this->rcc.enableClock();

        auto dac = this->dac;

        // apply internal config (trigger, DMA) only to first enabled channel
        uint32_t cr = int(config & dac::Config::CR_MASK);
        dac->CR = cr | ((cr & DAC_CR_EN1) != 0 ? int(internalConfig) : (int(internalConfig) << 16));
    }

    /**
     * Configure DAC with trigger for use in single or dual mode (enable clock, configure and enable DAC)
     * @param config configuration
     * @param trigger trigger
     * @param internalConfig additional config such as and DMA enable, gets applied to both channels (which has effect only for enabled channels)
     */
    void configure(dac::Config config, dac::Trigger trigger, InternalConfig internalConfig = InternalConfig::NONE) const
    {
        configure(config, InternalConfig(int(trigger) | DAC_CR_TEN1 | int(internalConfig)));
    }

    /**
     * Map a DMA channel to the first channel of the DAC
     * Check reference manual which mappings are possible
     * @param dmaInfo DMA info
     * @param channelIndex channel index 0 or 1
     */
    void map(const dma::Info &dmaInfo, int channelIndex) const {
        dmaInfo.setSel(7);
    }
};


static const Info DAC1_INFO{DAC1, RCC_APB1ENR_DACEN, TIM6_DAC_IRQn};

} // namespace dac
} // namespace coco

#endif // DAC1
