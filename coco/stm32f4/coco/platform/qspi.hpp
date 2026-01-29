#pragma once

#include "qspiF4L4G4.hpp"


#ifdef HAVE_QUADSPI
namespace coco {
namespace qspi {

template <dma::Feature F2>
void Info::map(const dma::Info<F2> &dmaInfo) const {
    dmaInfo.setRequest(40);
}


static const Info QUADSPI_INFO{QUADSPI, {RCC_AHB3ENR_QSPIEN}, QUADSPI_IRQn, 40};

} // namespace qspi
} // namespace coco
#endif // HAVE_QUADSPI