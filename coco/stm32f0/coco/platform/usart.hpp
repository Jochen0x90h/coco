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
        switch (uintptr_t(this->usart)) {
        default: // USART1
            if (dmaInfo.channelIndex == 4)
                SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_USART1RX_DMA_RMP;
            break;
#ifdef USART2
        case USART2_BASE:
#ifdef SYSCFG_CFGR1_USART2_DMA_RMP
            if (dmaInfo.channelIndex == 5)
                SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_USART2_DMA_RMP;
#endif
            break;
#endif
#ifdef USART3
        case USART3_BASE:
            if (dmaInfo.channelIndex == 2)
                SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_USART3_DMA_RMP;
            break;
#endif
        }
    }

    /**
     * Map a DMA channel to the TX channel of the USART
     * Check reference manual which mappings are possible
     */
    void mapTx(const dma::Info &dmaInfo) const {
        switch (uintptr_t(this->usart)) {
        default: // USART1
            if (dmaInfo.channelIndex == 3)
                SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_USART1TX_DMA_RMP;
            break;
#ifdef USART2
        case USART2_BASE:
#ifdef SYSCFG_CFGR1_USART2_DMA_RMP
            if (dmaInfo.channelIndex == 6)
                SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_USART2_DMA_RMP;
#endif
            break;
#endif
#ifdef USART3
        case USART3_BASE:
            if (dmaInfo.channelIndex == 1)
                SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_USART3_DMA_RMP;
            break;
#endif
        }
    }

    /**
     * Map DMA channels to the RX and TX channels of the USART
     * Check reference manual which mappings are possible
     */
    void map(const dma::Info2 &dmaInfo) const {
        switch (uintptr_t(this->usart)) {
        default: // USART1
            if (dmaInfo.channelIndex1 == 4)
                SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_USART1RX_DMA_RMP;
            if (dmaInfo.channelIndex2 == 3)
                SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_USART1TX_DMA_RMP;
            break;
#ifdef USART2
        case USART2_BASE:
#ifdef SYSCFG_CFGR1_USART2_DMA_RMP
            if (dmaInfo.channelIndex == 5)
                SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_USART2_DMA_RMP;
#endif
            break;
#endif
#ifdef USART3
        case USART3_BASE:
            if (dmaInfo.channelIndex == 2)
                SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_USART3_DMA_RMP;
            break;
#endif
        }
    }
};


static const Info USART1_INFO{USART1, {&RCC->APB2ENR, RCC_APB2ENR_USART1EN}, USART1_IRQn};
#ifdef USART2
static const Info USART2_INFO{USART2, {&RCC->APB1ENR, RCC_APB1ENR_USART2EN}, USART2_IRQn};
#endif
#ifdef USART3
static const Info USART3_INFO{USART3, {&RCC->APB1ENR, RCC_APB1ENR_USART3EN}, USART3_6_IRQn};
#endif
#ifdef USART4
static const Info USART4_INFO{USART4, {&RCC->APB1ENR, RCC_APB1ENR_USART4EN}, USART3_6_IRQn};
#endif
#ifdef USART5
static const Info USART5_INFO{USART5, {&RCC->APB1ENR, RCC_APB1ENR_USART5EN}, USART3_6_IRQn};
#endif

} // namespace usart
} // namespace coco
