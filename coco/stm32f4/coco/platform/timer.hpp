#pragma once

#include "platform.hpp"
#include "rcc.hpp"
#include "timerAll.hpp"
#include <coco/bits.hpp>


namespace coco {
namespace timer {

/// @brief Timer trigger
/// Reference manual section 17.4.3
enum class Trigger {
    // TIM1 triggered by...
    TIM1_TIM5_TRGO = 0, // ITR0
    TIM1_TIM2_TRGO = TIM_SMCR_TS_0, // ITR1
    TIM1_TIM3_TRGO = TIM_SMCR_TS_1, // ITR2
    TIM1_TIM4_TRGO = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // ITR3

    // TIM8 triggered by...
    TIM8_TIM1_TRGO = 0, // ITR0
    TIM8_TIM2_TRGO = TIM_SMCR_TS_0, // ITR1
    TIM8_TIM4_TRGO = TIM_SMCR_TS_1, // ITR2
    TIM8_TIM5_TRGO = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // ITR3

    // Any timer triggered by...
    INPUT1_EDGE = TIM_SMCR_TS_2, // TI1 Edge Detector
    INPUT1 = TIM_SMCR_TS_2 | TIM_SMCR_TS_0, // Filtered Timer Input 1
    INPUT2 = TIM_SMCR_TS_2 | TIM_SMCR_TS_1, // Filtered Timer Input 2
    EXTERNAL = TIM_SMCR_TS_2 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0 // External Trigger Input
};


/// @brief Timer input selection
/// F4 lacks this feature
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
    rcc::Info rcc;

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
static const BasicInfo TIM6_INFO{TIM6, {&RCC->APB1ENR, RCC_APB1ENR_TIM6EN}, {TIM6_DAC_IRQn}};
#endif
#ifdef TIM7
static const BasicInfo TIM7_INFO{TIM7, {&RCC->APB1ENR, RCC_APB1ENR_TIM7EN}, {TIM7_IRQn}};
#endif

// general purpose timers
static const FullGpInfo32 TIM2_INFO{TIM2, {&RCC->APB1ENR, RCC_APB1ENR_TIM2EN}, {TIM2_IRQn}}; // 32 bit
#ifdef TIM3
static const FullGpInfo TIM3_INFO{TIM3, {&RCC->APB1ENR, RCC_APB1ENR_TIM3EN}, {TIM3_IRQn}};
#endif
static const FullGpInfo TIM4_INFO{TIM4, {&RCC->APB1ENR, RCC_APB1ENR_TIM4EN}, {TIM4_IRQn}};
static const FullGpInfo32 TIM5_INFO{TIM5, {&RCC->APB1ENR, RCC_APB1ENR_TIM5EN}, {TIM5_IRQn}}; // 32 bit
static const BasicGpInfo TIM9_INFO{TIM9, {&RCC->APB2ENR, RCC_APB2ENR_TIM9EN}, {TIM1_BRK_TIM9_IRQn}};
#ifdef TIM10
static const BasicGpInfo TIM10_INFO{TIM10, {&RCC->APB2ENR, RCC_APB2ENR_TIM10EN}, {TIM1_UP_TIM10_IRQn}};
#endif
static const BasicGpInfo TIM11_INFO{TIM11, {&RCC->APB2ENR, RCC_APB2ENR_TIM11EN}, {TIM1_TRG_COM_TIM11_IRQn}};
#ifdef TIM12
static const BasicGpInfo2 TIM12_INFO{TIM12, {&RCC->APB1ENR, RCC_APB1ENR_TIM12EN}, {TIM8_BRK_TIM12_IRQn}};
#endif
#ifdef TIM13
static const BasicGpInfo TIM13_INFO{TIM13, {&RCC->APB1ENR, RCC_APB1ENR_TIM13EN}, {TIM8_UP_TIM13_IRQn}};
#endif
#ifdef TIM14
static const BasicGpInfo TIM14_INFO{TIM14, {&RCC->APB1ENR, RCC_APB1ENR_TIM14EN}, {TIM8_TRG_COM_TIM14_IRQn}};
#endif

// advanced control timers
static const AcInfo TIM1_INFO{TIM1, {&RCC->APB2ENR, RCC_APB2ENR_TIM1EN}, {TIM1_BRK_TIM9_IRQn, TIM1_UP_TIM10_IRQn, TIM1_TRG_COM_TIM11_IRQn, TIM1_CC_IRQn}};
#ifdef TIM8
static const AcInfo TIM8_INFO{TIM8, {&RCC->APB2ENR, RCC_APB2ENR_TIM8EN}, {TIM8_BRK_TIM12_IRQn, TIM8_UP_TIM13_IRQn, TIM8_TRG_COM_TIM14_IRQn, TIM8_CC_IRQn}};
#endif

} // namespace timer
} // namespace coco
