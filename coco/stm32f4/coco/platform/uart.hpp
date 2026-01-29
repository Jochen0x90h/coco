#pragma once

#include "platform.hpp"
#include "dma.hpp"
#include "uartF1L1F4.hpp"


namespace coco {

/// @brief USART helpers.
/// Refernece manual:
///   Section 30
namespace uart {

template <Feature F>
template <dma::Feature F2>
void Info<F>::mapRx(const dma::Info<F2> &dmaInfo) const {
    int rxChannel = drq;
    dmaInfo.setRequest(rxChannel);
}

template <Feature F>
template <dma::Feature F2>
void Info<F>::mapTx(const dma::Info<F2> &dmaInfo) const {
    int txChannel = drq;
#ifdef UART3
    if (usart == USART3 && dmaInfo.channelIndex == 4)
        txChannel = 7;
#endif
    dmaInfo.setRequest(txChannel);
}

template <Feature F>
template <dma::Feature F2>
void Info<F>::map(const dma::DualInfo<F2> &dmaInfo) const {
    int rxChannel = drq;
    int txChannel = drq;
#ifdef UART3
    if (usart == USART3 && dmaInfo.channelIndex2 == 4)
        txChannel = 7;
#endif
    dmaInfo.setRequest1(rxChannel);
    dmaInfo.setRequest2(txChannel);
}


using FullInfo = Info<Feature::BAUD_RATE | Feature::ALL_MODES>;

static const FullInfo USART1_INFO{USART1, {&RCC->APB2ENR, RCC_APB2ENR_USART1EN}, USART1_IRQn, 4};
static const FullInfo USART2_INFO{USART2, {&RCC->APB1ENR, RCC_APB1ENR_USART2EN}, USART2_IRQn, 4};
#ifdef USART3
static const FullInfo USART3_INFO{USART3, {&RCC->APB1ENR, RCC_APB1ENR_USART3EN}, USART3_IRQn, 4};
#endif
#ifdef UART4
static const FullInfo UART4_INFO{UART4, {&RCC->APB1ENR, RCC_APB1ENR_UART4EN}, UART4_IRQn, 4};
#endif
#ifdef UART5
static const FullInfo UART5_INFO{UART5, {&RCC->APB1ENR, RCC_APB1ENR_UART5EN}, UART5_IRQn, 4};
#endif
#ifdef USART6
static const FullInfo USART6_INFO{USART6, {&RCC->APB2ENR, RCC_APB2ENR_USART6EN}, USART6_IRQn, 5};
#endif
#ifdef UART7
static const FullInfo UART7_INFO{UART7, {&RCC->APB1ENR, RCC_APB1ENR_UART7EN}, UART7_IRQn, 5};
#endif
#ifdef UART8
static const FullInfo UART8_INFO{UART8, {&RCC->APB1ENR, RCC_APB1ENR_UART8EN}, UART8_IRQn, 5};
#endif

} // namespace uart
} // namespace coco
