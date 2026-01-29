#pragma once

#include "dmaF0L0C0G0F1L1F3L4G4.hpp"


namespace coco {

/// @brief DMA helpers
/// Refernece manual: https://www.st.com/resource/en/reference_manual/dm00031936-stm32f0x1stm32f0x2stm32f0x8-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
/// DMA: Section 10
namespace dma {

/// @brief DMA channel info
/// @tparam F Feature flags
template <Feature F = Feature::NONE>
struct Info {
    // registers
    DMA_TypeDef *dma;

    // reset and clock control
    rcc::Info2<offsetof(RCC_TypeDef, AHBRSTR), offsetof(RCC_TypeDef, AHBENR)> rcc;

    // DMA channel index (0 for Channel1)
    uint8_t channelIndex;

    // DMA interrupt index
    uint8_t irq;


    /// @brief Cast to another info structure.
    /// Requires that all features are supported.
    /// @tparam F2 Feature set of info to cast to
    template <Feature F2> requires ((F2 & F) == F2)
    operator Info<F2> () const {
        return {dma, rcc, channelIndex, irq};
    }

    auto channelCegisters() const {return (DMA_Channel_TypeDef *)(intptr_t(dma) + 8 + channelIndex * 0x14);}

    /// @brief Enable clock and return a channel wrapper.
    /// @tparam M Mode of Channel
    /// @return Channel (wrapper for channel registers)
    template <Mode M>
    Channel<M> enableClock() const {
        rcc.enableClock();
        return {channelCegisters(), channelIndex * 4};
    }

    /// @brief Get wrapper for the the DMA channel registers.
    /// @tparam M Mode of Channel
    /// @return Channel (wrapper for channel registers)
    template <Mode M>
    Channel<M> channel() const {
        return {channelCegisters(), channelIndex * 4};
    }
};

/// @brief Dual DMA channel info
/// @tparam F Feature flags
template <Feature F = Feature::NONE>
struct DualInfo {
    // registers
    DMA_TypeDef *dma;

    // reset and clock control
    rcc::Info2<offsetof(RCC_TypeDef, AHBRSTR), offsetof(RCC_TypeDef, AHBENR)> rcc;

    // DMA channel index (0 for Channel1)
    uint8_t channelIndex1;
    uint8_t channelIndex2;

    // DMA interrupt index
    uint8_t irq1;
    uint8_t irq2;


    /// @brief Cast to another info structure.
    /// Requires that all features are supported.
    /// @tparam F2 Feature set of info to cast to
    template <Feature F2> requires ((F2 & F) == F2)
    operator DualInfo<F2> () const {
        return {dma, rcc, channelIndex1, channelIndex2, irq1, irq2};
    }

    auto channelRegisters1() const {return (DMA_Channel_TypeDef *)(intptr_t(dma) + 8 + channelIndex1 * 0x14);}
    auto channelRegisters2() const {return (DMA_Channel_TypeDef *)(intptr_t(dma) + 8 + channelIndex2 * 0x14);}

    /// @brief Enable clock and return a channel wrappers.
    /// @tparam M1 Mode of first channel
    /// @tparam M2 Mode of second channel
    /// @return Channels (wrapper for channel registers)
    template <Mode M1, Mode M2>
    std::pair<Channel<M1>, Channel<M2>> enableClock() const {
        rcc.enableClock();
        return {
            {channelRegisters1(), channelIndex1 * 4},
            {channelRegisters2(), channelIndex2 * 4}
        };
    }

    /// @brief Get wrapper for the the first DMA channel registers.
    /// @tparam M1 Mode of first channel
    /// @return First channel (wrapper for channel registers of the first channel)
    template <Mode M1>
    Channel<M1> channel1() const {
        return {channelRegisters1(), channelIndex1 * 4};
    }

    /// @brief Get wrapper for the the second DMA channel registers.
    /// @tparam M2 Mode of second channel
    /// @return Second channel (wrapper for channel registers of the second channel)
    template <Mode M2>
    Channel<M2> channel2() const {
        return {channelRegisters2(), channelIndex2 * 4};
    }
};


using FullInfo = Info<Feature::CIRCULAR>;
using FullDualInfo = DualInfo<Feature::CIRCULAR>;

// single channel DMA infos
static const FullInfo DMA1_CH1_INFO{DMA1, RCC_AHBENR_DMA1EN, 0, DMA1_Channel1_IRQn};
static const FullInfo DMA1_CH2_INFO{DMA1, RCC_AHBENR_DMA1EN, 1, DMA1_Channel2_3_IRQn};
static const FullInfo DMA1_CH3_INFO{DMA1, RCC_AHBENR_DMA1EN, 2, DMA1_Channel2_3_IRQn};
static const FullInfo DMA1_CH4_INFO{DMA1, RCC_AHBENR_DMA1EN, 3, DMA1_Channel4_5_IRQn};
static const FullInfo DMA1_CH5_INFO{DMA1, RCC_AHBENR_DMA1EN, 4, DMA1_Channel4_5_IRQn};
#ifdef DMA1_Channel6
static const FullInfo DMA1_CH6_INFO{DMA1, RCC_AHBENR_DMA1EN, 5, DMA1_Channel4_5_6_7_IRQn};
#endif
#ifdef DMA1_Channel7
static const FullInfo DMA1_CH7_INFO{DMA1, RCC_AHBENR_DMA1EN, 6, DMA1_Channel4_5_6_7_IRQn};
#endif

/*
todo: map methods of peripherals should support DMA2
#ifdef DMA2
static const FullInfo DMA2_CH1_INFO{DMA2, RCC_AHBENR_DMA2EN, 0, DMA1_Ch2_3_DMA2_Ch1_2_IRQn};
static const FullInfo DMA2_CH2_INFO{DMA2, RCC_AHBENR_DMA2EN, 1, DMA1_Ch2_3_DMA2_Ch1_2_IRQn};
static const FullInfo DMA2_CH3_INFO{DMA2, RCC_AHBENR_DMA2EN, 2, DMA1_Ch4_7_DMA2_Ch3_5_IRQn};
static const FullInfo DMA2_CH4_INFO{DMA2, RCC_AHBENR_DMA2EN, 3, DMA1_Ch4_7_DMA2_Ch3_5_IRQn};
static const FullInfo DMA2_CH5_INFO{DMA2, RCC_AHBENR_DMA2EN, 4, DMA1_Ch4_7_DMA2_Ch3_5_IRQn};
#endif
*/

// dual channel DMA infos
static const FullDualInfo DMA1_CH2_CH3_INFO{DMA1, RCC_AHBENR_DMA1EN, 1, 2, DMA1_Channel2_3_IRQn, DMA1_Channel2_3_IRQn};
static const FullDualInfo DMA1_CH3_CH2_INFO{DMA1, RCC_AHBENR_DMA1EN, 2, 1, DMA1_Channel2_3_IRQn, DMA1_Channel2_3_IRQn};
static const FullDualInfo DMA1_CH4_CH5_INFO{DMA1, RCC_AHBENR_DMA1EN, 3, 4, DMA1_Channel4_5_IRQn, DMA1_Channel4_5_IRQn};
static const FullDualInfo DMA1_CH5_CH4_INFO{DMA1, RCC_AHBENR_DMA1EN, 4, 3, DMA1_Channel4_5_IRQn, DMA1_Channel4_5_IRQn};

} // namespace dma
} // namespace coco
