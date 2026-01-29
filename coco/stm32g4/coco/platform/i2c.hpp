#pragma once

#include "i2cF0C0G0F3L4G4H5U3U5.hpp"


namespace coco {
namespace i2c {

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


static const Info I2C1_INFO{I2C1, {&RCC->APB1ENR1, RCC_APB1ENR1_I2C1EN}, I2C1_EV_IRQn, 16};
static const Info I2C2_INFO{I2C2, {&RCC->APB1ENR1, RCC_APB1ENR1_I2C2EN}, I2C2_EV_IRQn, 18};
static const Info I2C3_INFO{I2C3, {&RCC->APB1ENR1, RCC_APB1ENR1_I2C3EN}, I2C3_EV_IRQn, 20};
#ifdef I2C4
static const Info I2C4_INFO{I2C4, {&RCC->APB1ENR2, RCC_APB1ENR2_I2C4EN}, I2C4_EV_IRQn, 22};
#endif

} // namespace i2c
} // namespace coco
