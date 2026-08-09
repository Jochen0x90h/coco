#pragma once

#include "xspiH5U3U5.hpp"


#ifdef HAVE_XSPI
namespace coco {
namespace xspi {

template <dma::Feature F2>
void Info::map(const dma::Info<F2> &dmaInfo) const {
    dmaInfo.setRequest(40);
}


static const Info OCTOSPI1_INFO{OCTOSPI1, {RCC_AHB2ENR2_OCTOSPI1EN}, OCTOSPI1_IRQn, 40};

} // namespace xspi
} // namespace coco
#endif // HAVE_XSPI
