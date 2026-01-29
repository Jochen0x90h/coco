#pragma once

#include "dmaF0L0C0G0F1L1F3L4G4.hpp"


namespace coco {
namespace dma {

/// @brief DMA channel info
/// @tparam F Feature flags
template <Feature F = Feature::NONE>
struct Info {
    // registers
    DMA_TypeDef *dma;

    // reset and clock control
    rcc::Info2<offsetof(RCC_TypeDef, AHB1ENR)> rcc;

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

    /// @brief Get DMA index (0 for DMA1, 1 for DMA2)
    ///
    int index() const {
        return dma == DMA1 ? 0 : 1;
    }

    /// @brief Get the DMAMUX structure for the DMA channel
    ///
    DMAMUX_Channel_TypeDef *mux() const {return (DMAMUX_Channel_TypeDef *)(DMAMUX1_Channel0_BASE + (index() * 8 + channelIndex) * 4);}

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
    rcc::Info2<offsetof(RCC_TypeDef, AHB1ENR)> rcc;

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

    auto channel1Registers() const {return (DMA_Channel_TypeDef *)(intptr_t(dma) + 8 + channelIndex1 * 0x14);}
    auto channel2Registers() const {return (DMA_Channel_TypeDef *)(intptr_t(dma) + 8 + channelIndex2 * 0x14);}

    /// @brief Enable clock and return a channel wrappers.
    /// @tparam M1 Mode of first channel
    /// @tparam M2 Mode of second channel
    /// @return Channels (wrapper for channel registers)
    template <Mode M1, Mode M2>
    std::pair<Channel<M1>, Channel<M2>> enableClock() const {
        rcc.enableClock();
        return {
            {channel1Registers(), channelIndex1 * 4},
            {channel2Registers(), channelIndex2 * 4}
        };
    }

    /// @brief Get wrapper for the the first DMA channel registers.
    /// @tparam M1 Mode of first channel
    /// @return First channel (wrapper for channel registers of the first channel)
    template <Mode M1>
    Channel<M1> channel1() const {
        return {channel1Registers(), channelIndex1 * 4};
    }

    /// @brief Get wrapper for the the second DMA channel registers.
    /// @tparam M2 Mode of second channel
    /// @return Second channel (wrapper for channel registers of the second channel)
    template <Mode M2>
    Channel<M2> channel2() const {
        return {channel2Registers(), channelIndex2 * 4};
    }

    /// @brief Get DMA index (0 for DMA1, 1 for DMA2)
    ///
    int index() const {
        return dma == DMA1 ? 0 : 1;
    }

    /// @brief Get the DMAMUX structure for the first DMA channel
    ///
    DMAMUX_Channel_TypeDef *mux1() const {return (DMAMUX_Channel_TypeDef *)(DMAMUX1_Channel0_BASE + (index() * 8 + channelIndex1) * 4);}

    /// @brief Get the DMAMUX structure for the second DMA channel
    ///
    DMAMUX_Channel_TypeDef *mux2() const {return (DMAMUX_Channel_TypeDef *)(DMAMUX1_Channel0_BASE + (index() * 8 + channelIndex2) * 4);}

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
static const FullInfo DMA1_CH1_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 0, DMA1_Channel1_IRQn};
static const FullInfo DMA1_CH2_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 1, DMA1_Channel2_IRQn};
static const FullInfo DMA1_CH3_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 2, DMA1_Channel3_IRQn};
static const FullInfo DMA1_CH4_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 3, DMA1_Channel4_IRQn};
static const FullInfo DMA1_CH5_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 4, DMA1_Channel5_IRQn};
static const FullInfo DMA1_CH6_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 5, DMA1_Channel6_IRQn};
#ifdef DMA1_Channel7
static const FullInfo DMA1_CH7_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 6, DMA1_Channel7_IRQn};
#endif
#ifdef DMA1_Channel8
static const FullInfo DMA1_CH8_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 7, DMA1_Channel8_IRQn};
#endif

static const FullInfo DMA2_CH1_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 0, DMA2_Channel1_IRQn};
static const FullInfo DMA2_CH2_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 1, DMA2_Channel2_IRQn};
static const FullInfo DMA2_CH3_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 2, DMA2_Channel3_IRQn};
static const FullInfo DMA2_CH4_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 3, DMA2_Channel4_IRQn};
static const FullInfo DMA2_CH5_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 4, DMA2_Channel5_IRQn};
static const FullInfo DMA2_CH6_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 5, DMA2_Channel6_IRQn};
#ifdef DMA2_Channel7
static const FullInfo DMA2_CH7_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 6, DMA2_Channel7_IRQn};
#endif
#ifdef DMA2_Channel8
static const FullInfo DMA2_CH8_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 7, DMA2_Channel8_IRQn};
#endif

// dual channel DMA infos
static const FullDualInfo DMA1_CH1_CH2_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 0, 1, DMA1_Channel1_IRQn, DMA1_Channel2_IRQn};
static const FullDualInfo DMA1_CH2_CH3_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 1, 2, DMA1_Channel2_IRQn, DMA1_Channel3_IRQn};
static const FullDualInfo DMA1_CH3_CH4_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 2, 3, DMA1_Channel3_IRQn, DMA1_Channel4_IRQn};
static const FullDualInfo DMA1_CH4_CH5_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 3, 4, DMA1_Channel4_IRQn, DMA1_Channel5_IRQn};
static const FullDualInfo DMA1_CH5_CH6_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 4, 5, DMA1_Channel5_IRQn, DMA1_Channel6_IRQn};
#ifdef DMA1_Channel7
static const FullDualInfo DMA1_CH6_CH7_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 5, 6, DMA1_Channel6_IRQn, DMA1_Channel7_IRQn};
#endif
#ifdef DMA1_Channel8
static const FullDualInfo DMA1_CH7_CH8_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 6, 7, DMA1_Channel7_IRQn, DMA1_Channel8_IRQn};
#endif

static const FullDualInfo DMA2_CH1_CH2_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 0, 1, DMA2_Channel1_IRQn, DMA2_Channel2_IRQn};
static const FullDualInfo DMA2_CH2_CH3_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 1, 2, DMA2_Channel2_IRQn, DMA2_Channel3_IRQn};
static const FullDualInfo DMA2_CH3_CH4_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 2, 3, DMA2_Channel3_IRQn, DMA2_Channel4_IRQn};
static const FullDualInfo DMA2_CH4_CH5_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 3, 4, DMA2_Channel4_IRQn, DMA2_Channel5_IRQn};
static const FullDualInfo DMA2_CH5_CH6_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 4, 5, DMA2_Channel5_IRQn, DMA2_Channel6_IRQn};
#ifdef DMA1_Channel7
static const FullDualInfo DMA2_CH6_CH7_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 5, 6, DMA2_Channel6_IRQn, DMA2_Channel7_IRQn};
#endif
#ifdef DMA1_Channel8
static const FullDualInfo DMA2_CH7_CH8_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 6, 7, DMA2_Channel7_IRQn, DMA2_Channel8_IRQn};
#endif

} // namespace dma
} // namespace coco
