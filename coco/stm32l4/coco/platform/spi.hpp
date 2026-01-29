#pragma once

#include "spiF0C0G0F3L4G4.hpp"


namespace coco {

/// @brief SPI helpers
///
namespace spi {

template <dma::Feature F2>
void Info::mapRx(const dma::Info<F2> &dmaInfo) const {
    int index = dmaInfo.index() == 0 ? 1 : 3 + (dmaInfo.channelIndex >> 1);
    dmaInfo.setRequest(index);
}

/// @brief Map a DMA channel to the TX channel of the SPI
/// Check reference manual which mappings are possible
template <dma::Feature F2>
void Info::mapTx(const dma::Info<F2> &dmaInfo) const {
    int index = dmaInfo.index() == 0 ? 1 : 3 + (dmaInfo.channelIndex >> 1);
    dmaInfo.setRequest(index);
}

/// @brief Map DMA channels to the RX and TX channels of the SPI
/// Check reference manual which mappings are possible
template <dma::Feature F2>
void Info::map(const dma::DualInfo<F2> &dmaInfo) const {
    int index = dmaInfo.index() == 0 ? 1 : 3 + (dmaInfo.channelIndex1 >> 1);
    dmaInfo.setRequest1(index); // rx
    dmaInfo.setRequest2(index); // tx
}


static const Info SPI1_INFO{SPI1, {&RCC->APB2ENR, RCC_APB2ENR_SPI1EN}, SPI1_IRQn};
#ifdef SPI2
static const Info SPI2_INFO{SPI2, {&RCC->APB1ENR1, RCC_APB1ENR1_SPI2EN}, SPI2_IRQn};
#endif
static const Info SPI3_INFO{SPI3, {&RCC->APB1ENR1, RCC_APB1ENR1_SPI3EN}, SPI3_IRQn};

} // namespace spi
} // namespace coco
