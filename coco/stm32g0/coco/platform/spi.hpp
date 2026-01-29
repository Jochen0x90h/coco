#pragma once

#include "spiF0C0G0F3L4G4.hpp"


namespace coco {
namespace spi {

template <dma::Feature F2>
void Info::mapRx(const dma::Info<F2> &dmaInfo) const {
    dmaInfo.setRequest(16);
}

/// @brief Map a DMA channel to the TX channel of the SPI
///
template <dma::Feature F2>
void Info::mapTx(const dma::Info<F2> &dmaInfo) const {
    dmaInfo.setRequest(17);
}

/// @brief Map DMA channels to the RX and TX channels of the SPI
///
template <dma::Feature F2>
void Info::map(const dma::DualInfo<F2> &dmaInfo) const {
    dmaInfo.setRequest1(16);
    dmaInfo.setRequest2(17);
}


#ifdef SPI3
constexpr int SPI2_IRQn = SPI2_3_IRQn;
#endif

static const Info SPI1_INFO{SPI1, RCC_APBENR2_SPI1EN, SPI1_IRQn, 16};
static const Info SPI2_INFO{SPI2, RCC_APBENR1_SPI2EN, SPI2_IRQn, 18};
#ifdef SPI3
static const Info SPI3_INFO{SPI3, RCC_APBENR1_SPI3EN, SPI2_3_IRQn, 66};
#endif

} // namespace spi
} // namespace coco
