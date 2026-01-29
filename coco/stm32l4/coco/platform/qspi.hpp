#pragma once

#include "qspiF4L4G4.hpp"


namespace coco {
namespace qspi {

template <dma::Feature F2>
void Info::map(const dma::Info<F2> &dmaInfo) const {
    int index = dmaInfo.index() == 0 ? 5 : 7;
    dmaInfo.setRequest(index);
}


static const Info QUADSPI_INFO{QUADSPI, {RCC_AHB3ENR_QSPIEN}, QUADSPI_IRQn};

} // namespace qspi
} // namespace coco
