#pragma once

#include "platform.hpp"
#include "dma.hpp"
#include "spiConfig.hpp"


namespace coco {

/**
 * SPI helpers
 */
namespace spi {

/**
 * SPI info
 */
struct Info {
    // registers
    SPI_TypeDef *spi;

    // reset and clock control
    rcc::Info rcc;

    // interrupt index
    uint8_t irq;

    // DMA mux index
    uint8_t mux;


    /**
     * Map a DMA channel to the RX channel of the SPI
     */
    void mapRx(const dma::Info &dmaInfo) const {
        dmaInfo.setMux(this->mux + 0);
    }

    /**
     * Map a DMA channel to the TX channel of the SPI
     */
    void mapTx(const dma::Info &dmaInfo) const {
        dmaInfo.setMux(this->mux + 1);
    }

    /**
     * Map DMA channels to the RX and TX channels of the SPI
     */
    void map(const dma::Info2 &dmaInfo) const {
        dmaInfo.setMux1(this->mux + 0); // rx
        dmaInfo.setMux2(this->mux + 1); // tx
    }
};


static const Info SPI1_INFO{SPI1, {&RCC->APB2ENR, RCC_APB2ENR_SPI1EN}, SPI1_IRQn, 10};
static const Info SPI2_INFO{SPI2, {&RCC->APB1ENR1, RCC_APB1ENR1_SPI2EN}, SPI2_IRQn, 12};
static const Info SPI3_INFO{SPI3, {&RCC->APB1ENR1, RCC_APB1ENR1_SPI3EN}, SPI3_IRQn, 14};

} // namespace spi
} // namespace coco
