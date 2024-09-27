#pragma once

#include "platform.hpp"
#include "dma.hpp"


namespace coco {
namespace i2c {

/**
 * I2C info
 */
struct Info {
    // registers
    I2C_TypeDef *i2c;

    // reset and clock control
    rcc::Info2<offsetof(RCC_TypeDef, APB1ENR)> rcc;

    // interrupt index
    uint8_t irq;


    /**
     * Map a DMA channel to the RX channel of the I2C
     * Check reference manual which mappings are possible
     */
    void mapRx(const dma::Info &dmaInfo) const {
        switch (uintptr_t(this->i2c)) {
        default: // I2C1
#ifdef SYSCFG_CFGR1_I2C1_DMA_RMP
            if (dmaInfo.channelIndex == 6)
                SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_I2C1_DMA_RMP;
#endif
            break;
#ifdef I2C2
        case I2C2_BASE:
            break;
#endif
#ifdef I2C3
        case I2C3_BASE:
            break;
#endif
        }
    }

    /**
     * Map a DMA channel to the TX channel of the I2C
     * Check reference manual which mappings are possible
     */
    void mapTx(const dma::Info &dmaInfo) const {
        switch (uintptr_t(this->i2c)) {
        default: // I2C1
#ifdef SYSCFG_CFGR1_I2C1_DMA_RMP
            if (dmaInfo.channelIndex == 5)
                SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_I2C1_DMA_RMP;
#endif
            break;
#ifdef I2C2
        case I2C2_BASE:
            break;
#endif
#ifdef I2C3
        case I2C3_BASE:
            break;
#endif
        }
    }

    /**
     * Map DMA channels to the RX and TX channels of the I2C
     * Check reference manual which mappings are possible
     */
    void map(const dma::Info2 &dmaInfo) const {
        switch (uintptr_t(this->i2c)) {
        default: // I2C1
#ifdef SYSCFG_CFGR1_I2C1_DMA_RMP
            if (dmaInfo.channelIndex1 == 6)
                SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_I2C1_DMA_RMP;
#endif
            break;
#ifdef I2C2
        case I2C2_BASE:
            break;
#endif
#ifdef I2C3
        case I2C3_BASE:
            break;
#endif
        }
    }
};


static const Info I2C1_INFO{I2C1, RCC_APB1ENR_I2C1EN, I2C1_IRQn};
#ifdef I2C2
static const Info I2C2_INFO{I2C2, RCC_APB1ENR_I2C2EN, I2C2_IRQn};
#endif
#ifdef I2C3
static const Info I2C3_INFO{I2C3, RCC_APB1ENR_I2C3EN, I2C3_IRQn};
#endif

} // namespace i2c
} // namespace coco
