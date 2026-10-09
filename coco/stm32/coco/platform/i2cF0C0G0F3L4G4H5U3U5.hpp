#pragma once

// do not include directly, use #include <coco/platform/i2c.hpp>

#include "dma.hpp"
#include <coco/enum.hpp>


namespace coco {

/// @brief I2C Helpers.
/// G4 https://www.st.com/en/microcontrollers-microprocessors/stm32g4-series/documentation.html Section 39
/// H503 https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series/documentation.html Section 34
/// H5 https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series/documentation.html Section 48
/// U3 https://www.st.com/en/microcontrollers-microprocessors/stm32u3-series/documentation.html Section 45
namespace i2c {

/// @brief Configuration (CR1 register).
enum class Config : uint32_t {
    NONE = 0,

    // default configuration: only controller (master) mode
    DEFAULT = 0,

    // enable hardware byte control in target (slave) mode
    TARGET_BYTE_CONTROL = I2C_CR1_SBC,

    // disable clock stretching, only target (slave) mode
    NO_STRETCH = I2C_CR1_NOSTRETCH,
};
COCO_ENUM(Config);


/// @brief Interrupt enable flags (CR1 register).
///
enum class Interrupt : uint32_t {
    NONE = 0,

    // transmit interrupt
    TX = I2C_CR1_TXIE,

    // receive interrupt
    RX = I2C_CR1_RXIE,

    // address matched (target mode)
    ADDRESS_MATCH = I2C_CR1_ADDRIE,

    // NACK received after a byte transmission
    NOT_ACKNOWLEDGE = I2C_CR1_NACKIE,

    // stop condition detected
    STOP_DETECTED = I2C_CR1_STOPIE,

    // transfer complete and reload
    TRANSFER_COMPLETE = I2C_CR1_TCIE,

    // interrupt on error (bus error, arbitration lost, overrun/underrun, PEC error, timeout)
    ERROR = I2C_CR1_ERRIE,

    ALL = TX | RX | ADDRESS_MATCH | NOT_ACKNOWLEDGE | STOP_DETECTED | TRANSFER_COMPLETE | ERROR
};
COCO_ENUM(Interrupt)


/// @brief Status flags (ISR register).
///
enum class Status : uint32_t {
    NONE = 0,

    // transmit data register empty (cleared by writing to TXDR)
    TX_EMPTY = I2C_ISR_TXE,

    // transmit data register empty and needs to be written (cleared by writing to TXDR)
    TX = I2C_ISR_TXIS,

    // receive data register not empty and needs to be read (cleared by reading RXDR)
    RX = I2C_ISR_RXNE,

    // address matched (target mode)
    ADDRESS_MATCH = I2C_ISR_ADDR,

    // NACK received after a byte transmission
    NOT_ACKNOWLEDGE = I2C_ISR_NACKF,

    // stop condition detected
    STOP_DETECTED = I2C_ISR_STOPF,

    // transfer complete (controller mode)
    TRANSFER_COMPLETE = I2C_ISR_TC,

    // reload after NBYTES were transferred (transfer complete reload)
    RELOAD = I2C_ISR_TCR,

    // bus error (misplaced start/stop, unexpected SCL low, etc.)
    BUS_ERROR = I2C_ISR_BERR,

    // arbitration lost error
    ARBITRATION_LOST = I2C_ISR_ARLO,

    // overrun/underrun error
    OVERRUN = I2C_ISR_OVR,
    UNDERRUN = I2C_ISR_OVR,

    // timeout error
    TIMEOUT = I2C_ISR_TIMEOUT,

    BUSY = I2C_ISR_BUSY,

    DIRECTION_READ = I2C_ISR_DIR,
    DIRECTION_WRITE = 0,
    DIRECTION_MASK = I2C_ISR_DIR,

    ADDRESS_MASK = I2C_ISR_ADDCODE_Msk,
};
COCO_ENUM(Status)

inline int getAddress(const Status &status) {
    return int(status & Status::ADDRESS_MASK) >> I2C_ISR_ADDCODE_Pos;
}


/// @brief DMA request flags (CR1 register).
///
enum class DmaRequest : uint32_t {
    NONE = 0,

    // receive DMA request
    RX = I2C_CR1_RXDMAEN,

    // transmit DMA request
    TX = I2C_CR1_TXDMAEN,

    RX_TX = RX | TX,

    ALL = RX_TX
};
COCO_ENUM(DmaRequest)


/// @brief Wrapper for I2C registers
///
struct Instance {
    I2C_TypeDef *i2c;


    I2C_TypeDef *operator ->() const {return i2c;}
    operator I2C_TypeDef *() const {return i2c;}

    /// @brief Configure the I2C (set CR1, CR2 and TIMINGR).
    /// @param config Configuration
    /// @param timing TIMINGR register value
    /// @param interrupts Interrupts to enable
    /// @param dmaRequests DMA requests to enable
    /// @return *this
    auto &configure(Config config, uint32_t timing,
        Interrupt interrupts = Interrupt::NONE, DmaRequest dmaRequests = DmaRequest::NONE)
    {
        i2c->TIMINGR = timing;
        auto cr1 = uint32_t(config) | uint32_t(interrupts) | uint32_t(dmaRequests);
        i2c->CR1 = cr1;
        return *this;
    }

    /// @brief Enable the I2C.
    /// @return *this
    auto &enable() {
        i2c->CR1 = i2c->CR1 | I2C_CR1_PE;
        return *this;
    }

    /// @brief Disable the I2C.
    /// @return *this
    auto &disable() {
        i2c->CR1 = i2c->CR1 & ~I2C_CR1_PE;
        return *this;
    }

    /// @brief Combination of configure() and enable().
    /// @param config Configuration
    /// @param timing TIMINGR register value
    /// @param interrupts Interrupts to enable
    /// @param dmaRequests DMA requests to enable
    /// @return *this
    auto &enable(Config config, uint32_t timing,
        Interrupt interrupts = Interrupt::NONE, DmaRequest dmaRequests = DmaRequest::NONE)
    {
        return configure(config | Config(I2C_CR1_PE), timing, interrupts, dmaRequests);
    }

    /// @brief Enable interrupts.
    /// @param interrupts Interrupt flags to set
    /// @return *this
    auto &enable(Interrupt interrupts) {
        i2c->CR1 = i2c->CR1 | uint32_t(interrupts);
        return *this;
    }

    /// @brief Disable interrupts.
    /// @param interrupts Interrupt flags to clear
    /// @return *this
    auto &disable(Interrupt interrupts) {
        i2c->CR1 = i2c->CR1 & ~uint32_t(interrupts);
        return *this;
    }

    /// @brief Get the currently enabled Interrupts.
    /// @return Enabled interrupts
    Interrupt interrupts() const {
        return Interrupt(i2c->CR1) & Interrupt::ALL;
    }

    /// @brief Get status flags.
    /// @return Current status flags
    Status status() const {
        return Status(i2c->ISR);
    }

    /// @brief Clear status flags.
    /// Note that not all flags can be cleared
    /// @param status Status flags to clear
    /// @return *this
    auto &clear(Status status) {
        i2c->ICR = uint32_t(status);
        return *this;
    }

    /// @brief Wait until reload flag (TCR) disappears to prevent interrupt tail-chaining delay
    ///
    void waitReload() {
        while (i2c->ISR & I2C_ISR_TCR);
    }

    /// @brief Wait until tranfer complete flag (TC) disappears to prevent interrupt tail-chaining delay
    ///
    void waitTransferComplete() {
        while (i2c->ISR & I2C_ISR_TC);
    }
};


/// @brief I2C info
///
struct Info {
    using Instance = i2c::Instance;

    // registers
    I2C_TypeDef *i2c;

    // reset and clock control
    rcc::I2cInfo rcc;

    // interrupt request index
    uint8_t irq;

    // First DMA request index
    uint8_t drq;


    /// @brief Enable clock and return an instance wrapper.
    /// @return Instance (wrapper for registers)
    Instance enableClock() const {
        rcc.enableClock();
        return {i2c};
    }

    /// @brief Map a DMA channel to the RX channel of the I2C
    ///
    template <dma::Feature F2>
    void mapRx(const dma::Info<F2> &dmaInfo) const;

    /// @brief Map a DMA channel to the TX channel of the I2C
    ///
    template <dma::Feature F2>
    void mapTx(const dma::Info<F2> &dmaInfo) const;

    /// @brief Map DMA channels to the RX and TX channels of the I2C
    ///
    template <dma::Feature F2>
    void map(const dma::DualInfo<F2> &dmaInfo) const;
};

} // namespace i2c
} // namespace coco
