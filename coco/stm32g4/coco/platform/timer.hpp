#pragma once

#include "timerAll.hpp"
#include <coco/bits.hpp>


namespace coco {
namespace timer {

/// @brief Timer trigger
/// Reference manual section 28.3.2
/// TRGO is defined by the master mode of the triggering timer
/// OC1 is output 1 after dead time generation
enum class Trigger {
    // TIM1 triggered by...
    TIM1_TIM2_TRGO = TIM_SMCR_TS_0, // tim_itr1
    TIM1_TIM3_TRGO = TIM_SMCR_TS_1, // tim_itr2
    TIM1_TIM4_TRGO = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr3
    TIM1_TIM5_TRGO = TIM_SMCR_TS_3, // tim_itr4
    TIM1_TIM8_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_0, // tim_itr5
    TIM1_TIM15_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_1, // tim_itr6
    TIM1_TIM16_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr7
    TIM1_TIM17_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2, // tim_itr8
#ifdef TIM20
    TIM1_TIM20_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_0, // tim_itr9
#endif
#ifdef HRTIM1
    TIM1_HRTIM_OUT_SYNC2 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_1, // tim_itr10
#endif

    // TIM2 triggered by...
    TIM2_TIM1_TRGO = 0, // tim_itr0
    TIM2_TIM3_TRGO = TIM_SMCR_TS_1, // tim_itr2
    TIM2_TIM4_TRGO = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr3
    TIM2_TIM5_TRGO = TIM_SMCR_TS_3, // tim_itr4
    TIM2_TIM8_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_0, // tim_itr5
    TIM2_TIM15_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_1, // tim_itr6
    TIM2_TIM16_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr7
    TIM2_TIM17_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2, // tim_itr8
#ifdef TIM20
    TIM2_TIM20_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_0, // tim_itr9
#endif
#ifdef HRTIM1
    TIM2_HRTIM_OUT_SYNC2 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_1, // tim_itr10
#endif
    TIM2_USB_SOF_SYNC = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr11

    // TIM3 triggered by...
    TIM3_TIM1_TRGO = 0, // tim_itr0
    TIM3_TIM2_TRGO = TIM_SMCR_TS_0, // tim_itr1
    TIM3_TIM4_TRGO = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr3
    TIM3_TIM5_TRGO = TIM_SMCR_TS_3, // tim_itr4
    TIM3_TIM8_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_0, // tim_itr5
    TIM3_TIM15_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_1, // tim_itr6
    TIM3_TIM16_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr7
    TIM3_TIM17_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2, // tim_itr8
#ifdef TIM20
    TIM3_TIM20_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_0, // tim_itr9
#endif
#ifdef HRTIM1
    TIM3_HRTIM_OUT_SYNC2 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_1, // tim_itr10
#endif

    // TIM4 triggered by...
    TIM4_TIM1_TRGO = 0, // tim_itr0
    TIM4_TIM2_TRGO = TIM_SMCR_TS_0, // tim_itr1
    TIM4_TIM3_TRGO = TIM_SMCR_TS_1, // tim_itr2
    TIM4_TIM5_TRGO = TIM_SMCR_TS_3, // tim_itr4
    TIM4_TIM8_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_0, // tim_itr5
    TIM4_TIM15_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_1, // tim_itr6
    TIM4_TIM16_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr7
    TIM4_TIM17_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2, // tim_itr8
#ifdef TIM20
    TIM4_TIM20_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_0, // tim_itr9
#endif
#ifdef HRTIM1
    TIM4_HRTIM_OUT_SYNC2 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_1, // tim_itr10
#endif

    // TIM5 triggered by...
    TIM5_TIM1_TRGO = 0, // tim_itr0
    TIM5_TIM2_TRGO = TIM_SMCR_TS_0, // tim_itr1
    TIM5_TIM3_TRGO = TIM_SMCR_TS_1, // tim_itr2
    TIM5_TIM4_TRGO = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr3
    TIM5_TIM8_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_0, // tim_itr5
    TIM5_TIM15_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_1, // tim_itr6
    TIM5_TIM16_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr7
    TIM5_TIM17_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2, // tim_itr8
#ifdef TIM20
    TIM5_TIM20_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_0, // tim_itr9
#endif
#ifdef HRTIM1
    TIM5_HRTIM_OUT_SYNC2 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_1, // tim_itr10
#endif

    // TIM8 triggered by...
    TIM8_TIM1_TRGO = 0, // tim_itr0
    TIM8_TIM2_TRGO = TIM_SMCR_TS_0, // tim_itr1
    TIM8_TIM3_TRGO = TIM_SMCR_TS_1, // tim_itr2
    TIM8_TIM4_TRGO = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr3
    TIM8_TIM5_TRGO = TIM_SMCR_TS_3, // tim_itr4
    TIM8_TIM15_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_1, // tim_itr6
    TIM8_TIM16_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr7
    TIM8_TIM17_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2, // tim_itr8
#ifdef TIM20
    TIM8_TIM20_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_0, // tim_itr9
#endif
#ifdef HRTIM1
    TIM8_HRTIM_OUT_SYNC2 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_1, // tim_itr10
#endif

#ifdef TIM20
    // TIM20 triggered by...
    TIM20_TIM1_TRGO = 0, // tim_itr0
    TIM20_TIM2_TRGO = TIM_SMCR_TS_0, // tim_itr1
    TIM20_TIM3_TRGO = TIM_SMCR_TS_1, // tim_itr2
    TIM20_TIM4_TRGO = TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr3
    TIM20_TIM5_TRGO = TIM_SMCR_TS_3, // tim_itr4
    TIM20_TIM8_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_0, // tim_itr5
    TIM20_TIM15_TRGO = TIM_SMCR_TS_3 | TIM_SMCR_TS_1, // tim_itr6
    TIM20_TIM16_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0, // tim_itr7
    TIM20_TIM17_OC1 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2, // tim_itr8
#ifdef HRTIM1
    TIM20_HRTIM_OUT_SYNC2 = TIM_SMCR_TS_3 | TIM_SMCR_TS_2 | TIM_SMCR_TS_1, // tim_itr10
#endif
#endif

    // Any timer triggered by...
    INPUT1_EDGE = TIM_SMCR_TS_2, // Timer Input 1 Edge Detector (tim_ti1f_ed)
    INPUT1 = TIM_SMCR_TS_2 | TIM_SMCR_TS_0, // Filtered Timer Input 1 (tim_ti1fp1)
    INPUT2 = TIM_SMCR_TS_2 | TIM_SMCR_TS_1, // Filtered Timer Input 2 (tim_ti2fp2)
    EXTERNAL = TIM_SMCR_TS_2 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0 // External Trigger Input (tim_etrf)
};


/// @brief Timer input selection
/// Reference manual section 28.3.2, 29.4.2
enum class Input1 {
    // input pin
    CH1 = 0, // default

    TIM1_COMP1 = 1,
    TIM1_COMP2 = 2,
    TIM1_COMP3 = 3,
    TIM1_COMP4 = 4,

    TIM2_COMP1 = 1,
    TIM2_COMP2 = 2,
    TIM2_COMP3 = 3,
    TIM2_COMP4 = 4,
    TIM2_COMP5 = 5,

    TIM3_COMP1 = 1,
    TIM3_COMP2 = 2,
    TIM3_COMP3 = 3,
    TIM3_COMP4 = 4,
    TIM3_COMP5 = 5,
    TIM3_COMP6 = 6,
    TIM3_COMP7 = 7,

    TIM4_COMP1 = 1,
    TIM4_COMP2 = 2,
    TIM4_COMP3 = 3,
    TIM4_COMP4 = 4,
    TIM4_COMP5 = 5,
    TIM4_COMP6 = 6,
    TIM4_COMP7 = 7,

    TIM5_LSI = 1,
    TIM5_LSE = 2,
    TIM5_RTC = 3,
    TIM5_COMP1 = 4,
    TIM5_COMP2 = 5,
    TIM5_COMP3 = 6,
    TIM5_COMP4 = 7,
    TIM5_COMP5 = 8,
    TIM5_COMP6 = 9,
    TIM5_COMP7 = 10,

    TIM8_COMP1 = 1,
    TIM8_COMP2 = 2,
    TIM8_COMP3 = 3,
    TIM8_COMP4 = 4,

    TIM15_LSE = 1,
    TIM15_COMP1 = 2,
    TIM15_COMP2 = 3,
    TIM15_COMP5 = 4,
    TIM15_COMP7 = 5,

    TIM16_COMP6 = 1,
    TIM16_MCO = 2,
    TIM16_HSE_32 = 3,
    TIM16_RTC = 4,
    TIM16_LSE = 5,
    TIM16_LSI = 6,

    TIM17_COMP5 = 1,
    TIM17_MCO = 2,
    TIM17_HSE_32 = 3,
    TIM17_RTC = 4,
    TIM17_LSE = 5,
    TIM17_LSI = 6,

    TIM20_COMP1 = 1,
    TIM20_COMP2 = 2,
    TIM20_COMP3 = 3,
    TIM20_COMP4 = 4,
};
enum class Input2 {
    // input pin
    CH2 = 0, // default

    TIM2_COMP1 = 1,
    TIM2_COMP2 = 2,
    TIM2_COMP3 = 3,
    TIM2_COMP4 = 4,
    TIM2_COMP6 = 5,

    TIM3_COMP1 = 1,
    TIM3_COMP2 = 2,
    TIM3_COMP3 = 3,
    TIM3_COMP4 = 4,
    TIM3_COMP5 = 5,
    TIM3_COMP6 = 6,
    TIM3_COMP7 = 7,

    TIM4_COMP1 = 1,
    TIM4_COMP2 = 2,
    TIM4_COMP3 = 3,
    TIM4_COMP4 = 4,
    TIM4_COMP5 = 5,
    TIM4_COMP6 = 6,
    TIM4_COMP7 = 7,

    TIM5_COMP1 = 1,
    TIM5_COMP2 = 2,
    TIM5_COMP3 = 3,
    TIM5_COMP4 = 4,
    TIM5_COMP5 = 5,
    TIM5_COMP6 = 6,
    TIM5_COMP7 = 7,

    TIM15_COMP2 = 1,
    TIM15_COMP3 = 2,
    TIM15_COMP6 = 3,
    TIM15_COMP7 = 4,
};
enum class Input3 {
    // input pin
    CH3 = 0, // default

    TIM2_COMP4 = 1,

    TIM3_COMP3 = 1,

    TIM4_COMP5 = 1,
};
enum class Input4 {
    // input pin
    CH4 = 0, // default

    TIM2_COMP1 = 1,
    TIM2_COMP2 = 2,

    TIM4_COMP6 = 1,
};


/// @brief Base structure for timer info.
/// @tparam F Feature flags
/// @tparam I Interrupt flags
/// @tparam D DMA request flags
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
    /// @tparam I2 Interrupt request set of info to cast to
    /// @tparam D2 DMA request set of info to cast to
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

    /// @brief Map DMA channel to a DMA request of the timer
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
using AcInfo = Info<Feature::SLAVE | Feature::CC1_4 | Feature::COMPARE5_6 | Feature::DEAD_TIME | Feature::ENCODER, Irq::SEPARATE, Drq::ALL>;
using AcInfo2 = Info<Feature::SLAVE | Feature::CC1_4 | Feature::COMPARE5_6 | Feature::DEAD_TIME | Feature::ENCODER, Irq::SEPARATE, Drq::CH1_4 | Drq::UPDATE>;

// basic timers
static const BasicInfo TIM6_INFO{TIM6, {&RCC->APB1ENR1, RCC_APB1ENR1_TIM6EN}, {TIM6_DAC_IRQn}};
static const BasicInfo TIM7_INFO{TIM7, {&RCC->APB1ENR1, RCC_APB1ENR1_TIM7EN}, {TIM7_DAC_IRQn}};

// general purpose timers
static const FullGpInfo32 TIM2_INFO{TIM2, {&RCC->APB1ENR1, RCC_APB1ENR1_TIM2EN}, {TIM2_IRQn}}; // 32 bit
static const FullGpInfo TIM3_INFO{TIM3, {&RCC->APB1ENR1, RCC_APB1ENR1_TIM3EN}, {TIM3_IRQn}};
static const FullGpInfo TIM4_INFO{TIM4, {&RCC->APB1ENR1, RCC_APB1ENR1_TIM4EN}, {TIM4_IRQn}};
#ifdef TIM5
static const FullGpInfo32 TIM5_INFO{TIM5, {&RCC->APB1ENR1, RCC_APB1ENR1_TIM5EN}, {TIM5_IRQn}}; // 32 bit
#endif
static const BasicGpInfo2 TIM15_INFO{TIM15, {&RCC->APB2ENR, RCC_APB2ENR_TIM15EN}, {TIM1_BRK_TIM15_IRQn}};
static const BasicGpInfo TIM16_INFO{TIM16, {&RCC->APB2ENR, RCC_APB2ENR_TIM16EN}, {TIM1_UP_TIM16_IRQn}};
static const BasicGpInfo TIM17_INFO{TIM17, {&RCC->APB2ENR, RCC_APB2ENR_TIM17EN}, {TIM1_TRG_COM_TIM17_IRQn}};

// advanced control timers
static const AcInfo TIM1_INFO{TIM1, {&RCC->APB2ENR, RCC_APB2ENR_TIM1EN}, {TIM1_BRK_TIM15_IRQn, TIM1_UP_TIM16_IRQn, TIM1_TRG_COM_TIM17_IRQn, TIM1_CC_IRQn}, {42, 43, 44, 45, 46, 47, 48}};
static const AcInfo TIM8_INFO{TIM8, {&RCC->APB2ENR, RCC_APB2ENR_TIM8EN}, {TIM8_BRK_IRQn, TIM8_UP_IRQn, TIM8_TRG_COM_IRQn, TIM8_CC_IRQn}, {49, 50, 51, 52, 53, 54, 55}};
#ifdef TIM20
static const AcInfo2 TIM20_INFO{TIM20, {&RCC->APB2ENR, RCC_APB2ENR_TIM20EN}, {TIM20_BRK_IRQn, TIM20_UP_IRQn, TIM20_TRG_COM_IRQn, TIM20_CC_IRQn}, {86, 87, 88, 89, 90}};
#endif

} // namespace timer
} // namespace coco
