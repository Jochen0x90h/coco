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
    dmaInfo.setRxRequest1(drq + 0); // rx
    dmaInfo.setTxRequest2(drq + 1); // tx
}


static const Info I2C1_INFO{I2C1, {&RCC->APB1LENR, RCC_APB1LENR_I2C1EN}, I2C1_EV_IRQn, 12};
static const Info I2C2_INFO{I2C1, {&RCC->APB1LENR, RCC_APB1LENR_I2C2EN}, I2C2_EV_IRQn, 15};
#ifdef I2C3
static const Info I2C3_INFO{I2C1, {&RCC->APB3ENR, RCC_APB3ENR_I2C3EN}, I2C3_EV_IRQn, 18};
#endif
#ifdef I2C4
static const Info I2C4_INFO{I2C1, {&RCC->APB3ENR, RCC_APB3ENR_I2C4EN}, I2C4_EV_IRQn, 124};
#endif

} // namespace i2c
} // namespace coco
