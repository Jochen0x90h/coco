#pragma once

#include "uartF0L0C0G0F3L4G4H5U3U5.hpp"


namespace coco {
namespace uart {

template <Feature F>
template <dma::Feature F2>
void Info<F>::mapRx(const dma::Info<F2> &dmaInfo) const {
    // nothing to do, check reference manual which DMA channel is hard-wired to which RX channel
}

template <Feature F>
template <dma::Feature F2>
void Info<F>::mapTx(const dma::Info<F2> &dmaInfo) const {
    // nothing to do, check reference manual which DMA channel is hard-wired to which TX channel
}

template <Feature F>
template <dma::Feature F2>
void Info<F>::map(const dma::DualInfo<F2> &dmaInfo) const {
    // nothing to do, check reference manual which DMA channel is hard-wired to which RX/TX channels
}


using FullInfo = Info<Feature::BAUD_RATE | Feature::MODBUS | Feature::ALL_MODES>;
using BasicInfo1 = Info<Feature::BAUD_RATE | Feature::MODBUS | Feature::ASYNC_MODE | Feature::LIN_MODE>;
using BasicInfo2 = Info<Feature::BAUD_RATE | Feature::ASYNC_MODE | Feature::SYNC_MODE>;

static const FullInfo USART1_INFO{USART1, {&RCC->APB2ENR, RCC_APB2ENR_USART1EN}, USART1_IRQn};
#if defined(UART4) && defined(UART5)
static const FullInfo USART2_INFO{USART2, {&RCC->APB1ENR, RCC_APB1ENR_USART2EN}, USART2_IRQn};
static const FullInfo USART3_INFO{USART3, {&RCC->APB1ENR, RCC_APB1ENR_USART3EN}, USART3_IRQn};
static const BasicInfo1 UART4_INFO{UART4, {&RCC->APB1ENR, RCC_APB1ENR_UART4EN}, UART4_IRQn};
static const BasicInfo1 UART5_INFO{UART5, {&RCC->APB1ENR, RCC_APB1ENR_UART5EN}, UART5_IRQn};
#else
static const BasicInfo2 USART2_INFO{USART2, {&RCC->APB1ENR, RCC_APB1ENR_USART2EN}, USART2_IRQn};
static const BasicInfo2 USART3_INFO{USART3, {&RCC->APB1ENR, RCC_APB1ENR_USART3EN}, USART3_IRQn};
#endif

} // namespace uart
} // namespace coco
