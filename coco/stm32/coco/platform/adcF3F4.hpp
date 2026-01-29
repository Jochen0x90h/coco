#pragma once

// do not include directly, use #include <coco/platform/adc.hpp>

#include "dma.hpp"
#include <coco/Array.hpp>
#include <coco/bits.hpp>
#include <coco/enum.hpp>
#include <coco/debug.hpp> // for debug::sleep()


/*
    Defines:
    HAVE_ADC_DUAL_MODE          Supports two ADCs in dual mode
*/


// detect ADC dual mode
#ifdef ADC2
#define HAVE_ADC_DUAL_MODE
#endif


namespace coco {

/// @brief ADC helpers.
///
namespace adc {

// device-specific enums
enum class ClockConfig : uint32_t;
enum class Config : uint32_t;
enum class Trigger : uint32_t;
enum class DmaMode : uint32_t;


#ifdef HAVE_ADC_DUAL_MODE
/// @brief ADC common configuration
///
enum class CommonConfig {
    // mode
    INDEPENDENT = 0,
    REGULAR_SIMULTANEOUS = ADC_CCR_MULTI_2 | ADC_CCR_MULTI_1,

    // DMA
    MDMA_12_10 = ADC_CCR_DMA_1,
    MDMA_8_6 = ADC_CCR_DMA_0 | ADC_CCR_DMA_1,
    CIRCULAR_DMA = ADC_CCR_DDS,
};
#endif // HAVE_ADC_DUAL_MODE



/// @brief ADC input, combination of channel index and sampling cycles.
/// Note: See setSequence() for hardware limitations
enum class Input : uint16_t {
    CHANNEL_0 = 0,
    CHANNEL_1 = 1,
    CHANNEL_2 = 2,
    CHANNEL_3 = 3,
    CHANNEL_4 = 4,
    CHANNEL_5 = 5,
    CHANNEL_6 = 6,
    CHANNEL_7 = 7,
    CHANNEL_8 = 8,
    CHANNEL_9 = 9,
    CHANNEL_10 = 10,
    CHANNEL_11 = 11,
    CHANNEL_12 = 12,
    CHANNEL_13 = 13,
    CHANNEL_14 = 14,
    CHANNEL_15 = 15,
    CHANNEL_MASK = 0x1f,

    // number of sampling cycles
    CYCLES_3 = 0x0000,
    CYCLES_15 = 0x0100,
    CYCLES_28 = 0x0200,
    CYCLES_56 = 0x0300,
    CYCLES_84 = 0x0400,
    CYCLES_112 = 0x0500,
    CYCLES_144 = 0x0600,
    CYCLES_480 = 0x0700,
    CYCLES_MASK = 0x0700,
};
COCO_ENUM(Input);

/// @brief Status of ADC (SR register).
///
enum class Status : uint32_t {
    NONE = 0,

    // end of conversion
    EOC = ADC_SR_EOC,

#ifdef ADC_SR_OVR
    // overrun
    OVERRUN = ADC_SR_OVR,
#endif

    // analog watchdog
    WATCHDOG1 = ADC_SR_AWD,

    ALL = 0xffffffff
};
COCO_ENUM(Status);

/*
// enable ADC
inline void enable(ADC_TypeDef *adc) {
    adc->CR2 = ADC_CR2_ADON;
}

// enable dual ADC
inline void enable(ADC_TypeDef *adc1, ADC_TypeDef *adc2) {
    adc1->CR2 = ADC_CR2_ADON;
    adc2->CR2 = ADC_CR2_ADON;
}
*/

/// Set ADC input.
/// @param adc ADC registers
/// @param input Input and sampling cycles
inline void setInput(ADC_TypeDef *adc, Input input) {
    int channel = int(input & Input::CHANNEL_MASK);

    // select input
    adc->SQR1 = 0;
    adc->SQR3 = channel;

    // set sampling cycles
    uint32_t smpr[2] = {0, 0};
    smpr[channel / 10] = extract(int(input), int(Input::CYCLES_MASK)) << (channel % 10) * 3;
    adc->SMPR2 = smpr[0]; // note SMPR2 contains channels 0-9
    adc->SMPR1 = smpr[1];
}

/// @brief Set ADC input sequence list.
/// Note: When using a channel more than once in a sequence, the sampling cycles must be the same or can be omitted
/// @param adc ADC registers
/// @param sequence Input sequence of up to 16 inputs and sampling cycles for each input
inline void setSequence(ADC_TypeDef *adc, const Array<const Input> &sequence) {
    // sequence register SQR
    auto SQRn = &adc->SQR3;
    uint32_t sqrValue = 0;
    int sqrPos = ADC_SQR3_SQ1_Pos;
    const int sqWidth = ADC_SQR3_SQ2_Pos - ADC_SQR3_SQ1_Pos;

    // sampling rate registers SMPR
    uint32_t smpr[2] = {0, 0};

    for (auto input : sequence) {
        if (sqrPos > 32 - sqWidth) {
            *SQRn = sqrValue;
            --SQRn; // reverse order of SQR registers
            sqrValue = 0;
            sqrPos = 0;
        }

        // sequence
        int channel = int(input & Input::CHANNEL_MASK);
        sqrValue |= channel << sqrPos;

        // sampling cycles
        smpr[channel / 10] |= extract(int(input), int(Input::CYCLES_MASK)) << channel % 10;

        sqrPos += sqWidth;
    }
    *SQRn = sqrValue;
    adc->SQR1 = adc->SQR1 | ((sequence.size() - 1) << ADC_SQR1_L_Pos);
    adc->SMPR2 = smpr[0]; // note SMPR2 contains channels 0-9
    adc->SMPR1 = smpr[1];
}
/*
/// @brief Start conversion or wait for trigger if a trigger is enabled.
/// @param adc ADC instance
inline void start(ADC_TypeDef *adc) {
    adc->CR2 = adc->CR2 | ADC_CR2_ADON | ADC_CR2_SWSTART;
}

/// @brief Stop conversion.
/// @param adc ADC instance
inline void stop(ADC_TypeDef *adc) {
}

/// @brief Wait for end of conversion (EOC).
/// Note that the EOC flag is not cleard, read the data register to clar
/// @param adc ADC instance
inline void wait(ADC_TypeDef *adc) {
    while ((adc->SR & ADC_SR_EOC) == 0);
}

/// @brief Wait for end of dual conversion
/// @param common common registers
inline void wait(ADC_Common_TypeDef *common) {
#ifdef ADC_CSR_EOC2
    while ((common->CSR & (ADC_CSR_EOC1 | ADC_CSR_EOC2)) != (ADC_CSR_EOC1 | ADC_CSR_EOC2));
#else
    while ((common->CSR & ADC_CSR_EOC1) == 0);
#endif
}
*/

/// @brief Wrapper for ADC registers
///
struct Instance {
    ADC_TypeDef *adc;


    ADC_TypeDef *operator ->() const {return adc;}
    operator ADC_TypeDef *() const {return adc;}

    /// @brief Dummy method for calibration, calibration is not necessary for this processor.
    /// @return *this
    auto &calibrate() {
        return *this;
    }

    /// @brief Configure the ADC.
    /// Note: Call only when no conversion is running.
    /// @param config Configuraton (resolution, alignment, etc.)
    /// @param trigger Trigger source
    /// @param dmaMode DMA mode (default: disabled)
    /// @return *this
    Instance &configure(Config config, Trigger trigger, DmaMode dmaMode = DmaMode(0)) {
        // set configuration (e.g. resolution), trigger and DMA mode
        adc->CR1 = int(config);
        adc->CR2 = int(trigger) | int(dmaMode);

        return *this;
    }

    /// @brief Enable the ADC.
    /// @return *this
    auto &enable() {
        adc->CR2 = adc->CR2 | ADC_CR2_ADON;
        return *this;
    }

    /// @brief Disable the ADC.
    /// @return *this
    auto &disable() {
        adc->CR2 = adc->CR2 & ~ADC_CR2_ADON;
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

    /// @brief Set input of ADC.
    /// @param input to sample from
    /// @return *this
    auto &setInput(Input input) {
        adc::setInput(adc, input);
        return *this;
    }

    /// @brief Set input sequence list.
    /// @param sequence input sequence to sample from
    /// @return *this
    auto &setSequence(const Array<const Input> &sequence) {
        adc::setSequence(adc, sequence);
        return *this;
    }

    /// @brief Start conversion or wait for trigger if a trigger is enabled.
    /// @return *this
    auto &start() {
        adc->CR2 = adc->CR2 | ADC_CR2_SWSTART;
        return *this;
    }

    /// @brief Stop conversion.
    /// @return *this
    auto &stop() {
        adc->CR2 = adc->CR2 & ~ADC_CR2_SWSTART;
        return *this;
    }

    /// @brief Get status flags.
    /// @return Current status flags
    Status status() const {
        return Status(adc->SR);
    }

    /// @brief Clear status flags.
    /// @param status Status flags to clear
    /// @return *this
    auto &clear(Status status) {
        adc->SR = ~uint32_t(status);
        return *this;
    }

    /// @brief Wait for end of conversion (EOC).
    /// Note that the EOC flag is not cleard, read the data register to clear.
    /// @return *this
    auto &wait() {
        while ((adc->SR & ADC_SR_EOC) == 0);
        return *this;
    }

    /// @brief Wait until all of the given status flags are true.
    /// Call clear() before or after wait() to clear active status flags
    /// @param status Status flags to wait for
    auto &wait(Status status) {
        while ((adc->SR & uint32_t(status)) != uint32_t(status));
        //adc->SR = uint32_t(status);
        return *this;
    }

    /// @brief Wait until at least one of the given status flags is true.
    /// Call clear() before or after wait() to clear active status flags
    /// @param status Status flags to wait for
    /// @return Active status flags
    Status waitOne(Status status) {
        uint32_t sr;
        do {
            sr = adc->SR;
        } while ((sr & uint32_t(status)) == 0);
        //adc->SR = sr;
        return Status(sr);
    }

    /// @brief Get conversion result.
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
    ADC_Common_TypeDef *common; // needed for common data register
    ADC_TypeDef *adc[2];


    /// @brief Get first or second ADC intance.
    /// @param index Index 0 or 1
    /// @return Instance
    Instance operator [](int index) {return {adc[index]};}

    /// @brief Dummy method for calibration, calibration is not necessary for this processor.
    /// @return *this
    auto &calibrate() {
        return *this;
    }

    /// @brief Configure the ADC.
    /// Note: Call only when no conversion is running.
    /// @param config Configuraton (resolution, alignment, etc.)
    /// @param trigger Trigger source
    /// @param dmaMode DMA mode (default: disabled)
    /// @return *this
    DualInstance &configure(Config config, Trigger trigger, DmaMode dmaMode = DmaMode(0)) {
        // set configuration (e.g. resolution) and trigger
        adc[0]->CR1 = int(config);
        adc[0]->CR1 = int(config);
        adc[1]->CR2 = int(trigger);
        adc[1]->CR2 = int(trigger);

        // set DMA mode in common registers
        uint32_t ccr = common->CCR & ~(ADC_CCR_DMA | ADC_CCR_DDS);
        if (dmaMode != DmaMode(0)) { // DmaMode::DISABLED
            uint32_t resolution = extract(uint32_t(config), ADC_CR1_RES_Msk); // Config::RES_MASK
            ccr |= ((2 + (resolution >> 1)) << ADC_CCR_DMA_Pos) // DMA mode
                | (extract(uint32_t(dmaMode), ADC_CR2_DDS) << ADC_CCR_DDS_Pos); // circular mode flag
        }
        common->CCR = ccr;

        return *this;
    }

    /// @brief Enable Athe DC.
    /// @return *this
    auto &enable() {
        adc[0]->CR2 = adc[0]->CR2 | ADC_CR2_ADON;
        adc[1]->CR2 = adc[1]->CR2 | ADC_CR2_ADON;
        return *this;
    }

    /// @brief Disable the ADC.
    /// @return *this
    auto &disable() {
        adc[0]->CR2 = adc[0]->CR2 & ~ADC_CR2_ADON;
        adc[1]->CR2 = adc[1]->CR2 & ~ADC_CR2_ADON;
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
        adc[0]->CR2 = adc[0]->CR2 | ADC_CR2_SWSTART;
        return *this;
    }

    /// @brief Stop dual conversion.
    /// @return *this
    auto &stop() {
        adc[0]->CR2 = adc[0]->CR2 & ~ADC_CR2_SWSTART;
        return *this;
    }

    /// @brief Get the or-combination of the status flags of both ADCs.
    /// @return Status flags
    Status status() const {
        uint32_t csr = common->CSR;
        return Status((csr | (csr >> 8)) & 0xff);
    }

    /// @brief Get the status flags of the first ADC.
    /// @return Status flags
    Status status1() const {
        return Status(adc[0]->SR);
    }

    /// @brief Get the status flags of the second ADC.
    /// @return Status flags
    Status status2() const {
        return Status(adc[1]->SR);
    }

    /// @brief Clear status flags of both ADCs.
    /// @param status Status flags to clear
    /// @return *this
    auto &clear(Status status) {
        adc[0]->SR = uint32_t(status);
        adc[1]->SR = uint32_t(status);
        return *this;
    }

    /// @brief Clear status flags of the first ADC.
    /// @param status Status flags to clear
    /// @return *this
    auto &clearStatus1(Status status) {
        adc[0]->SR = ~uint32_t(status);
        return *this;
    }

    /// @brief Clear status flags of the second ADC.
    /// @param status Status flags to clear
    /// @return *this
    auto &clearStatus2(Status status) {
        adc[1]->SR = ~uint32_t(status);
        return *this;
    }

    /// @brief Wait for end of dual conversion (EOC for both ADCs).
    /// Note that the EOC flags are not cleard, read the data register to clear.
    /// @return *this
    auto &wait() {
#ifdef ADC_CSR_EOC2
        while ((common->CSR & (ADC_CSR_EOC1 | ADC_CSR_EOC2)) != (ADC_CSR_EOC1 | ADC_CSR_EOC2));
#else
        while ((common->CSR & ADC_CSR_EOC1) == 0);
#endif
        return *this;
    }

    /// @brief Wait until all of the given status flags are true for both ADCs.
    /// Call clear() before or after wait() to clear active status flags
    /// @param status Status flags to wait for
    auto &wait(Status status) {
        uint32_t s = uint32_t(status) | (uint32_t(status) << 8);
        while ((common->CSR & s) != s);
        //adc[0]->SR = uint32_t(status);
        //adc[1]->SR = uint32_t(status);
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
        //adc[0]->SR = csr;
        //adc[1]->SR = csr >> 8;
        return Status((csr | (csr >> 8)) & 0xff);
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
#endif


/// @brief Info for an ADC in independent mode
/// Contains pointer to registers, reset and clock control, interrupt and DMA request index
struct Info {
    // registers
    ADC_Common_TypeDef *common;
    ADC_TypeDef *adc;

    // reset and clock control
    rcc::AdcInfo rcc;

    // interrupt request index
    uint8_t irq;

    // DMA request index
    uint8_t drq;


    /// @brief Enable clock.
    /// @param clockConfig Clock configuration
    /// @return Instance (wrapper for registers)
    Instance enableClock(ClockConfig clockConfig) const {
        // enable clock
        rcc.enableClock();

#ifdef ADC_CCR_ADCPRE
        // set clock config
        common->CCR = int(clockConfig);
#endif

        // return Instance
        return {adc};
    }

    /// @brief Map a DMA channel to the the ADC
    /// Check reference manual which mappings are possible
    /// @param dmaInfo DMA info
    template <dma::Feature F2>
    void map(const dma::Info<F2> &dmaInfo) const;
};

#ifdef HAVE_ADC_DUAL_MODE
/// @brief Info for two ADCs in dual mode
/// Contains pointer to registers, reset and clock control, interrupt and DMA request index
struct DualInfo {
    // registers
    ADC_Common_TypeDef *common;
    ADC_TypeDef *adc[2];

    // reset and clock control
    rcc::AdcInfo rcc;

    // interrupt of first channel
    uint8_t irq;

    // DMA request index
    uint8_t drq;


    /// @brief Enable clock.
    /// @param clockConfig Clock configuration
    /// @return Instance (wrapper for registers)
    DualInstance enableClock(ClockConfig clockConfig) const {
        // enable clock
        rcc.enableClock();

        // set clock config
        common->CCR = int(clockConfig) | ADC_CCR_MULTI_2 | ADC_CCR_MULTI_1; // regular simultaneous mode;

        // return DualInstance
        return {common, {adc[0], adc[1]}};
    }

    /// @brief Map a DMA channel to the the ADC
    /// Check reference manual which mappings are possible
    /// @param dmaInfo DMA info
    /// @param channelIndex channel index 0 or 1
    template <dma::Feature F2>
    void map(const dma::Info<F2> &dmaInfo, int channelIndex = 0) const;
};
#endif

} // namespace adc
} // namespace coco
