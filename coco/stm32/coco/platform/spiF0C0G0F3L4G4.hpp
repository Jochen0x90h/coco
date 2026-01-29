#pragma once

// do not include directly, use #include <coco/platform/spi.hpp>

#include "dma.hpp"
#include <coco/enum.hpp>


/*
    Defines:
    HAVE_SPI_DATA_16          Support for 16 data bits (is always supported)
    HAVE_SPI_DATA_4_16        Support for 4 to 16 data bits (is always supported)
    HAVE_SPI_DATA_4_32        Support for 4 to 32 data bits (is not supported)
    HAVE_SPI_TRANSFER_SIZE    Support for transfer size and start flag (is not supported)
*/
#define HAVE_SPI_DATA_16
#define HAVE_SPI_DATA_4_16

namespace coco {

/// @brief SPI helpers.
/// G0X0 https://www.st.com/en/microcontrollers-microprocessors/stm32g0-series/documentation.html Section 27
/// G0X1 https://www.st.com/en/microcontrollers-microprocessors/stm32g0-series/documentation.html Section 35
/// F3348 https://www.st.com/en/microcontrollers-microprocessors/stm32f3-series/documentation.html Section 29
/// G4 https://www.st.com/en/microcontrollers-microprocessors/stm32g4-series/documentation.html Sections 42
namespace spi {

constexpr int CONFIG_CR2_SHIFT = 16;
constexpr int CONFIG_CR1_MASK = SPI_CR1_MSTR | SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE | SPI_CR1_RXONLY;
constexpr int CONFIG_CR2_MASK = SPI_CR2_SSOE;

/// @brief Configuration (CR1 and CR2 registers).
///
enum class Config : uint32_t {
    NONE = 0,

    // default configuration: slave mode, slave select low active, full duplex
    DEFAULT = 0,

    // mode
    SINGLE_MASTER = SPI_CR1_MSTR | (SPI_CR2_SSOE << CONFIG_CR2_SHIFT),
    MULTI_MASTER = SPI_CR1_MSTR,
    SLAVE = 0, // default

    // slave select pin configuration
    SS_LOW_ACTIVE = 0, // default

    // communication mode
    COMM_FULL_DUPLEX = 0, // default
    COMM_SIMPLEX_TRANSMITTER = SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE,
    COMM_SIMPLEX_RECEIVER = SPI_CR1_RXONLY,
    COMM_HALF_DUPLEX = SPI_CR1_BIDIMODE,
};
COCO_ENUM(Config)

/// @brief Get CR1 register contents from Config.
/// @param config Configuration
/// @return Contents for CR1 register
constexpr uint32_t CR1(Config config) {
    return uint32_t(config) & CONFIG_CR1_MASK;
}

/// @brief Get CR2 register contents from Config.
/// @param config Configuration
/// @return Contents for CR2 register
constexpr uint32_t CR2(Config config) {
    return (uint32_t(config) >> CONFIG_CR2_SHIFT) & CONFIG_CR2_MASK;
}

constexpr int FORMAT_CR2_SHIFT = 16;
constexpr int FORMAT_CR1_MASK = SPI_CR1_BR | SPI_CR1_CPHA | SPI_CR1_CPOL | SPI_CR1_LSBFIRST;
constexpr int FORMAT_CR2_MASK = SPI_CR2_FRF | SPI_CR2_DS | SPI_CR2_FRXTH;

/// @brief Data format (CR1 and CR2 registers).
///
enum class Format : uint32_t {
    // clock prescaler
    CLOCK_DIV_2 = 0, // default
    CLOCK_DIV_4 = 1 << SPI_CR1_BR_Pos,
    CLOCK_DIV_8 = 2 << SPI_CR1_BR_Pos,
    CLOCK_DIV_16 = 3 << SPI_CR1_BR_Pos,
    CLOCK_DIV_32 = 4 << SPI_CR1_BR_Pos,
    CLOCK_DIV_64 = 5 << SPI_CR1_BR_Pos,
    CLOCK_DIV_128 = 6 << SPI_CR1_BR_Pos,
    CLOCK_DIV_256 = 7 << SPI_CR1_BR_Pos,

    // clock phase
    PHASE_0 = 0, // default
    PHASE_1 = SPI_CR1_CPHA,

    // clock polarity
    POLARITY_0 = 0, // default
    POLARITY_1 = SPI_CR1_CPOL,

    // convenience for clock pahse/polarity
    PHA0_POL0 = 0, // default
    PHA0_POL1 = POLARITY_1,
    PHA1_POL0 = PHASE_1,
    PHA1_POL1 = PHASE_1 | POLARITY_1,

    // endianness
    MSB_FIRST = 0, // default
    LSB_FIRST = SPI_CR1_LSBFIRST,

    // frame format
    FRAME_MOTOROLA = 0, // default
    FRAME_TI = SPI_CR2_FRF << FORMAT_CR2_SHIFT,

    // number of data bits
    DATA_4 = ((3 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH) << FORMAT_CR2_SHIFT,
    DATA_5 = ((4 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH) << FORMAT_CR2_SHIFT,
    DATA_6 = ((5 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH) << FORMAT_CR2_SHIFT,
    DATA_7 = ((6 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH) << FORMAT_CR2_SHIFT,
    DATA_8 = ((7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH) << FORMAT_CR2_SHIFT,
    DATA_9 = (8 << SPI_CR2_DS_Pos) << FORMAT_CR2_SHIFT,
    DATA_10 = (9 << SPI_CR2_DS_Pos) << FORMAT_CR2_SHIFT,
    DATA_11 = (10 << SPI_CR2_DS_Pos) << FORMAT_CR2_SHIFT,
    DATA_12 = (11 << SPI_CR2_DS_Pos) << FORMAT_CR2_SHIFT,
    DATA_13 = (12 << SPI_CR2_DS_Pos) << FORMAT_CR2_SHIFT,
    DATA_14 = (13 << SPI_CR2_DS_Pos) << FORMAT_CR2_SHIFT,
    DATA_15 = (14 << SPI_CR2_DS_Pos) << FORMAT_CR2_SHIFT,
    DATA_16 = (15 << SPI_CR2_DS_Pos) << FORMAT_CR2_SHIFT,

    // default format: clock/2, phase 0, polarity 0, MSB first, Motorola format, 8 data bits
    DEFAULT = DATA_8,
};
COCO_ENUM(Format)

/// @brief Get CR1 register contents from Format.
/// @param format Data frame Format
/// @return Contents for CR1 register
constexpr uint32_t CR1(Format format) {
    return uint32_t(format) & FORMAT_CR1_MASK;
}

/// @brief Get CR2 register contents from Config.
/// @param format Data frame Format
/// @return Contents for CR2 register
constexpr uint32_t CR2(Format format) {
    return (uint32_t(format) >> FORMAT_CR2_SHIFT) & FORMAT_CR2_MASK;
}

/// @brief Interrupt enable flags (CR2 register).
///
enum class Interrupt {
    NONE = 0,

    // received packet available in the FIFO
    RX = SPI_CR2_RXNEIE,

    // space for a transmit packet is free in the FIFO
    TX = SPI_CR2_TXEIE,

    RX_TX = RX | TX,

    ERROR = SPI_CR2_ERRIE,

    ALL = RX_TX | ERROR
};
COCO_ENUM(Interrupt)

/// @brief Status flags (SR register).
///
enum class Status : uint32_t {
    NONE = 0,

    // received packet available in the FIFO
    RX = SPI_SR_RXNE,

    // space for a transmit packet is free in the FIFO
    TX = SPI_SR_TXE,

    BUSY = SPI_SR_BSY,
    OVERRUN = SPI_SR_OVR,
#ifdef SPI_SR_UDR
    UNDERRUN = SPI_SR_UDR,
#endif
    CRC_ERROR = SPI_SR_CRCERR,

    ALL = RX | TX | BUSY | OVERRUN
#ifdef SPI_SR_UDR
        | UNDERRUN
#endif
        | CRC_ERROR
};
COCO_ENUM(Status)

/// @brief DMA request flags (CR2 register).
///
enum class DmaRequest {
    NONE = 0,

    RX = SPI_CR2_RXDMAEN,
    TX = SPI_CR2_TXDMAEN,
    RX_TX = RX | TX,

    ALL = RX_TX
};
COCO_ENUM(DmaRequest)


/// @brief SPI instance (wrapper for SPI registers).
///
struct Instance {
    SPI_TypeDef *spi;


    SPI_TypeDef *operator ->() const {return spi;}
    operator SPI_TypeDef *() const {return spi;}

    /// @brief Configure the SPI device.
    /// @param config Configuration
    /// @param format Data frame format
    /// @param interrupt Interrupts to enable
    /// @param dmaRequest DMA requests to enable
    /// @return *this
    auto &configure(Config config, Format format,
        Interrupt interrupt = Interrupt::NONE, DmaRequest dmaRequest = DmaRequest::NONE)
    {
        spi->CR2 = CR2(config)
            | CR2(format)
            | uint32_t(interrupt)
            | uint32_t(dmaRequest);
        spi->CR1 = spi::CR1(config)
            | CR1(format);
        return *this;
    }

    /// @brief Enable the SPI device.
    /// @return *this
    auto &enable() {
        spi->CR1 = spi->CR1 | SPI_CR1_SPE;
        return *this;
    }

    /// @brief Disable the SPI device.
    /// @return *this
    auto &disable() {
        spi->CR1 = spi->CR1 & ~SPI_CR1_SPE;
        return *this;
    }

    /// @brief Configure and enable the SPI device.
    /// @param config Configuration
    /// @param format Data frame format
    /// @param interrupt Interrupts to enable
    /// @param dmaRequest DMA requests to enable
    /// @return *this
    auto &enable(Config config, Format format,
        Interrupt interrupt = Interrupt::NONE, DmaRequest dmaRequest = DmaRequest::NONE)
    {
        spi->CR2 = CR2(config)
            | CR2(format)
            | uint32_t(interrupt)
            | uint32_t(dmaRequest);
        spi->CR1 = spi::CR1(config)
            | CR1(format)
            | SPI_CR1_SPE; // enable
        return *this;
    }

    /// @brief Set format.
    /// @param format Data frame format
    /// @return *this
    auto &setFormat(Format format) {
        spi->CR1 = (spi->CR1 & ~FORMAT_CR1_MASK) | CR1(format);
        spi->CR2 = (spi->CR2 & ~FORMAT_CR2_MASK) | CR2(format);
        return *this;
    }

    /// @brief Enable interrupts.
    /// @param interrupts Interrupt flags to set
    /// @return *this
    auto &enable(Interrupt interrupts) {
        spi->CR2 = spi->CR2 | uint32_t(interrupts);
        return *this;
    }

    /// @brief Disable interrupts.
    /// @param interrupts Interrupt flags to clear
    /// @return *this
    auto &disable(Interrupt interrupts) {
        spi->CR2 = spi->CR2 & ~uint32_t(interrupts);
        return *this;
    }

    /// @brief Get the currently enabled Interrupts.
    /// @return Enabled interrupts
    Interrupt interrupts() const {
        return Interrupt(spi->CR2) & Interrupt::ALL;
    }

    /// @brief Get status flags.
    /// @return Current status flags
    Status status() const {
        return Status(spi->SR);
    }

    /// @brief Clear status flags.
    /// Note that not all flags can be cleared
    /// @param status Status flags to clear
    /// @return *this
    auto &clear(Status status) {
        spi->SR = spi->SR & ~uint32_t(status);
        return *this;
    }

    /// @brief Enable DMA requests.
    /// @param dmaRequests DMA request flags to set
    /// @return *this
    auto &enable(DmaRequest dmaRequests) {
        spi->CR2 = spi->CR2 | uint32_t(dmaRequests);
        return *this;
    }

    /// @brief Disable interrupts.
    /// @param dmaRequests DMA request flags to clear
    /// @return *this
    auto &disable(DmaRequest dmaRequests) {
        spi->CR2 = spi->CR2 & ~uint32_t(dmaRequests);
        return *this;
    }

    /// @brief Get the currently enabled DMA requests.
    /// @return enabled DMA requests
    DmaRequest dmaRequests() const {
        return DmaRequest(spi->CR2) & DmaRequest::ALL;
    }

    /// @brief Start the SPI.
    /// Does nothing, only for compatibility with other devices.
    /// @return *this
    auto &start() {
        return *this;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
    /// @brief Get 8 bit reference to RX data register
    /// @return Reference to data register
    volatile uint8_t &RXDR8() {return reinterpret_cast<volatile uint8_t &>(spi->DR);}

    /// @brief Get 16 bit reference to RX data register
    /// @return Reference to data register
    volatile uint16_t &RXDR16() {return reinterpret_cast<volatile uint16_t &>(spi->DR);}

    /// @brief Get 32 bit reference to RX data register
    /// @return Reference to data register
    volatile uint32_t &RXDR32() {return spi->DR;}

    /// @brief Get 8 bit reference to TX data register
    /// @return Reference to data register
    volatile uint8_t &TXDR8() {return reinterpret_cast<volatile uint8_t &>(spi->DR);}

    /// @brief Get 16 bit reference to TX data register
    /// @return Reference to data register
    volatile uint16_t &TXDR16() {return reinterpret_cast<volatile uint16_t &>(spi->DR);}

    /// @brief Get 32 bit reference to TX data register
    /// @return Reference to data register
    volatile uint32_t &TXDR32() {return spi->DR;}
#pragma GCC diagnostic pop
};


/// @brief SPI instance info
/// Contains pointer to the SPI instance, interrupt index and methods to map DMA channels
struct Info {
    // registers
    SPI_TypeDef *spi;

    // reset and clock control
    rcc::SpiInfo rcc;

    // interrupt request index
    uint8_t irq;

    // First DMA request index
    uint8_t drq;


    /// @brief Enable clock and return an instance wrapper.
    /// @return Instance (wrapper for registers)
    Instance enableClock() const {
        rcc.enableClock();
        return {spi};
    }

    /// @brief Get the instance
    /// @return Instance (wrapper for registers)
    Instance instance() const {return {spi};}

    /// @brief Map a DMA channel to the RX channel of the SPI
    ///
    template <dma::Feature F2>
    void mapRx(const dma::Info<F2> &dmaInfo) const;

    /// @brief Map a DMA channel to the TX channel of the SPI
    ///
    template <dma::Feature F2>
    void mapTx(const dma::Info<F2> &dmaInfo) const;

    /// @brief Map DMA channels to the RX and TX channels of the SPI
    ///
    template <dma::Feature F2>
    void map(const dma::DualInfo<F2> &dmaInfo) const;

    /// @brief Enable pins.
    /// @param sckPin SCK pin
    /// @param mosiPin MOSI pin, may be gpio::Config::NONE
    /// @param misoPin MISO pin, may be gpio::Config::NONE
    static void enablePins(gpio::Config sckPin, gpio::Config mosiPin, gpio::Config misoPin) {
        gpio::enableAlternate(sckPin);
        if (mosiPin != gpio::Config::NONE)
            gpio::enableAlternate(mosiPin);
        if (misoPin != gpio::Config::NONE)
            gpio::enableAlternate(misoPin);
    }
};

} // namespace spi
} // namespace coco
