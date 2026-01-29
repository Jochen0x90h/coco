#pragma once

#include "platform.hpp"
#include "dma.hpp"
#include "i2cF1L1F4.hpp"


namespace coco {
namespace i2c {

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


static const Info I2C1_INFO{I2C1, RCC_APB1ENR_I2C1EN, I2C1_EV_IRQn, 1};
static const Info I2C2_INFO{I2C2, RCC_APB1ENR_I2C2EN, I2C2_EV_IRQn, 7};
#ifdef I2C3
static const Info I2C3_INFO{I2C3, RCC_APB1ENR_I2C3EN, I2C3_EV_IRQn, 3};
#endif

} // namespace i2c
} // namespace coco
