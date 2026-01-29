#pragma once

#include "timerAll.hpp"
#include <coco/bits.hpp>


namespace coco {
namespace timer {

/// @brief Timer trigger
/// Reference manual section 17.4.3
enum class Trigger {
    // TIM1 triggered by...
    TIM1_TIM15_TRGO = 0, // ITR0
    TIM1_TIM2_TRGO = TIM_SMCR_TS_0, // ITR1
    TIM1_TIM3_TRGO = TIM_SMCR_TS_1, // ITR2
    TIM1_TIM17_TRGO = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // ITR3

    // TIM2 triggered by...
    TIM2_TIM1_TRGO = 0, // ITR0
    TIM2_TIM15_TRGO = TIM_SMCR_TS_0, // ITR1
    TIM2_TIM3_TRGO = TIM_SMCR_TS_1, // ITR2
    TIM2_TIM14_TRGO = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // ITR3

    // TIM3 triggered by...
    TIM3_TIM1_TRGO = 0, // ITR0
    TIM3_TIM2_TRGO = TIM_SMCR_TS_0, // ITR1
    TIM3_TIM15_TRGO = TIM_SMCR_TS_1, // ITR2
    TIM3_TIM14_TRGO = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // ITR3

    // Any timer triggered by...
    INPUT1_EDGE = TIM_SMCR_TS_2, // TI1 Edge Detector
    INPUT1 = TIM_SMCR_TS_2 | TIM_SMCR_TS_0, // Filtered Timer Input 1
    INPUT2 = TIM_SMCR_TS_2 | TIM_SMCR_TS_1, // Filtered Timer Input 2
    EXTERNAL = TIM_SMCR_TS_2 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0 // External Trigger Input
};


/// @brief Timer input selection
/// F0 lacks this feature
enum class Input1 {
    // input pin
    CH1 = 0, // default
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
template <Feature F, Irq I = Irq::NONE>
struct Info {
    using Instance = timer::Instance<F>;

    // registers
    TIM_TypeDef *timer;

    // reset and clock control
    rcc::Info<offsetof(RCC_TypeDef, APB1RSTR) - offsetof(RCC_TypeDef, APB1ENR)> rcc;

    // interrupt index
    uint8_t irqs[std::max(popcount(I), 1)];


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
    /// @tparam I2 Interrupt request set of info to cast to
    template <Feature F2, Irq I2> requires ((F2 & F) == F2 && ((I2 & I) == I2 || (I & Irq::COMBINED) != 0))
    operator Info<F2, I2> () const {
        Info<F2, I2> info;
        info.timer = timer;
        info.rcc = rcc;
        for (int bits = int(I2); bits != 0; bits &= bits - 1) {
            int bit = bits & ~(bits - 1);
            info.irqs[popcountBefore(I2, bit)] = (int(I) & bit) != 0 ? irqs[popcountBefore(I, bit)] : irqs[0];
        }
        return info;
    }

    template <Irq I2>
    int irq() const {
        static_assert(popcount(I2 & I) == 1, "irq() requires exactly one supported interrupt request flag");
        return irqs[popcountBefore(I, I2)];
    }
};


using BasicInfo = Info<Feature::NONE, Irq::COMBINED>;
using FullGpInfo = Info<Feature::SLAVE | Feature::CC1_4 | Feature::ENCODER, Irq::COMBINED>;
using FullGpInfo32 = Info<Feature::BITS_32 | Feature::SLAVE | Feature::CC1_4 | Feature::ENCODER, Irq::COMBINED>;
using BasicGpInfo = Info<Feature::CC1 | Feature::DEAD_TIME, Irq::COMBINED>;
using BasicGpInfo2 = Info<Feature::CC1_2 | Feature::DEAD_TIME, Irq::COMBINED>;
using AcInfo = Info<Feature::SLAVE | Feature::CC1_4 | Feature::DEAD_TIME | Feature::ENCODER, Irq::SEPARATE>;


// basic timers
#ifdef TIM6
constexpr BasicInfo TIM6_INFO{TIM6, {&RCC->APB1ENR, RCC_APB1ENR_TIM6EN}, {TIM6_DAC_IRQn}};
#endif
#ifdef TIM7
constexpr BasicInfo TIM7_INFO{TIM7, {&RCC->APB1ENR, RCC_APB1ENR_TIM7EN}, {TIM7_IRQn}};
#endif

// general purpose timers
#ifdef TIM2
constexpr FullGpInfo32 TIM2_INFO{TIM2, {&RCC->APB1ENR, RCC_APB1ENR_TIM2EN}, {TIM2_IRQn}}; // 32 bit
#endif
#ifdef TIM3
constexpr FullGpInfo TIM3_INFO{TIM3, {&RCC->APB1ENR, RCC_APB1ENR_TIM3EN}, {TIM3_IRQn}};
#endif
static const BasicGpInfo TIM14_INFO{TIM14, {&RCC->APB2ENR, RCC_APB1ENR_TIM14EN}, {TIM14_IRQn}};
#ifdef TIM15
static const BasicGpInfo2 TIM15_INFO{TIM15, {&RCC->APB2ENR, RCC_APB2ENR_TIM15EN}, {TIM15_IRQn}};
#endif
static const BasicGpInfo TIM16_INFO{TIM16, {&RCC->APB2ENR, RCC_APB2ENR_TIM16EN}, {TIM16_IRQn}};
static const BasicGpInfo TIM17_INFO{TIM17, {&RCC->APB2ENR, RCC_APB2ENR_TIM17EN}, {TIM17_IRQn}};

// advanced control timers
static const AcInfo TIM1_INFO{TIM1, {&RCC->APB2ENR, RCC_APB2ENR_TIM1EN}, {TIM1_BRK_UP_TRG_COM_IRQn, TIM1_BRK_UP_TRG_COM_IRQn, TIM1_BRK_UP_TRG_COM_IRQn, TIM1_CC_IRQn}};

} // namespace timer
} // namespace coco
