#pragma once

// do not include directly, use #include <coco/platform/usart.hpp>

#include "dma.hpp"
#include <coco/enum.hpp>
#include <coco/frequency.hpp>


/*
    Defines:
    HAVE_USART_OVER_8           8x oversampling is supported (Config::OVER_8 has no effect if not supported)
    HAVE_USART_DE               Data enable is supported (is not supported)
    HAVE_USART_FIFO             Fifo is supported (is not supported)
    HAVE_USART_DATA_7           Data width of 7 bit is supported (is not supported)
    HAVE_USART_INVERT_DATA      Invert data bits is supported (is not supported)
    HAVE_USART_INVERT_RX_TX     Invert RX and TX pins is supported (is not supported)
    HAVE_USART_SYNC_SLAVE_MODE  Synchronous slave mode is supported (is not supported)
*/

// detect if 8x oversampling is supported
#ifdef USART_CR1_OVER8
#define HAVE_USART_OVER_8
#endif


namespace coco {

/// @brief USART/UART helpers.
/// F401 https://www.st.com/en/microcontrollers-microprocessors/stm32f4-series/documentation.html Section 19
namespace uart {

#ifdef HAVE_USART_OVER_8
constexpr uint32_t CONFIG_CR1_MASK = USART_CR1_OVER8;
#else
constexpr uint32_t CONFIG_CR1_MASK = 0;
#endif
constexpr uint32_t CONFIG_CR3_MASK = USART_CR3_RTSE | USART_CR3_CTSE | USART_CR3_ONEBIT;

/// @brief Configuration (CR3 register).
///
enum class Config : uint32_t {
    NONE = 0,

    // default configuration: 16x oversampling, no RTS/CTS, three sample bits
    DEFAULT = 0,

    // oversampling
    OVER_16 = 0, // default
#ifdef HAVE_USART_OVER_8
    OVER_8 = USART_CR1_OVER8, // 15
#else
    OVER_8 = 0, // OVER_8 has no effect if not supported, e.g. on stm32f401xe
#endif

    // enable RTS/CTS
    ENABLE_RTS = USART_CR3_RTSE, // 8
    ENABLE_CTS = USART_CR3_CTSE, // 9

    // number of sample bits
    SAMPLE_3 = 0, // default
    SAMPLE_1 = USART_CR3_ONEBIT, // 11
};
COCO_ENUM(Config);

/// @brief Get CR1 register contents from Config.
/// @param format
/// @return Contents for CR1 register
constexpr uint32_t CR1(Config config) {
    return uint32_t(config) & CONFIG_CR1_MASK;
}

/// @brief Get CR3 register contents from Config.
/// @param format
/// @return Contents for CR3 register
constexpr uint32_t CR3(Config config) {
    return uint32_t(config) & CONFIG_CR3_MASK;
}


constexpr int FORMAT_CR2_SHIFT = 12;
constexpr int FORMAT_CR1_MASK = USART_CR1_M | USART_CR1_PCE | USART_CR1_PS;
constexpr int FORMAT_CR2_MASK = USART_CR2_STOP;

/// @brief Frame format (CR1 and CR2 registers).
///
enum class Format : uint32_t {
    NONE = 0,

    // default format: 8 data bits, no parity, 1 stop bit
    DEFAULT = 0,

    // number of data bits
    DATA_8 = 0, // default
    DATA_9 = USART_CR1_M, // 12

    // parity
    PARITY_NONE = 0, // default
    PARITY_ODD = USART_CR1_PCE | USART_CR1_PS, // 9 - 10
    PARITY_EVEN = USART_CR1_PCE,

    // number of stop bits
    STOP_1 = 0, // default
    STOP_0_5 = 1 << (USART_CR2_STOP_Pos - FORMAT_CR2_SHIFT), // 12 - 13
    STOP_2 = 2 << (USART_CR2_STOP_Pos - FORMAT_CR2_SHIFT),
    STOP_1_5 = 3 << (USART_CR2_STOP_Pos - FORMAT_CR2_SHIFT),

    // endianness
    LSB_FIRST = 0, // default
};
COCO_ENUM(Format);

/// @brief Get CR1 register contents from Format.
/// @param format
/// @return Contents for CR1 register
constexpr uint32_t CR1(Format format) {
    return uint32_t(format) & FORMAT_CR1_MASK;
}

/// @brief Get CR2 register contents from Format.
/// @param format
/// @return Contents for CR2 register
constexpr uint32_t CR2(Format format) {
    return (uint32_t(format) << FORMAT_CR2_SHIFT) & FORMAT_CR2_MASK;
}

/// @brief Interrupt enable flags (CR1 register).
///
enum class Interrupt : uint32_t {
    NONE = 0,

    // idle
    IDLE = USART_CR1_IDLEIE,

    // receive fifo not empty interrupt
    RX_NOT_EMPTY = USART_CR1_RXNEIE,

    // transmit fifo not full interrupt
    TX_NOT_FULL = USART_CR1_TXEIE,

    // transmit complete interrupt, no more data to send
    TX_COMPLETE = USART_CR1_TCIE,

    // parity error
    PARITY_ERROR = USART_CR1_PEIE,

    // all interrupt flags
    ALL = IDLE | RX_NOT_EMPTY | TX_NOT_FULL | TX_COMPLETE | PARITY_ERROR
};
COCO_ENUM(Interrupt);

/// @brief Status flags (SR register).
///
enum class Status : uint32_t {
    NONE = 0,

    // idle line detected
    IDLE = USART_SR_IDLE,

    // receive fifo not empty
    RX_NOT_EMPTY = USART_SR_RXNE,

    // transmit fifo not full
    TX_NOT_FULL = USART_SR_TXE,

    // transmit complete, no more data to send
    TX_COMPLETE = USART_SR_TC,

    // parity error
    PARITY_ERROR = USART_SR_PE,

    // framing error
    FRAMING_ERROR = USART_SR_FE,

    // noise error
    NOISE_ERROR = USART_SR_NE,

    // overrun error
    OVERRUN_ERROR = USART_SR_ORE,

    // error flags
    ERROR = USART_SR_ORE | USART_SR_NE | USART_SR_FE | USART_SR_PE,

    // all status flags
    ALL = 0xffffffff
};
COCO_ENUM(Status);

/// @brief DMA request flags (CR3 register).
///
enum class DmaRequest : uint32_t {
    NONE = 0,

    // receive DMA request
    RX = USART_CR3_DMAR,

    // transmit DMA request
    TX = USART_CR3_DMAT,

    RX_TX = RX | TX,

    ALL = RX_TX
};
COCO_ENUM(DmaRequest)

/// @brief Feature flags.
///
enum class Feature {
    NONE = 0,

    // baud rate calculation of USART
    BAUD_RATE = 1 << 0,

    // standard asynchronous mode
    ASYNC_MODE = 1 << 4,

    // LIN mode
    LIN_MODE = 1 << 5,

    // synchronous mode
    SYNC_MASTER_MODE = 1 << 5,
    SYNC_MODE = SYNC_MASTER_MODE,

    // support for all modes
    ALL_MODES = ASYNC_MODE | LIN_MODE | SYNC_MODE
};
COCO_ENUM(Feature)


/// @brief Calculate BRR register value for 8x oversampling.
/// @param brr BRR register value
/// @return Modified register value for 8x oversampling.
constexpr uint32_t calcBrrOver8(uint32_t brr) {
    return ((brr & ~7) << 1) | (brr & 7);
}

/// @brief Calculate value of BRR register from baud rate in Hz.
/// @param clock Clock of the USART/UART, e.g. APB1 or APB2 clock
/// @param baudRate Baud rate
/// @param config Configuration, only Config::OVER_8 is checked
/// @return Baud rate register value
constexpr uint32_t calcBaudRateRegister(Hertz<> clock, Hertz<> baudRate, Config config = Config::DEFAULT) {
    // baud rate in Hz (register value = clock / baudRate)
    uint32_t brr = (int(clock) + (baudRate.value >> 1)) / baudRate.value;
    if ((config & Config::OVER_8) != 0)
        brr = calcBrrOver8(brr);
    return brr;
}

/// @brief Calculate value of BRR register from bit duration in nanoseconds.
/// @param clock Clock of the USART/UART, e.g. APB1 or APB2 clock
/// @param bitDuration  Bit duration
/// @param config Configuration, only Config::OVER_8 is checked
/// @return Baud rate register value
constexpr uint32_t calcBaudRateRegister(Hertz<> clock, Nanoseconds<> bitDuration, Config config = Config::DEFAULT) {
    // bit duration in nanoseconds (register value = clock / (1e9/bitDuration) = clock * bitDuration / 1e9)
    uint32_t brr = (int64_t(clock) * bitDuration.value + 500000000) / 1000000000;
    if ((config & Config::OVER_8) != 0)
        brr = calcBrrOver8(brr);
    return brr;
}


/// @brief USART instance (wrapper for USART registers)
/// @tparam F Feature flags
template <Feature F>
struct Instance {
    USART_TypeDef *usart;


    // needed because of converting constructor
    Instance() = default;
    Instance(USART_TypeDef *usart) : usart(usart) {}

    /// @brief Construct from another registers wrapper.
    /// Requires that all features are supported.
    /// @tparam F2 Feature set of source register wrapper
    /// @param r Source register wrapper
    template <Feature F2> requires ((F & F2) == F)
    Instance(const Instance<F2> &r) : usart(r.usart) {}

    USART_TypeDef *operator ->() const {return usart;}
    operator USART_TypeDef *() const {return usart;}

    /// @brief Configure the USART/UART (set CR1, CR2, CR3 and BRR).
    /// Don't forget to call startRx() and startTx() before actually sending/receiving.
    /// @param config Configuration
    /// @param format Frame format
    /// @param brr Baud rate register value
    /// @param interrupts Interrupts to enable
    /// @param dmaRequests DMA requests to enable
    /// @return *this
    auto &configure(Config config, Format format, uint32_t brr,
        Interrupt interrupts = Interrupt::NONE, DmaRequest dmaRequests = DmaRequest::NONE)
    {
        // set baud rate
        usart->BRR = brr;

        auto cr1 = CR1(config) | CR1(format) | uint32_t(interrupts);
        auto cr2 = CR2(format);
        auto cr3 = CR3(config) | uint32_t(dmaRequests);

        // enable a feature if it is the only one of multiple exclusive features
        if constexpr ((F & Feature::ALL_MODES) == Feature::LIN_MODE)
            cr2 |= USART_CR2_LINEN;
        if constexpr ((F & Feature::ALL_MODES) == Feature::SYNC_MASTER_MODE)
            cr2 |= USART_CR2_CLKEN;

        // set config registers (set CR1 last because it may contain the UE bit)
        usart->CR3 = cr3;
        usart->CR2 = cr2;
        usart->CR1 = cr1;
        return *this;
    }

    /// @brief Configure and enable the USART/UART (set CR1, CR2, CR3 and BRR).
    /// @tparam T Type of baud parameter (e.g. Hertz<> or Nanoseconds<>)
    /// @param config Configuration
    /// @param format Frame format
    /// @param clock Clock of the USART/UART, e.g. APB1 or APB2 clock
    /// @param baud Frequency for baud rate, duration for bit time
    /// @param interrupts Interrupts to enable
    /// @param dmaRequests DMA requests to enable
    /// @return *this
    template <typename T>
    auto &configure(Config config, Format format, Hertz<> clock, T baud,
        Interrupt interrupts = Interrupt::NONE, DmaRequest dmaRequests = DmaRequest::NONE)
    {
        static_assert((F & Feature::BAUD_RATE) != 0, "configure() not supported");
        uint32_t brr = calcBaudRateRegister(clock, baud, config);
        return configure(config, format, brr, interrupts, dmaRequests);
    }

    /// @brief Enable the USART/UART.
    /// @return *this
    auto &enable() {
        usart->CR1 = usart->CR1 | USART_CR1_UE;
        return *this;
    }

    /// @brief Disable the USART/UART.
    /// @return *this
    auto &disable() {
        usart->CR1 = usart->CR1 & ~USART_CR1_UE;
        return *this;
    }

    /// @brief Combination of configure() and enable().
    /// @param config Configuration
    /// @param format Frame format
    /// @param brr Baud rate register value
    /// @param interrupts Interrupts to enable
    /// @param dmaRequests DMA requests to enable
    /// @return *this
    auto &enable(Config config, Format format, int brr,
        Interrupt interrupts = Interrupt::NONE, DmaRequest dmaRequests = DmaRequest::NONE)
    {
        return configure(config | Config(USART_CR1_UE), format, brr, interrupts, dmaRequests);
    }

    /// @brief Combination of configure() and enable().
    /// @tparam T Type of baud parameter (e.g. Hertz<> or Nanoseconds<>)
    /// @param config Configuration
    /// @param format Frame format
    /// @param clock Clock of the USART/UART, e.g. APB1 or APB2 clock
    /// @param baud Frequency for baud rate, duration for bit time
    /// @param interrupts Interrupts to enable
    /// @param dmaRequests DMA requests to enable
    /// @return *this
    template <typename T>
    auto &enable(Config config, Format format, Hertz<> clock, T baud,
        Interrupt interrupts = Interrupt::NONE, DmaRequest dmaRequests = DmaRequest::NONE)
    {
        static_assert((F & Feature::BAUD_RATE) != 0, "enable() not supported");
        uint32_t brr = calcBaudRateRegister(clock, baud, config);
        return configure(config | Config(USART_CR1_UE), format, brr, interrupts, dmaRequests);
    }

    /// @brief Set the frame format.
    /// Useful for changing the frame format at a later time
    /// @param format
    /// @return *this
    auto &setFormat(Format format) {
        usart->CR2 = (usart->CR2 & ~FORMAT_CR2_MASK) | CR2(format);
        usart->CR1 = (usart->CR1 & ~FORMAT_CR1_MASK) | CR1(format);
        return *this;
    }

    /// @brief Set baud rate register.
    /// Useful for changing the baud rate while the UART is enabled.
    /// @param brr Baud rate register value
    /// @return *this
    auto &setBaudRate(uint32_t brr) {
        // disable USART
        uint32_t cr1 = usart->CR1;
        usart->CR1 = 0;//cr1 & ~USART_CR1_UE;

        // set baud rate
        usart->BRR = brr;

        // restore CR1
        usart->CR1 = cr1;
        return *this;
    }

    /// @brief Set baud rate (respects OVER_8 config flag).
    /// Useful for changing the baud rate while the UART is enabled.
    /// Note that the OVER8 flag is read from CR1, therefore configure() should be called first.
    /// @tparam T Type of baud parameter (e.g. Hertz<> or Nanoseconds<>)
    /// @param clock Clock of the USART/UART, e.g. APB1 or APB2 clock
    /// @param baud Frequency for baud rate, duration for bit time
    /// @return *this
    template <typename T>
    auto &setBaudRate(Hertz<> clock, T baud) {
        static_assert((F & Feature::BAUD_RATE) != 0, "setBaudRate() not supported");

        // disable USART
        uint32_t cr1 = usart->CR1;
        usart->CR1 = cr1 & ~USART_CR1_UE;

        // set baud rate
        int brr = calcBaudRateRegister(clock, baud, Config(cr1));
        usart->BRR = brr;

        // restore CR1
        usart->CR1 = cr1;
        return *this;
    }

    /// @brief Get current baud rate.
    /// Note the currend baud rate may deviate from the set baud rate due to rounding errors
    /// @param clock Clock of the USART/UART, e.g. APB1 or APB2 clock
    /// @return Actual baud rate
    Hertz<> getBaudRate(Hertz<> clock) {
        static_assert((F & Feature::BAUD_RATE) != 0, "setBaudRate() not supported");

        uint32_t brr = usart->BRR;
#ifdef HAVE_USART_OVER_8
        uint32_t cr1 = usart->CR1;
        if ((cr1 & USART_CR1_OVER8) != 0)
            brr = ((brr & ~15) >> 1) | (brr & 7);
#endif
        return Hertz<>(int(clock) / brr);
    }

    /// @brief Set standard asynchronous mode.
    /// Works only if more than one mode is enabled in the features
    /// @return *this
    auto &setAsyncMode() {
        static_assert((F & Feature::ASYNC_MODE) != 0, "setAsyncMode() not supported");
        static_assert((F & Feature::ALL_MODES) != Feature::ASYNC_MODE, "setAsyncMode() not supported (asynchronous mode always on)");
        usart->CR2 = usart->CR2 & ~(USART_CR2_LINEN | USART_CR2_CLKEN);
        return *this;
    }

    /// @brief Set LIN mode
    /// Works only if more than one mode is enabled in the features
    /// @return *this
    auto &setLinMode() {
        static_assert((F & Feature::LIN_MODE) != 0, "setLinMode() not supported");
        static_assert((F & Feature::ALL_MODES) != Feature::LIN_MODE, "setLinMode() not supported (LIN mode always on)");
        usart->CR2 = (usart->CR2 & ~(USART_CR2_CLKEN)) | USART_CR2_LINEN;
        return *this;
    }

    /// @brief Set synchronous master mode
    /// Works only if more than one mode is enabled in the features
    /// @return *this
    auto &setSyncMasterMode() {
        static_assert((F & Feature::SYNC_MASTER_MODE) != 0, "setSyncMasterMode() not supported");
        static_assert((F & Feature::ALL_MODES) != Feature::SYNC_MASTER_MODE, "setSyncMasterMode() not supported (synchronous master mode always on)");
        usart->CR2 = (usart->CR2 & ~(USART_CR2_LINEN)) | USART_CR2_CLKEN;
        return *this;
    }

    /// @brief Enable interrupts.
    /// @param interrupts Interrupt flags to set
    /// @return *this
    auto &enable(Interrupt interrupts) {
        usart->CR1 = usart->CR1 | uint32_t(interrupts);
        return *this;
    }

    /// @brief Disable interrupts.
    /// @param interrupts Interrupt flags to clear
    /// @return *this
    auto &disable(Interrupt interrupts) {
        usart->CR1 = usart->CR1 & ~uint32_t(interrupts);
        return *this;
    }

    /// @brief Get the currently enabled Interrupts.
    /// @return Enabled interrupts
    Interrupt interrupts() const {
        return Interrupt(usart->CR1) & Interrupt::ALL;
    }

    /// @brief Get status flags.
    /// @return Current status flags
    Status status() const {
        return Status(usart->SR);
    }

    /// @brief Clear status flags.
    /// @param status Status flags to clear
    /// @return *this
    auto &clear(Status status) {
        usart->SR = ~uint32_t(status);
        return *this;
    }

    /// @brief Enable DMA requests.
    /// @param dmaRequests DMA request flags to set
    /// @return *this
    auto &enable(DmaRequest dmaRequests) {
        usart->CR3 = usart->CR3 | uint32_t(dmaRequests);
        return *this;
    }

    /// @brief Disable interrupts.
    /// @param dmaRequests DMA request flags to clear
    /// @return *this
    auto &disable(DmaRequest dmaRequests) {
        usart->CR3 = usart->CR3 & ~uint32_t(dmaRequests);
        return *this;
    }

    /// @brief Get the currently enabled DMA requests.
    /// @return enabled DMA requests
    DmaRequest getDmaRequests() {
        return DmaRequest(usart->CR3) & DmaRequest::ALL;
    }

    /// @brief Enable receiver.
    /// @return *this
    auto &startRx() {
        usart->CR1 = usart->CR1 | USART_CR1_RE;
        return *this;
    }

    /// @brief Disable receiver.
    /// @return *this
    auto &stopRx() {
        usart->CR1 = usart->CR1 & ~USART_CR1_RE;
        return *this;
    }

    /// @brief Wait until data has been received.
    /// @return *this
    auto &waitRx() {
        while ((usart->SR & uint32_t(Status::RX_NOT_EMPTY)) == 0);
        return *this;
    }

    /// @brief Get received data.
    /// @return Received data
    int rx() {
        return usart->DR;
    }

    /// @brief Enable transmitter.
    /// @return *this
    auto &startTx() {
        usart->CR1 = usart->CR1 | USART_CR1_TE;
        return *this;
    }

    /// @brief Disable transmitter.
    /// @return *this
    auto &stopTx() {
        usart->CR1 = usart->CR1 & ~USART_CR1_TE;
        return *this;
    }

    /// @brief Wait until data can be sent.
    /// Note that waitTx() can be called directly after enableTx() (is different to NRF52).
    /// @return *this
    auto &waitTx() {
        while ((usart->SR & uint32_t(Status::TX_NOT_FULL)) == 0);
        return *this;
    }

    /// @brief Send data.
    /// @param data Data to send
    void tx(int data) {
        usart->DR = data;
    }

    /// @brief Send break.
    /// @return *this
    auto &txBreak() {
        usart->CR1 = usart->CR1 | USART_CR1_SBK;
        return *this;
    }

    volatile uint32_t &rxRegister() {return usart->DR;}
    volatile uint32_t &txRegister() {return usart->DR;}
};


/// @brief USART info
/// Contains pointer to registers, reset and clock control, interrupt and DMA request index
template <Feature F>
struct Info {
    using Instance = uart::Instance<F>;

    // registers
    USART_TypeDef *usart;

    // reset and clock control
    rcc::Info rcc;

    // interrupt request index
    uint8_t irq;

    // DMA request index
    uint8_t drq;


    /// @brief Cast to another info structure.
    /// Requires that all features are supported.
    /// @tparam F2 Feature set of info to cast to
    template <Feature F2> requires ((F2 & F) == F2)
    operator Info<F2> () const {
        return {usart, this->rcc, this->irq};
    }

    /// @brief Enable clock and return an instance wrapper.
    /// @return Instance (wrapper for registers)
    Instance enableClock() const {
        this->rcc.enableClock();
        return {usart};
    }

    /// @brief Get the instance
    /// @return Instance (wrapper for registers)
    Instance instance() const {return {usart};}

    /// @brief Map a DMA channel to the RX channel of the USART.
    /// Check reference manual which mappings are possible
    /// @param dmaInfo DMA info
    template <dma::Feature F2>
    void mapRx(const dma::Info<F2> &dmaInfo) const;

    /// @brief Map a DMA channel to the TX channel of the USART.
    /// Check reference manual which mappings are possible
    /// @param dmaInfo DMA info
    template <dma::Feature F2>
    void mapTx(const dma::Info<F2> &dmaInfo) const;

    /// @brief Map DMA channels to the RX and TX channels of the USART.
    /// Check reference manual which mappings are possible
    /// @param dmaInfo Dual channel DMA info
    template <dma::Feature F2>
    void map(const dma::DualInfo<F2> &dmaInfo) const;

    /// @brief Enable RX/TX pins.
    /// @param rxPin RX pin, supports INVERT flag, may be gpio::Config::NONE
    /// @param txPin TX pin, supports INVERT flag, may be gpio::Config::NONE
    /// @param config Configuration
    /// @return Configuration modified for using the pins
    static Config enableRxTxPins(gpio::Config rxPin, gpio::Config txPin, Config config) {
        if (rxPin != gpio::Config::NONE) {
            gpio::enableAlternate(rxPin);
        }
        if (txPin != gpio::Config::NONE) {
            gpio::enableAlternate(txPin);
        }
        return config;
    }

    /// @brief Enable RTS/CTS pins.
    /// @param rtsPin RTS pin, may be gpio::Config::NONE
    /// @param ctsPin CTS pin, may be gpio::Config::NONE
    /// @param config Configuration
    /// @return Configuration modified for using the pins
    static Config enableRtsCtsPins(gpio::Config rtsPin, gpio::Config ctsPin, Config config) {
        if (rtsPin != gpio::Config::NONE) {
            gpio::enableAlternate(rtsPin);
            config |= Config::ENABLE_RTS;
        }
        if (ctsPin != gpio::Config::NONE) {
            gpio::enableAlternate(ctsPin);
            config |= Config::ENABLE_CTS;
        }
        return config;
    }
};

} // namespace uart
} // namespace coco
