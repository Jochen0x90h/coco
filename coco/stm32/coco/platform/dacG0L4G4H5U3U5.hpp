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

// dac has dual mode
#define HAVE_DAC_DUAL_MODE

// detect if enableClock() needs AHB clock frequency as parameter
#ifdef DAC_MCR_HFSEL
#define HAVE_DAC_PARAMETER_AHB_CLOCK
#endif


namespace coco {

/// @brief DAC helpers.
/// G0X0 https://www.st.com/en/microcontrollers-microprocessors/stm32g0-series/documentation.html Not supported
/// G0X1 https://www.st.com/en/microcontrollers-microprocessors/stm32g0-series/documentation.html Section 16
/// G4 https://www.st.com/en/microcontrollers-microprocessors/stm32g4-series/documentation.html Section 22
/// H503 https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series/documentation.html Section 21
/// H5 https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series/documentation.html Section 28
/// U3 https://www.st.com/en/microcontrollers-microprocessors/stm32u3-series/documentation.html Section 23
namespace dac {

/// @brief DAC configuration (MCR register)
///
enum class Config : uint32_t {
    DEFAULT = 0,


    // signedness
    UNSIGNED = 0, // default
#ifdef DAC_MCR_SINFORMAT1
    SIGNED = DAC_MCR_SINFORMAT1,
#endif

    // sample and hold
    SAMPLE_AND_HOLD = DAC_MCR_MODE1_2,

    // connection to external pin or on-chip peripherals with optional buffer
    BUFFERED_EXTERNAL = 0,
    BUFFERED_EXTERNAL_INTERNAL = DAC_MCR_MODE1_0,
    EXTERNAL = DAC_MCR_MODE1_1,
    INTERNAL = DAC_MCR_MODE1_1 | DAC_MCR_MODE1_0,
};
COCO_ENUM(Config)

/// @brief DAC configuration in dual mode (MCR register)
///
enum class DualConfig : uint32_t {
    DEFAULT = 0,


    // channel 1 signedness
    CH1_UNSIGNED = 0, // default
#ifdef DAC_MCR_SINFORMAT1
    CH1_SIGNED = DAC_MCR_SINFORMAT1,
#endif

    // channel 1 sample and hold
    CH1_SAMPLE_AND_HOLD = DAC_MCR_MODE1_2,

    // channel 1 connection to external pin or on-chip peripherals with optional buffer
    CH1_BUFFERED_EXTERNAL = 0,
    CH1_BUFFERED_EXTERNAL_INTERNAL = DAC_MCR_MODE1_0,
    CH1_EXTERNAL = DAC_MCR_MODE1_1,
    CH1_INTERNAL = DAC_MCR_MODE1_1 | DAC_MCR_MODE1_0,


    // channel 2 signedness
    CH2_UNSIGNED = 0, // default
#ifdef DAC_MCR_SINFORMAT2
    CH2_SIGNED = DAC_MCR_SINFORMAT2,
#endif

    // channel 2 sample and hold
    CH2_SAMPLE_AND_HOLD = DAC_MCR_MODE2_2,

    // channel 2 connection to external pin or on-chip peripherals with optional buffer
    CH2_BUFFERED_EXTERNAL = 0,
    CH2_BUFFERED_EXTERNAL_INTERNAL = DAC_MCR_MODE2_0,
    CH2_EXTERNAL = DAC_MCR_MODE2_1,
    CH2_INTERNAL = DAC_MCR_MODE2_1 | DAC_MCR_MODE2_0,


    // dual signedness
    UNSIGNED = 0, // default
#ifdef DAC_MCR_SINFORMAT2
    SIGNED = CH1_SIGNED | CH2_SIGNED,
#endif

    // dual channel sample and hold
    SAMPLE_AND_HOLD = CH1_SAMPLE_AND_HOLD | CH2_SAMPLE_AND_HOLD,

    // dual channel connection to external pin or on-chip peripherals with optional buffer
    BUFFERED_EXTERNAL = 0,
    BUFFERED_EXTERNAL_INTERNAL = CH1_BUFFERED_EXTERNAL_INTERNAL | CH2_BUFFERED_EXTERNAL_INTERNAL,
    EXTERNAL = CH1_EXTERNAL | CH2_EXTERNAL,
    INTERNAL = CH1_INTERNAL | CH2_INTERNAL,
};
COCO_ENUM(DualConfig)


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
//COCO_ENUM(DmaRequest)


/// @brief DAC instance (wrapper for DAC registers)
///
struct Instance {
    DAC_TypeDef *registers;


    DAC_TypeDef *operator ->() const {return registers;}
    operator DAC_TypeDef *() const {return registers;}

    /// @brief Enable one DAC channel
    /// @param index Channel index (0 for channel 1, 1 for channel 2)
    /// @param config Configuration for the channel
    /// @param trigger Trigger
    /// @param dmaRequest Additional config such as trigger and DMA enable
    /// @return *this
    Instance &enable(int index, Config config, Trigger trigger = Trigger(0),
        DmaRequest dmaRequest = DmaRequest::NONE)
    {
        auto dac = registers;

        int shift = index * 16;
        uint32_t mask = ~(0xffff << shift);

        // keep clock and other channel, configure signedness and output
#ifdef DAC_MCR_HFSEL
        dac->MCR = (dac->MCR & (DAC_MCR_HFSEL | mask)) | (int(config) << shift);
#else
        dac->MCR = (dac->MCR & mask) | (int(config) << shift);
#endif

        // set trigger, DMA request and enable one channels
        dac->CR = (dac->CR & mask) | ((int(trigger) | int(dmaRequest) | DAC_CR_EN1) << shift);

        return *this;
    }

    /// @brief Enable DAC in dual mode
    /// Trigger and DMA are applied to the first channel
    /// @param config Configuration for both channels
    /// @param trigger Trigger
    /// @param dmaRequest Additional config such as trigger and DMA enable
    /// @return *this
    Instance &enable(DualConfig config, Trigger trigger = Trigger(0),
        DmaRequest dmaRequest = DmaRequest::NONE)
    {
        auto dac = registers;

        // keep clock, configure signedness and output
#ifdef DAC_MCR_HFSEL
        dac->MCR = (dac->MCR & DAC_MCR_HFSEL) | int(config);
#else
        dac->MCR = int(config);
#endif

        // set trigger, DMA request and enable both channels
        dac->CR = int(trigger) | int(dmaRequest) | DAC_CR_EN1 | DAC_CR_EN2;

        return *this;
    }

    /// @brief Disable both DAC channels
    /// @return *this
    Instance &disable() {
        registers->CR = 0;
        return *this;
    }
};


/// @brief DAC info
/// Contains pointer to registers, reset and clock control, interrupt and DMA request index
struct Info {
    // registers
    DAC_TypeDef *registers;

    // reset and clock control
    rcc::DacInfo rcc;

    // interrupt request index
    uint8_t irq;

    // DMA request index
    uint8_t drq;


    /// @brief Enable clock and return an instance wrapper
    /// @param ahbClock AHB clock
    /// @return Instance (wrapper for registers)
    Instance enableClock(Hertz<> ahbClock
#ifndef DAC_MCR_HFSEL
        = {}
#endif
    ) const
    {
        // enable clock
        rcc.enableClock();

        auto dac = registers;
#ifdef DAC_MCR_HFSEL
        // set HFSEL in MCR register
        uint32_t mcr = 0;
        if (ahbClock > 160MHz) {
            // AHB greater than 160MHz
            mcr |= DAC_MCR_HFSEL_1;
        } else if (ahbClock > 80MHz) {
            // AHB 81 to 160MHz
            mcr |= DAC_MCR_HFSEL_0;
        }
        dac->MCR = mcr;
#endif

        // return Registers wrapper
        return {dac};
    }

    /// @brief Map a DMA channel to the first channel of the DAC
    /// @param dmaInfo DMA info
    /// @param channelIndex Channel index (0 for channel 1, 1 for channel 2)
    template <dma::Feature F2>
    void map(const dma::Info<F2> &dmaInfo, int channelIndex) const;
};

} // namespace dac
} // namespace coco

#endif // DAC1
