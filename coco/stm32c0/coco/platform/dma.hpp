#pragma once

#include "dmaF0L0C0G0F1L1F3L4G4.hpp"


namespace coco {

/// @brief DMA helpers
/// Refernece manual: https://www.st.com/resource/en/reference_manual/rm0490-stm32c0x1-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
/// DMA: Section 9
/// DMAMUX: Section 10
namespace dma {

/// @brief DMA channel info
/// @tparam F Feature flags
template <Feature F = Feature::NONE>
struct Info {
    // registers
    DMA_TypeDef *dma;

    // reset and clock control
    rcc::Info2<offsetof(RCC_TypeDef, AHBENR)> rcc;

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

    auto channelRegisters() const {return (DMA_Channel_TypeDef *)(intptr_t(dma) + 8 + channelIndex * 0x14);}

    /// @brief Enable clock and return a channel wrapper.
    /// @tparam M Mode of Channel
    /// @return Channel (wrapper for channel registers)
    template <Mode M>
    Channel<M> enableClock() const {
        rcc.enableClock();
        return {channelRegisters(), channelIndex * 4};
    }

    /// @brief Get wrapper for the the DMA channel registers.
    /// @tparam M Mode of Channel
    /// @return Channel (wrapper for channel registers)
    template <Mode M>
    Channel<M> channel() const {
        return {channelRegisters(), channelIndex * 4};
    }

    /// @brief Get the DMAMUX structure for the DMA channel
    ///
    DMAMUX_Channel_TypeDef *mux() const {return (DMAMUX_Channel_TypeDef *)(DMAMUX1_Channel0_BASE + (channelIndex) * 4);}

    /// @brief Set peripheral DMA request index for the DMA channel (see reference manual for list)
    ///
    void setRequest(int index) const {mux()->CCR = index << DMAMUX_CxCR_DMAREQ_ID_Pos;}
};

/// @brief Dual DMA channel info
/// @tparam F Feature flags
template <Feature F = Feature::NONE>
struct DualInfo {
    // registers
    DMA_TypeDef *dma;

    // reset and clock control
    rcc::Info2<offsetof(RCC_TypeDef, AHBENR)> rcc;

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

    /// @brief Get the DMAMUX structure for the first DMA channel
    ///
    DMAMUX_Channel_TypeDef *mux1() const {return (DMAMUX_Channel_TypeDef *)(DMAMUX1_Channel0_BASE + (channelIndex1) * 4);}

    /// @brief Get the DMAMUX structure for the second DMA channel
    ///
    DMAMUX_Channel_TypeDef *mux2() const {return (DMAMUX_Channel_TypeDef *)(DMAMUX1_Channel0_BASE + (channelIndex2) * 4);}

    /// @brief Set peripheral DMA request index for the first DMA channel (see reference manual for list)
    ///
    void setRequest1(int index) const {mux1()->CCR = index << DMAMUX_CxCR_DMAREQ_ID_Pos;}

    /// @brief Set peripheral DMA request index for the second DMA channel (see reference manual for list)
    ///
    void setRequest2(int index) const {mux2()->CCR = index << DMAMUX_CxCR_DMAREQ_ID_Pos;}
};


using FullInfo = Info<Feature::CIRCULAR>;
using FullDualInfo = DualInfo<Feature::CIRCULAR>;

// single channel DMA infos
static const FullInfo DMA1_CH1_INFO{DMA1, RCC_AHBENR_DMA1EN, 0, DMA1_Channel1_IRQn};
static const FullInfo DMA1_CH2_INFO{DMA1, RCC_AHBENR_DMA1EN, 1, DMA1_Channel2_3_IRQn};
static const FullInfo DMA1_CH3_INFO{DMA1, RCC_AHBENR_DMA1EN, 2, DMA1_Channel2_3_IRQn};

// dual channel DMA infos
static const FullDualInfo DMA1_CH1_CH2_INFO{DMA1, RCC_AHBENR_DMA1EN, 0, 1, DMA1_Channel1_IRQn, DMA1_Channel2_3_IRQn};
static const FullDualInfo DMA1_CH2_CH3_INFO{DMA1, RCC_AHBENR_DMA1EN, 1, 2, DMA1_Channel2_3_IRQn, DMA1_Channel2_3_IRQn};

} // namespace dma
} // namespace coco
