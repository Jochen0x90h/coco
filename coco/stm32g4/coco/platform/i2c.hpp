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
    rcc::Info rcc;

    // interrupt index
    uint8_t irq;

    // DMA mux index
    uint8_t mux;


    /**
     * Map a DMA channel to the RX channel of the I2C
     */
    void mapRx(const dma::Info &dmaInfo) const {
        dmaInfo.setMux(this->mux + 0);
    }

    /**
     * Map a DMA channel to the TX channel of the I2C
     */
    void mapTx(const dma::Info &dmaInfo) const {
        dmaInfo.setMux(this->mux + 1);
    }

    /**
     * Map DMA channels to the RX and TX channels of the I2C
     */
    void map(const dma::Info2 &dmaInfo) const {
        dmaInfo.setMux1(this->mux + 0); // rx
        dmaInfo.setMux2(this->mux + 1); // tx
    }
};


static const Info I2C1_INFO{I2C1, {&RCC->APB1ENR1, RCC_APB1ENR1_I2C1EN}, I2C1_EV_IRQn, 16};
static const Info I2C2_INFO{I2C2, {&RCC->APB1ENR1, RCC_APB1ENR1_I2C2EN}, I2C2_EV_IRQn, 18};
static const Info I2C3_INFO{I2C3, {&RCC->APB1ENR1, RCC_APB1ENR1_I2C3EN}, I2C3_EV_IRQn, 20};
#ifdef I2C4
static const Info I2C4_INFO{I2C4, {&RCC->APB1ENR2, RCC_APB1ENR2_I2C4EN}, I2C4_EV_IRQn, 22};
#endif

} // namespace i2c
} // namespace coco
