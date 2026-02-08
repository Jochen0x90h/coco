#pragma once

// do not include directly, use #include <coco/platform/uart.hpp>

#include "dma.hpp"
#include <coco/enum.hpp>
#include <coco/Frequency.hpp>


/*
    Defines:
    HAVE_USART_OVER_8           8x oversampling is supported (is always supported)
    HAVE_USART_DE               Data enable is supported (is always supported)
    HAVE_USART_FIFO             Fifo is supported
    HAVE_USART_DATA_7           Data width of 7 bit is supported
    HAVE_USART_INVERT_DATA      Invert data bits is supported (is always supported)
    HAVE_USART_INVERT_RX_TX     Invert RX and TX pins is supported (is always supported)
    HAVE_USART_LIN_MODE         LIN mode is supported
    HAVE_USART_SYNC_SLAVE_MODE  Synchronous slave mode is supported
*/

// 8x oversampling is supported
#define HAVE_USART_OVER_8

// DE is supported
#define HAVE_USART_DE

// detect if fifo is supported
#ifdef USART_CR1_FIFOEN
#define HAVE_USART_FIFO
#endif

// detect if 7 bit mode is supported
#ifdef USART_CR1_M1
#define HAVE_USART_DATA_7
#endif

// data invert is supported
#define HAVE_USART_INVERT_DATA

// RX/TX pin invert is supported
#define HAVE_USART_INVERT_RX_TX

// detect if lin mode is supported
#ifdef USART_CR2_LINEN
#define HAVE_USART_LIN_MODE
#endif

// detect if synchronous slave mode is supported
#ifdef USART_CR2_SLVEN
#define HAVE_USART_SYNC_SLAVE_MODE
#endif



namespace coco {

/// @brief USART/UART/LPUART helpers.
/// F0 https://www.st.com/en/microcontrollers-microprocessors/stm32f0-series/documentation.html Section 27 (Code Examples: Section A.19)
/// G0X0 https://www.st.com/en/microcontrollers-microprocessors/stm32g0-series/documentation.html Section 26
/// G0X1 https://www.st.com/en/microcontrollers-microprocessors/stm32g0-series/documentation.html Sections 33 + 34
/// G4 https://www.st.com/en/microcontrollers-microprocessors/stm32g4-series/documentation.html Sections 37 + 38
/// H503 https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series/documentation.html Sections 36 + 37
/// H5 https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series/documentation.html Sections 50 + 51
/// U3 https://www.st.com/en/microcontrollers-microprocessors/stm32u3-series/documentation.html Sections 47 + 48
namespace uart {

// mode flags in the CR2 register
constexpr uint32_t USART_CR2_MODES = USART_CR2_CLKEN
#ifdef HAVE_USART_LIN_MODE
    | USART_CR2_LINEN
#endif
#ifdef HAVE_USART_SYNC_SLAVE_MODE
    | USART_CR2_SLVEN
#endif
    ;


constexpr int CONFIG_CR2_SHIFT = 3;
constexpr int CONFIG_CR3_SHIFT = 7;
#ifdef HAVE_USART_FIFO
constexpr uint32_t CONFIG_CR1_MASK = USART_CR1_FIFOEN | USART_CR1_DEAT | USART_CR1_DEDT | USART_CR1_OVER8 | USART_CR1_UE;
#else
constexpr uint32_t CONFIG_CR1_MASK = USART_CR1_DEAT | USART_CR1_DEDT | USART_CR1_OVER8 | USART_CR1_UE;
#endif
constexpr uint32_t CONFIG_CR2_MASK = USART_CR2_SWAP | USART_CR2_RXINV | USART_CR2_TXINV;
constexpr uint32_t CONFIG_CR3_MASK = USART_CR3_RTSE | USART_CR3_CTSE | USART_CR3_ONEBIT | USART_CR3_DEM | USART_CR3_DEP;

/// @brief Configuration (CR1, CR2 and CR3 registers).
/// Bit allocation:
/// 31  30  29  28  27  26  25  24  23  22  21  20  19  18  17  16  15  14  13  12  11  10   9   8   7   6   5   4   3   2   1   0
///         CR1             CR1 CR1 CR1 CR1 CR1 CR1 CR1 CR1 CR1 CR1 CR1 CR2 CR2 CR2             CR3 CR3         CR3     CR3 CR3 CR1
enum class Config : uint32_t {
    NONE = 0,

    // default configuration: 16x oversampling, no RTS/CTS, three sample bits
    DEFAULT = 0,

#ifdef HAVE_USART_FIFO
    // enable FIFO
    ENABLE_FIFO = USART_CR1_FIFOEN, // 29
#endif

    // oversampling
    OVER_16 = 0, // default
    OVER_8 = USART_CR1_OVER8, // 15

    // swap rx and tx pins
    SWAP_RX_TX = USART_CR2_SWAP >> CONFIG_CR2_SHIFT, // 15

    // invert RX
    INVERT_RX = USART_CR2_RXINV >> CONFIG_CR2_SHIFT, // 16

    // invert TX
    INVERT_TX = USART_CR2_TXINV >> CONFIG_CR2_SHIFT, // 17

    // enable RTS/CTS
    ENABLE_RTS = USART_CR3_RTSE >> CONFIG_CR3_SHIFT, // 8
    ENABLE_CTS = USART_CR3_CTSE >> CONFIG_CR3_SHIFT, // 9

    // number of sample bits
    SAMPLE_3 = 0, // default
    SAMPLE_1 = USART_CR3_ONEBIT >> CONFIG_CR3_SHIFT, // 11

    // enable DE
    ENABLE_DE = USART_CR3_DEM >> CONFIG_CR3_SHIFT, // 14

    // invert DE (active low instead of active high)
    INVERT_DE = USART_CR3_DEP >> CONFIG_CR3_SHIFT, // 15

    // DE assertion time in sampling cycles (bit time /8 or /16)
    DE_ASSERT_0T = 0, // default
    DE_ASSERT_1T = 1 << (USART_CR1_DEAT_Pos), // 21 - 25
    DE_ASSERT_2T = 2 << (USART_CR1_DEAT_Pos),
    DE_ASSERT_5T = 5 << (USART_CR1_DEAT_Pos),
    DE_ASSERT_10T = 10 << (USART_CR1_DEAT_Pos),
    DE_ASSERT_20T = 20 << (USART_CR1_DEAT_Pos),
    DE_ASSERT_31T = 31 << (USART_CR1_DEAT_Pos),

    // DE deassertion time in sampling cycles (bit time /8 or /16)
    DE_DEASSERT_0T = 0, // default
    DE_DEASSERT_1T = 1 << (USART_CR1_DEDT_Pos), // 16 - 20
    DE_DEASSERT_2T = 2 << (USART_CR1_DEDT_Pos),
    DE_DEASSERT_5T = 5 << (USART_CR1_DEDT_Pos),
    DE_DEASSERT_10T = 10 << (USART_CR1_DEDT_Pos),
    DE_DEASSERT_20T = 20 << (USART_CR1_DEDT_Pos),
    DE_DEASSERT_31T = 31 << (USART_CR1_DEDT_Pos),
};
COCO_ENUM(Config);

/// @brief Get CR1 register contents from Config.
/// @param config
/// @return Contents for CR1 register
constexpr uint32_t CR1(Config config) {
    return uint32_t(config) & CONFIG_CR1_MASK;
}

/// @brief Get CR2 register contents from Config.
/// @param config
/// @return Contents for CR2 register
constexpr uint32_t CR2(Config config) {
    return (uint32_t(config) << CONFIG_CR2_SHIFT) & CONFIG_CR2_MASK;
}

/// @brief Get CR3 register contents from Config
/// @param config
/// @return Contents for CR3 register
constexpr uint32_t CR3(Config config) {
    return (uint32_t(config) << CONFIG_CR3_SHIFT) & CONFIG_CR3_MASK;
}


constexpr int FORMAT_CR2_SHIFT = 12;
constexpr int FORMAT_CR1_MASK = USART_CR1_M | USART_CR1_PCE | USART_CR1_PS;
constexpr int FORMAT_CR2_MASK = USART_CR2_STOP | USART_CR2_MSBFIRST | USART_CR2_DATAINV;

/// @brief Frame format (CR1 and CR2 registers).
///
enum class Format : uint32_t {
    NONE = 0,

    // default format: 8 data bits, no parity, 1 stop bit
    DEFAULT = 0,

    // number of data bits
    DATA_8 = 0, // default
#ifdef USART_CR1_M1
    DATA_7 = USART_CR1_M1, // 12, 28
    DATA_9 = USART_CR1_M0,
#else
    DATA_9 = USART_CR1_M,
#endif

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
    MSB_FIRST = USART_CR2_MSBFIRST >> FORMAT_CR2_SHIFT, // 19

    // invert data
    INVERT_DATA = USART_CR2_DATAINV >> FORMAT_CR2_SHIFT, // 18
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

    // idle line detected
    IDLE = USART_CR1_IDLEIE,

    // receive fifo not empty
    RX_NOT_EMPTY = 1 << 5, //USART_CR1_RXNEIE_RXFNEIE,

    // receive timeout
    RX_TIMEOUT = USART_CR1_RTOIE,

    // transmit fifo not full
    TX_NOT_FULL = 1 << 7, //USART_CR1_TXEIE_TXFNFIE,

    // transmit complete, no more data to send
    TX_COMPLETE = USART_CR1_TCIE,

    // parity error
    PARITY_ERROR = USART_CR1_PEIE,

    // overrun error
    OVERRUN_ERROR = USART_ISR_ORE,

    // character match
    CHARACTER_MATCH = USART_CR1_CMIE,

    // all interrupt flags
    ALL = IDLE | RX_NOT_EMPTY | RX_TIMEOUT | TX_NOT_FULL | TX_COMPLETE | PARITY_ERROR | CHARACTER_MATCH
};
COCO_ENUM(Interrupt);

/// @brief Status flags (ISR register).
///
enum class Status : uint32_t {
    NONE = 0,

    // idle line detected
    IDLE = USART_ISR_IDLE,

    // receive fifo not empty
    RX_NOT_EMPTY = 1 << 5, //USART_ISR_RXNE_RXFNE,

    // receive timeout occurred
    RX_TIMEOUT = USART_ISR_RTOF,

    // transmit fifo not full
    TX_NOT_FULL = 1 << 7, //USART_ISR_TXE_TXFNF,

    // transmit complete, no more data to send
    TX_COMPLETE = USART_ISR_TC,

    // parity error
    PARITY_ERROR = USART_ISR_PE,

    // framing error
    FRAMING_ERROR = USART_ISR_FE,

    // noise error
    NOISE_ERROR = USART_ISR_NE,

    // overrun error
    OVERRUN_ERROR = USART_ISR_ORE,

    // error flags
    ERROR = USART_ISR_ORE | USART_ISR_NE | USART_ISR_FE | USART_ISR_PE,

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

    // baud rate calculation of USART/UART
    BAUD_RATE = 1 << 0,

    // baud rate calculationof LPUART
    LP_BAUD_RATE = 1 << 1,

    // receive timeout
    RX_TIMEOUT = 1 << 2,

    // character match
    CHARACTER_MATCH = 1 << 3,

    // modbus needs receive timeout (binary) or character match (ascii)
    MODBUS = RX_TIMEOUT | CHARACTER_MATCH,

    // standard asynchronous mode
    ASYNC_MODE = 1 << 4,

    // LIN mode
    LIN_MODE = 1 << 5,

    // synchronous mode
    SYNC_MASTER_MODE = 1 << 5,
#ifdef HAVE_USART_SYNC_SLAVE_MODE
    SYNC_SLAVE_MODE = 1 << 6,
    SYNC_MODE = SYNC_MASTER_MODE | SYNC_SLAVE_MODE,
#else
    SYNC_MODE = SYNC_MASTER_MODE,
#endif

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

/// @brief Calculate value of BRR register from baud rate in Hz for low power UART.
/// @param clock Clock of the USART/UART, e.g. APB1 or APB2 clock
/// @param baudRate Baud rate
/// @return Baud rate register value
constexpr uint32_t calcBaudRateRegisterLp(Hertz<> clock, Hertz<> baudRate) {
    // baud rate in Hz (register value = 256 * clock / baudRate)
    uint32_t brr = (int64_t(clock) * 256 + (baudRate.value >> 1)) / baudRate.value;
    return brr;
}

/// @brief Calculate value of BRR register from bit duration in nanoseconds for low power UART.
/// @param clock Clock of the USART/UART, e.g. APB1 or APB2 clock
/// @param bitDuration  Bit duration
/// @return Baud rate register value
constexpr uint32_t calcBaudRateRegisterLp(Hertz<> clock, Nanoseconds<> bitDuration) {
    // bit duration in nanoseconds (register value = 256 * clock / (1e9/bitDuration) = 256 * clock * bitDuration / 1e9)
    uint32_t brr = (int64_t(clock) * 256 * bitDuration.value + 500000000) / 1000000000;
    return brr;
}


/// @brief USART instance (wrapper for USART registers)
/// @tparam F Feature flags
template <Feature F>
struct Instance {
    USART_TypeDef *uart;


    // needed because of converting constructor
    Instance() = default;
    Instance(USART_TypeDef *uart) : uart(uart) {}

    /// @brief Construct from another registers wrapper.
    /// Requires that all features are supported.
    /// @tparam F2 Feature set of source register wrapper
    /// @param r Source register wrapper
    template <Feature F2> requires ((F & F2) == F)
    Instance(const Instance<F2> &r) : uart(r.uart) {}

    USART_TypeDef *operator ->() const {return uart;}
    operator USART_TypeDef *() const {return uart;}

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
        uart->BRR = brr;

        auto cr1 = CR1(config) | CR1(format) | uint32_t(interrupts);
        auto cr2 = CR2(config) | CR2(format);
        auto cr3 = CR3(config) | uint32_t(dmaRequests);

        // enable a feature if it is the only one of multiple exclusive features
#ifdef HAVE_USART_LIN_MODE
        if constexpr ((F & Feature::ALL_MODES) == Feature::LIN_MODE)
            cr2 |= USART_CR2_LINEN;
#endif
        if constexpr ((F & Feature::ALL_MODES) == Feature::SYNC_MASTER_MODE)
            cr2 |= USART_CR2_CLKEN;
#ifdef HAVE_USART_SYNC_SLAVE_MODE
        if constexpr ((F & Feature::ALL_MODES) == Feature::SYNC_SLAVE_MODE)
            cr2 |= USART_CR2_SLVEN;
#endif

        // set config registers (set CR1 last because it may contain the UE bit)
        uart->CR3 = cr3;
        uart->CR2 = cr2;
        uart->CR1 = cr1;
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
        static_assert((F & (Feature::BAUD_RATE | Feature::LP_BAUD_RATE)) != 0, "configure() not supported");
        uint32_t brr;
        if constexpr ((F & Feature::BAUD_RATE) != 0) {
            brr = calcBaudRateRegister(clock, baud, config);
        } else {
            // LPUART baud rate
            brr = calcBaudRateRegisterLp(clock, baud);
        }
        return configure(config, format, brr, interrupts, dmaRequests);
    }

    /// @brief Enable the USART/UART.
    /// @return *this
    auto &enable() {
        uart->CR1 = uart->CR1 | USART_CR1_UE;
        return *this;
    }

    /// @brief Disable the USART/UART.
    /// @return *this
    auto &disable() {
        uart->CR1 = uart->CR1 & ~USART_CR1_UE;
        return *this;
    }

    /// @brief Combination of configure() and enable().
    /// @param config Configuration
    /// @param format Frame format
    /// @param brr Baud rate register value
    /// @param interrupts Interrupts to enable
    /// @param dmaRequests DMA requests to enable
    /// @return *this
    auto &enable(Config config, Format format, uint32_t brr,
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
        static_assert((F & (Feature::BAUD_RATE | Feature::LP_BAUD_RATE)) != 0, "enable() not supported");
        uint32_t brr;
        if constexpr ((F & Feature::BAUD_RATE) != 0) {
            brr = calcBaudRateRegister(clock, baud, config);
        } else {
            // LPUART baud rate
            brr = calcBaudRateRegisterLp(clock, baud);
        }
        return configure(config | Config(USART_CR1_UE), format, brr, interrupts, dmaRequests);
    }

    /// @brief Wait until receiver is enabled.
    /// @return
    /*auto &waitRxEnabled() {
        while ((registers->ISR & USART_ISR_REACK) == 0);
        return *this;
    }

    /// @brief Wait until transmitter is enabled.
    /// @return *this
    auto &waitTxEnabled() {
        while ((registers->ISR & USART_ISR_TEACK) == 0);
        return *this;
    }

    /// @brief Wait until receiver and transmitter are enabled.
    /// @return *this
    auto &waitRxTxEnabled() {
        while ((registers->ISR & (USART_ISR_REACK | USART_ISR_TEACK)) != (USART_ISR_REACK | USART_ISR_TEACK));
        return *this;
    }
*/


    /// @brief Set the frame format.
    /// Useful for changing the frame format while the UART is enabled.
    /// @param format
    /// @return *this
    auto &setFormat(Format format) {
        uart->CR2 = (uart->CR2 & ~FORMAT_CR2_MASK) | CR2(format);
        uart->CR1 = (uart->CR1 & ~FORMAT_CR1_MASK) | CR1(format);
        return *this;
    }

    /// @brief Set baud rate register.
    /// Useful for changing the baud rate while the UART is enabled.
    /// @param brr Baud rate register value
    /// @return *this
    auto &setBaudRate(uint32_t brr) {
        // disable USART
        uint32_t cr1 = uart->CR1;
        uart->CR1 = 0;//cr1 & ~USART_CR1_UE;

        // set baud rate
        uart->BRR = brr;

        // restore CR1
        uart->CR1 = cr1;
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
        static_assert((F & (Feature::BAUD_RATE | Feature::LP_BAUD_RATE)) != 0, "setBaudRate() not supported");

        // disable USART
        uint32_t cr1 = uart->CR1;
        uart->CR1 = cr1 & ~USART_CR1_UE;

        // set baud rate
        uint32_t brr;
        if constexpr ((F & Feature::BAUD_RATE) != 0) {
            brr = calcBaudRateRegister(clock, baud, Config(cr1));
        } else {
            // LPUART baud rate
            brr = calcBaudRateRegisterLp(clock, baud);
        }
        uart->BRR = brr;

        // restore CR1
        uart->CR1 = cr1;
        return *this;
    }

    /// @brief Get current baud rate.
    /// Note the currend baud rate may deviate from the set baud rate due to rounding errors
    /// @param clock Clock of the USART/UART, e.g. APB1 or APB2 clock
    /// @return Actual baud rate
    Hertz<> getBaudRate(Hertz<> clock) {
        static_assert((F & (Feature::BAUD_RATE | Feature::LP_BAUD_RATE)) != 0, "setBaudRate() not supported");

        uint32_t brr = uart->BRR;
        if constexpr ((F & Feature::BAUD_RATE) != 0) {
            uint32_t cr1 = uart->CR1;
            if ((cr1 & USART_CR1_OVER8) != 0)
                brr = ((brr & ~15) >> 1) | (brr & 7);
            return Hertz<>(int(clock) / brr);
        } else {
            // LPUART baud rate
            return Hertz<>(int((uint64_t(256) * int(clock)) / brr));
        }
    }

    /// @brief Set receiver timeout
    /// @param rxTimeout receiver timeout in bit times, 0 to disable
    /// @return *this
    auto &setRxTimeout(int rxTimeout) {
        static_assert((F & Feature::RX_TIMEOUT) != 0, "setReceiverTimeout() not supported");

        uart->RTOR = rxTimeout;
        uint32_t cr2 = uart->CR2 & ~(USART_CR2_RTOEN);
        if (rxTimeout > 0)
            cr2 |= USART_CR2_RTOEN;
        uart->CR2 = cr2;
        return *this;
    }

    /// @brief Set standard asynchronous mode.
    /// Works only if more than one mode is enabled in the features
    /// @return *this
    auto &setAsyncMode() {
        static_assert((F & Feature::ASYNC_MODE) != 0, "setAsyncMode() not supported");
        static_assert((F & Feature::ALL_MODES) != Feature::ASYNC_MODE, "setAsyncMode() not supported (asynchronous mode always on)");
        uart->CR2 = uart->CR2 & ~USART_CR2_MODES;
        return *this;
    }

#ifdef HAVE_USART_LIN_MODE
    /// @brief Set LIN mode
    /// Works only if more than one mode is enabled in the features
    /// @return *this
    auto &setLinMode() {
        static_assert((F & Feature::LIN_MODE) != 0, "setLinMode() not supported");
        static_assert((F & Feature::ALL_MODES) != Feature::LIN_MODE, "setLinMode() not supported (LIN mode always on)");
        uart->CR2 = (uart->CR2 & ~USART_CR2_MODES) | USART_CR2_LINEN;
        return *this;
    }
#endif

    /// @brief Set synchronous master mode
    /// Works only if more than one mode is enabled in the features
    /// @return *this
    auto &setSyncMasterMode() {
        static_assert((F & Feature::SYNC_MASTER_MODE) != 0, "setSyncMasterMode() not supported");
        static_assert((F & Feature::ALL_MODES) != Feature::SYNC_MASTER_MODE, "setSyncMasterMode() not supported (synchronous master mode always on)");
        uart->CR2 = (uart->CR2 & ~USART_CR2_MODES) | USART_CR2_CLKEN;
        return *this;
    }

#ifdef HAVE_USART_SYNC_SLAVE_MODE
    /// @brief Set synchronous slave mode
    /// Works only if more than one mode is enabled in the features
    /// @return *this
    auto &setSyncSlaveMode() {
        static_assert((F & Feature::SYNC_SLAVE_MODE) != 0, "setSyncSlaveMode() not supported");
        static_assert((F & Feature::ALL_MODES) != Feature::SYNC_SLAVE_MODE, "setSyncSlaveMode() not supported (synchronous slave mode always on)");
        uart->CR2 = (uart->CR2 & ~USART_CR2_MODES) | USART_CR2_SLVEN;
        return *this;
    }
#endif

    /// @brief Enable interrupts.
    /// @param interrupts Interrupt flags to set
    /// @return *this
    auto &enable(Interrupt interrupts) {
        uart->CR1 = uart->CR1 | uint32_t(interrupts);
        return *this;
    }

    /// @brief Disable interrupts.
    /// @param interrupts Interrupt flags to clear
    /// @return *this
    auto &disable(Interrupt interrupts) {
        uart->CR1 = uart->CR1 & ~uint32_t(interrupts);
        return *this;
    }

    /// @brief Get the currently enabled Interrupts.
    /// @return Enabled interrupts
    Interrupt interrupts() const {
        return Interrupt(uart->CR1) & Interrupt::ALL;
    }

    /// @brief Get status flags.
    /// @return Current status flags
    Status status() const {
        return Status(uart->ISR);
    }

    /// @brief Clear status flags.
    /// @param status Status flags to clear
    /// @return *this
    auto &clear(Status status) {
        uart->ICR = uint32_t(status);
        return *this;
    }

    /// @brief Enable DMA requests.
    /// @param dmaRequests DMA request flags to set
    /// @return *this
    auto &enable(DmaRequest dmaRequests) {
        uart->CR3 = uart->CR3 | uint32_t(dmaRequests);
        return *this;
    }

    /// @brief Disable interrupts.
    /// @param dmaRequests DMA request flags to clear
    /// @return *this
    auto &disable(DmaRequest dmaRequests) {
        uart->CR3 = uart->CR3 & ~uint32_t(dmaRequests);
        return *this;
    }

    /// @brief Get the currently enabled DMA requests.
    /// @return enabled DMA requests
    DmaRequest dmaRequests() const {
        return DmaRequest(uart->CR3) & DmaRequest::ALL;
    }

    /// @brief Enable receiver.
    /// @return *this
    auto &startRx() {
        uart->CR1 = uart->CR1 | USART_CR1_RE;
        while ((uart->ISR & USART_ISR_REACK) == 0);
        return *this;
    }

    /// @brief Disable receiver.
    /// @return *this
    auto &stopRx() {
        uart->CR1 = uart->CR1 & ~USART_CR1_RE;
        return *this;
    }

    /// @brief Wait until data has been received.
    /// @return *this
    auto &waitRx() {
        while ((uart->ISR & uint32_t(Status::RX_NOT_EMPTY)) == 0);
        return *this;
    }

    /// @brief Get received data.
    /// @return Received data
    int rx() {
        return uart->RDR;
    }

    /// @brief Enable transmitter.
    /// @return *this
    auto &startTx() {
        uart->CR1 = uart->CR1 | USART_CR1_TE;
        while ((uart->ISR & USART_ISR_TEACK) == 0);
        return *this;
    }

    /// @brief Disable transmitter.
    /// @return *this
    auto &stopTx() {
        uart->CR1 = uart->CR1 & ~USART_CR1_TE;
        return *this;
    }

    /// @brief Wait until data can be sent.
    /// Note that waitTx() can be called directly after enableTx() (is different to NRF52).
    /// @return *this
    auto &waitTx() {
        while ((uart->ISR & uint32_t(Status::TX_NOT_FULL)) == 0);
        return *this;
    }

    /// @brief Send data.
    /// @param data Data to send
    void tx(int data) {
        uart->TDR = data;
    }

    /// @brief Send break
    /// @return *this
    auto &txBreak() {
        uart->RQR = USART_RQR_SBKRQ;
        return *this;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
    volatile uint32_t &rxRegister() {return reinterpret_cast<volatile uint32_t &>(uart->RDR);}
    volatile uint32_t &txRegister() {return reinterpret_cast<volatile uint32_t &>(uart->TDR);}
#pragma GCC diagnostic pop
};


/// @brief USART info
/// Contains pointer to registers, reset and clock control, interrupt and DMA request index
template <Feature F>
struct Info {
    using Instance = uart::Instance<F>;

    // registers
    USART_TypeDef *uart;

    // reset and clock control
    rcc::UsartInfo rcc;

    // interrupt request index
    uint8_t irq;

    // DMA request index
    uint8_t drq;


    /// @brief Cast to another info structure.
    /// Requires that all features are supported.
    /// @tparam F2 Feature set of info to cast to
    template <Feature F2> requires ((F2 & F) == F2)
    operator Info<F2> () const {
        return {uart, rcc, irq, drq};
    }

    /// @brief Enable clock and return an instance wrapper.
    /// @return Instance (wrapper for registers)
    Instance enableClock() const {
        rcc.enableClock();
        return {uart};
    }

    /// @brief Get the instance
    /// @return Instance (wrapper for registers)
    Instance instance() const {return {uart};}

    /// @brief Map a DMA channel to the RX channel of the USART.
    /// @param dmaInfo DMA info
    template <dma::Feature F2>
    void mapRx(const dma::Info<F2> &dmaInfo) const;

    /// @brief Map a DMA channel to the TX channel of the USART.
    /// @param dmaInfo DMA info
    template <dma::Feature F2>
    void mapTx(const dma::Info<F2> &dmaInfo) const;

    /// @brief Map DMA channels to the RX and TX channels of the USART.
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

            // apply invert flags
            if ((rxPin & gpio::Config::INVERT) != 0)
                config ^= Config::INVERT_RX;
        }
        if (txPin != gpio::Config::NONE) {
            gpio::enableAlternate(txPin);

            // apply invert flags
            if ((txPin & gpio::Config::INVERT) != 0)
                config ^= Config::INVERT_TX;
        }
        return config;
    }

    /// @brief Enable DE pin.
    /// @param dePin DE pin, supports INVERT flag, may be gpio::Config::NONE
    /// @param config Configuration
    /// @return Configuration modified for using the pins
    static Config enableDePin(gpio::Config dePin, Config config) {
        if (dePin != gpio::Config::NONE) {
            gpio::enableAlternate(dePin);
            config |= Config::ENABLE_DE;

            // apply invert flag
            if ((dePin & gpio::Config::INVERT) != 0)
                config ^= Config::INVERT_DE; // invert DE
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
