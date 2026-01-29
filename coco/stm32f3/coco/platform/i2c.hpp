#pragma once

#include "i2cF0C0G0F3L4G4H5U3U5.hpp"


namespace coco {
namespace i2c {

template <dma::Feature F2>
void Info::mapRx(const dma::Info<F2> &dmaInfo) const {
#ifdef I2C2
    // I2C1, I2C2 and I2C3 are hardwired
#else
    // only I2C1
    int mapping = (dmaInfo.channelIndex + 1) >> 1;
    SYSCFG->CFGR3 = SYSCFG->CFGR3 | (mapping << SYSCFG_CFGR3_I2C1_RX_DMA_RMP_Pos);
#endif
}

template <dma::Feature F2>
void Info::mapTx(const dma::Info<F2> &dmaInfo) const {
#ifdef I2C2
    // I2C1, I2C2 and I2C3 are hardwired
#else
    // only I2C1
    int mapping = (dmaInfo.channelIndex) >> 1;
    SYSCFG->CFGR3 = SYSCFG->CFGR3 | (mapping << SYSCFG_CFGR3_I2C1_TX_DMA_RMP_Pos);
#endif
}

template <dma::Feature F2>
void Info::map(const dma::DualInfo<F2> &dmaInfo) const {
#ifdef I2C2
    // I2C1, I2C2 and I2C3 are hardwired
#else
    // only I2C1
    int rxMapping = (dmaInfo.channelIndex1 + 1) >> 1;
    int txMapping = (dmaInfo.channelIndex2) >> 1;
    SYSCFG->CFGR3 = SYSCFG->CFGR3 | (rxMapping << SYSCFG_CFGR3_I2C1_RX_DMA_RMP_Pos) | (txMapping << SYSCFG_CFGR3_I2C1_TX_DMA_RMP_Pos);
#endif
}


static const Info I2C1_INFO{I2C1, RCC_APB1ENR_I2C1EN, I2C1_EV_IRQn};
#ifdef I2C2
static const Info I2C2_INFO{I2C2, RCC_APB1ENR_I2C2EN, I2C2_EV_IRQn};
#endif
#ifdef I2C3
static const Info I2C3_INFO{I2C3, RCC_APB1ENR_I2C3EN, I2C3_EV_IRQn};
#endif

} // namespace i2c
} // namespace coco
