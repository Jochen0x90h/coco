#pragma once

#include "spiH5U3U5.hpp"


namespace coco {
namespace spi {

template <dma::Feature F2>
void Info::mapRx(const dma::Info<F2> &dmaInfo) const {
    dmaInfo.setRxRequest(drq + 0);
}

/// @brief Map a DMA channel to the TX channel of the SPI
///
template <dma::Feature F2>
void Info::mapTx(const dma::Info<F2> &dmaInfo) const {
    dmaInfo.setTxRequest(drq + 1);
}

/// @brief Map DMA channels to the RX and TX channels of the SPI
///
template <dma::Feature F2>
void Info::map(const dma::DualInfo<F2> &dmaInfo) const {
    dmaInfo.setRxRequest1(drq + 0); // rx
    dmaInfo.setTxRequest2(drq + 1); // tx
}


static const Info SPI1_INFO{SPI1, {&RCC->APB2ENR, RCC_APB2ENR_SPI1EN}, SPI1_IRQn, 6};
static const Info SPI2_INFO{SPI2, {&RCC->APB1LENR, RCC_APB1LENR_SPI2EN}, SPI2_IRQn, 8};
static const Info SPI3_INFO{SPI3, {&RCC->APB1LENR, RCC_APB1LENR_SPI3EN}, SPI3_IRQn, 10};

} // namespace spi
} // namespace coco
