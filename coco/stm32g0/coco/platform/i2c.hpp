#pragma once

#include "i2cF0C0G0F3L4G4H5U3U5.hpp"


namespace coco {
namespace i2c {

template <dma::Feature F2>
void Info::mapRx(const dma::Info<F2> &dmaInfo) const {
    dmaInfo.setRequest(10);
}

template <dma::Feature F2>
void Info::mapTx(const dma::Info<F2> &dmaInfo) const {
    dmaInfo.setRequest(11);
}

template <dma::Feature F2>
void Info::map(const dma::DualInfo<F2> &dmaInfo) const {
    dmaInfo.setRequest1(10);
    dmaInfo.setRequest2(11);
}


static const Info I2C1_INFO{I2C1, RCC_APBENR1_I2C1EN, I2C1_IRQn, 10};

} // namespace i2c
} // namespace coco
