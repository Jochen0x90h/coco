#pragma once

// do not include directly, use #include <coco/platform/spi.hpp>

#include "dma.hpp"
#include <coco/enum.hpp>


/*
    Defines:
    HAVE_SPI_DATA_16          Support for 16 data bits (is always supported)
    HAVE_SPI_DATA_4_16        Support for 4 to 16 data bits (is always supported)
    HAVE_SPI_DATA_4_32        Support for 4 to 32 data bits (is always supported)
    HAVE_SPI_TRANSFER_SIZE    Support for transfer size and start flag (is always supported)
*/
#define HAVE_SPI_DATA_16
#define HAVE_SPI_DATA_4_16
#define HAVE_SPI_DATA_4_32
#define HAVE_SPI_TRANSFER_SIZE

namespace coco {

/// @brief SPI helpers.
/// H503 https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series/documentation.html Section 38
/// H5 https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series/documentation.html Section 52
/// U3 https://www.st.com/en/microcontrollers-microprocessors/stm32u3-series/documentation.html Section 49
namespace spi {

/// @brief Configuration (CFG2 register).
///
enum class Config : uint32_t {
    NONE = 0,

    // default configuration: slave mode, slave select low active, full duplex, no pin swap
    DEFAULT = 0,

    // mode
    SINGLE_MASTER = SPI_CFG2_MASTER | SPI_CFG2_SSOE,
    MULTI_MASTER = SPI_CFG2_MASTER,
    SLAVE = 0, // default

    // slave select pin configuration
    SS_LOW_ACTIVE = 0, // default
    SS_HIGH_ACTIVE = SPI_CFG2_SSIOP,

    // communication mode
    COMM_FULL_DUPLEX = 0, // default
    COMM_SIMPLEX_TRANSMITTER = SPI_CFG2_COMM_0,
    COMM_SIMPLEX_RECEIVER = SPI_CFG2_COMM_1,
    COMM_HALF_DUPLEX = SPI_CFG2_COMM_1 | SPI_CFG2_COMM_0,

    // swap io pins (MOSI and MISO)
    SWAP_IO_PINS = SPI_CFG2_IOSWP,
};
COCO_ENUM(Config)

constexpr uint32_t FORMAT_CFG1_MASK = SPI_CFG1_BPASS | SPI_CFG1_MBR | SPI_CFG1_DSIZE;
constexpr uint32_t FORMAT_CFG2_MASK = SPI_CFG2_CPHA | SPI_CFG2_CPOL | SPI_CFG2_LSBFRST | SPI_CFG2_SP;

/// @brief Data format (CFG1 and CFG2 registers).
/// Bit allocation:
///  31   30   29   28   27   26   25   24   23   22   21   20   19   18   17   16   15   14   13   12   11   10    9    8    7    6    5    4    3    2    1    0
/// CFG1 CFG1 CFG1 CFG1           CFG2 CFG2 CFG2      CFG2 CFG2 CFG2                                              CFG1 CFG1 CFG1 CFG1 CFG1 CFG1 CFG1 CFG1 CFG1 CFG1
enum class Format : uint32_t {
    // clock prescaler
    CLOCK_DIV_1 = SPI_CFG1_BPASS,
    CLOCK_DIV_2 = 0, // default
    CLOCK_DIV_4 = 1 << SPI_CFG1_MBR_Pos,
    CLOCK_DIV_8 = 2 << SPI_CFG1_MBR_Pos,
    CLOCK_DIV_16 = 3 << SPI_CFG1_MBR_Pos,
    CLOCK_DIV_32 = 4 << SPI_CFG1_MBR_Pos,
    CLOCK_DIV_64 = 5 << SPI_CFG1_MBR_Pos,
    CLOCK_DIV_128 = 6 << SPI_CFG1_MBR_Pos,
    CLOCK_DIV_256 = 7 << SPI_CFG1_MBR_Pos,

    // clock phase
    PHASE_0 = 0, // default
    PHASE_1 = SPI_CFG2_CPHA,

    // clock polarity
    POLARITY_0 = 0, // default
    POLARITY_1 = SPI_CFG2_CPOL,

    // convenience for clock pahse/polarity
    PHA0_POL0 = 0, // default
    PHA0_POL1 = POLARITY_1,
    PHA1_POL0 = PHASE_1,
    PHA1_POL1 = PHASE_1 | POLARITY_1,

    // endianness
    MSB_FIRST = 0, // default
    LSB_FIRST = SPI_CFG2_LSBFRST,

    // frame format
    FRAME_MOTOROLA = 0, // default
    FRAME_TI = SPI_CFG2_SP_0,

    // number of data bits
    DATA_4 = 3 << SPI_CFG1_DSIZE_Pos,
    DATA_5 = 4 << SPI_CFG1_DSIZE_Pos,
    DATA_6 = 5 << SPI_CFG1_DSIZE_Pos,
    DATA_7 = 6 << SPI_CFG1_DSIZE_Pos,
    DATA_8 = 7 << SPI_CFG1_DSIZE_Pos,
    DATA_9 = 8 << SPI_CFG1_DSIZE_Pos,
    DATA_10 = 9 << SPI_CFG1_DSIZE_Pos,
    DATA_11 = 10 << SPI_CFG1_DSIZE_Pos,
    DATA_12 = 11 << SPI_CFG1_DSIZE_Pos,
    DATA_13 = 12 << SPI_CFG1_DSIZE_Pos,
    DATA_14 = 13 << SPI_CFG1_DSIZE_Pos,
    DATA_15 = 14 << SPI_CFG1_DSIZE_Pos,
    DATA_16 = 15 << SPI_CFG1_DSIZE_Pos,
    DATA_17 = 16 << SPI_CFG1_DSIZE_Pos,
    DATA_18 = 17 << SPI_CFG1_DSIZE_Pos,
    DATA_19 = 18 << SPI_CFG1_DSIZE_Pos,
    DATA_20 = 19 << SPI_CFG1_DSIZE_Pos,
    DATA_21 = 20 << SPI_CFG1_DSIZE_Pos,
    DATA_22 = 21 << SPI_CFG1_DSIZE_Pos,
    DATA_23 = 22 << SPI_CFG1_DSIZE_Pos,
    DATA_24 = 23 << SPI_CFG1_DSIZE_Pos,
    DATA_25 = 24 << SPI_CFG1_DSIZE_Pos,
    DATA_26 = 25 << SPI_CFG1_DSIZE_Pos,
    DATA_27 = 26 << SPI_CFG1_DSIZE_Pos,
    DATA_28 = 27 << SPI_CFG1_DSIZE_Pos,
    DATA_29 = 28 << SPI_CFG1_DSIZE_Pos,
    DATA_30 = 29 << SPI_CFG1_DSIZE_Pos,
    DATA_31 = 30 << SPI_CFG1_DSIZE_Pos,
    DATA_32 = 31 << SPI_CFG1_DSIZE_Pos,

    // fifo threshold (number of data frames in a single data packet)
    FIFO_1 = 0, // default
    FIFO_2 = SPI_CFG1_FTHLV_0,
    FIFO_3 = SPI_CFG1_FTHLV_1,
    FIFO_4 = SPI_CFG1_FTHLV_1 | SPI_CFG1_FTHLV_0,
    FIFO_5 = SPI_CFG1_FTHLV_2,
    FIFO_6 = SPI_CFG1_FTHLV_2 | SPI_CFG1_FTHLV_0,
    FIFO_7 = SPI_CFG1_FTHLV_2 | SPI_CFG1_FTHLV_1,
    FIFO_8 = SPI_CFG1_FTHLV_2 | SPI_CFG1_FTHLV_1 | SPI_CFG1_FTHLV_0,

    // default format: clock/2, phase 0, polarity 0, MSB first, Motorola format, 8 data bits
    DEFAULT = DATA_8,
};
COCO_ENUM(Format)

/// @brief Get CFG1 register contents from Format.
/// @param config
/// @return Contents for CFG1 register
constexpr uint32_t CFG1(Format format) {
    return uint32_t(format) & FORMAT_CFG1_MASK;
}

/// @brief Get CFG2 register contents from Format.
/// @param config
/// @return Contents for CFG2 register
constexpr uint32_t CFG2(Format format) {
    return uint32_t(format) & FORMAT_CFG2_MASK;
}


// todo: separate enum for SSOM, MIDI and MSSI in CFG2

/// @brief Interrupt enable flags (IER register).
///
enum class Interrupt {
    NONE = 0,

    // received packet available in the FIFO
    RX = SPI_IER_RXPIE,

    // space for a transmit packet is free in the FIFO
    TX = SPI_IER_TXPIE,

    RX_TX = RX | TX,

    // duplex (RX and TX)
    DUPLEX = SPI_IER_DXPIE,

    // all TX data of size TSIZE has been filled into the FIFO (but last packet not transferred yet)
    TX_FILLED = SPI_IER_TXTFIE,

    // end of transfer
    END_OF_TRANSFER = SPI_IER_EOTIE,

    OVERRUN = SPI_IER_OVRIE,
    UNDERRUN = SPI_IER_UDRIE,
    CRC_ERROR = SPI_IER_CRCEIE,

    ALL = RX_TX | DUPLEX | TX_FILLED | END_OF_TRANSFER | OVERRUN | UNDERRUN | CRC_ERROR
};
COCO_ENUM(Interrupt)

/// @brief Status flags (SR register).
///
enum class Status : uint32_t {
    NONE = 0,

    // received packet available in the FIFO
    RX = SPI_SR_RXP,

    // space for a transmit packet is free in the FIFO
    TX = SPI_SR_TXP,

    // duplex (RX and TX)
    DUPLEX = SPI_SR_DXP,

    // all TX data of size TSIZE has been filled into the FIFO (but last packet not transferred yet)
    TX_FILLED = SPI_SR_TXTF,

    // end of transfer
    END_OF_TRANSFER = SPI_SR_EOT,

    OVERRUN = SPI_SR_OVR,
    UNDERRUN = SPI_SR_UDR,
    CRC_ERROR = SPI_SR_CRCE,

    ALL = RX | TX | DUPLEX | TX_FILLED | END_OF_TRANSFER | OVERRUN | UNDERRUN | CRC_ERROR
};
COCO_ENUM(Status)

/// @brief DMA request flags (CFG1 register).
///
enum class DmaRequest {
    NONE = 0,

    RX = SPI_CFG1_RXDMAEN,
    TX = SPI_CFG1_TXDMAEN,
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
        spi->CFG1 = CFG1(format)
            | uint32_t(dmaRequest);
        spi->CFG2 = uint32_t(config)
            | CFG2(format)
            | SPI_CFG2_AFCNTR; // keep control of GPIOs
        spi->IER = uint32_t(interrupt);
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
        spi->CFG1 = CFG1(format)
            | uint32_t(dmaRequest);
        spi->CFG2 = uint32_t(config)
            | CFG2(format)
            | SPI_CFG2_AFCNTR; // keep control of GPIOs
        spi->IER = uint32_t(interrupt);
        spi->CR1 = SPI_CR1_SPE; // enable
        return *this;
    }

    /// @brief Set format.
    /// Disables SPI during the change, need to call start() again if auto start was used.
    /// @param format Data frame format
    /// @return *this
    auto &setFormat(Format format) {
        // disable SPI
        uint32_t cr1 = spi->CR1;
        spi->CR1 = 0;//cr1 & ~SPI_CR1_SPE;

        // set format
        spi->CFG1 = (spi->CFG1 & ~FORMAT_CFG1_MASK) | CFG1(format);
        spi->CFG2 = (spi->CFG2 & ~FORMAT_CFG2_MASK) | CFG2(format);

        // restore CR1
        spi->CR1 = cr1;
        return *this;
    }

    /// @brief Enable interrupts.
    /// @param interrupts Interrupt flags to set
    /// @return *this
    auto &enable(Interrupt interrupts) {
        spi->IER = spi->IER | uint32_t(interrupts);
        return *this;
    }

    /// @brief Disable interrupts.
    /// @param interrupts Interrupt flags to clear
    /// @return *this
    auto &disable(Interrupt interrupts) {
        spi->IER = spi->IER & ~uint32_t(interrupts);
        return *this;
    }

    /// @brief Get the currently enabled Interrupts.
    /// @return Enabled interrupts
    Interrupt interrupts() const {
        return Interrupt(spi->IER);
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
        spi->IFCR = uint32_t(status);
        return *this;
    }

    /// @brief Enable DMA requests.
    /// @param dmaRequests DMA request flags to set
    /// @return *this
    auto &enable(DmaRequest dmaRequests) {
        spi->CFG1 = spi->CFG1 | uint32_t(dmaRequests);
        return *this;
    }

    /// @brief Disable interrupts.
    /// @param dmaRequests DMA request flags to clear
    /// @return *this
    auto &disable(DmaRequest dmaRequests) {
        spi->CFG1 = spi->CFG1 & ~uint32_t(dmaRequests);
        return *this;
    }

    /// @brief Get the currently enabled DMA requests.
    /// @return enabled DMA requests
    DmaRequest dmaRequests() const {
        return DmaRequest(spi->CFG1) & DmaRequest::ALL;
    }

    /// @brief Automatically start on data available (TXDR register or DMA enabled).
    /// The transfer size does not need to be set before.
    /// @return *this
    auto &start() {
        spi->CR1 = spi->CR1 | SPI_CR1_CSTART;
        return *this;
    }

    /// @brief Set transfer size.
    /// @param size Transfer size
    /// @return *this
    auto &setTransferSize(int size) {
        spi->CR2 = size;
        return *this;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
    /// @brief Get 8 bit reference to RX data register
    /// @return Reference to data register
    volatile uint8_t &RXDR8() {return reinterpret_cast<volatile uint8_t &>(spi->RXDR);}

    /// @brief Get 16 bit reference to RX data register
    /// @return Reference to data register
    volatile uint16_t &RXDR16() {return reinterpret_cast<volatile uint16_t &>(spi->RXDR);}

    /// @brief Get 32 bit reference to RX data register
    /// @return Reference to data register
    volatile uint32_t &RXDR32() {return spi->RXDR;}

    /// @brief Get 8 bit reference to TX data register
    /// @return Reference to data register
    volatile uint8_t &TXDR8() {return reinterpret_cast<volatile uint8_t &>(spi->TXDR);}

    /// @brief Get 16 bit reference to TX data register
    /// @return Reference to data register
    volatile uint16_t &TXDR16() {return reinterpret_cast<volatile uint16_t &>(spi->TXDR);}

    /// @brief Get 32 bit reference to TX data register
    /// @return Reference to data register
    volatile uint32_t &TXDR32() {return spi->TXDR;}
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
