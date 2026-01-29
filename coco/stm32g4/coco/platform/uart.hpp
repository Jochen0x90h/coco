#pragma once

#include "uartF0L0C0G0F3L4G4H5U3U5.hpp"


namespace coco {
namespace uart {

template <Feature F>
template <dma::Feature F2>
void Info<F>::mapRx(const dma::Info<F2> &dmaInfo) const {
    dmaInfo.setRequest(drq + 0);
}

template <Feature F>
template <dma::Feature F2>
void Info<F>::mapTx(const dma::Info<F2> &dmaInfo) const {
    dmaInfo.setRequest(drq + 1);
}

template <Feature F>
template <dma::Feature F2>
void Info<F>::map(const dma::DualInfo<F2> &dmaInfo) const {
    dmaInfo.setRequest1(drq + 0); // rx
    dmaInfo.setRequest2(drq + 1); // tx
}


using FullInfo = Info<Feature::BAUD_RATE | Feature::MODBUS | Feature::ALL_MODES>;
using BasicInfo = Info<Feature::BAUD_RATE | Feature::MODBUS | Feature::ASYNC_MODE | Feature::LIN_MODE>;
using LpInfo = Info<Feature::LP_BAUD_RATE | Feature::ASYNC_MODE>;

static const FullInfo USART1_INFO{USART1, {&RCC->APB2ENR, RCC_APB2ENR_USART1EN}, USART1_IRQn, 24};
static const FullInfo USART2_INFO{USART2, {&RCC->APB1ENR1, RCC_APB1ENR1_USART2EN}, USART2_IRQn, 26};
static const FullInfo USART3_INFO{USART3, {&RCC->APB1ENR1, RCC_APB1ENR1_USART3EN}, USART3_IRQn, 28};
#ifdef UART4
static const BasicInfo UART4_INFO{UART4, {&RCC->APB1ENR1, RCC_APB1ENR1_UART4EN}, UART4_IRQn, 30};
#endif
#ifdef UART5
static const BasicInfo UART5_INFO{UART5, {&RCC->APB1ENR1, RCC_APB1ENR1_UART5EN}, UART5_IRQn, 32};
#endif
static const LpInfo LPUART1_INFO{LPUART1, {&RCC->APB1ENR2, RCC_APB1ENR2_LPUART1EN}, LPUART1_IRQn, 34};

} // namespace uart
} // namespace coco
