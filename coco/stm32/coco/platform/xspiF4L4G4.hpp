#pragma once

// do not include directly, use #include <coco/platform/xspi.hpp>

#include "dma.hpp"
#include <coco/enum.hpp>


/*
    Defines:
    HAVE_XSPI              QUADSPI supported
    HAVE_XSPI_DUAL_BANK    Dual bank supported
*/

#ifdef QUADSPI
#define HAVE_XSPI

#ifdef QUADSPI_CR_FSEL
#define HAVE_XSPI_DUAL_BANK
#endif

namespace coco {

/// @brief QUADSPI (extended SPI) helpers.
/// F469/479 https://www.st.com/en/microcontrollers-microprocessors/stm32f4-series/documentation.html Section 13
/// L46- https://www.st.com/en/microcontrollers-microprocessors/stm32l4-series/documentation.html Section 15
/// L47+ https://www.st.com/en/microcontrollers-microprocessors/stm32l4-series/documentation.html Section 17
/// G4 https://www.st.com/en/microcontrollers-microprocessors/stm32g4-series/documentation.html Section 20
namespace xspi {

constexpr uint32_t FORMAT_CR_MASK = QUADSPI_CR_PRESCALER | QUADSPI_CR_SSHIFT
#ifdef HAVE_XSPI_DUAL_BANK
    | QUADSPI_CR_FSEL | QUADSPI_CR_DFM;
#endif
    ;
constexpr uint32_t FORMAT_DCR_MASK = QUADSPI_DCR_CKMODE | QUADSPI_DCR_FSIZE;
static_assert((FORMAT_CR_MASK & FORMAT_DCR_MASK) == 0, "Bit overlap in enum Format!");

/// @brief Format (CR, DCR registers).
///
enum class Format : uint32_t {
    NONE = 0,

    // clock prescaler
    CLOCK_DIV_1 = 0, // default
    CLOCK_DIV_2 = 1 << QUADSPI_CR_PRESCALER_Pos,
    CLOCK_DIV_3 = 2 << QUADSPI_CR_PRESCALER_Pos,
    CLOCK_DIV_4 = 3 << QUADSPI_CR_PRESCALER_Pos,
    CLOCK_DIV_6 = 5 << QUADSPI_CR_PRESCALER_Pos,
    CLOCK_DIV_8 = 7 << QUADSPI_CR_PRESCALER_Pos,
    CLOCK_DIV_12 = 11 << QUADSPI_CR_PRESCALER_Pos,
    CLOCK_DIV_16 = 15 << QUADSPI_CR_PRESCALER_Pos,
    CLOCK_DIV_24 = 23 << QUADSPI_CR_PRESCALER_Pos,
    CLOCK_DIV_32 = 31 << QUADSPI_CR_PRESCALER_Pos,
    CLOCK_DIV_48 = 47 << QUADSPI_CR_PRESCALER_Pos,
    CLOCK_DIV_64 = 63 << QUADSPI_CR_PRESCALER_Pos,
    CLOCK_DIV_96 = 95 << QUADSPI_CR_PRESCALER_Pos,
    CLOCK_DIV_128 = 127 << QUADSPI_CR_PRESCALER_Pos,
    CLOCK_DIV_192 = 191u << QUADSPI_CR_PRESCALER_Pos,
    CLOCK_DIV_256 = 255u << QUADSPI_CR_PRESCALER_Pos,

    // clock mode, 0: CLK low while NCS high, 3: CLK high while NCS high
    CLOCK_MODE_0 = 0, // default
    CLOCK_MODE_3 = 1 << QUADSPI_DCR_CKMODE_Pos,

    // bank selection
    BANK_1 = 0, // default
#ifdef HAVE_XSPI_DUAL_BANK
    BANK_2 = QUADSPI_CR_FSEL,
    DUAL_BANK = QUADSPI_CR_DFM,
#endif

    // memory size in bytes
    MEMORY_16B = 3 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_32B = 4 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_64B = 5 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_128B = 6 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_256B = 7 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_512B = 8 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_1KB = 9 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_2KB = 10 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_4KB = 11 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_8KB = 12 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_16KB = 13 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_32KB = 14 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_64KB = 15 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_128KB = 16 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_256KB = 17 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_512KB = 18 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_1MB = 19 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_2MB = 20 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_4MB = 21 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_8MB = 22 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_16MB = 23 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_32MB = 24 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_64MB = 25 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_128MB = 26 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_256MB = 27 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_512MB = 28 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_1GB = 29 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_2GB = 30 << QUADSPI_DCR_FSIZE_Pos,
    MEMORY_4GB = 31 << QUADSPI_DCR_FSIZE_Pos,

    // sample data one CLK cycle after data is driven by external device (instead of 1/2 CLK cycle, no DDR mode)
    SAMPLE_SHIFT = QUADSPI_CR_SSHIFT,
};
COCO_ENUM(Format)

/// @brief Get CR register contents from format.
/// @param format Format enum
/// @return Contents of CR register
constexpr uint32_t CR(Format format) {
    return uint32_t(format) & FORMAT_CR_MASK;
}

/// @brief Get DCR register contents from format.
/// @param format Format enum
/// @return Contents of DCR register
constexpr uint32_t DCR(Format format) {
    return uint32_t(format) & FORMAT_DCR_MASK;
}


/// @brief Poll config (CR register).
///
enum class PollConfig : uint32_t {
    AUTO_STOP = QUADSPI_CR_APMS,

    MATCH_AND = 0, // default
    MATCH_OR = QUADSPI_CR_PMM,
};


/// @brief Functional mode (CCR register)
///
enum class Function : uint32_t {
    INDIRECT_WRITE = 0, // default
    INDIRECT_READ = QUADSPI_CCR_FMODE_0,
    STATUS_POLLING = QUADSPI_CCR_FMODE_1,
    MEMORY_MAPPED = QUADSPI_CCR_FMODE_1 | QUADSPI_CCR_FMODE_0,
};


/// @brief Communication mode (CCR register).
/// Mode for instruction, address, data
enum class Mode : uint32_t {
    // instruction mode
    INSTRUCTION_NONE = 0, // default
    INSTRUCTION_1_LINE = QUADSPI_CCR_IMODE_0,
    INSTRUCTION_2_LINES = QUADSPI_CCR_IMODE_1,
    INSTRUCTION_4_LINES = QUADSPI_CCR_IMODE_1 | QUADSPI_CCR_IMODE_0,
    INSTRUCTION_MASK = QUADSPI_CCR_IMODE_Msk,

    // address mode
    ADDRESS_NONE = 0, // default
    ADDRESS_1_LINE = QUADSPI_CCR_ADMODE_0,
    ADDRESS_2_LINES = QUADSPI_CCR_ADMODE_1,
    ADDRESS_4_LINES = QUADSPI_CCR_ADMODE_1 | QUADSPI_CCR_ADMODE_0,
    ADDRESS_MASK = QUADSPI_CCR_ADMODE_Msk,

    // alternate-byte mode
    ALTERNATE_NONE = 0, // default
    ALTERNATE_1_LINE = QUADSPI_CCR_ABMODE_0,
    ALTERNATE_2_LINES = QUADSPI_CCR_ABMODE_1,
    ALTERNATE_4_LINES = QUADSPI_CCR_ABMODE_1 | QUADSPI_CCR_ABMODE_0,
    ALTERNATE_MASK = QUADSPI_CCR_ABMODE_Msk,

    // data mode
    DATA_NONE = 0, // default
    DATA_1_LINE = QUADSPI_CCR_DMODE_0,
    DATA_2_LINES = QUADSPI_CCR_DMODE_1,
    DATA_4_LINES = QUADSPI_CCR_DMODE_1 | QUADSPI_CCR_DMODE_0,
    DATA_MASK = QUADSPI_CCR_DMODE_Msk,

    // double data rate (DDR)
    DDR = QUADSPI_CCR_DDRM,
};
COCO_ENUM(Mode)

// convenience for Mode: Number of lines for instruction, addres, data, x2 for double data rate
constexpr auto MODE_DISABLED = Mode::INSTRUCTION_NONE | Mode::ADDRESS_NONE | Mode::DATA_NONE;
constexpr auto MODE_1_0_0 = Mode::INSTRUCTION_1_LINE | Mode::ADDRESS_NONE | Mode::DATA_NONE;
constexpr auto MODE_1_0_1 = Mode::INSTRUCTION_1_LINE | Mode::ADDRESS_NONE | Mode::DATA_1_LINE;
constexpr auto MODE_1_1_1 = Mode::INSTRUCTION_1_LINE | Mode::ADDRESS_1_LINE | Mode::DATA_1_LINE;
constexpr auto MODE_1_1_2 = Mode::INSTRUCTION_1_LINE | Mode::ADDRESS_1_LINE | Mode::DATA_2_LINES;
constexpr auto MODE_1_1_4 = Mode::INSTRUCTION_1_LINE | Mode::ADDRESS_1_LINE | Mode::DATA_4_LINES;
constexpr auto MODE_1_2_2 = Mode::INSTRUCTION_1_LINE | Mode::ADDRESS_2_LINES | Mode::DATA_2_LINES;
constexpr auto MODE_1_4_4 = Mode::INSTRUCTION_1_LINE | Mode::ADDRESS_4_LINES | Mode::DATA_4_LINES;
constexpr auto MODE_1S_4D_4D = Mode::INSTRUCTION_1_LINE | Mode::ADDRESS_4_LINES | Mode::DATA_4_LINES | Mode::DDR;
constexpr auto MODE_2_0_0 = Mode::INSTRUCTION_2_LINES | Mode::ADDRESS_NONE | Mode::DATA_NONE;
constexpr auto MODE_2_2_2 = Mode::INSTRUCTION_2_LINES | Mode::ADDRESS_2_LINES | Mode::DATA_2_LINES;
constexpr auto MODE_4_0_0 = Mode::INSTRUCTION_4_LINES | Mode::ADDRESS_NONE | Mode::DATA_NONE;
constexpr auto MODE_4_4_4 = Mode::INSTRUCTION_4_LINES | Mode::ADDRESS_4_LINES | Mode::DATA_4_LINES;
constexpr auto MODE_4S_4D_4D = Mode::INSTRUCTION_4_LINES | Mode::ADDRESS_4_LINES | Mode::DATA_4_LINES | Mode::DDR;


/// @brief Timing (CCR register).
///
enum class Timing : uint32_t {
    DEFAULT = 0,

    // dummy cycles (0 to 31)
    DUMMY_CYCLES_0 = 0, // default
    DUMMY_CYCLES_1 = 1 << QUADSPI_CCR_DCYC_Pos,
    DUMMY_CYCLES_2 = 2 << QUADSPI_CCR_DCYC_Pos,
    DUMMY_CYCLES_3 = 3 << QUADSPI_CCR_DCYC_Pos,
    DUMMY_CYCLES_4 = 4 << QUADSPI_CCR_DCYC_Pos,
    DUMMY_CYCLES_5 = 5 << QUADSPI_CCR_DCYC_Pos,
    DUMMY_CYCLES_6 = 6 << QUADSPI_CCR_DCYC_Pos,
    DUMMY_CYCLES_7 = 7 << QUADSPI_CCR_DCYC_Pos,
    DUMMY_CYCLES_8 = 8 << QUADSPI_CCR_DCYC_Pos,
    DUMMY_CYCLES_9 = 9 << QUADSPI_CCR_DCYC_Pos,
    DUMMY_CYCLES_10 = 10 << QUADSPI_CCR_DCYC_Pos,
    DUMMY_CYCLES_11 = 11 << QUADSPI_CCR_DCYC_Pos,
    DUMMY_CYCLES_12 = 12 << QUADSPI_CCR_DCYC_Pos,
    DUMMY_CYCLES_13 = 13 << QUADSPI_CCR_DCYC_Pos,
    DUMMY_CYCLES_14 = 14 << QUADSPI_CCR_DCYC_Pos,
    DUMMY_CYCLES_15 = 15 << QUADSPI_CCR_DCYC_Pos,
    DUMMY_CYCLES_16 = 16 << QUADSPI_CCR_DCYC_Pos,
    DUMMY_CYCLES_MASK = QUADSPI_CCR_DCYC,

    // note that SAMPLE_SHIFT is in enum Format

    // delay the data output by 1/4 of the QUADSPI output clock cycle in DDR mode
    DDR_DELAY_QUARTER = QUADSPI_CCR_DHHC, // CCR bit 30
};
COCO_ENUM(Timing)

constexpr Timing makeTiming(Timing timing, int dummyCycles) {
    return (timing & ~Timing::DUMMY_CYCLES_MASK) | Timing((dummyCycles << QUADSPI_CCR_DCYC_Pos));
}


using CommConfig = uint32_t;

/// @brief Make communication configuration.
/// @tparam I Type of instruction, e.g. int or enum
/// @param mode Mode (number of lines for instruction, address, data)
/// @param instruction SPI Instruction (8 bit)
/// @param addressBytes Number of address bytes (1 to 4)
/// @param dummyCycles Number of SPI clock cycles (0 to 31)
/// @return Contents of CCR register
template <typename I>
constexpr CommConfig makeCommConfig(Mode mode, I instruction, int addressBytes, Timing timing = Timing::DEFAULT) {
    return uint32_t(mode)
        | (int(instruction) << QUADSPI_CCR_INSTRUCTION_Pos)
        | (addressBytes - 1) << QUADSPI_CCR_ADSIZE_Pos
        | uint32_t(timing);
}


/// @brief Interrupt enable flags (CR register).
///
enum class Interrupt : uint32_t {
    NONE = 0,

    // FIFO has reached the threshold
    FIFO_THRESHOLD = QUADSPI_CR_FTIE,

    // transfer complete
    TRANSFER_COMPLETE = QUADSPI_CR_TCIE,

    // transfer error
    ERROR = QUADSPI_CR_TEIE,

    ALL = FIFO_THRESHOLD | TRANSFER_COMPLETE | ERROR,
};
COCO_ENUM(Interrupt)


/// @brief Status flags (SR register).
///
enum class Status : uint32_t {
    NONE = 0,

    // transfer error
    ERROR = QUADSPI_SR_TEF,

    // transfer complete
    TRANSFER_COMPLETE = QUADSPI_SR_TCF,

    // FIFO has reached the threshold
    FIFO_THRESHOLD = QUADSPI_SR_FTF,

    // status match (for automatic polling mode)
    STATUS_MATCH = QUADSPI_SR_SMF,

    // timeout
    TIMEOUT = QUADSPI_SR_TOF,

    // busy
    BUSY = QUADSPI_SR_BUSY
};
COCO_ENUM(Status)


/// @brief DMA request flags (CR register).
///
enum class DmaRequest : uint32_t {
    NONE = 0,

    RX_TX = QUADSPI_CR_DMAEN,

    ALL = RX_TX
};
COCO_ENUM(DmaRequest)


/// @brief QUADSPI instance (wrapper for QUADSPI registers).
///
struct Instance {
    QUADSPI_TypeDef *qspi;


    QUADSPI_TypeDef *operator ->() {return qspi;}
    operator QUADSPI_TypeDef *() const {return qspi;}

    /// @brief Configure the QUADSPI instance.
    /// @param format Format enum
    /// @param fifoThreshold Fifo threshold (0 = 1 byte read or 1 byte free to write sets FIFO_THRESHOLD status)
    /// @param interrupt Interrupts to enable
    /// @param dmaRequest DMA requests to enable
    /// @return *this
    auto &configure(Format format, int fifoThreshold, PollConfig pollConfig, Interrupt interrupt = Interrupt::NONE, DmaRequest dmaRequest = DmaRequest::NONE) {
        qspi->DCR = DCR(format);
        qspi->CR = CR(format)
            | (fifoThreshold << QUADSPI_CR_FTHRES_Pos)
            | uint32_t(pollConfig)
            | uint32_t(interrupt)
            | uint32_t(dmaRequest);
        return *this;
    }

    /// @brief Enable the QUADSPI instance.
    /// @return *this
    auto &enable() {
        qspi->CR = qspi->CR | QUADSPI_CR_EN;
        return *this;
    }

    /// @brief Disable the QUADSPI instance.
    /// @return *this
    auto &disable() {
        qspi->CR = qspi->CR & ~QUADSPI_CR_EN;
        return *this;
    }

    /// @brief Combination of configure() and enable().
    /// @param format Format enum
    /// @param fifoThreshold Fifo threshold (0 = 1 byte read or 1 byte free to write sets FIFO_THRESHOLD status)
    /// @param interrupt Interrupts to enable
    /// @param dmaRequest DMA requests to enable
    /// @return *this
    auto &enable(Format format, int fifoThreshold, Interrupt interrupt = Interrupt::NONE, DmaRequest dmaRequest = DmaRequest::NONE) {
        qspi->DCR = DCR(format);
        qspi->CR = CR(format)
            | (fifoThreshold << QUADSPI_CR_FTHRES_Pos)
            | uint32_t(interrupt)
            | uint32_t(dmaRequest)
            | QUADSPI_CR_EN;
        return *this;
    }

    /// @brief Get status flags.
    /// @return Current status flags
    Status status() const {
        return Status(qspi->SR);
    }

    /// @brief Clear status flags.
    /// Note that not all flags can be cleared
    /// @param status Status flags to clear
    /// @return *this
    auto &clear(Status status) {
        qspi->FCR = uint32_t(status);
        return *this;
    }

    /// @brief Set the format (CR and DCR registers).
    /// @param format Format enum
    /// @return *this
    auto &setFormat(Format format) {
        qspi->DCR = DCR(format);
        qspi->CR = (qspi->CR & ~FORMAT_CR_MASK) | CR(format);
        return *this;
    }

    auto &setPollConfig(PollConfig pollConfig, uint32_t mask, uint32_t match, int interval) {
        qspi->CR = (qspi->CR & ~(QUADSPI_CR_APMS | QUADSPI_CR_PMM)) | uint32_t(pollConfig);
        qspi->PSMKR = mask;
        qspi->PSMAR = match;
        qspi->PIR = interval;
        return *this;
    }

    /// @brief Set communication configuration (CCR register).
    /// @tparam I Type of instruction, e.g. int or enum
    /// @param function Function (read or write)
    /// @param mode Mode (number of lines for instruction, address, data)
    /// @param instruction SPI Instruction (8 bit)
    /// @param addressBytes Number of address bytes (1 to 4)
    /// @param timing Timing, e.g. number of dummy cycles (0 to 31)
    /// @return *this
    template <typename I>
    auto &setCommConfig(Function function, Mode mode, I instruction, int addressBytes, Timing timing = Timing::DEFAULT) {
        qspi->CCR = uint32_t(function)
            | uint32_t(mode)
            | (int(instruction) << QUADSPI_CCR_INSTRUCTION_Pos)
            | (addressBytes - 1) << QUADSPI_CCR_ADSIZE_Pos
            | uint32_t(timing);
        return *this;
    }

    /// @brief Set communication configuration (CCR register).
    /// @param function Function (read or write)
    /// @param commConfig Communication configuration
    auto &setCommConfig(Function function, CommConfig commConfig) {
        qspi->CCR = uint32_t(function)
            | uint32_t(commConfig);
        return *this;
    }

    /// @brief Set memory address.
    /// @param address Address in memory
    /// @return *this
    auto &setAddress(uint32_t address) {
        qspi->AR = address;
        return *this;
    }

    /// @brief Set transfer size in bytes.
    /// @param length Length of data to transfer
    /// @return *this
    auto &setSize(uint32_t size) {
        qspi->DLR = size - 1;
        return *this;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
    /// @brief Get 8 bit reference to RX data register
    /// @return Reference to data register
    volatile uint8_t &DR8() {return reinterpret_cast<volatile uint8_t &>(qspi->DR);}

    /// @brief Get 16 bit reference to RX data register
    /// @return Reference to data register
    volatile uint16_t &DR16() {return reinterpret_cast<volatile uint16_t &>(qspi->DR);}

    /// @brief Get 32 bit reference to RX data register
    /// @return Reference to data register
    volatile uint32_t &DR32() {return qspi->DR;}
#pragma GCC diagnostic pop
};

/// @brief SPI instance info
/// Contains pointer to the SPI instance, interrupt index and methods to map DMA channels
struct Info {
    using Instance = xspi::Instance;

    // registers
    QUADSPI_TypeDef *qspi;

    // reset and clock control
    rcc::QuadSpiInfo rcc;

    // interrupt request index
    uint8_t irq;

    // First DMA request index
    uint8_t drq;


    /// @brief Enable clock and return an instance wrapper.
    /// @return Instance (wrapper for registers)
    Instance enableClock() const {
        rcc.enableClock();
        return {qspi};
    }

    /// @brief Get the instance
    /// @return Instance (wrapper for registers)
    Instance instance() const {return {qspi};}

    /// @brief Map DMA channels to the RX and TX channels of the SPI
    ///
    template <dma::Feature F2>
    void map(const dma::Info<F2> &dmaInfo) const;
};

} // namespace xspi
} // namespace coco
#endif // QUADSPI
