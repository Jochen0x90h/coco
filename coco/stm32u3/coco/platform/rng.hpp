#pragma once

#include "rngG0F4L4G4H5U3U5.hpp"


namespace coco {
namespace rng {

// interrupt request index
constexpr int irq = RNG_IRQn;


/// @brief Initalize the random number generator.
/// When HSI48 is selected as RNG clock source in the RCC->CCIPR2 register, HSI48 gets enabled.
/// @return Instance (wrapper for registers)
inline Instance enableClock() {
    if ((RCC->CCIPR2 & RCC_CCIPR2_RNGSEL_Msk) == 0) {
        // enable HSI48 and wait until ready
        RCC->CR = RCC->CR | RCC_CR_HSI48ON;
        while ((RCC->CR & RCC_CR_HSI48RDY) == 0);
    }

    // enable clock
    RCC->AHB2ENR1 = RCC->AHB2ENR1 | RCC_AHB2ENR1_RNGEN;
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
