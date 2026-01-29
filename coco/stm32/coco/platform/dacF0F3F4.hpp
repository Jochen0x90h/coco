#pragma once

// do not include directly, use #include <coco/platform/dac.hpp>

#include "platform.hpp"
#include "dma.hpp"
#include <coco/enum.hpp>
#include <coco/Frequency.hpp>


/*
    Defines:
    HAVE_DAC               DAC supported
    HAVE_DAC_DUAL_MODE     DAC with dual mode (two channels) supported
*/


#ifdef DAC1

// dac supported
#define HAVE_DAC

// detect if dac has dual mode
#ifdef DAC_CR_EN2
#define HAVE_DAC_DUAL_MODE
#endif


namespace coco {

/// @brief DAC helpers.
///
namespace dac {

/// @brief DAC configuration (CR register)
///
enum class Config : uint32_t {
    DEFAULT = 0,

    // signedness
    UNSIGNED = 0, // default

#ifdef STM32F3
    // buffer off (check reference manual if supported for the DAC/channel)
    BUFFER_OFF = 1,

    // output enable (check reference manual if supported for the DAC/channel)
    OUTPUT_ENABLE = 1,
#endif
};
COCO_ENUM(Config)

#ifdef HAVE_DAC_DUAL_MODE
/// @brief DAC configuration in dual mode
///
enum class DualConfig : uint32_t  {
    DEFAULT = 0,

    // channel 1 signedness
    CH1_UNSIGNED = 0, // default

#ifdef STM32F3
    // buffer off (check reference manual if supported for the DAC/channel)
    CH1_BUFFER_OFF = 1,

    // output enable (check reference manual if supported for the DAC/channel)
    CH1_OUTPUT_ENABLE = 1,
#endif


    // channel 2 signedness
    CH2_UNSIGNED = 0, // default

#ifdef STM32F3
    // buffer off (check reference manual if supported for the DAC/channel)
    CH2_BUFFER_OFF = 1 << 16,

    // output enable (check reference manual if supported for the DAC/channel)
    CH2_OUTPUT_ENABLE = 1 << 16,
#endif


    // dual signedness
    UNSIGNED = 0, // default

#ifdef STM32F3
    // buffer off (check reference manual if supported for the DAC/channel)
    BUFFER_OFF = CH1_BUFFER_OFF | CH2_BUFFER_OFF,

    // output enable (check reference manual if supported for the DAC/channel)
    OUTPUT_ENABLE = CH1_OUTPUT_ENABLE | CH2_OUTPUT_ENABLE,
#endif
};
COCO_ENUM(DualConfig)
#endif


/// @brief Supported formats
/// Note: Values can be used to index the data registers
enum Format {
    RES_12_RIGHT = 0,
    RES_12_LEFT = 1,
    RES_8 = 2,
};

// Trigger is device specific
enum class Trigger;


/// @brief DMA request flags (CR register).
///
enum class DmaRequest : uint32_t {
    NONE = 0,

    // enable
    ENABLE = DAC_CR_DMAEN1,
};
COCO_ENUM(DmaRequest)


// update SYSCFG registers according to trigger
int updateSYSCFG(Trigger trigger);


/// @brief DAC instance (wrapper for DAC registers)
///
struct Instance {
    DAC_TypeDef *dac;


    DAC_TypeDef *operator ->() const {return dac;}
    operator DAC_TypeDef *() const {return dac;}

    /// @brief Enable one DAC channel
    /// @param channel Channel index (0 for channel 1, 1 for channel 2)
    /// @param config Configuration for the channel
    /// @param trigger Trigger
    /// @param dmaRequest Additional config such as trigger and DMA enable
    /// @return *this
    Instance &enable(int channel, Config config, Trigger trigger = Trigger(0),
        DmaRequest dmaRequest = DmaRequest::NONE)
    {
        int shift = channel * 16;
        uint32_t mask = ~(0xffff << shift);

        // set config trigger, DMA request and enable one channels
        dac->CR = (dac->CR & mask) | ((int(config) | updateSYSCFG(trigger) | int(dmaRequest) | DAC_CR_EN1) << shift);

        return *this;
    }

#ifdef HAVE_DAC_DUAL_MODE
    /// @brief Enable DAC in dual mode
    /// Trigger and DMA are applied to the first channel
    /// @param config Configuration for both channels
    /// @param trigger Trigger
    /// @param dmaRequest Additional config such as trigger and DMA enable
    /// @return *this
    Instance &enable(DualConfig config, Trigger trigger = Trigger(0),
        DmaRequest dmaRequest = DmaRequest::NONE)
    {
        // set config, trigger, DMA request and enable both channels
        dac->CR = int(config) | updateSYSCFG(trigger) | int(dmaRequest) | DAC_CR_EN1 | DAC_CR_EN2;

        return *this;
    }
#endif

    /// @brief Disable both DAC channels
    /// @return *this
    Instance &disable() {
        dac->CR = 0;
        return *this;
    }
};


/// @brief DAC info
///
struct Info {
    DAC_TypeDef *dac;

    // reset and clock control
    rcc::DacInfo rcc;

    // interrupt request index
    uint8_t irq;

    // DMA request index
    uint8_t drq;


    /// @brief Enable clock and return an instance wrapper
    /// @param ahbClock AHB clock (not used for this processor type)
    /// @return Instance (wrapper for registers)
    Instance enableClock(Hertz<> ahbClock = {}) const {
        rcc.enableClock();
        return {dac};
    }

    /// @brief Map a DMA channel to the first channel of the DAC
    /// Check reference manual which mappings are possible
    /// @param dmaInfo DMA info
    /// @param channelIndex Channel index 0 or 1
    template <dma::Feature F2>
    void map(const dma::Info<F2> &dmaInfo, int channelIndex) const;
};

} // namespace dac
} // namespace coco

#endif // DAC1
