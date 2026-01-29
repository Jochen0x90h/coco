#pragma once

#include "spiF0C0G0F3L4G4.hpp"


namespace coco {

/// @brief SPI helpers
///
namespace spi {

template <dma::Feature F2>
void Info::mapRx(const dma::Info<F2> &dmaInfo) const {
#ifdef SYSCFG_CFGR1_SPI2_DMA_RMP
    if (uintptr_t(spi) == SPI2_BASE && dmaInfo.channelIndex == 5)
        SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_SPI2_DMA_RMP;
#endif
}

template <dma::Feature F2>
void Info::mapTx(const dma::Info<F2> &dmaInfo) const {
#ifdef SYSCFG_CFGR1_SPI2_DMA_RMP
    if (uintptr_t(spi) == SPI2_BASE && dmaInfo.channelIndex == 6)
        SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_SPI2_DMA_RMP;
#endif
}

template <dma::Feature F2>
void Info::map(const dma::DualInfo<F2> &dmaInfo) const {
#ifdef SYSCFG_CFGR1_SPI2_DMA_RMP
    if (uintptr_t(spi) == SPI2_BASE && dmaInfo.channelIndex1 == 5)
        SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_SPI2_DMA_RMP;
#endif
}


static const Info SPI1_INFO{SPI1, {&RCC->APB2ENR, RCC_APB2ENR_SPI1EN}, SPI1_IRQn};
#ifdef SPI2
static const Info SPI2_INFO{SPI2, {&RCC->APB1ENR, RCC_APB1ENR_SPI2EN}, SPI2_IRQn};
#endif

} // namespace spi
} // namespace coco
