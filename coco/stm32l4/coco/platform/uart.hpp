#pragma once

#include "uartF0L0C0G0F3L4G4H5U3U5.hpp"


namespace coco {

/// @brief USART helpers.
/// L46 Refernece manual:
///   Sections 39 + 40
/// L47 Refernece manual:
///   Sections 40 + 41
namespace uart {

template <Feature F>
template <dma::Feature F2>
void Info<F>::mapRx(const dma::Info<F2> &dmaInfo) const {
    int index = uart == LPUART1 ? 4 : 2;
    dmaInfo.setRequest(index);
}

template <Feature F>
template <dma::Feature F2>
void Info<F>::mapTx(const dma::Info<F2> &dmaInfo) const {
    int index = uart == LPUART1 ? 4 : 2;
    dmaInfo.setRequest(index);
}

template <Feature F>
template <dma::Feature F2>
void Info<F>::map(const dma::DualInfo<F2> &dmaInfo) const {
    int index = uart == LPUART1 ? 4 : 2;
    dmaInfo.setRequest1(index);
    dmaInfo.setRequest2(index);
}


using FullInfo = Info<Feature::BAUD_RATE | Feature::MODBUS | Feature::ALL_MODES>;
using BasicInfo = Info<Feature::BAUD_RATE | Feature::MODBUS | Feature::ASYNC_MODE | Feature::LIN_MODE>;
using LpInfo = Info<Feature::LP_BAUD_RATE | Feature::ASYNC_MODE>;

static const FullInfo USART1_INFO{USART1, {&RCC->APB2ENR, RCC_APB2ENR_USART1EN}, USART1_IRQn};
static const FullInfo USART2_INFO{USART2, {&RCC->APB1ENR1, RCC_APB1ENR1_USART2EN}, USART2_IRQn};
#ifdef USART3
static const FullInfo USART3_INFO{USART3, {&RCC->APB1ENR1, RCC_APB1ENR1_USART3EN}, USART3_IRQn};
#endif
#ifdef UART4
static const BasicInfo UART4_INFO{UART4, {&RCC->APB1ENR1, RCC_APB1ENR1_UART4EN}, UART4_IRQn};
#endif
static const LpInfo LPUART1_INFO{LPUART1, {&RCC->APB1ENR2, RCC_APB1ENR2_LPUART1EN}, LPUART1_IRQn};

} // namespace uart
} // namespace coco
