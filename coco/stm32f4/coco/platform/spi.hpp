#pragma once

#include "platform.hpp"
#include "dma.hpp"
#include "spiF1L1F4.hpp"


namespace coco {

/// @brief SPI helpers
///
namespace spi {

template <dma::Feature F2>
void Info::mapRx(const dma::Info<F2> &dmaInfo) const {
    dmaInfo.setRequest(drq);
}

template <dma::Feature F2>
void Info::mapTx(const dma::Info<F2> &dmaInfo) const {
    dmaInfo.setRequest(drq);
}

template <dma::Feature F2>
void Info::map(const dma::DualInfo<F2> &dmaInfo) const {
    dmaInfo.setRequest1(drq); // rx
    dmaInfo.setRequest2(drq); // tx
}


static const Info SPI1_INFO{SPI1, {&RCC->APB2ENR, RCC_APB2ENR_SPI1EN}, SPI1_IRQn, 3};
#ifdef SPI2
static const Info SPI2_INFO{SPI2, {&RCC->APB1ENR, RCC_APB1ENR_SPI2EN}, SPI2_IRQn, 0};
#endif
#ifdef SPI3
static const Info SPI3_INFO{SPI3, {&RCC->APB1ENR, RCC_APB1ENR_SPI3EN}, SPI3_IRQn, 0};
#endif
#ifdef SPI4
static const Info SPI4_INFO{SPI4, {&RCC->APB2ENR, RCC_APB2ENR_SPI4EN}, SPI4_IRQn, 4};
#endif
#ifdef SPI5
static const Info SPI5_INFO{SPI5, {&RCC->APB2ENR, RCC_APB2ENR_SPI5EN}, SPI5_IRQn, 7};
#endif
#ifdef SPI6
static const Info SPI6_INFO{SPI6, {&RCC->APB2ENR, RCC_APB2ENR_SPI6EN}, SPI6_IRQn, 1};
#endif

} // namespace spi
} // namespace coco
