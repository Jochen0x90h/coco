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
        dmaInfo.setMux1(this->mux + 0); // rx
        dmaInfo.setMux2(this->mux + 1); // tx
    }
};


static const Info USART1_INFO{USART1, {&RCC->APB2ENR, RCC_APB2ENR_USART1EN}, USART1_IRQn, 24};
static const Info USART2_INFO{USART2, {&RCC->APB1ENR1, RCC_APB1ENR1_USART2EN}, USART2_IRQn, 26};
static const Info USART3_INFO{USART3, {&RCC->APB1ENR1, RCC_APB1ENR1_USART3EN}, USART3_IRQn, 28};
#ifdef UART4
static const Info UART4_INFO{UART4, {&RCC->APB1ENR1, RCC_APB1ENR1_UART4EN}, UART4_IRQn, 30};
#endif
#ifdef UART5
static const Info UART5_INFO{UART5, {&RCC->APB1ENR1, RCC_APB1ENR1_UART5EN}, UART5_IRQn, 32};
#endif
static const Info LPUART1_INFO{LPUART1, {&RCC->APB1ENR2, RCC_APB1ENR2_LPUART1EN}, LPUART1_IRQn, 34};

} // namespace usart
} // namespace coco
