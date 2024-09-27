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
    rcc::Info2<offsetof(RCC_TypeDef, APBENR2)> rcc;

    // interrupt index
    uint8_t irq;


    /**
     * Map a DMA channel to the RX channel of the SPI
     */
    void mapRx(const dma::Info &dmaInfo) const {
        dmaInfo.setMux(16);
    }

    /**
     * Map a DMA channel to the TX channel of the SPI
     */
    void mapTx(const dma::Info &dmaInfo) const {
        dmaInfo.setMux(17);
    }

    /**
     * Map DMA channels to the RX and TX channels of the SPI
     */
    void map(const dma::Info2 &dmaInfo) const {
        dmaInfo.setMux1(16);
        dmaInfo.setMux2(17);
    }
};


static const Info SPI1_INFO{SPI1, RCC_APBENR2_SPI1EN, SPI1_IRQn};

} // namespace spi
} // namespace coco
