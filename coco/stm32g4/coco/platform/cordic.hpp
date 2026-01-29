#pragma once

#include "platform.hpp"
#include "dma.hpp"
#include "cordicG4H5U5.hpp"


namespace coco {
namespace cordic {

// interrupt request index
constexpr int irq = CORDIC_IRQn;

// First DMA request index
constexpr int drq = 112;


/// @brief Initalize the CORDIC.
/// @return Instance (wrapper for registers)
inline Instance enableClock() {
    // enable clock
    RCC->AHB1ENR = RCC->AHB1ENR | RCC_AHB1ENR_CORDICEN;

    // Two cycles wait time until peripherals can be accessed, see STM32G4 reference manual section 7.2.17
    __NOP();
    __NOP();

    return {CORDIC};
}

/// @brief Get the instance
/// @return Instance (wrapper for registers)
inline Instance instance() {
    return {CORDIC};
}

/// @brief Map a DMA channel to the read channel of the CORDIC.
/// @param dmaInfo DMA info
template <dma::Feature F2>
void mapRead(const dma::Info<F2> &dmaInfo) {
    dmaInfo.setRequest(drq + 0);
}

/// @brief Map a DMA channel to the write channel of the CORDIC.
/// @param dmaInfo DMA info
template <dma::Feature F2>
void mapWrite(const dma::Info<F2> &dmaInfo) {
    dmaInfo.setRequest(drq + 1);
}

/// @brief Map DMA channels to the read and write channels of the CORDIC.
/// @param dmaInfo Dual channel DMA info
template <dma::Feature F2>
void map(const dma::DualInfo<F2> &dmaInfo) {
    dmaInfo.setRequest1(drq + 0); // read
    dmaInfo.setRequest2(drq + 1); // write
}

} // namespace cordic
} // namespace coco
