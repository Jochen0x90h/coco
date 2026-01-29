#pragma once

// do not include directly, use #include <coco/platform/timer.hpp>

#include "dma.hpp"
#include "rcc.hpp"
#include <coco/enum.hpp>
#include <coco/Frequency.hpp>
#include <coco/Time.hpp>


namespace coco {

/// @brief Timer helpers.
/// G0X0 https://www.st.com/en/microcontrollers-microprocessors/stm32g0-series/documentation.html Sections 15 - 19
/// G0X1 https://www.st.com/en/microcontrollers-microprocessors/stm32g0-series/documentation.html Sections 21 - 26
/// G4 https://www.st.com/en/microcontrollers-microprocessors/stm32g4-series/documentation.html Sections 29 - 32
/// H503 https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series/documentation.html Sections 26 - 28
/// H5 https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series/documentation.html Sections 38-42
/// U3 https://www.st.com/en/microcontrollers-microprocessors/stm32u3-series/documentation.html Sections 35 - 38
namespace timer {

/// @brief Master Mode (CR2 register), generates TRGO (tim_trgo) to trigger other timer, ADC, DAC etc.
///
enum class MasterMode : uint32_t {
    RESET = 0, // default: The UG bit from the TIMx_EGR register is used as trigger output (tim_trgo)
    ENABLE = TIM_CR2_MMS_0, // the Counter Enable signal CNT_EN is used as trigger output (tim_trgo)
    UPDATE = TIM_CR2_MMS_1, // the update (overflow) event is selected as trigger output (tim_trgo)
    COMPARE1 = TIM_CR2_MMS_1 | TIM_CR2_MMS_0, // the trigger output sends a positive pulse when the CC1IF flag is to be set (even if it was already high), as soon as a capture or compare match occurs (tim_trgo)
    OUTPUT1 = TIM_CR2_MMS_2, // tim_oc1refc signal is used as trigger output (tim_trgo)
    OUTPUT2 = TIM_CR2_MMS_2 | TIM_CR2_MMS_0, // tim_oc2refc signal is used as trigger output (tim_trgo)
    OUTPUT3 = TIM_CR2_MMS_2 | TIM_CR2_MMS_1, // tim_oc3refc signal is used as trigger output (tim_trgo)
    OUTPUT4 = TIM_CR2_MMS_2 | TIM_CR2_MMS_1 | TIM_CR2_MMS_0, // tim_oc4refc signal is used as trigger output (tim_trgo)
#ifdef TIM_CR2_MMS_3
    ENCODER_CLOCK = TIM_CR2_MMS_3, // The encoder clock signal is used as trigger output (tim_trgo)
#endif
};

/// @brief Master Mode 2 (CR2 register), generates TRGO2 (tim_trgo2) to trigger other timer, ADC, DAC etc.
///
enum class MasterMode2 : uint32_t {
    RESET = 0, // default: The UG bit from the TIMx_EGR register is used as trigger output (tim_trgo2)
#ifdef TIM_CR2_MMS2
    ENABLE = TIM_CR2_MMS2_0, // the Counter Enable signal CNT_EN is used as trigger output (tim_trgo2)
    UPDATE = TIM_CR2_MMS2_1, // the update (overflow) event is selected as trigger output (tim_trgo2)
    COMPARE1 = TIM_CR2_MMS2_1 | TIM_CR2_MMS2_0, // the trigger output sends a positive pulse when the CC1IF flag is to be set (even if it was already high), as soon as a capture or compare match occurs (tim_trgo2)
    OUTPUT1 = TIM_CR2_MMS2_2, // tim_oc1refc signal is used as trigger output (tim_trgo2)
    OUTPUT2 = TIM_CR2_MMS2_2 | TIM_CR2_MMS2_0, // tim_oc2refc signal is used as trigger output (tim_trgo2)
    OUTPUT3 = TIM_CR2_MMS2_2 | TIM_CR2_MMS2_1, // tim_oc3refc signal is used as trigger output (tim_trgo2)
    OUTPUT4 = TIM_CR2_MMS2_2 | TIM_CR2_MMS2_1 | TIM_CR2_MMS2_0, // tim_oc4refc signal is used as trigger output (tim_trgo2)
    OUTPUT5 = TIM_CR2_MMS2_3, // tim_oc5refc signal is used as trigger output (tim_trgo2)
    OUTPUT6 = TIM_CR2_MMS2_3 | TIM_CR2_MMS_0, // tim_oc6refc signal is used as trigger output (tim_trgo2)
    PULSE_BOTH4 = TIM_CR2_MMS2_3 | TIM_CR2_MMS2_1, // tim_oc4refc rising or falling edges generate pulses on tim_trgo2
    PULSE_BOTH6 = TIM_CR2_MMS2_3 | TIM_CR2_MMS2_1 | TIM_CR2_MMS2_0, // tim_oc6refc rising or falling edges generate pulses on tim_trgo2
    PULSE_RISING4_RISING6 = TIM_CR2_MMS2_3 | TIM_CR2_MMS2_2, // tim_oc4refc or tim_oc6refc rising edges generate pulses on tim_trgo2
    PULSE_RISING4_FALLING6 = TIM_CR2_MMS2_3 | TIM_CR2_MMS2_2 | TIM_CR2_MMS2_0, // tim_oc4refc rising or tim_oc6refc falling edges generate pulses on tim_trgo2
    PULSE_RISING5_RISING6 = TIM_CR2_MMS2_3 | TIM_CR2_MMS2_2 | TIM_CR2_MMS2_1, // tim_oc5refc or tim_oc6refc rising edges generate pulses on tim_trgo2
    PULSE_RISING5_FALLING6 = TIM_CR2_MMS2_3 | TIM_CR2_MMS2_2 | TIM_CR2_MMS2_1 | TIM_CR2_MMS2_0 // tim_oc5refc rising or tim_oc6refc falling edges generate pulses on tim_trgo2
#endif
};

/// @brief Trigger mode (SMCR register)
///
enum class TriggerMode : uint32_t {
    RESET = TIM_SMCR_SMS_2,
    GATED = TIM_SMCR_SMS_2 | TIM_SMCR_SMS_0,
    START = TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1,
    CLOCK = TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1 | TIM_SMCR_SMS_0,
#ifdef TIM_SMCR_SMS_3
    RESET_START = TIM_SMCR_SMS_3,
    GATED_RESET = TIM_SMCR_SMS_3 | TIM_SMCR_SMS_0
#endif
};

// device specific triggers are defined in timer.hpp of the device
enum class Trigger;


/// @brief Encoder mode (SMCR register)
/// Reference manual section 30.4.18
/// Inputs tim_ti1fp1/tim_ti2fp2 can be either pins CH1/CH2, comparator outputs or other (see device specific
/// enum Input), use setInput1() and setInput2().
/// Also select the capture input, e.g. setCaptureMode(timer::CaptureMode::INPUT_DEFAULT)
enum class EncoderMode : uint32_t {
    // Quadrature, counts up/down on tim_ti1fp1 edge depending on tim_ti2fp2 level (x2 mode)
    QUADRATURE1_X2 = TIM_SMCR_SMS_0,

    // Quadrature, counts up/down on tim_ti2fp2 edge depending on tim_ti1fp1 level (x2 mode)
    QUADRATURE2_X2 = TIM_SMCR_SMS_1,

    // Counts up/down on both tim_ti1fp1 and tim_ti2fp2 edges depending on the level of the other input (x4 mode)
    QUADRATURE_X4 = TIM_SMCR_SMS_1 | TIM_SMCR_SMS_0,
#ifdef TIM_SMCR_SMS_3
    // Clock plus direction, both edges (x2 mode)
    CLOCK_DIRECTION_X2 = TIM_SMCR_SMS_3 | TIM_SMCR_SMS_1,

    // Clock plus direction, edge selected by CC2P (x1 mode)
    CLOCK_DIRECTION_X1 = TIM_SMCR_SMS_3 | TIM_SMCR_SMS_1 | TIM_SMCR_SMS_0,

    // Two clocks, one counts up and one counts down, both edges (x2 mode)
    DIRECTIONAL_CLOCKS_X2 = TIM_SMCR_SMS_3 | TIM_SMCR_SMS_2,

    // Two clocks, one counts up and one counts down, edge selected by CC1P/CC2P (x1 mode)
    DIRECTIONAL_CLOCKS_X1 = TIM_SMCR_SMS_3 | TIM_SMCR_SMS_2 | TIM_SMCR_SMS_0,

    // Quadrature, counts on tim_ti1fp1 edge selected by CC1P (x1 mode)
    QUADRATURE1_X1 = TIM_SMCR_SMS_3 | TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1,

    // Quadrature, counts on tim_ti2fp2 edge selected by CC2P (x1 mode)
    QUADRATURE2_X1 = TIM_SMCR_SMS_3 | TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1 | TIM_SMCR_SMS_0,
#endif
};

// device specific input selections are defined in timer.hpp of the device
enum class Input1;
enum class Input2;
enum class Input3;
enum class Input4;

/// @brief Input configuration (CCMRx, CCER and TISEL registers)
/// Processing pipeline is: filter -> input selection -> edge detection -> polarity -> prescaler -> capture enable
enum class InputConfig : uint32_t {
    DEFAULT = 0,

    // optional input filter
    FILTER_NONE = 0, // default
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

    // edge selection for TriggerMode::RESET, TriggerMode::START, TriggerMode::CLOCK, TriggerMode::RESET_START and TriggerMode::GATED_RESET
    EDGE_RISING = 0, // default
    EDGE_FALLING = TIM_CCER_CC1P << 8,
    EDGE_BOTH = (TIM_CCER_CC1P | TIM_CCER_CC1NP) << 8,

    // polarity selection for TriggerMode::GATED and EncoderMode
    POLARITY_NORMAL = EDGE_RISING, // default
    POLARITY_INVERTED = EDGE_FALLING,

    // optional input prescaler
    PRESCALER_NONE = 0, // default
    PRESCALER_2 = TIM_CCMR1_IC1PSC_0,
    PRESCALER_4 = TIM_CCMR1_IC1PSC_1,
    PRESCALER_8 = TIM_CCMR1_IC1PSC_1 | TIM_CCMR1_IC1PSC_0,

    // input capture enable
    CAPTURE = TIM_CCER_CC1E << 8,
};
COCO_ENUM(InputConfig)

/// @brief Input mode (CCMRx register)
///
enum class InputMode : uint32_t {
    // own input
    DEFAULT = TIM_CCMR1_CC1S_0,

    // alternate input (1 -> 2, 2 -> 1, 3 -> 4, 4 -> 3) behind filter, edge/polarity selection and prescaler apply
    ALTERNATE = TIM_CCMR1_CC1S_1,

    // trigger from slave mode controller, only prescaler applies
    TRIGGER = TIM_CCMR1_CC1S_1 | TIM_CCMR1_CC1S_0,
};


/// @brief Output configuration (CCMRx and CCER registers)
///
enum class OutputConfig : uint32_t {
    // compare mode (should use one)
    FROZEN = 0, // default
    SET_ON_MATCH = TIM_CCMR1_OC1M_0,
    CLEAR_ON_MATCH = TIM_CCMR1_OC1M_1,
    TOGGLE_ON_MATCH = TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0,
    SET = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_0,
    CLEAR = TIM_CCMR1_OC1M_2,
    PWM1 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1, // active -> inactive
    PWM2 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0, // inactive -> active
#ifdef TIM_CCMR1_OC1M_3
    OPM1 = TIM_CCMR1_OC1M_3,
    OPM2 = TIM_CCMR1_OC1M_3 | TIM_CCMR1_OC1M_0,
    COMBINED_PWM1 = TIM_CCMR1_OC1M_3 | TIM_CCMR1_OC1M_2, // OR combination of channel pair (other channel must be COMBINED_PWM2)
    COMBINED_PWM2 = TIM_CCMR1_OC1M_3 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_0, // AND combination of channel pair (other channel must be COMBINED_PWM1)
    ASYMMETRIC_PWM1 = TIM_CCMR1_OC1M_3 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1, // direction bit selects channel of pair
    ASYMMETRIC_PWM2 = TIM_CCMR1_OC1M_3 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0, // direction bit selects channel of pair
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
COCO_ENUM(OutputConfig)

/// @brief Interrupt enable flags (DIER register)
///
enum class Interrupt : uint32_t {
    NONE = 0,

    UPDATE = TIM_DIER_UIE,
    TRIGGER = TIM_DIER_TIE,

    // capture
    CAPTURE1 = TIM_DIER_CC1IE,
    CAPTURE2 = TIM_DIER_CC2IE,
    CAPTURE3 = TIM_DIER_CC3IE,
    CAPTURE4 = TIM_DIER_CC4IE,

    // compare (same as capture)
    COMPARE1 = TIM_DIER_CC1IE,
    COMPARE2 = TIM_DIER_CC2IE,
    COMPARE3 = TIM_DIER_CC3IE,
    COMPARE4 = TIM_DIER_CC4IE,

    // encoder mode
#ifdef TIM_DIER_IDXIE
    INDEX = TIM_DIER_IDXIE,
    DIRECTION_CHANGE = TIM_DIER_DIRIE,
    INDEX_ERROR = TIM_DIER_IERRIE,
    TRANSITION_ERROR = TIM_DIER_TERRIE,

    ALL = UPDATE | TRIGGER | CAPTURE1 | CAPTURE2 | CAPTURE3 | CAPTURE4 | INDEX | DIRECTION_CHANGE | INDEX_ERROR | TRANSITION_ERROR
#else
    ALL = UPDATE | TRIGGER | CAPTURE1 | CAPTURE2 | CAPTURE3 | CAPTURE4
#endif
};
COCO_ENUM(Interrupt)

/// @brief DMA request flags (DIER register)
///
enum class DmaRequest : uint32_t {
    NONE = 0,

    TRIGGER = TIM_DIER_TDE,

    // capture
    CAPTURE1 = TIM_DIER_CC1DE,
    CAPTURE2 = TIM_DIER_CC2DE,
    CAPTURE3 = TIM_DIER_CC3DE,
    CAPTURE4 = TIM_DIER_CC4DE,

    // compare (same as capture)
    COMPARE1 = TIM_DIER_CC1DE,
    COMPARE2 = TIM_DIER_CC2DE,
    COMPARE3 = TIM_DIER_CC3DE,
    COMPARE4 = TIM_DIER_CC4DE,

    ALL = TRIGGER | CAPTURE1 | CAPTURE2 | CAPTURE3 | CAPTURE4
};
COCO_ENUM(DmaRequest)

/// @brief Status flags (SR register)
///
enum class Status : uint32_t {
    NONE = 0,

    UPDATE = TIM_SR_UIF,
    TRIGGER = TIM_SR_TIF,
    COM = TIM_SR_COMIF,
    BREAK = TIM_SR_BIF,
#ifdef TIM_SR_B2IF
    BREAK2 = TIM_SR_B2IF,
#endif

    // capture
    CAPTURE1 = TIM_SR_CC1IF,
    CAPTURE2 = TIM_SR_CC2IF,
    CAPTURE3 = TIM_SR_CC3IF,
    CAPTURE4 = TIM_SR_CC4IF,
    OVERCAPTURE1 = TIM_SR_CC1OF,
    OVERCAPTURE2 = TIM_SR_CC2OF,
    OVERCAPTURE3 = TIM_SR_CC3OF,
    OVERCAPTURE4 = TIM_SR_CC4OF,

    // compare (same as capture)
    COMPARE1 = TIM_SR_CC1IF,
    COMPARE2 = TIM_SR_CC2IF,
    COMPARE3 = TIM_SR_CC3IF,
    COMPARE4 = TIM_SR_CC4IF,
#ifdef TIM_SR_CC5IF
    COMPARE5 = TIM_SR_CC5IF,
#endif
#ifdef TIM_SR_CC6IF
    COMPARE6 = TIM_SR_CC6IF,
#endif

    // encoder mode
#ifdef TIM_SR_IDXF
    INDEX = TIM_SR_IDXF,
    DIRECTION_CHANGE = TIM_SR_DIRF,
    INDEX_ERROR = TIM_SR_IERRF,
    TRANSITION_ERROR = TIM_SR_TERRF,
#endif

    ALL = 0xffffffff
};
COCO_ENUM(Status)

/// @brief Event flags (EGR register)
///
enum class Event : uint32_t {
    NONE = 0,

    UPDATE = TIM_EGR_UG,
    TRIGGER = TIM_EGR_TG,
    COM = TIM_EGR_COMG,
    BREAK = TIM_EGR_BG,
#ifdef TIM_EGR_B2G
    BREAK2 = TIM_EGR_B2G,
#endif

    // capture
    CAPTURE1 = TIM_EGR_CC1G,
    CAPTURE2 = TIM_EGR_CC2G,
    CAPTURE3 = TIM_EGR_CC3G,
    CAPTURE4 = TIM_EGR_CC4G,

    // compare (same as capture)
    COMPARE1 = TIM_EGR_CC1G,
    COMPARE2 = TIM_EGR_CC2G,
    COMPARE3 = TIM_EGR_CC3G,
    COMPARE4 = TIM_EGR_CC4G,
};
COCO_ENUM(Event)

/// @brief Configuration for advanced control timers
///
enum class ControlConfig : uint32_t {
    // enable output pins
    MAIN_OUTPUT_ENABLE = TIM_BDTR_MOE,

    // off-state selection in idle mode (MOE = 0)
    OSSI = TIM_BDTR_OSSI,

    // off-state selection in run mode (MOE = 1)
    OSSR = TIM_BDTR_OSSR,

    // dead time = (value + 64) * 2, maximum value is 63
    DEAD_TIME_PLUS_64_X2 = TIM_BDTR_DTG_7,

    // dead time = (value + 32) * 8, maximum value is 31
    DEAD_TIME_PLUS_32_X8 = TIM_BDTR_DTG_7 | TIM_BDTR_DTG_6,

    // dead time = (value + 32) * 16, maximum value is 31
    DEAD_TIME_PLUS_32_X16 = TIM_BDTR_DTG_7 | TIM_BDTR_DTG_6 | TIM_BDTR_DTG_5,
};
COCO_ENUM(ControlConfig)

/// @brief Run mode and configuration flags
///
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

    // load ARR register on update event
    AUTORELOAD_PRELOAD_ENABLE = TIM_CR1_ARPE,

#ifdef TIM_CR1_DITHEN
    // enable dithering
    DITHERING_ENABLE = TIM_CR1_DITHEN,
#endif
};
COCO_ENUM(RunMode)


/// @brief Timer feature flags.
///
enum class Feature {
    NONE = 0,

    // 32 bit timer
    BITS_32 = 1 << 0,

    // advanced control timer
    ADVANCED_CONTROL = 1 << 1,

    // slave mode control register
    SLAVE = 1 << 2,

    // capture/compare 1
    CC1 = 1 << 3,

    // capture/compare 2
    CC2 = 1 << 4,

    // capture/compare 3-4
    CC3_4 = 1 << 5,

    CC1_2 = CC1 | CC2,
    CC1_4 = CC1 | CC2 | CC3_4,

    // output compare 5-6
    COMPARE5_6 = 1 << 6,

    // dead time
    DEAD_TIME = 1 << 7,

    // encoder mode
    ENCODER = 1 << 8,
};
COCO_ENUM(Feature)

/// @brief Interrupt request flags.
/// Interrupt requests that are reported to the NVIC, not to be confused with the timer interrupt enable flags.
enum class Irq {
    NONE = 0,

    // one combined irq
    COMBINED = 1,

    // separate irqs
    BREAK = 2,
    UPDATE = 4,
    TRIGGER = 8,
    CC = 16,
    SEPARATE = BREAK | UPDATE | TRIGGER | CC
};
COCO_ENUM(Irq)

/// @brief DMA request flags.
/// DMA requests that are reported to the DMA controller, not to be confused with the timer DMA enable flags.
enum class Drq {
    NONE = 0,
    CH1 = 1,
    CH2 = 2,
    CH3 = 4,
    CH4 = 8,
    CH1_4 = CH1 | CH2 | CH3 | CH4,
    TRIGGER = 16,
    COMMUTATE = 32,
    UPDATE = 64,
    ALL = CH1_4 | TRIGGER | COMMUTATE | UPDATE
};
COCO_ENUM(Drq)


/// @brief Timer instance (wrapper for Timer registers)
/// @tparam F Feature flags
template <Feature F>
struct Instance {
    TIM_TypeDef *timer;


    Instance() = default;
    Instance(TIM_TypeDef *timer) : timer(timer) {}

    /// @brief Construct from another registers wrapper.
    /// Requires that all features are supported.
    /// @tparam F2 Feature set of source register wrapper
    /// @param r Source register wrapper
    template <Feature F2> requires ((F & F2) == F)
    Instance(const Instance<F2> &r) : timer(r.timer) {}

    TIM_TypeDef *operator ->() const {return timer;}
    operator TIM_TypeDef *() const {return timer;}

    /// @brief Get counter register value.
    /// @return Counter
    int count() {
        return timer->CNT;
    }

    /// @brief Set counter register value.
    /// @param count Count
    /// @return *this
    auto &setCount(uint32_t count) {
        timer->CNT = count;
        return *this;
    }

    /// @brief Set the prescaler value to PSC register
    /// @param prescalerValue prescaler value
    /// @return *this
    auto &setPrescaler(uint32_t prescalerValue) {
        timer->PSC = prescalerValue;
        timer->EGR = TIM_EGR_UG; // update generation so that prescaler takes effect
        return *this;
    }

    /// @brief Set count frequency by calculating the prescaler value
    /// @param clock timer clock frequency (e.g. APB1_CLOCK)
    /// @param countFrequency count frequency of the timer
    /// @return *this
    auto &setCountFrequency(Hertz<> clock, Hertz<> countFrequency) {
        int div = countFrequency.value;
        timer->PSC = (clock.value + (div >> 1)) / div - 1;
        timer->EGR = TIM_EGR_UG; // update generation so that prescaler takes effect
        return *this;
    }

    /// @brief Set count duration by calculating the prescaler value
    /// @param clock timer clock frequency (e.g. APB1_CLOCK)
    /// @param countDuration duration of one timer tick
    /// @return *this
    auto &setCountDuration(Hertz<> clock, Nanoseconds<> countDuration) {
        timer->PSC = (int64_t(clock.value) * countDuration.value + 500000000) / 1000000000 - 1;
        timer->EGR = TIM_EGR_UG; // update generation so that prescaler takes effect
        return *this;
    }

    /// @brief Set the reload value to ARR regsiter
    /// this sets reloadValue to the ARR register, i.e. if reloadValue is 4, the counter counts 0, 1, 2, 3, 4 and then
    /// rolls over to 0.
    /// @param reloadValue reload value
    /// @return *this
    auto &setReload(uint32_t reloadValue) {
        timer->ARR = reloadValue;
        return *this;
    }

    /// @brief Set the timer period
    /// This sets period - 1 to ARR register, i.e. if period is 4, the counter counts 0, 1, 2, 3 and then rolls over to
    /// 0.
    /// @param period Period
    /// @return *this
    auto &setPeriod(uint32_t period) {
        timer->ARR = period - 1;
        return *this;
    }

    /// @brief Set update frequency
    /// @param countFrequency timer count frequency (is clock frequency such as APB1_CLOCK if no prescaler or count frequency was set)
    /// @param updateFrequency update frequency of the timer
    /// @return *this
    auto &setUpdateFrequency(Hertz<> countFrequency, Hertz<> updateFrequency) {
        int div = updateFrequency.value;
        timer->ARR = (countFrequency.value + (div >> 1)) / div - 1;
        return *this;
    }

    /// @brief Set clock mode, the prescaler is clocked directly by the internal clock
    /// Overrides setTriggerMode() and setEncoderMode()
    /// @return *this
    auto &setClockMode() {
        timer->SMCR = 0;
        return *this;
    }

    /// @brief Set trigger mode (trigger related slave mode)
    /// Overrides setClockMode() and setEncoderMode()
    /// @param triggerMode trigger mode
    /// @param trigger trigger source
    /// @return *this
    auto &setTriggerMode(TriggerMode triggerMode, Trigger trigger) {
        timer->SMCR = uint32_t(triggerMode) | uint32_t(trigger);
        return *this;
    }

    /// @brief Set encoder mode (encoder related slave mode)
    /// Overrides setClockMode() and setTriggerMode()
    /// @param encoderMode encoder mode
    /// @return *this
    auto &setEncoderMode(EncoderMode encoderMode) {
        static_assert((F & Feature::ENCODER) != 0, "setEncoderMode() not supported");
        timer->SMCR = uint32_t(encoderMode);
        return *this;
    }

    /// @brief Set master mode which controls trigger output TRGO and TRGO2
    /// @param masterMode Master mode controls TRGO
    /// @param masterMode2 Master mode 2 controls TGRO2
    /// @return *this
    auto &setMasterMode(MasterMode masterMode, MasterMode2 masterMode2 = MasterMode2::RESET) {
        timer->CR2 = uint32_t(masterMode) | uint32_t(masterMode2);
        return *this;
    }

    /// @brief Set input for input channel 1
    /// @param input input to select
    /// @return *this
    auto &selectInput1(Input1 input) {
        static_assert((F & (Feature::CC1 | Feature::ENCODER)) != 0, "selectInput1() not supported");
#ifdef TIM_TISEL_TI1SEL
        timer->TISEL = (timer->TISEL & 0xffffff00) | uint32_t(input);
#endif
        return *this;
    }

    auto &selectInput2(Input2 input) {
        static_assert((F & (Feature::CC2 | Feature::ENCODER)) != 0, "selectInput2() not supported");
#ifdef TIM_TISEL_TI1SEL
        timer->TISEL = (timer->TISEL & 0xffff00ff) | (uint32_t(input) << 8);
#endif
        return *this;
    }

    auto &selectInput3(Input3 input) {
        static_assert((F & Feature::CC3_4) != 0, "selectInput3() not supported");
#ifdef TIM_TISEL_TI1SEL
        timer->TISEL = (timer->TISEL & 0xff00ffff) | (uint32_t(input) << 16);
#endif
        return *this;
    }

    auto &selectInput4(Input4 input) {
        static_assert((F & Feature::CC3_4) != 0, "selectInput4() not supported");
#ifdef TIM_TISEL_TI1SEL
        timer->TISEL = (timer->TISEL & 0x00ffffff) | (uint32_t(input) << 24);
#endif
        return *this;
    }

    /// @brief Enable and configure input 1.
    /// Input capture and output compare are mutually exclusive, therefore enableInput1() overrides enableOutput1().
    /// @param mode Capture mode
    /// @param input Capture input selection
    /// @return *this
    auto &enableInput1(InputConfig config, InputMode mode = InputMode::DEFAULT) {
        static_assert((F & (Feature::CC1 | Feature::ENCODER)) != 0, "enableInput1() not supported");
        timer->CCMR1 = (timer->CCMR1 & 0xff00) | ((uint32_t(config) | uint32_t(mode)) & 0x00ff);
        timer->CCER = (timer->CCER & 0xfffffff0) | ((uint32_t(config) >> 8) & 0x0000000f);
        return *this;
    }
    auto &enableInput2(InputConfig config, InputMode mode = InputMode::DEFAULT) {
        static_assert((F & (Feature::CC2 | Feature::ENCODER)) != 0, "enableInput2() not supported");
        timer->CCMR1 = (timer->CCMR1 & 0x00ff) | (((uint32_t(config) | uint32_t(mode)) << 8) & 0xff00);
        timer->CCER = (timer->CCER & 0xffffff0f) | ((uint32_t(config) >> 4) & 0x000000f0);
        return *this;
    }
    auto &enableInput3(InputConfig config, InputMode mode = InputMode::DEFAULT) {
        static_assert((F & Feature::CC3_4) != 0, "enableInput3() not supported");
        timer->CCMR2 = (timer->CCMR2 & 0xff00) | ((uint32_t(config) | uint32_t(mode)) & 0x00ff);
        timer->CCER = (timer->CCER & 0xfffff0ff) | (uint32_t(config) & 0x00000f00);
        return *this;
    }
    auto &enableInput4(InputConfig config, InputMode mode = InputMode::DEFAULT) {
        static_assert((F & Feature::CC3_4) != 0, "enableInput4() not supported");
        timer->CCMR2 = (timer->CCMR2 & 0x00ff) | (((uint32_t(config) | uint32_t(mode)) << 8) & 0xff00);
        timer->CCER = (timer->CCER & 0xffff0fff) | ((uint32_t(config) << 4) & 0x0000f000);
        return *this;
    }

    auto capture1() {
        return timer->CCR1;
    }

    auto capture2() {
        return timer->CCR2;
    }

    auto capture3() {
        return timer->CCR3;
    }

    auto capture4() {
        return timer->CCR4;
    }

    /// @brief Set value for compare 1
    /// @param value compare value
    /// @return *this
    auto &setCompare1(uint32_t value) {
        static_assert((F & Feature::CC1) != 0, "setCompare1() not supported");
        timer->CCR1 = value;
        return *this;
    }
    auto &setCompare2(uint32_t value) {
        static_assert((F & Feature::CC2) != 0, "setCompare2() not supported");
        timer->CCR2 = value;
        return *this;
    }
    auto &setCompare3(uint32_t value) {
        static_assert((F & Feature::CC3_4) != 0, "setCompare3() not supported");
        timer->CCR3 = value;
        return *this;
    }
    auto &setCompare4(uint32_t value) {
        static_assert((F & Feature::CC3_4) != 0, "setCompare4() not supported");
        timer->CCR4 = value;
        return *this;
    }
#ifdef TIM_CCR5_CCR5
    auto &setCompare5(uint32_t value) {
        static_assert((F & Feature::COMPARE5_6) != 0, "setCompare5() not supported");
        timer->CCR5 = value;
        return *this;
    }
#endif
#ifdef TIM_CCR6_CCR6
    auto &setCompare6(uint32_t value) {
        static_assert((F & Feature::COMPARE5_6) != 0, "setCompare6() not supported");
        timer->CCR6 = value;
        return *this;
    }
#endif

    auto compare1() {
        return timer->CCR1;
    }

    auto compare2() {
        return timer->CCR2;
    }

    auto compare3() {
        return timer->CCR3;
    }

    auto compare4() {
        return timer->CCR4;
    }
#ifdef TIM_CCR5_CCR5
    auto compare5() {
        return timer->CCR5;
    }
#endif
#ifdef TIM_CCR6_CCR6
    auto compare6() {
        return timer->CCR6;
    }
#endif

    /// @brief Set mode for output 1.
    /// Input capture and output compare are mutually exclusive, therefore enableOutput1() overrides enableInput1().
    /// @param mode Output mode
    /// @return *this
    auto &enableOutput1(OutputConfig config) {
        static_assert((F & Feature::CC1) != 0, "enableOutput1() not supported");
        timer->CCMR1 = (timer->CCMR1 & 0xff00ff00) | (uint32_t(config) & 0x00ff00ff);
        timer->CCER = (timer->CCER & 0xfffffff0) | ((uint32_t(config) >> 8) & 0x0000000f);
        return *this;
    }
    auto &enableOutput2(OutputConfig config) {
        static_assert((F & Feature::CC2) != 0, "enableOutput2() not supported");
        timer->CCMR1 = (timer->CCMR1 & 0x00ff00ff) | ((uint32_t(config) << 8) & 0xff00ff00);
        timer->CCER = (timer->CCER & 0xffffff0f) | ((uint32_t(config) >> 4) & 0x000000f0);
        return *this;
    }
    auto &enableOutput3(OutputConfig config) {
        static_assert((F & Feature::CC3_4) != 0, "enableOutput3() not supported");
        timer->CCMR2 = (timer->CCMR2 & 0xff00ff00) | (uint32_t(config) & 0x00ff00ff);
        timer->CCER = (timer->CCER & 0xfffff0ff) | (uint32_t(config) & 0x00000f00);
        return *this;
    }
    auto &enableOutput4(OutputConfig config) {
        static_assert((F & Feature::CC3_4) != 0, "enableOutput4() not supported");
        timer->CCMR2 = (timer->CCMR2 & 0x00ff00ff) | ((uint32_t(config) << 8) & 0xff00ff00);
        timer->CCER = (timer->CCER & 0xffff0fff) | ((uint32_t(config) << 4) & 0x0000f000);
        return *this;
    }
#ifdef TIM_CCMR3_OC5M
    auto &enableOutput5(OutputConfig config) {
        static_assert((F & Feature::COMPARE5_6) != 0, "enableOutput5() not supported");
        timer->CCMR3 = (timer->CCMR3 & 0xff00ff00) | (uint32_t(config) & 0x00ff00ff);
        timer->CCER = (timer->CCER & 0xfff0ffff) | ((uint32_t(config) << 8) & 0x000f0000);
        return *this;
    }
#endif
#ifdef TIM_CCMR3_OC6M
    auto &enableOutput6(OutputConfig config) {
        static_assert((F & Feature::COMPARE5_6) != 0, "enableOutput6() not supported");
        timer->CCMR3 = (timer->CCMR3 & 0x00ff00ff) | ((uint32_t(config) << 8) & 0xff00ff00);
        timer->CCER = (timer->CCER & 0xff0fffff) | ((uint32_t(config) << 12) & 0x00f00000);
        return *this;
    }
#endif

    /// @brief Set main output enable for advanced control timer and other timers with BDTR register.
    /// Sets the main output enable bit in the BDTR register, all other bits are cleared.
    /// It is possible to use this method when presence of BDTR register is unknown as long as at least one CC channel
    /// is supported.
    /// @return *this
    auto &setMainOutputEnable() {
        static_assert((F & (Feature::CC1 | Feature::CC2 | Feature::CC3_4 | Feature::COMPARE5_6)) != 0, "setMainOutputEnable() not supported");
        timer->BDTR = TIM_BDTR_MOE;
        return *this;
    }

    /// @brief Set configuration for advanced control timer and other timers with BDTR register.
    /// @param breakDeadTimeMode break and dead-time mode
    /// @return *this
    auto &setControlConfig(ControlConfig controlConfig, int deadTime) {
        static_assert((F & Feature::DEAD_TIME) != 0, "setControlConfig() not supported");
        timer->BDTR = uint32_t(controlConfig) | (deadTime << TIM_BDTR_DTG_Pos);
        return *this;
    }

    /// @brief Disable channel 1.
    /// @return *this
    auto &disable1() {
        static_assert((F & (Feature::CC1 | Feature::ENCODER)) != 0, "disable1() not supported");
        timer->CCMR1 = timer->CCMR1 & 0xff00ff00;
        timer->CCER = timer->CCER & 0xfffffff0;
        return *this;
    }
    auto &disable2() {
        static_assert((F & (Feature::CC2 | Feature::ENCODER)) != 0, "disable2() not supported");
        timer->CCMR1 = timer->CCMR1 & 0x00ff00ff;
        timer->CCER = timer->CCER & 0xffffff0f;
        return *this;
    }
    auto &disable3() {
        static_assert((F & Feature::CC3_4) != 0, "disable3() not supported");
        timer->CCMR2 = timer->CCMR2 & 0xff00ff00;
        timer->CCER = timer->CCER & 0xfffff0ff;
        return *this;
    }
    auto &disable4() {
        static_assert((F & Feature::CC3_4) != 0, "disable4() not supported");
        timer->CCMR2 = timer->CCMR2 & 0x00ff00ff;
        timer->CCER = timer->CCER & 0xffff0fff;
        return *this;
    }
#ifdef TIM_CCMR3_OC5M
    auto &disable5() {
        static_assert((F & Feature::COMPARE5_6) != 0, "disable5() not supported");
        timer->CCMR3 = timer->CCMR3 & 0xff00ff00;
        timer->CCER = timer->CCER & 0xfff0ffff;
        return *this;
    }
#endif
#ifdef TIM_CCMR3_OC6M
    auto &disable6() {
        static_assert((F & Feature::COMPARE5_6) != 0, "disable6() not supported");
        timer->CCMR3 = timer->CCMR3 & 0x00ff00ff;
        timer->CCER = timer->CCER & 0xff0fffff;
        return *this;
    }
#endif

    /// @brief Set or clear all interrupt and DMA request flags.
    /// Note that all flags which are not specified are cleared.
    /// @param flags Flags to set (all others are cleared)
    /// @return *this
    auto &set(Interrupt interrupts, DmaRequest dmaRequests) {
        timer->DIER = uint32_t(interrupts) | uint32_t(dmaRequests);
        return *this;
    }

    /// @brief Enable interrupts and DMA requests.
    /// @param flags Flags to set
    /// @return *this
    auto &enable(Interrupt interrupts, DmaRequest dmaRequests = DmaRequest::NONE) {
        timer->DIER = timer->DIER | (uint32_t(interrupts) | uint32_t(dmaRequests));
        return *this;
    }

    /// @brief Disable interrupts and DMA requests.
    /// @param flags Flags to clear
    /// @return *this
    auto &disable(Interrupt interrupts, DmaRequest dmaRequests = DmaRequest::NONE) {
        timer->DIER = timer->DIER & ~(uint32_t(interrupts) | uint32_t(dmaRequests));
        return *this;
    }

    /// @brief Get the currently enabled Interrupts.
    /// @return Enabled interrupts
    Interrupt interrupts() {
        return Interrupt(timer->DIER) & Interrupt::ALL;
    }

    /// @brief Enable interrupts.
    /// @param flags Flags to set
    /// @return *this
    auto &enable(DmaRequest dmaRequests) {
        timer->DIER = timer->DIER | uint32_t(dmaRequests);
        return *this;
    }

    /// @brief Disable interrupts.
    /// @param flags Flags to clear
    /// @return *this
    auto &disable(DmaRequest dmaRequests) {
        timer->DIER = timer->DIER & ~uint32_t(dmaRequests);
        return *this;
    }

    /// @brief Get the currently enabled DMA requests.
    /// @return enabled DMA requests
    DmaRequest dmaRequests() {
        return DmaRequest(timer->DIER) & DmaRequest::ALL;
    }

    /// @brief Start the timer
    /// @param runMode run mode
    /// @return *this
    auto &start(RunMode runMode = RunMode::UP) {
        timer->CR1 = uint32_t(runMode);
        return *this;
    }

    /// @brief Stop the timer
    /// @return *this
    auto &stop() {
        timer->CR1 = 0;
        return *this;
    }

    /// @brief Check if timer is running
    /// @return true if running
    bool running() const {
        return (timer->CR1 & TIM_CR1_CEN) != 0;
    }

    /// @brief Clear counter value.
    /// @return *this
    auto &clear() {
        timer->CNT = 0;
        return *this;
    }

    /// @brief Get the status flags.
    /// @return Status flags
    Status status() const {
        return Status(timer->SR);
    }

    /// @brief Clear status flags.
    /// @param status Status flags to clear
    /// @return *this
    auto &clear(Status status) {
        timer->SR = ~uint32_t(status);
        return *this;
    }

    /// @brief Generate an event.
    /// For example call generate(timer::Event::UPDATE) to clear the counter and prescaler and to apply changes to
    /// capture/compare registers when OutputConfig::PRELOAD_ENABLE is set.
    /// @param event Event to generate
    /// @return *this
    auto &generate(Event event) {
        timer->EGR = uint32_t(event);
        return *this;
    }

    /// @brief Generate update event.
    /// Equivalent to generate(timer::Event::UPDATE)
    /// @return *this
    auto &update() {
        timer->EGR = TIM_EGR_UG;
        return *this;
    }
};

} // namespace timer
} // namespace coco
