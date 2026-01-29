#pragma once

#include "spiF0C0G0F3L4G4.hpp"


namespace coco {

/// @brief SPI helpers
///
namespace spi {

template <dma::Feature F2>
void Info::mapRx(const dma::Info<F2> &dmaInfo) const {
    dmaInfo.setRequest(16);
}

template <dma::Feature F2>
void Info::mapTx(const dma::Info<F2> &dmaInfo) const {
    dmaInfo.setRequest(17);
}

template <dma::Feature F2>
void Info::map(const dma::DualInfo<F2> &dmaInfo) const {
    dmaInfo.setRequest1(16);
    dmaInfo.setRequest2(17);
}


static const Info SPI1_INFO{SPI1, RCC_APBENR2_SPI1EN, SPI1_IRQn, 16};

} // namespace spi
} // namespace coco
