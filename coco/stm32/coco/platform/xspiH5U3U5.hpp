#pragma once

// do not include directly, use #include <coco/platform/xspi.hpp>

#include "dma.hpp"
#include <coco/enum.hpp>


/*
    Defines:
    HAVE_XSPI              OCTOSPI supported
    HAVE_XSPI_DUAL_BANK    Dual bank supported (is always supported)
*/

#ifdef OCTOSPI1_NS
#define HAVE_XSPI

#define HAVE_XSPI_DUAL_BANK

namespace coco {

/// @brief OCTOSPI (extended SPI) helpers.
/// H5 https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series/documentation.html Reference Manual Section 23
namespace xspi {

constexpr uint32_t FORMAT_CR_MASK = XSPI_CR_MSEL | XSPI_CR_DMM;
constexpr uint32_t FORMAT_DCR1_MASK = XSPI_DCR1_CKMODE | XSPI_DCR1_MTYP | XSPI_DCR1_DEVSIZE;
constexpr uint32_t FORMAT_DCR3_MASK = XSPI_DCR3_CSBOUND;
constexpr uint32_t FORMAT_DCR3_SHIFT = 11;
static_assert((FORMAT_CR_MASK & FORMAT_DCR1_MASK & (FORMAT_DCR3_MASK << FORMAT_DCR3_SHIFT)) == 0, "Bit overlap in enum Format!");

/// @brief Format (CR, DCR1, DCR2, DCR3).
///
enum class Format : uint64_t {
    NONE = 0,

    // clock prescaler
    CLOCK_DIV_1 = 0, // default
    CLOCK_DIV_2 = UINT64_C(1) << (XSPI_DCR2_PRESCALER_Pos + 32),
    CLOCK_DIV_3 = UINT64_C(2) << (XSPI_DCR2_PRESCALER_Pos + 32),
    CLOCK_DIV_4 = UINT64_C(3) << (XSPI_DCR2_PRESCALER_Pos + 32),
    CLOCK_DIV_6 = UINT64_C(5) << (XSPI_DCR2_PRESCALER_Pos + 32),
    CLOCK_DIV_8 = UINT64_C(7) << (XSPI_DCR2_PRESCALER_Pos + 32),
    CLOCK_DIV_12 = UINT64_C(11) << (XSPI_DCR2_PRESCALER_Pos + 32),
    CLOCK_DIV_16 = UINT64_C(15) << (XSPI_DCR2_PRESCALER_Pos + 32),
    CLOCK_DIV_24 = UINT64_C(23) << (XSPI_DCR2_PRESCALER_Pos + 32),
    CLOCK_DIV_32 = UINT64_C(31) << (XSPI_DCR2_PRESCALER_Pos + 32),
    CLOCK_DIV_48 = UINT64_C(47) << (XSPI_DCR2_PRESCALER_Pos + 32),
    CLOCK_DIV_64 = UINT64_C(63) << (XSPI_DCR2_PRESCALER_Pos + 32),
    CLOCK_DIV_96 = UINT64_C(95) << (XSPI_DCR2_PRESCALER_Pos + 32),
    CLOCK_DIV_128 = UINT64_C(127) << (XSPI_DCR2_PRESCALER_Pos + 32),
    CLOCK_DIV_192 = UINT64_C(191) << (XSPI_DCR2_PRESCALER_Pos + 32),
    CLOCK_DIV_256 = UINT64_C(255) << (XSPI_DCR2_PRESCALER_Pos + 32),

    // clock mode, 0: CLK low while NCS high, 3: CLK high while NCS high
    CLOCK_MODE_0 = 0, // default
    CLOCK_MODE_3 = 1 << XSPI_DCR1_CKMODE_Pos,

    // bank selection
    BANK_1 = 0, // default
    BANK_2 = XSPI_CR_MSEL, // IO[7:4]
    DUAL_BANK = XSPI_CR_DMM,

    // memory type
    TYPE_MICRON = 0, // default
    TYPE_MACRONIX = 1 << XSPI_DCR1_MTYP_Pos,
    TYPE_STANDARD = 2 << XSPI_DCR1_MTYP_Pos,
    TYPE_MACRONIX_RAM = 3 << XSPI_DCR1_MTYP_Pos,
    TYPE_HYPERBUS_MEMORY = 4 << XSPI_DCR1_MTYP_Pos,
    TYPE_HYPERBUS_REGISTER = 5 << XSPI_DCR1_MTYP_Pos,

    // memory size in bytes
    MEMORY_16B = 3 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_32B = 4 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_64B = 5 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_128B = 6 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_256B = 7 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_512B = 8 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_1KB = 9 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_2KB = 10 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_4KB = 11 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_8KB = 12 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_16KB = 13 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_32KB = 14 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_64KB = 15 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_128KB = 16 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_256KB = 17 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_512KB = 18 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_1MB = 19 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_2MB = 20 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_4MB = 21 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_8MB = 22 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_16MB = 23 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_32MB = 24 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_64MB = 25 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_128MB = 26 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_256MB = 27 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_512MB = 28 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_1GB = 29 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_2GB = 30 << XSPI_DCR1_DEVSIZE_Pos,
    MEMORY_4GB = 31 << XSPI_DCR1_DEVSIZE_Pos,

    // wrap size
    WRAP_NONE = 0, // default
    WRAP_16 = UINT64_C(2) << (XSPI_DCR2_WRAPSIZE_Pos + 32),
    WRAP_32 = UINT64_C(3) << (XSPI_DCR2_WRAPSIZE_Pos + 32),
    WRAP_64 = UINT64_C(4) << (XSPI_DCR2_WRAPSIZE_Pos + 32),
    WRAP_128 = UINT64_C(5) << (XSPI_DCR2_WRAPSIZE_Pos + 32),

    CSBOUND_NONE = 0, // default
    CSBOUND_8 = 3 << (XSPI_DCR3_CSBOUND_Pos + FORMAT_DCR3_SHIFT),
    CSBOUND_16 = 4 << (XSPI_DCR3_CSBOUND_Pos + FORMAT_DCR3_SHIFT),
    CSBOUND_32 = 5 << (XSPI_DCR3_CSBOUND_Pos + FORMAT_DCR3_SHIFT),
    CSBOUND_64 = 6 << (XSPI_DCR3_CSBOUND_Pos + FORMAT_DCR3_SHIFT),
    CSBOUND_128 = 7 << (XSPI_DCR3_CSBOUND_Pos + FORMAT_DCR3_SHIFT),
    CSBOUND_256 = 8 << (XSPI_DCR3_CSBOUND_Pos + FORMAT_DCR3_SHIFT),
    // todo
};
COCO_ENUM(Format)

/// @brief Get CR register contents from format.
/// @param format Format enum
/// @return Contents of CR register
constexpr uint32_t CR(Format format) {
    return uint32_t(format) & FORMAT_CR_MASK;
}

/// @brief Get DCR1 register contents from format.
/// @param format Format enum
/// @return Contents of DCR1 register
constexpr uint32_t DCR1(Format format) {
    return uint32_t(format) & FORMAT_DCR1_MASK;
}

/// @brief Get DCR2 register contents from format.
/// @param format Format enum
/// @return Contents of DCR2 register
constexpr uint32_t DCR2(Format format) {
    return uint64_t(format) >> 32;
}

/// @brief Get DCR3 register contents from format.
/// @param format Format enum
/// @return Contents of DCR3 register
constexpr uint32_t DCR3(Format format) {
    return (uint32_t(format) >> FORMAT_DCR3_SHIFT) & FORMAT_DCR3_MASK;
}


/// @brief Poll config (CR).
///
enum class PollConfig : uint32_t {
    AUTO_STOP = XSPI_CR_APMS,

    MATCH_AND = 0, // default
    MATCH_OR = XSPI_CR_PMM,
};


/// @brief Functional mode (CCR)
///
enum class Function : uint32_t {
    INDIRECT_WRITE = 0, // default
    INDIRECT_READ = XSPI_CR_FMODE_0,
    STATUS_POLLING = XSPI_CR_FMODE_1,
    MEMORY_MAPPED = XSPI_CR_FMODE_1 | XSPI_CR_FMODE_0,
};


/// @brief Communication mode (CCR).
/// Mode for instruction, address, dummy, data
enum class Mode : uint32_t {
    // instruction mode
    INSTRUCTION_NONE = 0, // default
    INSTRUCTION_1_LINE = XSPI_CCR_IMODE_0,
    INSTRUCTION_2_LINES = XSPI_CCR_IMODE_1,
    INSTRUCTION_4_LINES = XSPI_CCR_IMODE_1 | XSPI_CCR_IMODE_0,
    INSTRUCTION_8_LINES = XSPI_CCR_IMODE_2,
    INSTRUCTION_MASK = XSPI_CCR_IMODE_Msk,

    // address mode
    ADDRESS_NONE = 0, // default
    ADDRESS_1_LINE = XSPI_CCR_ADMODE_0,
    ADDRESS_2_LINES = XSPI_CCR_ADMODE_1,
    ADDRESS_4_LINES = XSPI_CCR_ADMODE_1 | XSPI_CCR_ADMODE_0,
    ADDRESS_8_LINES = XSPI_CCR_ADMODE_2,
    ADDRESS_MASK = XSPI_CCR_ADMODE_Msk,

    // alternate-byte mode
    ALTERNATE_NONE = 0, // default
    ALTERNATE_1_LINE = XSPI_CCR_ABMODE_0,
    ALTERNATE_2_LINES = XSPI_CCR_ABMODE_1,
    ALTERNATE_4_LINES = XSPI_CCR_ABMODE_1 | XSPI_CCR_ABMODE_0,
    ALTERNATE_8_LINES = XSPI_CCR_ABMODE_2,
    ALTERNATE_MASK = XSPI_CCR_ABMODE_Msk,

    // data mode
    DATA_NONE = 0, // default
    DATA_1_LINE = XSPI_CCR_DMODE_0,
    DATA_2_LINES = XSPI_CCR_DMODE_1,
    DATA_4_LINES = XSPI_CCR_DMODE_1 | XSPI_CCR_DMODE_0,
    DATA_8_LINES = XSPI_CCR_DMODE_2,
    DATA_MASK = XSPI_CCR_DMODE_Msk,

    // double transfer rate (DTR)
    INSTRUCTION_DTR = XSPI_CCR_IDTR,
    ADDRESS_DTR = XSPI_CCR_ADDTR,
    ALTERNATE_DTR = XSPI_CCR_ABDTR,
    DATA_DTR = XSPI_CCR_DDTR,
    DTR = XSPI_CCR_IDTR | XSPI_CCR_ADDTR | XSPI_CCR_ABDTR | XSPI_CCR_DDTR,
};
COCO_ENUM(Mode)

// convenience for Mode: Number of lines for instruction, addres, data
constexpr auto MODE_DISABLED = Mode::INSTRUCTION_NONE | Mode::ADDRESS_NONE | Mode::DATA_NONE;
constexpr auto MODE_1_0_0 = Mode::INSTRUCTION_1_LINE | Mode::ADDRESS_NONE | Mode::DATA_NONE;
constexpr auto MODE_1_0_1 = Mode::INSTRUCTION_1_LINE | Mode::ADDRESS_NONE | Mode::DATA_1_LINE;
constexpr auto MODE_1_1_1 = Mode::INSTRUCTION_1_LINE | Mode::ADDRESS_1_LINE | Mode::DATA_1_LINE;
constexpr auto MODE_1_1_2 = Mode::INSTRUCTION_1_LINE | Mode::ADDRESS_1_LINE | Mode::DATA_2_LINES;
constexpr auto MODE_1_1_4 = Mode::INSTRUCTION_1_LINE | Mode::ADDRESS_1_LINE | Mode::DATA_4_LINES;
constexpr auto MODE_1_1_8 = Mode::INSTRUCTION_1_LINE | Mode::ADDRESS_1_LINE | Mode::DATA_8_LINES;
constexpr auto MODE_1_2_2 = Mode::INSTRUCTION_1_LINE | Mode::ADDRESS_2_LINES | Mode::DATA_2_LINES;
constexpr auto MODE_1_4_4 = Mode::INSTRUCTION_1_LINE | Mode::ADDRESS_4_LINES | Mode::DATA_4_LINES;
constexpr auto MODE_1S_4D_4D = Mode::INSTRUCTION_1_LINE | Mode::ADDRESS_4_LINES | Mode::DATA_4_LINES | Mode::ADDRESS_DTR | Mode::DATA_DTR;
constexpr auto MODE_1_8_8 = Mode::INSTRUCTION_1_LINE | Mode::ADDRESS_4_LINES | Mode::DATA_8_LINES;
constexpr auto MODE_1S_8D_8D = Mode::INSTRUCTION_1_LINE | Mode::ADDRESS_4_LINES | Mode::DATA_8_LINES | Mode::ADDRESS_DTR | Mode::DATA_DTR;
constexpr auto MODE_2_0_0 = Mode::INSTRUCTION_2_LINES | Mode::ADDRESS_NONE | Mode::DATA_NONE;
constexpr auto MODE_2_2_2 = Mode::INSTRUCTION_2_LINES | Mode::ADDRESS_2_LINES | Mode::DATA_2_LINES;
constexpr auto MODE_4_0_0 = Mode::INSTRUCTION_4_LINES | Mode::ADDRESS_NONE | Mode::DATA_NONE;
constexpr auto MODE_4_4_4 = Mode::INSTRUCTION_4_LINES | Mode::ADDRESS_4_LINES | Mode::DATA_4_LINES;
constexpr auto MODE_4S_4D_4D = Mode::INSTRUCTION_4_LINES | Mode::ADDRESS_4_LINES | Mode::DATA_4_LINES | Mode::ADDRESS_DTR | Mode::DATA_DTR;
constexpr auto MODE_8_0_0 = Mode::INSTRUCTION_8_LINES | Mode::ADDRESS_NONE | Mode::DATA_NONE;
constexpr auto MODE_8_8_8 = Mode::INSTRUCTION_8_LINES | Mode::ADDRESS_8_LINES | Mode::DATA_8_LINES;
constexpr auto MODE_8S_8D_8D = Mode::INSTRUCTION_8_LINES | Mode::ADDRESS_8_LINES | Mode::DATA_8_LINES | Mode::ADDRESS_DTR | Mode::DATA_DTR;
constexpr auto MODE_8D_8D_8D = Mode::INSTRUCTION_8_LINES | Mode::ADDRESS_8_LINES | Mode::DATA_8_LINES | Mode::DTR;


/// @brief Timing (TCR).
///
enum class Timing : uint32_t {
    DEFAULT = 0,

    // dummy cycles (0 to 31)
    DUMMY_CYCLES_1 = 1 << XSPI_TCR_DCYC_Pos,
    DUMMY_CYCLES_2 = 2 << XSPI_TCR_DCYC_Pos,
    DUMMY_CYCLES_3 = 3 << XSPI_TCR_DCYC_Pos,
    DUMMY_CYCLES_4 = 4 << XSPI_TCR_DCYC_Pos,
    DUMMY_CYCLES_5 = 5 << XSPI_TCR_DCYC_Pos,
    DUMMY_CYCLES_6 = 6 << XSPI_TCR_DCYC_Pos,
    DUMMY_CYCLES_7 = 7 << XSPI_TCR_DCYC_Pos,
    DUMMY_CYCLES_8 = 8 << XSPI_TCR_DCYC_Pos,
    DUMMY_CYCLES_9 = 9 << XSPI_TCR_DCYC_Pos,
    DUMMY_CYCLES_10 = 10 << XSPI_TCR_DCYC_Pos,
    DUMMY_CYCLES_11 = 11 << XSPI_TCR_DCYC_Pos,
    DUMMY_CYCLES_12 = 12 << XSPI_TCR_DCYC_Pos,
    DUMMY_CYCLES_13 = 13 << XSPI_TCR_DCYC_Pos,
    DUMMY_CYCLES_14 = 14 << XSPI_TCR_DCYC_Pos,
    DUMMY_CYCLES_15 = 15 << XSPI_TCR_DCYC_Pos,
    DUMMY_CYCLES_16 = 16 << XSPI_TCR_DCYC_Pos,
    DUMMY_CYCLES_MASK = XSPI_TCR_DCYC,

    // sample data one CLK cycle after data is driven by external device (instead of 1/2 CLK cycle, no DTR mode)
    SAMPLE_SHIFT = XSPI_TCR_SSHIFT,

    // delay the data output by 1/4 of the output clock cycle in DTR mode
    DELAY_QUARTER = XSPI_TCR_DHQC,
};
COCO_ENUM(Timing)

constexpr Timing makeTiming(Timing timing, int dummyCycles) {
    return (timing & ~Timing::DUMMY_CYCLES_MASK) | Timing((dummyCycles << XSPI_TCR_DCYC_Pos));
}

struct CommConfig {
    uint32_t ccr;
    uint32_t ir;
    uint32_t tcr;
};

/// @brief Make communication configuration.
/// @tparam I Type of instruction, e.g. int or enum
/// @param mode Mode (number of lines for instruction, address, data)
/// @param instruction SPI Instruction (8 bit)
/// @param addressBytes Number of address bytes (1 to 4)
/// @param timing Timing (dummy cycles, sample shift, delay quarter)
/// @return Contents of several registers
template <typename I>
constexpr CommConfig makeCommConfig(Mode mode, I instruction, int addressBytes, Timing timing = Timing::DEFAULT) {
    return {
        uint32_t(uint32_t(mode) | ((addressBytes - 1) << XSPI_CCR_ADSIZE_Pos)),
        uint32_t(instruction),
        uint32_t(timing)};
}


/// @brief Interrupt enable flags (CR register).
///
enum class Interrupt : uint32_t {
    NONE = 0,

    // FIFO has reached the threshold
    FIFO_THRESHOLD = XSPI_CR_FTIE,

    // transfer complete
    TRANSFER_COMPLETE = XSPI_CR_TCIE,

    // transfer error
    ERROR = XSPI_CR_TEIE,

    ALL = FIFO_THRESHOLD | TRANSFER_COMPLETE | ERROR,
};
COCO_ENUM(Interrupt)


/// @brief Status flags (SR register).
///
enum class Status : uint32_t {
    NONE = 0,

    // transfer error
    ERROR = XSPI_SR_TEF,

    // transfer complete
    TRANSFER_COMPLETE = XSPI_SR_TCF,

    // FIFO has reached the threshold
    FIFO_THRESHOLD = XSPI_SR_FTF,

    // status match (for automatic polling mode)
    STATUS_MATCH = XSPI_SR_SMF,

    // timeout
    TIMEOUT = XSPI_SR_TOF,

    // busy
    BUSY = XSPI_SR_BUSY
};
COCO_ENUM(Status)


/// @brief DMA request flags (CR register).
///
enum class DmaRequest : uint32_t {
    NONE = 0,

    RX_TX = XSPI_CR_DMAEN,

    ALL = RX_TX
};
COCO_ENUM(DmaRequest)


/// @brief OCTOSPI instance (wrapper for OCTOSPI registers).
///
struct Instance {
    XSPI_TypeDef *ospi;


    XSPI_TypeDef *operator ->() {return ospi;}
    operator XSPI_TypeDef *() const {return ospi;}

    /// @brief Configure the OCTOSPI instance.
    /// @param format Format enum
    /// @param fifoThreshold Fifo threshold (0 = 1 byte read or 1 byte free to write sets FIFO_THRESHOLD status)
    /// @param interrupt Interrupts to enable
    /// @param dmaRequest DMA requests to enable
    /// @return *this
    auto &configure(Format format, int fifoThreshold, PollConfig pollConfig, Interrupt interrupt = Interrupt::NONE, DmaRequest dmaRequest = DmaRequest::NONE) {
        ospi->DCR1 = DCR1(format);
        ospi->DCR2 = DCR2(format);
        ospi->DCR3 = DCR3(format);
        ospi->CR = CR(format)
            | (fifoThreshold << XSPI_CR_FTHRES_Pos)
            | uint32_t(pollConfig)
            | uint32_t(interrupt)
            | uint32_t(dmaRequest);
        return *this;
    }

    /// @brief Enable the OCTOSPI instance.
    /// @return *this
    auto &enable() {
        ospi->CR = ospi->CR | XSPI_CR_EN;
        return *this;
    }

    /// @brief Disable the OCTOSPI instance.
    /// @return *this
    auto &disable() {
        ospi->CR = ospi->CR & ~XSPI_CR_EN;
        return *this;
    }

    /// @brief Combination of configure() and enable().
    /// @param format Format enum
    /// @param fifoThreshold Fifo threshold (0 = 1 byte read or 1 byte free to write sets FIFO_THRESHOLD status)
    /// @param interrupt Interrupts to enable
    /// @param dmaRequest DMA requests to enable
    /// @return *this
    auto &enable(Format format, int fifoThreshold, Interrupt interrupt = Interrupt::NONE, DmaRequest dmaRequest = DmaRequest::NONE) {
        ospi->DCR1 = DCR1(format);
        ospi->DCR2 = DCR2(format);
        ospi->DCR3 = DCR3(format);
        ospi->CR = CR(format)
            | (fifoThreshold << XSPI_CR_FTHRES_Pos)
            | uint32_t(interrupt)
            | uint32_t(dmaRequest)
            | XSPI_CR_EN;
        return *this;
    }

    /// @brief Get status flags.
    /// @return Current status flags
    Status status() const {
        return Status(ospi->SR);
    }

    /// @brief Clear status flags.
    /// Note that not all flags can be cleared
    /// @param status Status flags to clear
    /// @return *this
    auto &clear(Status status) {
        ospi->FCR = uint32_t(status);
        return *this;
    }

    /// @brief Set the format (CR and DCR registers).
    /// @param format Format enum
    /// @return *this
    auto &setFormat(Format format) {
        ospi->DCR1 = DCR1(format);
        ospi->DCR2 = DCR2(format);
        ospi->DCR3 = DCR3(format);
        ospi->CR = (ospi->CR & ~FORMAT_CR_MASK) | CR(format);
        return *this;
    }

    auto &setPollConfig(PollConfig pollConfig, uint32_t mask, uint32_t match, int interval) {
        ospi->CR = (ospi->CR & ~(XSPI_CR_APMS | XSPI_CR_PMM)) | uint32_t(pollConfig);
        ospi->PSMKR = mask;
        ospi->PSMAR = match;
        ospi->PIR = interval;
        return *this;
    }

    /// @brief Set communication configuration (IR, CCR, CR, TCR registers).
    /// @tparam I Type of instruction, e.g. int or enum
    /// @param function Function (read or write)
    /// @param mode Mode (number of lines for instruction, address, data)
    /// @param instruction SPI Instruction (8 bit)
    /// @param addressBytes Number of address bytes (1 to 4)
    /// @param timing Timing, e.g. number of dummy cycles (0 to 31)
    /// @return *this
    template <typename I>
    auto &setCommConfig(Function function, Mode mode, I instruction, int addressBytes, Timing timing = Timing::DEFAULT) {
        ospi->CR = (ospi->CR & ~XSPI_CR_FMODE) | uint32_t(function);
        ospi->CCR = uint32_t(mode) | ((addressBytes - 1) << XSPI_CCR_ADSIZE_Pos);
        ospi->IR = uint32_t(instruction);
        ospi->TCR = uint32_t(timing);
        return *this;
    }

    /// @brief Set communication configuration (IR, CCR, CR, TCR registers).
    /// @param function Function (read or write)
    /// @param commConfig Communication configuration
    auto &setCommConfig(Function function, CommConfig commConfig) {
        ospi->CR = (ospi->CR & ~XSPI_CR_FMODE) | uint32_t(function);
        ospi->CCR = commConfig.ccr;
        ospi->IR = commConfig.ir;
        ospi->TCR = commConfig.tcr;
        return *this;
    }

    /// @brief Set memory address.
    /// @param address Address in memory
    /// @return *this
    auto &setAddress(uint32_t address) {
        ospi->AR = address;
        return *this;
    }

    /// @brief Set transfer size in bytes.
    /// @param length Length of data to transfer
    /// @return *this
    auto &setSize(uint32_t size) {
        ospi->DLR = size - 1;
        return *this;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
    /// @brief Get 8 bit reference to RX data register
    /// @return Reference to data register
    volatile uint8_t &DR8() {return reinterpret_cast<volatile uint8_t &>(ospi->DR);}

    /// @brief Get 16 bit reference to RX data register
    /// @return Reference to data register
    volatile uint16_t &DR16() {return reinterpret_cast<volatile uint16_t &>(ospi->DR);}

    /// @brief Get 32 bit reference to RX data register
    /// @return Reference to data register
    volatile uint32_t &DR32() {return ospi->DR;}
#pragma GCC diagnostic pop
};

/// @brief SPI instance info
/// Contains pointer to the SPI instance, interrupt index and methods to map DMA channels
struct Info {
    using Instance = xspi::Instance;

    // registers
    XSPI_TypeDef *ospi;

    // reset and clock control
    rcc::OctoSpiInfo rcc;

    // interrupt request index
    uint8_t irq;

    // First DMA request index
    uint8_t drq;


    /// @brief Enable clock and return an instance wrapper.
    /// @return Instance (wrapper for registers)
    Instance enableClock() const {
        rcc.enableClock();
        return {ospi};
    }

    /// @brief Get the instance
    /// @return Instance (wrapper for registers)
    Instance instance() const {return {ospi};}

    /// @brief Map DMA channels to the RX and TX channels of the SPI
    ///
    template <dma::Feature F2>
    void map(const dma::Info<F2> &dmaInfo) const;
};

} // namespace xspi
} // namespace coco
#endif // OCTOSPI1_NS
