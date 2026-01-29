#pragma once

#include "uartF0L0C0G0F3L4G4H5U3U5.hpp"


namespace coco {
namespace uart {

template <Feature F>
template <dma::Feature F2>
void Info<F>::mapRx(const dma::Info<F2> &dmaInfo) const {
    dmaInfo.setRxRequest(drq + 0);
}

template <Feature F>
template <dma::Feature F2>
void Info<F>::mapTx(const dma::Info<F2> &dmaInfo) const {
    dmaInfo.setTxRequest(drq + 1);
}

template <Feature F>
template <dma::Feature F2>
void Info<F>::map(const dma::DualInfo<F2> &dmaInfo) const {
    dmaInfo.setRxRequest1(drq + 0); // rx
    dmaInfo.setTxRequest2(drq + 1); // tx
}


using FullInfo = Info<Feature::BAUD_RATE | Feature::MODBUS | Feature::ALL_MODES>;
using BasicInfo = Info<Feature::BAUD_RATE | Feature::MODBUS | Feature::ASYNC_MODE | Feature::LIN_MODE>;
using LpInfo = Info<Feature::LP_BAUD_RATE | Feature::ASYNC_MODE>;

static const FullInfo USART1_INFO{USART1, {&RCC->APB2ENR, RCC_APB2ENR_USART1EN}, USART1_IRQn, 21};
static const FullInfo USART2_INFO{USART2, {&RCC->APB1LENR, RCC_APB1LENR_USART2EN}, USART2_IRQn, 23};
static const FullInfo USART3_INFO{USART3, {&RCC->APB1LENR, RCC_APB1LENR_USART3EN}, USART3_IRQn, 25};
#ifdef UART4
static const BasicInfo UART4_INFO{UART4, {&RCC->APB1LENR, RCC_APB1LENR_UART4EN}, UART4_IRQn, 27};
#endif
#ifdef UART5
static const BasicInfo UART5_INFO{UART5, {&RCC->APB1LENR, RCC_APB1LENR_UART5EN}, UART5_IRQn, 29};
#endif
#ifdef USART6
static const FullInfo USART6_INFO{USART6, {&RCC->APB1LENR, RCC_APB1LENR_USART6EN}, USART6_IRQn, 31};
#endif
#ifdef UART7
static const BasicInfo UART7_INFO{UART7, {&RCC->APB1LENR, RCC_APB1LENR_UART7EN}, UART7_IRQn, 33};
#endif
#ifdef UART8
static const BasicInfo UART8_INFO{UART8, {&RCC->APB1LENR, RCC_APB1LENR_UART8EN}, UART8_IRQn, 35};
#endif
#ifdef UART9
static const BasicInfo UART9_INFO{UART9, {&RCC->APB1HENR, RCC_APB1HENR_UART9EN}, UART9_IRQn, 37};
#endif
#ifdef USART10
static const FullInfo USART10_INFO{USART10, {&RCC->APB1LENR, RCC_APB1LENR_USART10EN}, USART10_IRQn, 39};
#endif
#ifdef USART11
static const FullInfo USART11_INFO{USART11, {&RCC->APB1LENR, RCC_APB1LENR_USART11EN}, USART11_IRQn, 41};
#endif
#ifdef UART12
static const BasicInfo UART12_INFO{UART12, {&RCC->APB1HENR, RCC_APB1HENR_UART12EN}, UART12_IRQn, 43};
#endif
static const LpInfo LPUART1_INFO{LPUART1, {&RCC->APB3ENR, RCC_APB3ENR_LPUART1EN}, LPUART1_IRQn, 45};

} // namespace uart
} // namespace coco
