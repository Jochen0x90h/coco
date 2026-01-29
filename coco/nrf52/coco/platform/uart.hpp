#pragma once

// do not include directly, use #include <coco/platform/uart.hpp>

#include "platform.hpp"
#include "gpio.hpp"
#include <coco/enum.hpp>
#include <coco/Frequency.hpp>
#include <coco/Time.hpp>


namespace coco {

/// @brief UART/UARTE helpers.
/// Reference manual:
///   https://docs.nordicsemi.com/bundle/ps_nrf52840/page/uart.html
///   https://docs.nordicsemi.com/bundle/ps_nrf52840/page/uarte.html
namespace uart {

/// @brief Configuration
///
enum class Config : uint32_t {
    NONE = 0,

    // default configuration: no RTS/CTS
    DEFAULT = 0,

    // control signals
    ENABLE_RTS_CTS = UARTE_CONFIG_HWFC_Enabled << UARTE_CONFIG_HWFC_Pos,
};
COCO_ENUM(Config);

/// @brief Frame format
///
enum class Format : uint32_t {
    NONE = 0,

    // default format: 8 data bits, no parity, 1 stop bit
    DEFAULT = 0,

    // number of data bits
    DATA_8 = 0, // default

    // parity
    PARITY_NONE = 0, // default
    PARITY_EVEN = UARTE_CONFIG_PARITY_Included << UARTE_CONFIG_PARITY_Pos,

    // number of stop bits
    STOP_1 = 0, // default
    STOP_2 = UARTE_CONFIG_STOP_Two << UARTE_CONFIG_STOP_Pos,
};
COCO_ENUM(Format);

/// @brief Interrupt enable flags (CR1 register).
///
enum class Interrupt : uint32_t {
    NONE = 0,

    // a data byte is ready to read from RXD register
    RXDRDY = UARTE_INTENSET_RXDRDY_Set << UARTE_INTENSET_RXDRDY_Pos,

    // reception has started after triggering TASKS_STARTRX
    RXSTARTED = UARTE_INTENSET_RXSTARTED_Set << UARTE_INTENSET_RXSTARTED_Pos,

    // receive timeout
    RXTO = UARTE_INTENSET_RXTO_Set << UARTE_INTENSET_RXTO_Pos,

    // reception complete (RXD.MAXCNT reached or TASKS_STOPRX triggered)
    ENDRX = UARTE_INTENSET_ENDRX_Set << UARTE_INTENSET_ENDRX_Pos,

    // a data byte has been sent from TXD register
    TXDRDY = UARTE_INTENSET_TXDRDY_Set << UARTE_INTENSET_TXDRDY_Pos,

    // stransmission has started after triggering TASKS_STARTTX
    TXSTARTED = UARTE_INTENSET_TXSTARTED_Set << UARTE_INTENSET_TXSTARTED_Pos,

    // transmission has stopped after triggering TASKS_STOPTX
    TXSTOPPED = UARTE_INTENSET_TXSTOPPED_Set << UARTE_INTENSET_TXSTOPPED_Pos,

    // transmission complete (TXD.MAXCNT reached or TASKS_STOPTX triggered)
    ENDTX = UARTE_INTENSET_ENDTX_Set << UARTE_INTENSET_ENDTX_Pos,

    // CTS (clear to send) is activated (set low)
    CTS = UARTE_INTENSET_CTS_Set << UARTE_INTENSET_CTS_Pos,

    // CTS (clear to send) is deactivated (set high)
    NCTS = UARTE_INTENSET_NCTS_Set << UARTE_INTENSET_NCTS_Pos,

    // error detected
    ERROR = UARTE_INTENSET_ERROR_Set << UARTE_INTENSET_ERROR_Pos,

    // all interrupt flags
    ALL = RXDRDY | RXSTARTED | RXTO | ENDRX | TXDRDY | TXSTARTED | TXSTOPPED
        | ENDTX | CTS | NCTS | ERROR
};
COCO_ENUM(Interrupt);

/// @brief Feature flags.
///
enum class Feature {
    NONE = 0,

    // supports EasyDMA
    DMA = 1 << 0,
};
COCO_ENUM(Feature);


/// @brief Calculate value of BAUDRATE register, pass-through for unit-less values.
/// @param brr Baud rate register value
/// @return Baud rate register value
constexpr uint32_t calcBaudRateRegister(uint32_t brr) {
    return brr;
}

/// @brief Calculate value of BAUDRATE register from baud rate in Hz.
/// https://devzone.nordicsemi.com/f/nordic-q-a/391/uart-baudrate-register-values
/// https://docs.nordicsemi.com/bundle/ps_nrf52840/page/uart.html#ariaid-title34
/// @param baudRate Baud rate
/// @return Baud rate register value
constexpr uint32_t calcBaudRateRegister(Hertz<> baudRate) {
    // baud rate in Hz (register value = baudrate * 2^32 / 16000000)
    int brr = (int64_t(baudRate) << 32) / 16000000;
    return (brr + 0x800) & 0xFFFFF000;
}

/// @brief Calculate value of BAUDRATE register from bit duration in nanoseconds.
/// @param bitDuration  Bit duration
/// @return Baud rate register value
constexpr uint32_t calcBaudRateRegister(Nanoseconds<> bitDuration) {
    // bit duration in nanoseconds (register value = (1e9/baudrate) * 2^32 / 16000000)
    int brr = INT64_C(268435456000) / int(bitDuration.value);
    return (brr + 0x800) & 0xFFFFF000;
}


template <Feature F>
struct RegistersSelector {
    using Registers = NRF_UART_Type;
    static constexpr uint32_t ENABLE = N(UART_ENABLE_ENABLE, Enabled);
};

template <>
struct RegistersSelector<Feature::DMA> {
    using Registers = NRF_UARTE_Type;
    static constexpr uint32_t ENABLE = N(UARTE_ENABLE_ENABLE, Enabled);
};


/// @brief USART instance (wrapper for UART registers)
/// @tparam F Feature flags
template <Feature F>
struct Instance {
    using Registers = RegistersSelector<F>::Registers;
    Registers *uart;


    Registers *operator ->() const {return uart;}
    operator Registers *() const {return uart;}

    /// @brief Configure the UART/UARTE.
    /// @tparam T Type of baud parameter (e.g. uint32_t, Hertz<> or Nanoseconds<>)
    /// @param config Configuration
    /// @param format Frame format
    /// @param baud int for baud rate register value, frequency for baud rate, duration for bit time
    /// @param interrupts Interrupts to enable
    /// @return *this
    template <typename T>
    auto &configure(Config config, Format format, T baud, Interrupt interrupts = Interrupt::NONE) {
        uart->CONFIG = uint32_t(config) | uint32_t(format);
        uart->BAUDRATE = calcBaudRateRegister(baud);
        uart->INTENSET = uint32_t(interrupts);
        uart->INTENCLR = ~uint32_t(interrupts);
        return *this;
    }

    /// @brief Enable the UART/UARTE.
    /// @return *this
    auto &enable() {
        uart->ENABLE = RegistersSelector<F>::ENABLE;
        return *this;
    }

    /// @brief Disable the UART/UARTE.
    /// @return *this
    auto &disable() {
        uart->ENABLE = 0;
        return *this;
    }

    /// @brief Combination of configure() and enable().
    /// @tparam T Type of baud parameter (e.g. uint32_t, Hertz<> or Nanoseconds<>)
    /// @param config Configuration
    /// @param format Frame format
    /// @param baud int for baud rate register value, frequency for baud rate, duration for bit time
    /// @param interrupts Interrupts to enable
    /// @return *this
    template <typename T>
    auto &enable(Config config, Format format, T baud, Interrupt interrupts = Interrupt::NONE) {
        configure(config, format, baud, interrupts);
        return enable();
    }

    /// @brief Set the data frame format.
    /// Useful for changing the frame format while the UART is enabled
    /// @param format Frame format
    /// @return *this
    auto &setFormat(Format format) {
        // keep hardware flow control (RTS/CTS) setting
        uart->CONFIG = (uart->CONFIG & UARTE_CONFIG_HWFC_Msk) | uint32_t(format);
        return *this;
    }

    /// @brief Set baud rate.
    /// Useful for changing the baud rate while the UART is enabled
    /// @tparam T Type of baud parameter (e.g. uint32_t, Hertz<> or Nanoseconds<>)
    /// @param baud int for baud rate register value, frequency for baud rate, duration for bit time
    /// @return *this
    template <typename T>
    auto &setBaudRate(T baud) {
        uart->BAUDRATE = calcBaudRateRegister(baud);
        return *this;
    }

    /// @brief Enable interrupts.
    /// @param interrupts Interrupt flags to set
    /// @return *this
    auto &enable(Interrupt interrupts) {
        uart->INTENSET = uint32_t(interrupts);
        return *this;
    }

    /// @brief Disable interrupts.
    /// @param interrupts Interrupt flags to clear
    /// @return *this
    auto &disable(Interrupt interrupts) {
        uart->INTENCLR = uint32_t(interrupts);
        return *this;
    }

    /// @brief Get the currently enabled Interrupts.
    /// @return Enabled interrupts
    Interrupt interrupts() const {
        return Interrupt(uart->INTENSET);
    }

    /// @brief Set receive data.
    /// @param data Receive data
    /// @param size Data size
    /// @return *this
    template <typename T>
    auto &setRxData(T *data, int size) {
        static_assert((F & Feature::DMA) != 0, "setRxData() not supported");
        static_assert(std::is_volatile_v<T>, "Rx data must point to a volatile-qualified type");
        uart->RXD.PTR = uint32_t(data);
        uart->RXD.MAXCNT = size;
        return *this;
    }

    /// @brief Get received data size..
    /// @return Data size
    int rxSize() const {
        static_assert((F & Feature::DMA) != 0, "rxSize() not supported");
        return uart->RXD.AMOUNT;
    }

    /// @brief Set transmit data.
    /// @param data Transmit data
    /// @param size Data size
    /// @return *this
    template <typename T>
    auto &setTxData(T *data, int size) {
        static_assert((F & Feature::DMA) != 0, "setTxData() not supported");
        static_assert(std::is_volatile_v<T>, "Tx data must point to a volatile-qualified type");
        static_assert(!std::is_const_v<T>, "Tx data must not point to a const-qualified type");
        uart->TXD.PTR = uint32_t(data);
        uart->TXD.MAXCNT = size;
        return *this;
    }

    /// @brief Get transmitted data size..
    /// @return Data size
    int txSize() const {
        static_assert((F & Feature::DMA) != 0, "txSize() not supported");
        return uart->TXD.AMOUNT;
    }

    /// @brief Start receiver.
    /// @return *this
    auto &startRx() {
        uart->TASKS_STARTRX = TRIGGER;
        return *this;
    }

    /// @brief Stop receiver.
    /// @return *this
    auto &stopRx() {
        uart->TASKS_STOPRX = TRIGGER;
        return *this;
    }

    /// @brief Flush receive buffer.
    /// @return *this
    auto &flushRx() {
        uart->TASKS_FLUSHRX = TRIGGER;
        return *this;
    }

    /// @brief Wait until data has been received.
    /// @return *this
    auto &waitRx() {
        while (uart->EVENTS_RXDRDY == 0);
        return *this;
    }

    /// @brief Get received data.
    /// Also clears EVENTS_RXDRDY (rx ready flag).
    /// @return Received data
    int rx() {
        uart->EVENTS_RXDRDY = 0;
        return uart->RXD;
    }

    /// @brief Start transmitter.
    /// Note that one byte has to be sent after startTx() before calling waitTx() (is different to STM32).
    /// @return *this
    auto &startTx() {
        uart->TASKS_STARTTX = TRIGGER;
        return *this;
    }

    /// @brief Stop transmitter.
    /// @return *this
    auto &stopTx() {
        uart->TASKS_STOPTX = TRIGGER;
        return *this;
    }

    /// @brief Wait until data has been sent.
    /// Note that one byte has to be sent after starting tx before calling waitTx() (is different to STM32).
    /// @return *this
    auto &waitTx() {
        while (uart->EVENTS_TXDRDY == 0);
        return *this;
    }

    /// @brief Send data.
    /// Also clears EVENTS_TXDRDY (tx ready flag).
    /// @param data Data to send
    void tx(int data) {
        uart->EVENTS_TXDRDY = 0;
        uart->TXD = data;
    }
};

/// @brief UART/UARTE info.
/// Contains pointer to registers and interrupt request index.
/// @tparam F Feature flags
template <Feature F>
struct Info {
    using Instance = uart::Instance<F>;

    // registers
    RegistersSelector<F>::Registers *uart;

    // interrupt index
    uint8_t irq;


    /// @brief Get the UART instance.
    /// @return UART instance
    Instance instance() const {
        return {uart};
    }

    /// @brief Enable RX/TX pins.
    /// @param rxPin RX pin, may be gpio::Config::NONE
    /// @param txPin TX pin, may be gpio::Config::NONE
    /// @param config Configuration
    /// @return Configuration modified for using the pins
    Config enableRxTxPins(gpio::Config rxPin, gpio::Config txPin, Config config) const {
        if (rxPin != gpio::Config::NONE)
            gpio::enableAlternate(rxPin);
        if (txPin != gpio::Config::NONE)
            gpio::enableAlternate(txPin);
        uart->PSEL.RXD = uint32_t(rxPin);
        uart->PSEL.TXD = uint32_t(txPin);
        return config;
    }

    /// @brief Enable RTS/CTS pins.
    /// @param rtsPin RTS pin
    /// @param ctsPin CTS pin
    /// @param config Configuration
    /// @return Configuration modified for using the pins
    Config enableRtsCtsPins(gpio::Config rtsPin, gpio::Config ctsPin, Config config) const {
        if (rtsPin != gpio::Config::NONE)
            gpio::enableAlternate(rtsPin);
        if (ctsPin != gpio::Config::NONE)
            gpio::enableAlternate(ctsPin);
        uart->PSEL.RTS = uint32_t(rtsPin);
        uart->PSEL.CTS = uint32_t(ctsPin);
        return config | Config::ENABLE_RTS_CTS;
    }
};


using FullInfo = Info<Feature::DMA>;
using BasicInfo = Info<Feature::NONE>;

static const BasicInfo UART0_INFO{NRF_UART0, UARTE0_UART0_IRQn};
static const FullInfo UARTE0_INFO{NRF_UARTE0, UARTE0_UART0_IRQn};
static const FullInfo UARTE1_INFO{NRF_UARTE1, UARTE1_IRQn};

} // namespace uart
} // namespace coco
