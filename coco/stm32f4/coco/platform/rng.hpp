#pragma once

#include "rngG0F4L4G4H5U3U5.hpp"


#ifdef HAVE_RNG

namespace coco {

/// @brief RNG helpers.
/// Refernece manual:
///   Section 24
namespace rng {

// interrupt request index
constexpr int irq = HASH_RNG_IRQn;


/// @brief Initalize the random number generator.
/// @return Instance (wrapper for registers)
inline Instance enableClock() {
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

#endif // HAVE_RNG
