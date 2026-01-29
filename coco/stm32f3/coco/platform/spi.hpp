#pragma once

#include "spiF0C0G0F3L4G4.hpp"


namespace coco {

/// @brief SPI helpers
///
namespace spi {

template <dma::Feature F2>
void Info::mapRx(const dma::Info<F2> &dmaInfo) const {
#ifdef SPI2
    // SPI1 and SPI2 are hardwired
#else
    // only SPI1
    int mapping = dmaInfo.channelIndex >> 1;
    SYSCFG->CFGR3 = SYSCFG->CFGR3 | (mapping << SYSCFG_CFGR3_SPI1_RX_DMA_RMP_Pos);
#endif
}

template <dma::Feature F2>
void Info::mapTx(const dma::Info<F2> &dmaInfo) const {
#ifdef SPI2
    // SPI1 and SPI2 are hardwired
#else
    // only SPI1
    int mapping = (dmaInfo.channelIndex - 1) >> 1;
    SYSCFG->CFGR3 = SYSCFG->CFGR3 | (mapping << SYSCFG_CFGR3_SPI1_TX_DMA_RMP_Pos);
#endif
}

template <dma::Feature F2>
void Info::map(const dma::DualInfo<F2> &dmaInfo) const {
#ifdef SPI2
    // SPI1 and SPI2 are hardwired
#else
    // only SPI1
    int rxMapping = dmaInfo.channelIndex1 >> 1;
    int txMapping = (dmaInfo.channelIndex2 - 1) >> 1;
    SYSCFG->CFGR3 = SYSCFG->CFGR3 | (rxMapping << SYSCFG_CFGR3_SPI1_RX_DMA_RMP_Pos) | (txMapping << SYSCFG_CFGR3_SPI1_TX_DMA_RMP_Pos);
#endif
}


static const Info SPI1_INFO{SPI1, {&RCC->APB2ENR, RCC_APB2ENR_SPI1EN}, SPI1_IRQn};
#ifdef SPI2
static const Info SPI2_INFO{SPI2, {&RCC->APB1ENR, RCC_APB1ENR_SPI2EN}, SPI2_IRQn};
#endif
#ifdef SPI3
static const Info SPI3_INFO{SPI3, {&RCC->APB1ENR, RCC_APB1ENR_SPI3EN}, SPI3_IRQn};
#endif

} // namespace spi
} // namespace coco
