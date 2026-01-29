#pragma once

#include "timerAll.hpp"
#include <coco/bits.hpp>


namespace coco {
namespace timer {

/// @brief Timer trigger
/// Reference manual section 15.4.3
enum class Trigger {
    // TIM1 triggered by...
    TIM1_TIM3_TRGO = TIM_SMCR_TS_1, // ITR2
    TIM1_TIM17_OC1 = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // ITR3

    // TIM3 triggered by...
    TIM3_TIM1_TRGO = 0, // ITR0
    TIM3_TIM14_OC1 = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // ITR3

    // Any timer triggered by...
    INPUT1_EDGE = TIM_SMCR_TS_2, // Timer Input 1 Edge Detector (TI1F_ED)
    INPUT1 = TIM_SMCR_TS_2 | TIM_SMCR_TS_0, // Filtered Timer Input 1 (TI1FP1)
    INPUT2 = TIM_SMCR_TS_2 | TIM_SMCR_TS_1, // Filtered Timer Input 2 (TI2FP2)
    EXTERNAL = TIM_SMCR_TS_2 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0 // External Trigger Input (ETRF)
};


/// @brief Timer input selection
/// Reference manual section 15.4.28, 16.4.23, 17.4.12, 18.4.20
enum class Input1 {
    // input pin
    CH1 = 0, // default

    TIM14_RTC = 1,
    TIM14_HSE_32 = 2,
    TIM14_MCO = 3,
    TIM14_MCO2 = 4,

    TIM16_LSI = 1,
    TIM16_LSE = 2,
    TIM16_MCO2 = 4,

    TIM17_HSE_32 = 2,
    TIM17_MCO = 3,
    TIM17_MCO2 = 4,
};
enum class Input2 {
    // input pin
    CH2 = 0, // default
};
enum class Input3 {
    // input pin
    CH3 = 0, // default
};
enum class Input4 {
    // input pin
    CH4 = 0, // default
};


/// @brief Base structure for timer info.
/// @tparam F Feature flags
/// @tparam I Interrupt flags
template <Feature F, Irq I = Irq::NONE, Drq D = Drq::NONE>
struct Info {
    using Instance = timer::Instance<F>;

    // registers
    TIM_TypeDef *timer;

    // reset and clock control
    rcc::Info rcc;

    // interrupt index
    uint8_t irqs[std::max(popcount(I), 1)];

    uint8_t drqs[std::max(popcount(D), 1)];


    /// @brief Enable clock and return an instance wrapper.
    /// @return Instance (wrapper for registers)
    Instance enableClock() const {
        rcc.enableClock();
        return {timer};
    }

    /// @brief Get the instance
    /// @return Instance (wrapper for registers)
    Instance instance() const {
        return {timer};
    }

    /// @brief Cast to another info structure.
    /// Requires that all features, interrupts and DMA requests are supported.
    /// @tparam F2 Feature set of info to cast to
    /// @tparam I2 Interrupt set of info to cast to
    template <Feature F2, Irq I2, Drq D2> requires ((F2 & F) == F2 && ((I2 & I) == I2 || (I & Irq::COMBINED) != 0) && (D2 & D) == D2)
    operator Info<F2, I2, D2> () const {
        Info<F2, I2, D2> info;
        info.timer = timer;
        info.rcc = rcc;
        for (int bits = int(I2); bits != 0; bits &= bits - 1) {
            int bit = bits & ~(bits - 1);
            info.irqs[popcountBefore(I2, bit)] = (int(I) & bit) != 0 ? irqs[popcountBefore(I, bit)] : irqs[0];
        }
        for (int bits = int(D2); bits != 0; bits &= bits - 1) {
            int bit = bits & ~(bits - 1);
            info.drqs[popcountBefore(D2, bit)] = drqs[popcountBefore(D, bit)];
        }
        return info;
    }

    template <Irq I2>
    int irq() const {
        static_assert(popcount(I2 & I) == 1, "irq() requires exactly one supported interrupt request flag");
        return irqs[popcountBefore(I, I2)];
    }

    /// @brief Get DMA request index
    /// @tparam D2 DMA request to get index for (e.g. Drq::UPDATE)
    /// @return Index of DMA request
    template <Drq D2>
    int drq() const {
        static_assert(popcount(D2 & D) == 1, "drq() requires exactly one supported DMA request flag");
        return drqs[popcountBefore(D, D2)];
    }

    /// @brief Map DMA channel to the timer
    /// @tparam D2 DMA request to map (e.g. Drq::UPDATE)
    /// @param dmaInfo DMA channel info
    template <Drq D2, dma::Feature F2>
    void map(const dma::Info<F2> &dmaInfo) const {
        static_assert(popcount(D2 & D) == 1, "map() requires exactly one supported DMA request flag");
        dmaInfo.setRequest(drqs[popcountBefore(D, D2)]);
    }
};


using BasicInfo = Info<Feature::NONE, Irq::COMBINED>;
using FullGpInfo = Info<Feature::SLAVE | Feature::CC1_4 | Feature::ENCODER, Irq::COMBINED>;
using FullGpInfo32 = Info<Feature::BITS_32 | Feature::SLAVE | Feature::CC1_4 | Feature::ENCODER, Irq::COMBINED>;
using BasicGpInfo = Info<Feature::CC1 | Feature::DEAD_TIME, Irq::COMBINED>;
using BasicGpInfo2 = Info<Feature::CC1_2 | Feature::DEAD_TIME, Irq::COMBINED>;
using AcInfo = Info<Feature::SLAVE | Feature::CC1_4 | Feature::DEAD_TIME | Feature::ENCODER, Irq::SEPARATE, Drq::ALL>;
using AcInfo2 = Info<Feature::SLAVE | Feature::CC1_4 | Feature::DEAD_TIME | Feature::ENCODER, Irq::SEPARATE, Drq::CH1_4 | Drq::UPDATE>;

// general purpose timers
static const FullGpInfo TIM3_INFO{TIM3, {&RCC->APBENR1, RCC_APBENR1_TIM3EN}, {TIM3_IRQn}};
static const BasicGpInfo TIM14_INFO{TIM14, {&RCC->APBENR2, RCC_APBENR2_TIM14EN}, {TIM14_IRQn}};
static const BasicGpInfo TIM16_INFO{TIM16, {&RCC->APBENR2, RCC_APBENR2_TIM16EN}, {TIM16_IRQn}};
static const BasicGpInfo TIM17_INFO{TIM17, {&RCC->APBENR2, RCC_APBENR2_TIM17EN}, {TIM17_IRQn}};

// advanced control timers
static const AcInfo TIM1_INFO{TIM1, {&RCC->APBENR2, RCC_APBENR2_TIM1EN}, {TIM1_BRK_UP_TRG_COM_IRQn, TIM1_BRK_UP_TRG_COM_IRQn, TIM1_BRK_UP_TRG_COM_IRQn, TIM1_CC_IRQn}, {20, 21, 22, 23, 24, 24, 25}};

} // namespace timer
} // namespace coco
