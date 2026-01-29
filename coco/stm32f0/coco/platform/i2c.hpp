#pragma once

#include "i2cF0C0G0F3L4G4H5U3U5.hpp"


namespace coco {
namespace i2c {

template <dma::Feature F2>
void Info::mapRx(const dma::Info<F2> &dmaInfo) const {
#ifdef SYSCFG_CFGR1_I2C1_DMA_RMP
    if (uintptr_t(i2c) == I2C1_BASE && dmaInfo.channelIndex == 6)
        SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_I2C1_DMA_RMP;
#endif
}

template <dma::Feature F2>
void Info::mapTx(const dma::Info<F2> &dmaInfo) const {
#ifdef SYSCFG_CFGR1_I2C1_DMA_RMP
    if (uintptr_t(i2c) == I2C1_BASE && dmaInfo.channelIndex == 5)
        SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_I2C1_DMA_RMP;
#endif
}

template <dma::Feature F2>
void Info::map(const dma::DualInfo<F2> &dmaInfo) const {
#ifdef SYSCFG_CFGR1_I2C1_DMA_RMP
    if (uintptr_t(i2c) == I2C1_BASE && dmaInfo.channelIndex1 == 6)
        SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_I2C1_DMA_RMP;
#endif
}


static const Info I2C1_INFO{I2C1, RCC_APB1ENR_I2C1EN, I2C1_IRQn};
#ifdef I2C2
static const Info I2C2_INFO{I2C2, RCC_APB1ENR_I2C2EN, I2C2_IRQn};
#endif
#ifdef I2C3
static const Info I2C3_INFO{I2C3, RCC_APB1ENR_I2C3EN, I2C3_IRQn};
#endif

} // namespace i2c
} // namespace coco
