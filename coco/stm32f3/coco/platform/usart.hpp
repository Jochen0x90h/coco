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


    /**
     * Map a DMA channel to the RX channel of the USART
     * Check reference manual which mappings are possible
     */
    void mapRx(const dma::Info &dmaInfo) const {
        // nothing to do, check reference manual which DMA channel is hard-wired to which RX channel
    }

    /**
     * Map a DMA channel to the TX channel of the USART
     * Check reference manual which mappings are possible
     */
    void mapTx(const dma::Info &dmaInfo) const {
        // nothing to do, check reference manual which DMA channel is hard-wired to which TX channel
    }

    /**
     * Map DMA channels to the RX and TX channels of the USART
     * Check reference manual which mappings are possible
     */
    void map(const dma::Info2 &dmaInfo) const {
        // nothing to do, check reference manual which DMA channel is hard-wired to which TX channel
    }
};


static const Info USART1_INFO{USART1, {&RCC->APB2ENR, RCC_APB2ENR_USART1EN}, USART1_IRQn};
static const Info USART2_INFO{USART2, {&RCC->APB1ENR, RCC_APB1ENR_USART2EN}, USART2_IRQn};
static const Info USART3_INFO{USART3, {&RCC->APB1ENR, RCC_APB1ENR_USART3EN}, USART3_IRQn};
#ifdef UART4
static const Info UART4_INFO{UART4, {&RCC->APB1ENR, RCC_APB1ENR_UART4EN}, UART4_IRQn};
#endif
#ifdef UART5
static const Info UART5_INFO{UART5, {&RCC->APB1ENR, RCC_APB1ENR_UART5EN}, UART5_IRQn};
#endif

} // namespace usart
} // namespace coco
