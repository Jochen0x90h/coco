#pragma once

#include "platform.hpp"
#include <coco/enum.hpp>


namespace coco {

/// @brief SPI master helpers.
/// Reference manual:
///   https://docs.nordicsemi.com/bundle/ps_nrf52840/page/spim.html
namespace spim {

constexpr uint32_t FORMAT_FREQUENCY_MASK = 0xfffffff0;
constexpr uint32_t FORMAT_CONFIG_MASK = 0x0000000f;

enum class Format : uint32_t {
    // speed
    FREQUENCY_125K = 0x02000000, // 125 kbps
    FREQUENCY_250K = 0x04000000, // 250 kbps
    FREQUENCY_500K = 0x08000000, // 500 kbps
    FREQUENCY_1M = 0x10000000, // 1 Mbps
    FREQUENCY_2M = 0x20000000, // 2 Mbps
    FREQUENCY_4M = 0x40000000, // 4 Mbps
    FREQUENCY_8M = 0x80000000, // 8 Mbps
    FREQUENCY_16M = 0x0A000000, // 16 Mbps
    FREQUENCY_32M = 0x14000000, // 32 Mbps

    // clock phase
    PHASE_0 = 0, // default
    PHASE_1 = SPIM_CONFIG_CPHA_Trailing << SPIM_CONFIG_CPHA_Pos,

    // clock polarity
    POLARITY_0 = 0, // default
    POLARITY_1 = SPIM_CONFIG_CPOL_ActiveLow << SPIM_CONFIG_CPOL_Pos,

    // convenience for clock pahse/polarity
    PHA0_POL0 = 0, // default
    PHA0_POL1 = POLARITY_1,
    PHA1_POL0 = PHASE_1,
    PHA1_POL1 = PHASE_1 | POLARITY_1,

    // endianness
    MSB_FIRST = 0, // default
    LSB_FIRST = SPIM_CONFIG_ORDER_LsbFirst << SPIM_CONFIG_ORDER_Pos,

    // number of data bits
    DATA_8 = 0, // default

    // default format: 1mbit, phase 0, polarity 0, MSB first, 8 data bits
    DEFAULT = FREQUENCY_1M | PHA0_POL0 | MSB_FIRST | DATA_8,
};
COCO_ENUM(Format);

/// @brief Interrupt enable flags (CR1 register).
///
enum class Interrupt : uint32_t {
    NONE = 0,

    // transfer complete (RXD.MAXCNT and TXD.MAXCNT reached or TASKS_STOP triggered)
    END = SPIM_INTENSET_END_Set << SPIM_INTENSET_END_Pos,

    // reception complete (RXD.MAXCNT reached or TASKS_STOP triggered)
    ENDRX = SPIM_INTENSET_ENDRX_Set << UARTE_INTENSET_ENDRX_Pos,

    // transmission complete (TXD.MAXCNT reached or TASKS_STOP triggered)
    ENDTX = SPIM_INTENSET_ENDTX_Set << UARTE_INTENSET_ENDTX_Pos,
};
COCO_ENUM(Interrupt);

/// @brief USART instance (wrapper for UART registers)
///
struct Instance {
    NRF_SPIM_Type *spi;


    NRF_SPIM_Type *operator ->() const {return spi;}
    operator NRF_SPIM_Type *() const {return spi;}

    /// @brief Configure the SPI master.
    /// @param format Data frame format
    /// @param interrupts Interrupts to enable
    /// @return *this
    auto &configure(Format format, Interrupt interrupts = Interrupt::NONE) {
        spi->CONFIG = uint32_t(format) & FORMAT_CONFIG_MASK;
        spi->FREQUENCY = uint32_t(format) & FORMAT_FREQUENCY_MASK;
        spi->INTENSET = uint32_t(interrupts);
        spi->INTENCLR = ~uint32_t(interrupts);
        return *this;
    }

    /// @brief Enable the UART/UARTE.
    /// @return *this
    auto &enable() {
        spi->ENABLE = N(SPIM_ENABLE_ENABLE, Enabled);
        return *this;
    }

    /// @brief Disable the UART/UARTE.
    /// @return *this
    auto &disable() {
        spi->ENABLE = 0;
        return *this;
    }

    /// @brief Configure and enable the SPI master.
    /// @param format Data frame format
    /// @param interrupts Interrupts to enable
    /// @return *this
    auto &enable(Format format, Interrupt interrupts = Interrupt::NONE) {
        spi->FREQUENCY = uint32_t(format) & FORMAT_FREQUENCY_MASK;
        spi->CONFIG = uint32_t(format) & FORMAT_CONFIG_MASK;
        spi->INTENSET = uint32_t(interrupts);
        spi->ENABLE = N(SPIM_ENABLE_ENABLE, Enabled);
        return *this;
    }

    /// @brief Set the data frame format.
    /// Useful for changing the frame format while the SPI master is enabled
    /// @param format Frame format
    /// @return &this
    auto &setFormat(Format format) {
        spi->FREQUENCY = uint32_t(format) & FORMAT_FREQUENCY_MASK;
        spi->CONFIG = uint32_t(format) & FORMAT_CONFIG_MASK;
        return *this;
    }

    /// @brief Enable interrupts.
    /// @param interrupts Interrupt flags to set
    /// @return *this
    auto &enable(Interrupt interrupts) {
        spi->INTENSET = uint32_t(interrupts);
        return *this;
    }

    /// @brief Disable interrupts.
    /// @param interrupts Interrupt flags to clear
    /// @return *this
    auto &disable(Interrupt interrupts) {
        spi->INTENCLR = uint32_t(interrupts);
        return *this;
    }

    /// @brief Get the currently enabled Interrupts.
    /// @return Enabled interrupts
    Interrupt interrupts() const {
        return Interrupt(spi->INTENSET);
    }

    /// @brief Set receive data.
    /// @param data Receive data
    /// @param size Data size
    /// @return *this
    template <typename T>
    auto &setRxData(T *data, int size) {
        static_assert(std::is_volatile_v<T>, "Rx data must point to a volatile-qualified type");
        spi->RXD.PTR = uint32_t(data);
        spi->RXD.MAXCNT = size;
        return *this;
    }

    int rxSize() const {
        return spi->RXD.AMOUNT;
    }

    /// @brief Set transmit data.
    /// @param data Transmit data
    /// @param size Data size
    /// @return *this
    template <typename T>
    auto &setTxData(T *data, int size) {
        static_assert(std::is_volatile_v<T>, "Tx data must point to a volatile-qualified type");
        static_assert(!std::is_const_v<T>, "Tx data must not point to a const-qualified type");
        spi->TXD.PTR = uint32_t(data);
        spi->TXD.MAXCNT = size;
        return *this;
    }

    int txSize() const {
        return spi->TXD.AMOUNT;
    }

    /// @brief Start transmission.
    /// @return *this
    auto &start() {
        spi->TASKS_START = TRIGGER;
        return *this;
    }

    /// @brief Stop transmission.
    /// @return *this
    auto &stop() {
        spi->TASKS_STOP = TRIGGER;
        return *this;
    }

    /// @brief Suspend transmission.
    /// @return *this
    auto &suspend() {
        spi->TASKS_SUSPEND = TRIGGER;
        return *this;
    }

    /// @brief Resume transmission.
    /// @return *this
    auto &resume() {
        spi->TASKS_RESUME = TRIGGER;
        return *this;
    }
};

/// @brief SPIM info, SPI master with EasyDMA.
/// Contains pointer to registers and interrupt request index.
struct Info {
    NRF_SPIM_Type *spi;

    // interrupt index
    uint8_t irq;


    Instance instance() const {
        return {spi};
    }

    /// @brief Enable pins.
    /// @param sckPin SCK pin
    /// @param mosiPin MOSI pin, may be gpio::Config::NONE
    /// @param misoPin MISO pin, may be gpio::Config::NONE
    void enablePins(gpio::Config sckPin, gpio::Config mosiPin, gpio::Config misoPin) const {
        gpio::enableAlternate(sckPin);
        if (mosiPin != gpio::Config::NONE)
            gpio::enableAlternate(mosiPin);
        if (misoPin != gpio::Config::NONE)
            gpio::enableAlternate(misoPin);
        spi->PSEL.SCK = uint32_t(sckPin);
        spi->PSEL.MOSI = uint32_t(mosiPin);
        spi->PSEL.MISO = uint32_t(misoPin);
    }

};

static const Info SPIM0_INFO{NRF_SPIM0, SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn};
#ifdef NRF_SPIM1
static const Info SPIM1_INFO{NRF_SPIM1, SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQn};
#endif
#ifdef NRF_SPIM2
static const Info SPIM2_INFO{NRF_SPIM2, SPIM2_SPIS2_SPI2_IRQn};
#endif
#ifdef NRF_SPIM3
static const Info SPIM3_INFO{NRF_SPIM3, SPIM3_IRQn};
#endif

} // namespace spim
} // namespace coco
