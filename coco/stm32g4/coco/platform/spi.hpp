#pragma once

#include "spiF0C0G0F3L4G4.hpp"


namespace coco {
namespace spi {

template <dma::Feature F2>
void Info::mapRx(const dma::Info<F2> &dmaInfo) const {
    dmaInfo.setRequest(drq + 0);
}

template <dma::Feature F2>
void Info::mapTx(const dma::Info<F2> &dmaInfo) const {
    dmaInfo.setRequest(drq + 1);
}

template <dma::Feature F2>
void Info::map(const dma::DualInfo<F2> &dmaInfo) const {
    dmaInfo.setRequest1(drq + 0); // rx
    dmaInfo.setRequest2(drq + 1); // tx
}


static const Info SPI1_INFO{SPI1, {&RCC->APB2ENR, RCC_APB2ENR_SPI1EN}, SPI1_IRQn, 10};
static const Info SPI2_INFO{SPI2, {&RCC->APB1ENR1, RCC_APB1ENR1_SPI2EN}, SPI2_IRQn, 12};
static const Info SPI3_INFO{SPI3, {&RCC->APB1ENR1, RCC_APB1ENR1_SPI3EN}, SPI3_IRQn, 14};

} // namespace spi
} // namespace coco
