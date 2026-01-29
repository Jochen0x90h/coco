#pragma once

#include "i2cF0C0G0F3L4G4H5U3U5.hpp"


namespace coco {
namespace i2c {

template <dma::Feature F2>
void Info::mapRx(const dma::Info<F2> &dmaInfo) const {
    // todo
}

/// @brief Map a DMA channel to the TX channel of the I2C
/// Check reference manual which mappings are possible
template <dma::Feature F2>
void Info::mapTx(const dma::Info<F2> &dmaInfo) const {
    // todo
}

/// @brief Map DMA channels to the RX and TX channels of the I2C
/// Check reference manual which mappings are possible
template <dma::Feature F2>
void Info::map(const dma::DualInfo<F2> &dmaInfo) const {
    // todo
}


static const Info I2C1_INFO{I2C1, RCC_APB1ENR1_I2C1EN, I2C1_EV_IRQn};
static const Info I2C2_INFO{I2C2, RCC_APB1ENR1_I2C2EN, I2C2_EV_IRQn};
#ifdef I2C3
static const Info I2C3_INFO{I2C3, RCC_APB1ENR1_I2C3EN, I2C3_EV_IRQn};
#endif

} // namespace i2c
} // namespace coco
