#pragma once

// do not include directly, use #include <coco/platform/i2c.hpp>

#include "dma.hpp"
#include <coco/enum.hpp>


namespace coco {

/// @brief I2C helpers.
///
namespace i2c {

/// @brief Wrapper for I2C registers
///
struct Instance {
    I2C_TypeDef *i2c;


    I2C_TypeDef *operator ->() const {return i2c;}
    operator I2C_TypeDef *() const {return i2c;}

    auto &enable(uint32_t cr1) {
        // configure
        i2c->CR1 = cr1;

        return *this;
    }
};


/// @brief I2C info
///
struct Info {
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
