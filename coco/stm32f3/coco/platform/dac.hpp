#pragma once

#include "platform.hpp"
#include "dma.hpp"


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
enum class Config {
    // channel 1 enable
    CH1 = DAC_CR_EN1,

    // channel 1 mode
    CH1_UNSIGNED = 0, // default

    // output enable / buffer off
    DAC1_CH1_BUFFER_OFF = DAC_CR_BOFF1,
    DAC2_CH1_OUTPUT_ENABLE = DAC_CR_OUTEN1,


    // channel 2 enable
    CH2 = DAC_CR_EN2,

    // channel 2 mode
    CH2_UNSIGNED = 0, // default

    // output enable
    DAC1_CH2_OUTPUT_ENABLE = DAC_CR_OUTEN2,


    // dual enable
    DUAL = CH1 | CH2,

    // dual channel mode
    UNSIGNED = 0, // default


    // masks
    CR_MASK = DAC_CR_EN1 | DAC_CR_BOFF1 | DAC_CR_OUTEN1 | DAC_CR_EN2 | DAC_CR_OUTEN2,
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
 * Reference manual section 14.5.4
 * Use TEN1 flag as indicator that a remap flag in SYSCFG needs to be set (see reference manual and Info::configure())
 */
enum class Trigger {
    // DAC1 triggered by...
    DAC1_SOFTWARE = 7 << DAC_CR_TSEL1_Pos,
    DAC1_TIM2_TRGO = 4 << DAC_CR_TSEL1_Pos,
    DAC1_TIM3_TRGO = (1 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
    DAC1_TIM6_TRGO = 0,
    DAC1_TIM7_TRGO = 2 << DAC_CR_TSEL1_Pos,
    DAC1_TIM15_TRGO = 3 << DAC_CR_TSEL1_Pos,
    DAC1_HRTIM1_DACTRG1 = (3 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
    DAC1_HRTIM1_DACTRG2 = (5 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
    DAC1_EXTI9 = 6 << DAC_CR_TSEL1_Pos,

    // DAC2 triggered by...
    DAC2_SOFTWARE = 7 << DAC_CR_TSEL1_Pos,
    DAC2_TIM2_TRGO = 4 << DAC_CR_TSEL1_Pos,
    DAC2_TIM3_TRGO = (1 << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1,
    DAC2_TIM6_TRGO = 0,
    DAC2_TIM7_TRGO = 2 << DAC_CR_TSEL1_Pos,
    DAC2_TIM15_TRGO = 3 << DAC_CR_TSEL1_Pos,
    DAC1_HRTIM1_DACTRG3 = 5 << DAC_CR_TSEL1_Pos,
    DAC2_EXTI9 = 6 << DAC_CR_TSEL1_Pos,
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
    void configure(dac::Config config, dac::Trigger trigger,
        InternalConfig internalConfig = InternalConfig::NONE) const
    {
        // configure trigger in SYSCFG
        if (int(trigger) & DAC_CR_TEN1) {
            switch (int(trigger) >> DAC_CR_TSEL1_Pos) {
            case 1:
                SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_DAC1_TRIG1_RMP;
                break;
            case 3:
                SYSCFG->CFGR3 = SYSCFG->CFGR3 | SYSCFG_CFGR3_DAC1_TRG3_RMP;
                break;
            case 5:
                SYSCFG->CFGR3 = SYSCFG->CFGR3 | SYSCFG_CFGR3_DAC1_TRG5_RMP;
                break;
            }
        }

        configure(config, InternalConfig(int(trigger) | DAC_CR_TEN1 | int(internalConfig)));
    }

    /**
     * Map a DMA channel to the first channel of the DAC
     * Check reference manual which mappings are possible
     * @param dmaInfo DMA info
     * @param channelIndex channel index 0 or 1
     */
    void map(const dma::Info &dmaInfo, int channelIndex) const {
        switch (uintptr_t(this->dac)) {
        default: // DAC1
            SYSCFG->CFGR1 = SYSCFG->CFGR1 | (channelIndex == 0 ? SYSCFG_CFGR1_TIM6DAC1Ch1_DMA_RMP : SYSCFG_CFGR1_TIM7DAC1Ch2_DMA_RMP);
            break;
#ifdef DAC2
        case DAC2_BASE:
            if (channelIndex == 0)
                SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_DAC2Ch1_DMA_RMP;
            break;
#endif
        }
    }
};


static const Info DAC1_INFO{DAC1, RCC_APB1ENR_DAC1EN, TIM6_DAC1_IRQn};
#ifdef DAC2
static const Info DAC2_INFO{DAC2, RCC_APB1ENR_DAC2EN, TIM7_DAC2_IRQn};
#endif

} // namespace dac
} // namespace coco
