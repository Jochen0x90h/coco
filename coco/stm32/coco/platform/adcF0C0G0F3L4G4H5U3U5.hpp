#pragma once

// do not include directly, use #include <coco/platform/adc.hpp>

#include "dma.hpp"
#include <coco/Array.hpp>
#include <coco/enum.hpp>
#include <coco/debug.hpp> // for debug::sleep()


/*
    Defines:
    HAVE_ADC_DUAL_MODE          Supports two ADCs in dual mode
    HAVE_ADC_VOLTAGE_REGULATOR  Has a voltage regulator enable bit
    HAVE_ADC_DIFFERENTIAL_MODE  Supports differential inputs
*/

// detect ADC dual mode
#ifdef ADC2
#define HAVE_ADC_DUAL_MODE
#endif

// detect voltage regulator
#ifdef ADC_CR_ADVREGEN
#define HAVE_ADC_VOLTAGE_REGULATOR
#endif

// detect differential mode
#ifdef ADC_CR_ADCALDIF
#define HAVE_ADC_DIFFERENTIAL_MODE
#endif

namespace coco {

/// @brief ADC helpers.
/// G0X0 https://www.st.com/en/microcontrollers-microprocessors/stm32g0-series/documentation.html Section 14
/// G0X1 https://www.st.com/en/microcontrollers-microprocessors/stm32g0-series/documentation.html Section 14
/// G4 https://www.st.com/en/microcontrollers-microprocessors/stm32g4-series/documentation.html Section 21
/// H503 https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series/documentation.html Section 19
/// H5 https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series/documentation.html Section 26
/// U3 https://www.st.com/en/microcontrollers-microprocessors/stm32u3-series/documentation.html Section 22
namespace adc {

// device-specific enums
enum class ClockConfig : uint32_t;
enum class Config : uint32_t;
enum class Trigger : uint32_t;
enum class DmaMode : uint32_t;
enum class Input : uint16_t;

/// @brief Status of ADC (ISR register).
///
enum class Status : uint32_t {
    NONE = 0,

    // end of conversion
    EOC = ADC_ISR_EOC,

    // end of sequence
    EOS = ADC_ISR_EOS,

    // overrun
    OVERRUN = ADC_ISR_OVR,

    // analog watchdog
    WATCHDOG1 = ADC_ISR_AWD1,
#ifdef ADC_ISR_AWD2
    WATCHDOG2 = ADC_ISR_AWD2,
#endif
#ifdef ADC_ISR_AWD3
    WATCHDOG3 = ADC_ISR_AWD2,
#endif

    ALL = 0xffffffff
};
COCO_ENUM(Status);


// device-specific functions
void setInput(ADC_TypeDef *adc, Input input);
void setSequence(ADC_TypeDef *adc, const Array<const Input> &sequence);


#ifdef HAVE_ADC_VOLTAGE_REGULATOR
constexpr uint32_t DEFAULT_CR = ADC_CR_ADVREGEN;

/// @brief Enable voltage regulator and wait tADCVREG_STUP
/// @param adc ADC instance
inline void enableVoltageRegulator(ADC_TypeDef *adc) {
    adc->CR = ADC_CR_ADVREGEN;

    // wait tADCVREG_STUP
    debug::sleep(20us);
}

/// @brief Enable dual voltage regulators and wait tADCVREG_STUP
/// @param adc1 First ADC instance
/// @param adc2 Second ADC instance
inline void enableVoltageRegulator(ADC_TypeDef *adc1, ADC_TypeDef *adc2) {
    adc1->CR = ADC_CR_ADVREGEN;
    adc2->CR = ADC_CR_ADVREGEN;

    // wait tADCVREG_STUP
    debug::sleep(20us);
}

#else
constexpr uint32_t DEFAULT_CR = 0;
#endif

/*
/// @brief Calibrate single ended
/// @param adc ADC instance
inline void calibrate(ADC_TypeDef *adc)  {
    adc->CR = DEFAULT_CR | ADC_CR_ADCAL;
    while ((adc->CR & ADC_CR_ADCAL) != 0) {}
    for (int i = 0; i < 8; ++i)
        __NOP();
}

/// @brief Calibrate differential
/// @param adc ADC instance
#ifdef HAVE_ADC_DIFFERENTIAL_MODE
inline void calibrateDifferential(ADC_TypeDef *adc)  {
    adc->CR = DEFAULT_CR | ADC_CR_ADCAL | ADC_CR_ADCALDIF;
    while ((adc->CR & ADC_CR_ADCAL) != 0) {}
    for (int i = 0; i < 8; ++i)
        __NOP();
}
#endif

/// @brief Enable ADC and wait until ready
/// @param adc ADC instance
inline void enable(ADC_TypeDef *adc) {
    adc->CR = DEFAULT_CR | ADC_CR_ADEN;
    while ((adc->ISR & ADC_ISR_ADRDY) == 0);
}

/// @brief Enable dual ADC and wait until ready
/// @param adc1 First ADC instance
/// @param adc2 Second ADC instance
inline void enable(ADC_TypeDef *adc1, ADC_TypeDef *adc2) {
    adc1->CR = DEFAULT_CR | ADC_CR_ADEN;
    adc2->CR = DEFAULT_CR | ADC_CR_ADEN;
    while ((adc1->ISR & adc2->ISR & ADC_ISR_ADRDY) == 0);
}

/// @brief Disable ADC and wait until disabled
/// @param adc ADC instance
inline void disable(ADC_TypeDef *adc) {
    adc->CR = DEFAULT_CR | ADC_CR_ADDIS;
    while ((adc->CR & ADC_CR_ADDIS) != 0);
}

/// @brief Start conversion or wait for trigger if a trigger is enabled.
/// @param adc ADC instance
inline void start(ADC_TypeDef *adc) {
    adc->CR = DEFAULT_CR | ADC_CR_ADEN | ADC_CR_ADSTART;
}

/// @brief Stop conversion.
/// @param adc ADC instance
inline void stop(ADC_TypeDef *adc) {
    adc->CR = DEFAULT_CR | ADC_CR_ADEN | ADC_CR_ADSTP;
}

/// @brief Wait for end of conversion (EOC).
/// Note that the EOC flag is not cleard, read the data register to clear.
/// @param adc ADC instance
inline void wait(ADC_TypeDef *adc) {
    while ((adc->ISR & ADC_ISR_EOC) == 0);
}

/// @brief Wait for end of conversion (EOC) and clear EOC and EOS flags.
/// @param adc ADC instance
inline void waitConversion(ADC_TypeDef *adc) {
    while ((adc->ISR & ADC_ISR_EOC) == 0);
    adc->ISR = ADC_ISR_EOC | ADC_ISR_EOS;
}

/// @brief Wait for end of sequence (EOC and EOS) and clear EOC and EOS flags.
/// @param adc ADC instance
inline void waitSequence(ADC_TypeDef *adc) {
    while ((adc->ISR & (ADC_ISR_EOC | ADC_ISR_EOS)) != (ADC_ISR_EOC | ADC_ISR_EOS));
    adc->ISR = ADC_ISR_EOC | ADC_ISR_EOS;
}

#ifdef HAVE_ADC_DUAL_MODE
/// @brief Wait for end of dual conversion (EOC for both ADCs).
/// @param common Common registers
inline void wait(ADC_Common_TypeDef *common) {
    while ((common->CSR & (ADC_CSR_EOC_MST | ADC_CSR_EOC_SLV)) != (ADC_CSR_EOC_MST | ADC_CSR_EOC_SLV));
}

/// @brief Wait for end of dual conversion (EOC for both ADCs) and clear EOC and EOS flags of both ADCs.
/// @param common Common registers
/// @param adc1 First ADC instance
/// @param adc2 Second ADC instance
inline void waitConversion(ADC_Common_TypeDef *common, ADC_TypeDef *adc1, ADC_TypeDef *adc2) {
    while ((common->CSR & (ADC_CSR_EOC_MST | ADC_CSR_EOC_SLV)) != (ADC_CSR_EOC_MST | ADC_CSR_EOC_SLV));
    adc1->ISR = ADC_ISR_EOC | ADC_ISR_EOS;
    adc2->ISR = ADC_ISR_EOC | ADC_ISR_EOS;
}

/// @brief Wait for end of dual sequence (EOC for both ADCs, EOS for first ADC) and clear EOC and EOS flags of both ADCs.
/// @param common Common registers
/// @param adc1 First ADC instance
/// @param adc2 Second ADC instance
inline void waitSequence(ADC_Common_TypeDef *common, ADC_TypeDef *adc1, ADC_TypeDef *adc2) {
    while ((common->CSR & (ADC_CSR_EOC_MST | ADC_CSR_EOC_SLV | ADC_CSR_EOS_MST)) != (ADC_CSR_EOC_MST | ADC_CSR_EOC_SLV | ADC_CSR_EOS_MST));
    adc1->ISR = ADC_ISR_EOC | ADC_ISR_EOS;
    adc2->ISR = ADC_ISR_EOC | ADC_ISR_EOS;
}
#endif
*/


/// @brief Wrapper for ADC registers
///
struct Instance {
    ADC_TypeDef *adc;


    ADC_TypeDef *operator ->() const {return adc;}
    operator ADC_TypeDef *() const {return adc;}

    /// @brief Calibrate the ADC in single ended mode.
    /// @return *this
    auto &calibrateSingleEnded() {
        adc->CR = DEFAULT_CR | ADC_CR_ADCAL;
        while ((adc->CR & ADC_CR_ADCAL) != 0) {}
        for (int i = 0; i < 8; ++i)
            __NOP();
        return *this;
    }

#ifdef HAVE_ADC_DIFFERENTIAL_MODE
    /// @brief Calibrate the ADC in differential mode.
    /// @return *this
    auto &calibrateDifferential() {
        adc->CR = DEFAULT_CR | ADC_CR_ADCAL | ADC_CR_ADCALDIF;
        while ((adc->CR & ADC_CR_ADCAL) != 0) {}
        for (int i = 0; i < 8; ++i)
            __NOP();
        return *this;
    }
#endif

    /// @brief Calibrate ADC in both single ended and differential mode.
    /// @return *this
    auto &calibrate() {
        calibrateSingleEnded();
#ifdef HAVE_ADC_DIFFERENTIAL_MODE
        calibrateDifferential();
#endif
        return *this;
    }

    /// @brief Configure the ADC.
    /// Note: Call only when no conversion is running.
    /// @param config Configuraton (resolution, alignment, etc.)
    /// @param trigger Trigger source
    /// @param dmaMode DMA mode (default: disabled)
    /// @return *this
    Instance &configure(Config config, Trigger trigger, DmaMode dmaMode = DmaMode(0));
    // implementation is platform dependent

    /// @brief Enable the ADC and wait until ready.
    /// @return *this
    auto &enable() {
        do {
            adc->CR = DEFAULT_CR | ADC_CR_ADEN;
        } while ((adc->ISR & ADC_ISR_ADRDY) == 0);
        return *this;
    }

    /// @brief Disable the ADC and wait until disabled.
    /// @return *this
    auto &disable() {
        adc->CR = DEFAULT_CR | ADC_CR_ADDIS;
        while ((adc->CR & ADC_CR_ADDIS) != 0) {}
        return *this;
    }

    /// @brief Combination of configure() and enable().
    /// Note: Call only when no conversion is running.
    /// @param config Configuraton (resolution, alignment, etc.)
    /// @param trigger Trigger source
    /// @param dmaMode DMA mode (default: disabled)
    /// @return *this
    auto &enable(Config config, Trigger trigger, DmaMode dmaMode = DmaMode(0)) {
        configure(config, trigger, dmaMode);
        return enable();
    }

    /// @brief Set input of ADC
    /// Note: Call only when no conversion is running.
    /// @param input to sample from
    /// @return *this
    auto &setInput(Input input) {
        adc::setInput(adc, input);
        return *this;
    }

    /// @brief Set input sequence list
    /// Note: Call only when no conversion is running.
    /// @param sequence input sequence to sample from
    /// @return *this
    auto &setSequence(const Array<const Input> &sequence) {
        adc::setSequence(adc, sequence);
        return *this;
    }

    /// @brief Start conversion or wait for trigger if a trigger is enabled.
    /// @return *this
    auto &start() {
        adc->CR = DEFAULT_CR | ADC_CR_ADEN | ADC_CR_ADSTART;
        return *this;
    }

    /// @brief Stop conversion.
    /// @return *this
    auto &stop() {
        adc->CR = DEFAULT_CR | ADC_CR_ADEN | ADC_CR_ADSTP;
        return *this;
    }

    /// @brief Wait until stop completes.
    /// @return *this
    auto &waitStop() {
        while ((adc->CR & ADC_CR_ADSTP) != 0);
        return *this;
    }

    /// @brief Get status flags.
    /// @return Current status flags
    Status status() const {
        return Status(adc->ISR);
    }

    /// @brief Clear status flags.
    /// @param status Status flags to clear
    /// @return *this
    auto &clear(Status status) {
        adc->ISR = uint32_t(status);
        return *this;
    }

    /// @brief Wait for end of conversion (EOC).
    /// Note that the EOC flag is not cleard, read the data register to clear.
    /// @return *this
    auto &wait() {
        while ((adc->ISR & ADC_ISR_EOC) == 0);
        return *this;
    }

    /// @brief Wait until all of the given status flags are true.
    /// Clears all given status flags before return
    /// @param status Status flags to wait for
    auto &wait(Status status) {
        while ((adc->ISR & uint32_t(status)) != uint32_t(status));
        return *this;
    }

    /// @brief Wait until at least one of the given status flags is true.
    /// Clears active status flags before return
    /// @param status Status flags to wait for
    /// @return Active status flags
    Status waitOne(Status status) {
        uint32_t isr;
        do {
            isr = adc->ISR;
        } while ((isr & uint32_t(status)) == 0);
        return Status(isr);
    }

    /// @brief Get conversion result
    /// @return Conversion result from data register
    int data() {return adc->DR;}

#pragma GCC diagnostic ignored "-Wstrict-aliasing"
    /// @brief Get 8 bit reference to data register
    /// @return Reference to data register
    volatile uint8_t &DR8() {return reinterpret_cast<volatile uint8_t &>(adc->DR);}

    /// @brief Get 16 bit reference to data register
    /// @return Reference to data register
    volatile uint16_t &DR16() {return reinterpret_cast<volatile uint16_t &>(adc->DR);}
#pragma GCC diagnostic pop

    /// @brief Get 32 bit reference to data register
    /// @return Reference to data register
    volatile uint32_t &DR32() {return adc->DR;}
};


#ifdef HAVE_ADC_DUAL_MODE
/// @brief Wrapper for ADC regsiters in dual mode
///
struct DualInstance {
    ADC_Common_TypeDef *common;
    ADC_TypeDef *adc[2];


    /// @brief Get first or second ADC intance.
    /// @param index Index 0 or 1
    /// @return Instance
    Instance operator [](int index) {return {adc[index]};}

    /// @brief Calibrate the ADC in single ended mode.
    /// @return *this
    auto &calibrateSingleEnded() {
        adc[0]->CR = DEFAULT_CR | ADC_CR_ADCAL;
        adc[1]->CR = DEFAULT_CR | ADC_CR_ADCAL;
        while (((adc[0]->CR | adc[1]->CR) & ADC_CR_ADCAL) != 0) {}
        for (int i = 0; i < 8; ++i)
            __NOP();
        return *this;
    }

#ifdef HAVE_ADC_DIFFERENTIAL_MODE
    /// @brief Calibrate the ADC in differential mode.
    /// @return *this
    auto &calibrateDifferential() {
        adc[0]->CR = DEFAULT_CR | ADC_CR_ADCAL | ADC_CR_ADCALDIF;
        adc[1]->CR = DEFAULT_CR | ADC_CR_ADCAL | ADC_CR_ADCALDIF;
        while (((adc[0]->CR | adc[1]->CR) & ADC_CR_ADCAL) != 0) {}
        for (int i = 0; i < 8; ++i)
            __NOP();
        return *this;
    }
#endif

    /// @brief Calibrate ADC in both single ended and differential mode.
    /// @return *this
    auto &calibrate() {
        calibrateSingleEnded();
#ifdef HAVE_ADC_DIFFERENTIAL_MODE
        calibrateDifferential();
#endif
        return *this;
    }

    /// @brief Configure the ADC.
    /// Note: Call only when no conversion is running.
    /// @param config Configuraton (resolution, alignment, etc.)
    /// @param trigger Trigger source
    /// @param dmaMode DMA mode (default: disabled)
    /// @return *this
    DualInstance &configure(Config config, Trigger trigger, DmaMode dmaMode = DmaMode(0));
    // implementation is platform dependent

    /// @brief Enable the ADC and wait until ready.
    /// @return *this
    auto &enable() {
        do {
            adc[0]->CR = DEFAULT_CR | ADC_CR_ADEN;
            adc[1]->CR = DEFAULT_CR | ADC_CR_ADEN;
        } while ((adc[0]->ISR & adc[1]->ISR & ADC_ISR_ADRDY) == 0);
        return *this;
    }

    /// @brief Disable the ADC and wait until disabled.
    /// @return *this
    auto &disable() {
        adc[0]->CR = DEFAULT_CR | ADC_CR_ADDIS;
        adc[1]->CR = DEFAULT_CR | ADC_CR_ADDIS;
        while (((adc[0]->CR | adc[1]->CR) & ADC_CR_ADDIS) != 0) {}
        return *this;
    }

    /// @brief Combination of configure() and enable().
    /// Note: Call only when no conversion is running.
    /// @param config Configuraton (resolution, alignment, etc.)
    /// @param trigger Trigger source
    /// @param dmaMode DMA mode (default: disabled)
    /// @return *this
    auto &enable(Config config, Trigger trigger, DmaMode dmaMode = DmaMode(0)) {
        configure(config, trigger, dmaMode);
        return enable();
    }

    /// @brief Set input of first ADC.
    /// @param input to sample from
    /// @return *this
    auto &setInput1(Input input) {
        adc::setInput(adc[0], input);
        return *this;
    }

    /// @brief Set input of second ADC.
    /// @param input to sample from
    /// @return *this
    auto &setInput2(Input input) {
        adc::setInput(adc[1], input);
        return *this;
    }

    /// @brief Set input sequence list of first ADC.
    /// @param sequence input sequence to sample from
    /// @return *this
    auto &setSequence1(const Array<const Input> &sequence) {
        adc::setSequence(adc[0], sequence);
        return *this;
    }

    /// @brief Set input sequence list of second ADC.
    /// @param sequence input sequence to sample from
    /// @return *this
    auto &setSequence2(const Array<const Input> &sequence) {
        adc::setSequence(adc[1], sequence);
        return *this;
    }

    /// @brief Start dual conversion or wait for trigger if a trigger is enabled.
    /// @return *this
    auto &start() {
        adc[0]->CR = DEFAULT_CR | ADC_CR_ADEN | ADC_CR_ADSTART;
        return *this;
    }

    /// @brief Stop dual conversion.
    /// @return *this
    auto &stop() {
        adc[0]->CR = DEFAULT_CR | ADC_CR_ADEN | ADC_CR_ADSTP;
        return *this;
    }

    /// @brief Wait until stop completes.
    /// @return *this
    auto &waitStop() {
        while ((adc[0]->CR & ADC_CR_ADSTP) != 0);
        return *this;
    }

    /// @brief Get the or-combination of the status flags of both ADCs.
    /// @return Status flags
    Status status() const {
        uint32_t csr = common->CSR;
        return Status((csr | (csr >> 16)) & 0xffff);
    }

    /// @brief Get the status flags of the first ADC.
    /// @return Status flags
    Status status1() const {
        return Status(adc[0]->ISR);
    }

    /// @brief Get the status flags of the second ADC.
    /// @return Status flags
    Status status2() const {
        return Status(adc[1]->ISR);
    }

    /// @brief Clear status flags of both ADCs.
    /// @param status Status flags to clear
    /// @return *this
    auto &clear(Status status) {
        adc[0]->ISR = uint32_t(status);
        adc[1]->ISR = uint32_t(status);
        return *this;
    }

    /// @brief Clear status flags of the first ADC.
    /// @param status Status flags to clear
    /// @return *this
    auto &clearStatus1(Status status) {
        adc[0]->ISR = uint32_t(status);
        return *this;
    }

    /// @brief Clear status flags of the second ADC.
    /// @param status Status flags to clear
    /// @return *this
    auto &clearStatus2(Status status) {
        adc[1]->ISR = uint32_t(status);
        return *this;
    }

    /// @brief Wait for end of dual conversion (EOC for both ADCs).
    /// Note that the EOC flags are not cleard, read the data register to clear.
    /// @return *this
    auto &wait() {
        while ((common->CSR & (ADC_CSR_EOC_MST | ADC_CSR_EOC_SLV)) != (ADC_CSR_EOC_MST | ADC_CSR_EOC_SLV));
        return *this;
    }

    /// @brief Wait until all of the given status flags are true for both ADCs.
    /// Call clear() before or after wait() to clear active status flags
    /// @param status Status flags to wait for
    auto &wait(Status status) {
        uint32_t s = uint32_t(status) | (uint32_t(status) << 16);
        while ((common->CSR & s) != s);
        return *this;
    }

    /// @brief Wait until at least one of the given status flags is true for both ADCs.
    /// Call clear() before or after wait() to clear active status flags
    /// @param status Status flags to wait for
    /// @return Active status flags
    Status waitOne(Status status) {
        uint32_t csr;
        do {
            csr = common->CSR;
        } while (((csr & uint32_t(status)) == 0) || (((csr >> 16) & uint32_t(status)) == 0));
        return Status((csr | (csr >> 16)) & 0xffff);
    }

    /// @brief Get first conversion result.
    /// @return First conversion result from data register
    int data1() {return adc[0]->DR;}

    /// @brief Get second conversion result.
    /// @return Second conversion result from data register
    int data2() {return adc[1]->DR;}

    /// @brief Get combined conversion result.
    /// @return Pair containing both conversion results from common data register
    std::pair<int, int> data() {
        uint32_t cdr = common->CDR;
        return {int(cdr & 0xffff), int(cdr >> 16)};
    }

    /// @brief Get 32 bit reference to common data register
    /// @return Reference to data register
    volatile uint32_t &DR32() {return common->CDR;}
};
#endif // HAVE_ADC_DUAL_MODE


/// @brief Info for an ADC in independent mode.
/// Contains pointer to registers, reset and clock control, interrupt and DMA request index
struct Info {
    // registers
    ADC_Common_TypeDef *common;
    ADC_TypeDef *adc;

#ifndef STM32L4
    // reset and clock control
    rcc::AdcInfo rcc;
#endif

    // interrupt request index
    uint8_t irq;

    // DMA request index
    uint8_t drq;


    /// @brief Enable clock.
    /// Also enables the voltage regulator of the ADC.
    /// @param clockConfig Clock configuration
    /// @return Instance (wrapper for registers)
    Instance enableClock(ClockConfig clockConfig) const;

    /// @brief Get the instance
    /// @return Instance (wrapper for registers)
    Instance instance() const {return {adc};}

    /// @brief Map a DMA channel to the the ADC
    /// @param dmaInfo DMA info
    template <dma::Feature F2>
    void map(const dma::Info<F2> &dmaInfo) const;
};


#ifdef HAVE_ADC_DUAL_MODE
/// @brief Info for two ADCs in dual mode.
/// Contains pointer to registers, reset and clock control, interrupt and DMA request index
struct DualInfo {
    ADC_Common_TypeDef *common;
    ADC_TypeDef *adc[2];

#ifndef STM32L4
    // reset and clock control
    rcc::AdcInfo rcc;
#endif

    // interrupt request index
    uint8_t irq;

    // DMA request index
    uint8_t drq;


    /// @brief Enable clock.
    /// Also enables the voltage regulator of both ADCs.
    /// @param clockConfig Clock configuration
    /// @return Instance (wrapper for registers)
    DualInstance enableClock(ClockConfig clockConfig) const;

    /// @brief Get the instance
    /// @return Instance (wrapper for registers)
    DualInstance instance() const {return {common, {adc[0], adc[1]}};}

    /// @brief Map a DMA channel to the the ADC
    /// @param dmaInfo DMA info
    /// @param channelIndex channel index 0 or 1
    template <dma::Feature F2>
    void map(const dma::Info<F2> &dmaInfo, int channelIndex = 0) const;
};
#endif // HAVE_ADC_DUAL_MODE

} // namespace adc
} // namespace coco
