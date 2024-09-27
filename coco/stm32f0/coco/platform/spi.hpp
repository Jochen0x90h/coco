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


    /**
     * Map a DMA channel to the RX channel of the SPI
     * Check reference manual which mappings are possible
     */
    void mapRx(const dma::Info &dmaInfo) const {
        switch (uintptr_t(this->spi)) {
        default: // SPI1
            break;
#ifdef SPI2
        case SPI2_BASE:
#ifdef SYSCFG_CFGR1_SPI2_DMA_RMP
            if (dmaInfo.channelIndex == 5)
                SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_SPI2_DMA_RMP;
#endif
            break;
#endif
        }
    }

    /**
     * Map a DMA channel to the TX channel of the SPI
     * Check reference manual which mappings are possible
     */
    void mapTx(const dma::Info &dmaInfo) const {
        switch (uintptr_t(this->spi)) {
        default: // SPI1
            break;
#ifdef SPI2
        case SPI2_BASE:
#ifdef SYSCFG_CFGR1_SPI2_DMA_RMP
            if (dmaInfo.channelIndex == 6)
                SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_SPI2_DMA_RMP;
#endif
            break;
#endif
        }
    }

    /**
     * Map DMA channels to the RX and TX channels of the SPI
     * Check reference manual which mappings are possible
     */
    void map(const dma::Info2 &dmaInfo) const {
        switch (uintptr_t(this->spi)) {
        default: // SPI1
            break;
#ifdef SPI2
        case SPI2_BASE:
#ifdef SYSCFG_CFGR1_SPI2_DMA_RMP
            if (dmaInfo.channelIndex1 == 5)
                SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_SPI2_DMA_RMP;
#endif
            break;
#endif
        }
    }
};


static const Info SPI1_INFO{SPI1, {&RCC->APB2ENR, RCC_APB2ENR_SPI1EN}, SPI1_IRQn};
#ifdef SPI2
static const Info SPI2_INFO{SPI2, {&RCC->APB1ENR, RCC_APB1ENR_SPI2EN}, SPI2_IRQn};
#endif

} // namespace spi
} // namespace coco
