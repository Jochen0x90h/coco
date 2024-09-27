#pragma once

#include "platform.hpp"
#include "dma.hpp"
#include "usartConfig.hpp"


namespace coco {
namespace usart {

/**
 * USART info
 */
struct Info {
    // registers
    USART_TypeDef *usart;

    // reset and clock control
    rcc::Info rcc;

    // interrupt index
    uint8_t irq;

    // DMA mux index
    uint8_t mux;


    /**
     * Map a DMA channel to the RX channel of the USART
     */
    void mapRx(const dma::Info &dmaInfo) const {
        dmaInfo.setMux(this->mux + 0);
    }

    /**
     * Map a DMA channel to the TX channel of the USART
     */
    void mapTx(const dma::Info &dmaInfo) const {
        dmaInfo.setMux(this->mux + 1);
    }

    /**
     * Map DMA channels to the RX and TX channels of the USART
     */
    void map(const dma::Info2 &dmaInfo) const {
        dmaInfo.setMux1(this->mux + 0);
        dmaInfo.setMux2(this->mux + 1);
    }
};


static const Info USART1_INFO{USART1, {&RCC->APBENR2, RCC_APBENR2_USART1EN}, USART1_IRQn, 50};
static const Info USART2_INFO{USART2, {&RCC->APBENR1, RCC_APBENR1_USART2EN}, USART2_IRQn, 52};

} // namespace usart
} // namespace coco
