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

static const FullInfo USART1_INFO{USART1, {&RCC->APBENR2, RCC_APBENR2_USART1EN}, USART1_IRQn, 50};
static const BasicInfo USART2_INFO{USART2, {&RCC->APBENR1, RCC_APBENR1_USART2EN}, USART2_IRQn, 52};

} // namespace uart
} // namespace coco
