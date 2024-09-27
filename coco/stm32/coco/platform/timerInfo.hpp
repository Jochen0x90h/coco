#pragma once

#include <coco/enum.hpp>
#include <coco/Frequency.hpp>
#include <coco/Time.hpp>


// do not include directly, use #include <coco/platform/timer.hpp>

namespace coco {

/// @brief Timer helpers
///
namespace timer {

// device specific triggers
enum class Trigger;

/// @brief Master Mode (is in timer CR2 register), generates TRGO to trigger other timer, ADC, DAC etc.
///
enum class MasterMode : uint32_t {
    RESET = 0, // reset generates TRGO (the UG bit from the TIMx_EGR register is used as trigger output)
    ENABLE = TIM_CR2_MMS_0, // enabling the counter generates TRGO (the Counter Enable signal CNT_EN is used as trigger output)
    UPDATE = TIM_CR2_MMS_1, // update (overflow) generates TRGO (the update event is selected as trigger output)
    PULSE_CC1IF = TIM_CR2_MMS_1 | TIM_CR2_MMS_0, // the trigger output sends a positive pulse when the CC1IF flag is to be set (even if it was already high), as soon as a capture or compare match occurs (tim_trgo2)
    COMPARE1 = TIM_CR2_MMS_2, //  tim_oc1refc signal is used as trigger output (tim_trgo2)
    COMPARE2 = TIM_CR2_MMS_2 | TIM_CR2_MMS_0, // tim_oc2refc signal is used as trigger output (tim_trgo2)
    COMPARE3 = TIM_CR2_MMS_2 | TIM_CR2_MMS_1, // tim_oc3refc signal is used as trigger output (tim_trgo2)
    COMPARE4 = TIM_CR2_MMS_2 | TIM_CR2_MMS_1 | TIM_CR2_MMS_0, // tim_oc4refc signal is used as trigger output (tim_trgo2)
#ifdef TIM_CR2_MMS_3
    COMPARE5 = TIM_CR2_MMS_3, // tim_oc5refc signal is used as trigger output (tim_trgo2)
    COMPARE6 = TIM_CR2_MMS_3 | TIM_CR2_MMS_0, // tim_oc6refc signal is used as trigger output (tim_trgo2)
    PULSE_BOTH4 = TIM_CR2_MMS_3 | TIM_CR2_MMS_1, //  tim_oc4refc rising or falling edges generate pulses on tim_trgo2
    PULSE_BOTH6 = TIM_CR2_MMS_3 | TIM_CR2_MMS_1 | TIM_CR2_MMS_0, // tim_oc6refc rising or falling edges generate pulses on tim_trgo2
    PULSE_RISING4_RISING6 = TIM_CR2_MMS_3 | TIM_CR2_MMS_2, // tim_oc4refc or tim_oc6refc rising edges generate pulses on tim_trgo2
    PULSE_RISING4_FALLING6 = TIM_CR2_MMS_3 | TIM_CR2_MMS_2 | TIM_CR2_MMS_0, // tim_oc4refc rising or tim_oc6refc falling edges generate pulses on tim_trgo2
    PULSE_RISING5_RISING6 = TIM_CR2_MMS_3 | TIM_CR2_MMS_2 | TIM_CR2_MMS_1, // tim_oc5refc or tim_oc6refc rising edges generate pulses on tim_trgo2
    PULSE_RISING5_FALLING6 = TIM_CR2_MMS_3 | TIM_CR2_MMS_2 | TIM_CR2_MMS_1 | TIM_CR2_MMS_0 // tim_oc5refc rising or tim_oc6refc falling edges generate pulses on tim_trgo2
#endif
};

/// @brief Trigger mode (is in timer SMCR register)
///
enum class TriggerMode : uint32_t {
    DISABLED = 0,

    RESET = TIM_SMCR_SMS_2,
    GATED = TIM_SMCR_SMS_2 | TIM_SMCR_SMS_0,
    START = TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1,
    CLOCK = TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1 | TIM_SMCR_SMS_0,
#ifdef TIM_SMCR_SMS_3
    RESET_START = TIM_SMCR_SMS_3,
    GATED_RESET = TIM_SMCR_SMS_3 | TIM_SMCR_SMS_0
#endif
};

/// @brief Encoder mode (is in timer SMCR register)
///
enum class EncoderMode : uint32_t {
    DISABLED = 0,

    // encoder mode
    QUADRATURE1_X2 = TIM_SMCR_SMS_1,
    QUADRATURE2_X2 = TIM_SMCR_SMS_2,
    QUADRATURE_X4 = TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1,
#ifdef TIM_SMCR_SMS_3
    CLOCK_DIRECTION_X2 = TIM_SMCR_SMS_3 | TIM_SMCR_SMS_1,
#endif

};
COCO_ENUM(EncoderMode)

/// @brief Compare mode (is in timer CCMRx register)
///
enum class CompareMode : uint32_t {
    DISABLED = 0,

    // compare mode (should use one)
    FROZEN = 0, // default
    SET_ON_MATCH = TIM_CCMR1_OC1M_0,
    CLEAR_ON_MATCH = TIM_CCMR1_OC1M_1,
    TOGGLE_ON_MATCH = TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0,
    SET = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_0,
    CLEAR = TIM_CCMR1_OC1M_2,
    PWM1 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1,
    PWM2 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0,
#ifdef TIM_CCMR1_OC1M_3
    OPM1 = TIM_CCMR1_OC1M_3,
    OPM2 = TIM_CCMR1_OC1M_3 | TIM_CCMR1_OC1M_0,
    COMBINED_PWM1 = TIM_CCMR1_OC1M_3 | TIM_CCMR1_OC1M_2,
    COMBINED_PWM2 = TIM_CCMR1_OC1M_3 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_0,
    ASYMMETRIC_PWM1 = TIM_CCMR1_OC1M_3 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1,
    ASYMMETRIC_PWM2 = TIM_CCMR1_OC1M_3 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0,
#endif

    // optional enable flags
    CLEAR_ENABLE = TIM_CCMR1_OC1CE,
    PRELOAD_ENABLE = TIM_CCMR1_OC1PE,
    FAST_ENABLE = TIM_CCMR1_OC1FE,

    // optional output pins (see reference manual table 267, also use setBreakDeadTime() if it is an advanced control timer)
    OUTPUT_NORMAL = TIM_CCER_CC1E << 8,
    OUTPUT_INVERTED = (TIM_CCER_CC1E | TIM_CCER_CC1P) << 8,
    OUTPUT_N_NORMAL = TIM_CCER_CC1NE << 8,
    OUTPUT_N_INVERTED = (TIM_CCER_CC1NE | TIM_CCER_CC1NP) << 8,
};
COCO_ENUM(CompareMode)

/// @brief Capture configuration (is in timer CCMRx register)
enum class CaptureMode : uint32_t {
    DISABLED = 0,

    // edge selection for TriggerMode::RESET, TriggerMode::START, TriggerMode::CLOCK, TriggerMode::RESET_START and TriggerMode::GATED_RESET
    EDGE_RISING = TIM_CCER_CC1E << 8,
    EDGE_FALLING = (TIM_CCER_CC1E | TIM_CCER_CC1P) << 8,
    EDGE_BOTH = (TIM_CCER_CC1E | TIM_CCER_CC1P | TIM_CCER_CC1NP) << 8,

    // polarity selection for TriggerMode::GATED and EncoderMode (must use one of either EDGE_* or POLARITY_* unless DISABLED)
    POLARITY_NORMAL = EDGE_RISING,
    POLARITY_INVERTED = EDGE_FALLING,

    // capture input selection (must use one unless DISABLED)
    INPUT_DEFAULT = TIM_CCMR1_CC1S_0,
    INPUT_ALTERNATE = TIM_CCMR1_CC1S_1,
    INPUT_TRIGGER = TIM_CCMR1_CC1S_1 | TIM_CCMR1_CC1S_0,

    // optional capture prescaler
    PRESCALER_NONE = 0, // default
    PRESCALER_2 = TIM_CCMR1_IC1PSC_0,
    PRESCALER_4 = TIM_CCMR1_IC1PSC_1,
    PRESCALER_8 = TIM_CCMR1_IC1PSC_1 | TIM_CCMR1_IC1PSC_0,

    // optional capture filter
    FILTER_FDTS_NONE = 0, // default
    FILTER_FKER_N2 = TIM_CCMR1_IC1F_0,
    FILTER_FKER_N4 = TIM_CCMR1_IC1F_1,
    FILTER_FKER_N8 = TIM_CCMR1_IC1F_1 | TIM_CCMR1_IC1F_0,
    FILTER_FDTS_2_N6 = TIM_CCMR1_IC1F_2,
    FILTER_FDTS_2_N8 = TIM_CCMR1_IC1F_2 | TIM_CCMR1_IC1F_0,
    FILTER_FDTS_4_N6 = TIM_CCMR1_IC1F_2 | TIM_CCMR1_IC1F_1,
    FILTER_FDTS_4_N8 = TIM_CCMR1_IC1F_2 | TIM_CCMR1_IC1F_1 | TIM_CCMR1_IC1F_0,
    FILTER_FDTS_8_N6 = TIM_CCMR1_IC1F_3,
    FILTER_FDTS_8_N8 = TIM_CCMR1_IC1F_3 | TIM_CCMR1_IC1F_0,
    FILTER_FDTS_16_N5 = TIM_CCMR1_IC1F_3 | TIM_CCMR1_IC1F_1,
    FILTER_FDTS_16_N6 = TIM_CCMR1_IC1F_3 | TIM_CCMR1_IC1F_1 | TIM_CCMR1_IC1F_0,
    FILTER_FDTS_16_N8 = TIM_CCMR1_IC1F_3 | TIM_CCMR1_IC1F_2,
    FILTER_FDTS_32_N5 = TIM_CCMR1_IC1F_3 | TIM_CCMR1_IC1F_2 | TIM_CCMR1_IC1F_0,
    FILTER_FDTS_32_N6 = TIM_CCMR1_IC1F_3 | TIM_CCMR1_IC1F_2 | TIM_CCMR1_IC1F_1,
    FILTER_FDTS_32_N8 = TIM_CCMR1_IC1F_3 | TIM_CCMR1_IC1F_2 | TIM_CCMR1_IC1F_1 | TIM_CCMR1_IC1F_0,
};
COCO_ENUM(CaptureMode)

enum class BreakDeadTimeMode : uint32_t {
    MAIN_OUTPUT_ENABLE = TIM_BDTR_MOE,
};
COCO_ENUM(BreakDeadTimeMode)

enum class RunMode : uint32_t {
    DISABLED = 0,

    // run mode (must use one unless DISABLED)
    UP = TIM_CR1_CEN,
    DOWN = TIM_CR1_CEN | TIM_CR1_DIR,
    CENTER_CCxIF_DOWN = TIM_CR1_CEN | TIM_CR1_CMS_0, // The counter counts up and down alternatively. Output compare  interrupt flags of channels configured in output (CCxS=00 in TIMx_CCMRx register) are set only when the counter is counting down.
    CENTER_CCxIF_UP = TIM_CR1_CEN | TIM_CR1_CMS_1, // The counter counts up and down alternatively. Output compare interrupt flags of channels configured in output (CCxS=00 in TIMx_CCMRx register) are  set only when the counter is counting up.
    CENTER = TIM_CR1_CEN | TIM_CR1_CMS_1 | TIM_CR1_CMS_0, // The counter counts up and down alternatively. Output compare interrupt flags of channels configured in output (CCxS=00 in TIMx_CCMRx register) are set both when the counter is counting up or down.

    // optional one pulse flag, counter stops counting at the next update event (clearing the CEN bit)
    ONE_PULSE = TIM_CR1_OPM,
};
COCO_ENUM(RunMode)


/// @brief Wrapper for Timer registers
///
struct Registers {
    TIM_TypeDef *timer;


    TIM_TypeDef *operator ->() {return this->timer;}

    /// @brief Set master mode which controls trigger output TRGO
    /// @param masterMode
    /// @return this
    auto &setMaster(MasterMode masterMode) const {
        auto timer = this->timer;
        timer->CR2 = uint32_t(masterMode);
        return *this;
    }

    /// @brief Set trigger mode
    /// @param triggerMode trigger mode
    /// @param trigger trigger source
    /// @return this
    auto &setTrigger(TriggerMode triggerMode, Trigger trigger) const {
        auto timer = this->timer;
        timer->SMCR = uint32_t(trigger) | uint32_t(triggerMode);
        return *this;
    }

    /// @brief Set encoder mode (is exclusive with trigger mode)
    /// @param encoderMode encoder mode
    /// @return this
    auto &setEncoder(EncoderMode encoderMode) const {
        auto timer = this->timer;
        timer->SMCR = uint32_t(encoderMode);
        return *this;
    }

    /// @brief Set compare mode for register 1
    /// @param compareMode compare mode
    /// @param value compare value
    /// @return
    auto &setCompare1(CompareMode compareMode, uint32_t value) const {
        auto timer = this->timer;
        timer->CCR1 = value;
        timer->CCMR1 = (timer->CCMR1 & 0xff00ff00) | (uint32_t(compareMode) & 0x00ff00ff);
        timer->CCER = (timer->CCER & 0xfffffff0) | ((uint32_t(compareMode) >> 8) & 0x0000000f);
        return *this;
    }
    auto &setCompare2(CompareMode compareMode, uint32_t value) const {
        auto timer = this->timer;
        timer->CCR2 = value;
        timer->CCMR1 = (timer->CCMR1 & 0x00ff00ff) | ((uint32_t(compareMode) << 8) & 0xff00ff00);
        timer->CCER = (timer->CCER & 0xffffff0f) | ((uint32_t(compareMode) >> 4) & 0x000000f0);
        return *this;
    }
    auto &setCompare3(CompareMode compareMode, uint32_t value) const {
        auto timer = this->timer;
        timer->CCR3 = value;
        timer->CCMR2 = (timer->CCMR2 & 0xff00ff00) | (uint32_t(compareMode) & 0x00ff00ff);
        timer->CCER = (timer->CCER & 0xfffff0ff) | (uint32_t(compareMode) & 0x00000f00);
        return *this;
    }
    auto &setCompare4(CompareMode compareMode, uint32_t value) const {
        auto timer = this->timer;
        timer->CCR4 = value;
        timer->CCMR2 = (timer->CCMR2 & 0x00ff00ff) | ((uint32_t(compareMode) << 8) & 0xff00ff00);
        timer->CCER = (timer->CCER & 0xffff0fff) | ((uint32_t(compareMode) << 4) & 0x0000f000);
        return *this;
    }
#ifdef TIM_CCMR3_OC5M
    auto &setCompare5(CompareMode compareMode, uint32_t value) const {
        auto timer = this->timer;
        timer->CCR4 = value;
        timer->CCMR3 = (timer->CCMR3 & 0xff00ff00) | (uint32_t(compareMode) & 0x00ff00ff);
        timer->CCER = (timer->CCER & 0xfff0ffff) | ((uint32_t(compareMode) << 8) & 0x000f0000);
        return *this;
    }
    auto &setCompare6(CompareMode compareMode, uint32_t value) const {
        auto timer = this->timer;
        timer->CCR6 = value;
        timer->CCMR3 = (timer->CCMR3 & 0x00ff00ff) | ((uint32_t(compareMode) << 8) & 0xff00ff00);
        timer->CCER = (timer->CCER & 0xff0fffff) | ((uint32_t(compareMode) << 12) & 0x00f00000);
        return *this;
    }
#endif

    /// @brief Set capture mode for register 1
    /// @param captureMode capture mode
    /// @return this
    auto &setCapture1(CaptureMode captureMode) const {
        auto timer = this->timer;
        timer->CCMR1 = (timer->CCMR1 & 0xff00) | (uint32_t(captureMode) & 0x00ff);
        timer->CCER = (timer->CCER & 0xfffffff0) | ((uint32_t(captureMode) >> 8) & 0x0000000f);
        return *this;
    }
    auto &setCapture2(CaptureMode captureMode) const {
        auto timer = this->timer;
        timer->CCMR1 = (timer->CCMR1 & 0x00ff) | ((uint32_t(captureMode) << 8) & 0xff00);
        timer->CCER = (timer->CCER & 0xffffff0f) | ((uint32_t(captureMode) >> 4) & 0x000000f0);
        return *this;
    }
    auto &setCapture3(CaptureMode captureMode) const {
        auto timer = this->timer;
        timer->CCMR2 = (timer->CCMR2 & 0xff00) | (uint32_t(captureMode) & 0x00ff);
        timer->CCER = (timer->CCER & 0xfffff0ff) | (uint32_t(captureMode) & 0x00000f00);
        return *this;
    }
    auto &setCapture4(CaptureMode captureMode) const {
        auto timer = this->timer;
        timer->CCMR2 = (timer->CCMR2 & 0x00ff) | ((uint32_t(captureMode) << 8) & 0xff00);
        timer->CCER = (timer->CCER & 0xffff0fff) | ((uint32_t(captureMode) << 4) & 0x0000f000);
        return *this;
    }

    /// @brief Set break and dead-time for advanced control timer
    /// @param breakDeadTimeMode brak and dead-time mode
    /// @return this
    auto &setBreakDeadTime(BreakDeadTimeMode breakDeadTimeMode) const {
        this->timer->BDTR = uint32_t(breakDeadTimeMode);
        return *this;
    }

    /// @brief Start the timer
    /// @param runMode run mode
    /// @return this
    auto &start(RunMode runMode = RunMode::UP) const {
        this->timer->CR1 = uint32_t(runMode);
        return *this;
    }

    /// @brief Stop the timer
    /// @return this
    auto &stop() const {
        this->timer->CR1 = 0;
        return *this;
    }
};

/// @brief Base structure for timer info
///
struct Info {
    // registers
    TIM_TypeDef *timer;

    // reset and clock control
    rcc::Info rcc;


    /// @brief Configure timer to update (overflow) at a given frequency
    /// @param clock timer clock frequency (e.g. APB1_CLOCK)
    /// @param frequency update frequency (actual frequency gets rounded towards nearest possible frequency)
    /// @return timer which can be further configured e.g. by calling .master()
    Registers configure(Hertz<> clock, Hertz<> frequency) const {
        this->rcc.enableClock();

        auto timer = this->timer;
        int div = frequency.value;
        timer->ARR = (clock.value + (div >> 1)) / div - 1;
        return {timer};
    }

    /// @brief Configure timer to update (overflow) at a given frequency
    /// @param clock timer clock frequency (e.g. APB1_CLOCK)
    /// @param prescaler prescaler
    /// @param frequency update frequency (actual frequency gets rounded towards nearest possible frequency)
    /// @return timer which can be further configured e.g. by calling .master()
    Registers configure(Hertz<> clock, int prescaler, Hertz<> frequency) const {
        this->rcc.enableClock();

        auto timer = this->timer;
        int div = prescaler * frequency.value;
        timer->ARR = (clock.value + (div >> 1)) / div - 1;
        timer->PSC = prescaler - 1;
        timer->EGR = TIM_EGR_UG; // update generation so that prescaler takes effect
        return {timer};
    }

    /// @brief Configure timer to update (overflow) after a given duration
    /// @param clock timer clock frequency (e.g. APB1_CLOCK)
    /// @param duration update duration (actual duration gets rounded towards nearest possible duration)
    /// @return timer which can be further configured e.g. by calling .master()
    Registers configure(Hertz<> clock, Nanoseconds<> duration) const {
        this->rcc.enableClock();

        auto timer = this->timer;
        timer->ARR = (int64_t(clock.value) * duration.value + 500000000) / 1000000000 - 1;
        return {timer};
    }

    /// @brief Configure timer to update (overflow) at a given counter value
    /// @param count count number of input clock cycles per update
    /// @return timer which can be further configured e.g. by calling .master()
    Registers configure(uint32_t count) const {
        this->rcc.enableClock();

        auto timer = this->timer;
        timer->ARR = count - 1;
        return {timer};
    }
};

///
/// General purpose timer info
///
struct Info1 : public Info {
    // interrupt index
    uint8_t irq;
};

///
/// Advanced-control timer info
///
struct Info2 : public Info {
    // interrupt for break, transition error and index error
    uint8_t breakIrq;

    // interrupt for update
    uint8_t updateIrq;

    // interrupt for trigger, commutation, direction change and index
    uint8_t triggerIrq;

    // interrupt for capture compare
    uint8_t captureCompareIrq;
};

} // namespace timer
} // namespace coco
