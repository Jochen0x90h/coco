#pragma once

#include "rngG0F4L4G4H5U3U5.hpp"


namespace coco {

/// @brief RNG helpers.
/// L46 Refernece manual:
///   Section 24
/// L47 Refernece manual:
///   Section 27
namespace rng {

// interrupt request index
constexpr int irq = RNG_IRQn;


/// @brief Initalize the random number generator.
/// When HSI48 is selected as RNG clock source in the RCC->CCIPR register, HSI48 gets enabled.
/// @return Instance (wrapper for registers)
inline Instance enableClock() {
    if ((RCC->CCIPR & RCC_CCIPR_CLK48SEL_Msk) == 0) {
        // enable HSI48 and wait until ready
        RCC->CRRCR = RCC_CRRCR_HSI48ON;
        while ((RCC->CRRCR & RCC_CRRCR_HSI48RDY) == 0);
    }

    // enable clock
    RCC->AHB2ENR = RCC->AHB2ENR | RCC_AHB2ENR_RNGEN;
    __NOP();
    __NOP();

    RNG->SR = RNG_SR_CEIS | RNG_SR_SEIS;

    return {RNG};
}

/// @brief Get the instance
/// @return Instance (wrapper for registers)
inline Instance instance() {
    return {RNG};
}

} // namespace rng
} // namespace coco
