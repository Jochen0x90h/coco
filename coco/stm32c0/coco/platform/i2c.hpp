#pragma once

#include "platform.hpp"
#include "dma.hpp"


namespace coco {
namespace i2c {

/**
 * I2C info
 */
struct Info {
    // registers
    I2C_TypeDef *i2c;

    // reset and clock control
    rcc::Info2<offsetof(RCC_TypeDef, APBENR1)> rcc;

    // interrupt index
    uint8_t irq;


    /**
     * Map a DMA channel to the RX channel of the I2C
     */
    void mapRx(const dma::Info &dmaInfo) const {
        dmaInfo.setMux(10);
    }

    /**
     * Map a DMA channel to the TX channel of the I2C
     */
    void mapTx(const dma::Info &dmaInfo) const {
        dmaInfo.setMux(11);
    }

    /**
     * Map DMA channels to the RX and TX channels of the I2C
     */
    void map(const dma::Info2 &dmaInfo) const {
        dmaInfo.setMux1(10);
        dmaInfo.setMux2(11);
    }
};


static const Info I2C1_INFO{I2C1, RCC_APBENR1_I2C1EN, I2C1_IRQn};

} // namespace i2c
} // namespace coco
