#pragma once

#include "rngG0F4L4G4H5U3U5.hpp"


#ifdef HAVE_RNG

namespace coco {
namespace rng {

// interrupt request index
constexpr int irq = AES_RNG_IRQn;


/// @brief Initalize the random number generator.
/// Make sure a clock is selected in RCC->RCC_CCIPR that does not exceed 48MHz.
/// @return Instance (wrapper for registers)
inline Instance enableClock() {
    // enable clock
    RCC->AHBENR = RCC->AHBENR | RCC_AHBENR_RNGEN;
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

#endif // HAVE_RNG
