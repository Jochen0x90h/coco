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
using BasicInfo = Info<Feature::BAUD_RATE | Feature::ASYNC_MODE | Feature::SYNC_MODE>;
using LpInfo = Info<Feature::LP_BAUD_RATE | Feature::ASYNC_MODE>;

#ifdef LPUART2
constexpr int USART2_IRQn = USART2_LPUART2_IRQn;
#endif
#ifdef USART6
constexpr int USART3_IRQn = USART3_4_5_6_LPUART1_IRQn;
constexpr int USART4_IRQn = USART3_4_5_6_LPUART1_IRQn;
constexpr int LPUART1_IRQn = USART3_4_5_6_LPUART1_IRQn;
#else
constexpr int USART3_IRQn = USART3_4_LPUART1_IRQn;
constexpr int USART4_IRQn = USART3_4_LPUART1_IRQn;
constexpr int LPUART1_IRQn = USART3_4_LPUART1_IRQn;
#endif

static const FullInfo USART1_INFO{USART1, {&RCC->APBENR2, RCC_APBENR2_USART1EN}, USART1_IRQn, 50};
#ifndef USART5
#ifndef USART3
static const BasicInfo USART2_INFO{USART2, {&RCC->APBENR1, RCC_APBENR1_USART2EN}, USART2_IRQn, 52};
#else
static const FullInfo USART2_INFO{USART2, {&RCC->APBENR1, RCC_APBENR1_USART2EN}, USART2_IRQn, 52};
static const BasicInfo USART3_INFO{USART3, {&RCC->APBENR1, RCC_APBENR1_USART3EN}, USART3_IRQn, 54};
static const BasicInfo USART4_INFO{USART4, {&RCC->APBENR1, RCC_APBENR1_USART4EN}, USART4_IRQn, 56};
#endif
#else
static const FullInfo USART2_INFO{USART2, {&RCC->APBENR1, RCC_APBENR1_USART2EN}, USART2_IRQn, 52};
static const FullInfo USART3_INFO{USART3, {&RCC->APBENR1, RCC_APBENR1_USART3EN}, USART3_IRQn, 54};
static const BasicInfo USART4_INFO{USART4, {&RCC->APBENR1, RCC_APBENR1_USART4EN}, USART4_IRQn, 56};
static const BasicInfo USART5_INFO{USART5, {&RCC->APBENR1, RCC_APBENR1_USART5EN}, USART3_4_5_6_LPUART1_IRQn, 74};
static const BasicInfo USART6_INFO{USART6, {&RCC->APBENR1, RCC_APBENR1_USART6EN}, USART3_4_5_6_LPUART1_IRQn, 76};
#endif
#ifdef LPUART1
static const LpInfo LPUART1_INFO{LPUART1, {&RCC->APBENR1, RCC_APBENR1_LPUART1EN}, LPUART1_IRQn, 14};
#endif
#ifdef LPUART2
static const LpInfo LPUART2_INFO{LPUART2, {&RCC->APBENR1, RCC_APBENR1_LPUART2EN}, USART2_LPUART2_IRQn, 64};
#endif

} // namespace uart
} // namespace coco
