#pragma once

#include "crcAll.hpp"


namespace coco {

/// @brief CRC helpers
/// F37 Refernece manual: https://www.st.com/resource/en/reference_manual/rm0313-stm32f37xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
///   Section 5
namespace crc{

/// @brief Initalize the CRC
/// @return Instance (wrapper for registers)
inline Instance enableClock() {
    RCC->AHBENR = RCC->AHBENR | RCC_AHBENR_CRCEN; // enable clock

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
