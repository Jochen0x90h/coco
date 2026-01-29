#pragma once

#include "crcAll.hpp"


namespace coco {
namespace crc{

/// @brief Initalize the CRC.
/// @return Instance (wrapper for registers)
inline Instance enableClock() {
    RCC->AHB1ENR = RCC->AHB1ENR | RCC_AHB1ENR_CRCEN; // enable clock

    // Two cycles wait time until peripherals can be accessed, see STM32G4 reference manual section 7.2.17
    __NOP();
    __NOP();

    return {CRC};
}

/// @brief Get the instance
/// @return Instance (wrapper for registers)
inline Instance instance() {
    return {CRC};
}

} // namespace crc
} // namespace coco
