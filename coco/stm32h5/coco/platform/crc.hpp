#pragma once

#include "crcAll.hpp"


namespace coco {

/// @brief CRC helpers
/// H503 Reference manual: https://www.st.com/resource/en/reference_manual/rm0492-stm32h503-line-armbased-32bit-mcus-stmicroelectronics.pdf
///   Section 18
/// Reference manual: https://www.st.com/resource/en/reference_manual/rm0481-stm32h52333xx-stm32h56263xx-and-stm32h573xx-armbased-32bit-mcus-stmicroelectronics.pdf
///   Section 19
namespace crc{

/// @brief Initalize the CRC
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
