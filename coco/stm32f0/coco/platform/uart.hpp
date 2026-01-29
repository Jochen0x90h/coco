#pragma once

#include "uartF0L0C0G0F3L4G4H5U3U5.hpp"


namespace coco {
namespace uart {

template <Feature F>
template <dma::Feature F2>
void Info<F>::mapRx(const dma::Info<F2> &dmaInfo) const {
    switch (uintptr_t(uart)) {
    case USART1_BASE:
#ifdef SYSCFG_CFGR1_USART1RX_DMA_RMP
        if (dmaInfo.channelIndex == 4)
            SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_USART1RX_DMA_RMP;
#endif
        break;
#ifdef SYSCFG_CFGR1_USART2_DMA_RMP
    case USART2_BASE:
        if (dmaInfo.channelIndex == 5)
            SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_USART2_DMA_RMP;
        break;
#endif
#ifdef SYSCFG_CFGR1_USART3_DMA_RMP
    case USART3_BASE:
        if (dmaInfo.channelIndex == 2)
            SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_USART3_DMA_RMP;
        break;
#endif
    }
}

template <Feature F>
template <dma::Feature F2>
void Info<F>::mapTx(const dma::Info<F2> &dmaInfo) const {
    switch (uintptr_t(uart)) {
    case USART1_BASE:
#ifdef SYSCFG_CFGR1_USART1TX_DMA_RMP
        if (dmaInfo.channelIndex == 3)
            SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_USART1TX_DMA_RMP;
#endif
        break;
#ifdef SYSCFG_CFGR1_USART2_DMA_RMP
    case USART2_BASE:
        if (dmaInfo.channelIndex == 6)
            SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_USART2_DMA_RMP;
        break;
#endif
#ifdef SYSCFG_CFGR1_USART3_DMA_RMP
    case USART3_BASE:
        if (dmaInfo.channelIndex == 1)
            SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_USART3_DMA_RMP;
        break;
#endif
    }
}

template <Feature F>
template <dma::Feature F2>
void Info<F>::map(const dma::DualInfo<F2> &dmaInfo) const {
    switch (uintptr_t(uart)) {
    case USART1_BASE:
#ifdef SYSCFG_CFGR1_USART1RX_DMA_RMP
        if (dmaInfo.channelIndex1 == 4)
            SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_USART1RX_DMA_RMP;
#endif
#ifdef SYSCFG_CFGR1_USART1TX_DMA_RMP
        if (dmaInfo.channelIndex2 == 3)
            SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_USART1TX_DMA_RMP;
#endif
        break;
#ifdef SYSCFG_CFGR1_USART2_DMA_RMP
    case USART2_BASE:
        if (dmaInfo.channelIndex1 == 5)
            SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_USART2_DMA_RMP;
        break;
#endif
#ifdef SYSCFG_CFGR1_USART3_DMA_RMP
    case USART3_BASE:
        if (dmaInfo.channelIndex1 == 2)
            SYSCFG->CFGR1 = SYSCFG->CFGR1 | SYSCFG_CFGR1_USART3_DMA_RMP;
        break;
#endif
    }
}


using FullInfo = Info<Feature::BAUD_RATE | Feature::MODBUS | Feature::ALL_MODES>;
using BasicInfo = Info<Feature::BAUD_RATE| Feature::ASYNC_MODE | Feature::SYNC_MODE>;

// STM32F03x
#if defined(__STM32F030x6_H) || defined(__STM32F030x8_H) || defined(__STM32F030xC_H) || defined(__STM32F031x6_H) || defined(__STM32F038xx_H)
static const FullInfo USART1_INFO{USART1, {&RCC->APB2ENR, RCC_APB2ENR_USART1EN}, USART1_IRQn};
#endif

// STM32F05x
#if defined(__STM32F051x8_H) || defined(__STM32F058xx_H)
static const FullInfo USART1_INFO{USART1, {&RCC->APB2ENR, RCC_APB2ENR_USART1EN}, USART1_IRQn};
static const BasicInfo USART2_INFO{USART2, {&RCC->APB1ENR, RCC_APB1ENR_USART2EN}, USART2_IRQn};
#endif

// STM32F04x
#if defined(__STM32F042x6_H) || defined(__STM32F048xx_H)
static const FullInfo USART1_INFO{USART1, {&RCC->APB2ENR, RCC_APB2ENR_USART1EN}, USART1_IRQn};
static const BasicInfo USART2_INFO{USART2, {&RCC->APB1ENR, RCC_APB1ENR_USART2EN}, USART2_IRQn};
#endif

// STM32F07x
#if defined(__STM32F070x6_H) || defined(__STM32F070xB_H) || defined(__STM32F071xB_H) || defined(__STM32F072xB_H) || defined(__STM32F078xx_H)
static const FullInfo USART1_INFO{USART1, {&RCC->APB2ENR, RCC_APB2ENR_USART1EN}, USART1_IRQn};
static const FullInfo USART2_INFO{USART2, {&RCC->APB1ENR, RCC_APB1ENR_USART2EN}, USART2_IRQn};
#ifdef USART3
static const BasicInfo USART3_INFO{USART3, {&RCC->APB1ENR, RCC_APB1ENR_USART3EN}, USART3_4_IRQn};
#endif
#ifdef USART4
static const BasicInfo USART4_INFO{USART4, {&RCC->APB1ENR, RCC_APB1ENR_USART4EN}, USART3_4_IRQn};
#endif
#endif

// STM32F09x
#if defined(__STM32F091xC_H) || defined(__STM32F098xx_H)
static const FullInfo USART1_INFO{USART1, {&RCC->APB2ENR, RCC_APB2ENR_USART1EN}, USART1_IRQn};
static const FullInfo USART2_INFO{USART2, {&RCC->APB1ENR, RCC_APB1ENR_USART2EN}, USART2_IRQn};
static const FullInfo USART3_INFO{USART3, {&RCC->APB1ENR, RCC_APB1ENR_USART3EN}, USART3_8_IRQn};
static const BasicInfo USART4_INFO{USART4, {&RCC->APB1ENR, RCC_APB1ENR_USART4EN}, USART3_8_IRQn};
static const BasicInfo USART5_INFO{USART5, {&RCC->APB1ENR, RCC_APB1ENR_USART5EN}, USART3_8_IRQn};
static const BasicInfo USART6_INFO{USART5, {&RCC->APB2ENR, RCC_APB2ENR_USART6EN}, USART3_8_IRQn};
static const BasicInfo USART7_INFO{USART5, {&RCC->APB2ENR, RCC_APB2ENR_USART7EN}, USART3_8_IRQn};
static const BasicInfo USART8_INFO{USART5, {&RCC->APB2ENR, RCC_APB2ENR_USART8EN}, USART3_8_IRQn};
#endif

} // namespace uart
} // namespace coco
