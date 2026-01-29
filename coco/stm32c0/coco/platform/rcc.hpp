#pragma once

#include "rccC0G0.hpp"


namespace coco {
namespace rcc {

/// @brief Disable clocks and reset all peripherals except for PWR.
/// Useful for bootloaders before jumping to the application.
__STATIC_FORCEINLINE void resetAll() {
    // reset
    RCC->AHBRSTR = 0xFFFFFFFF;
    RCC->APBRSTR1 = ~RCC_APBRSTR1_PWRRST; // do not reset PWR
    RCC->APBRSTR2 = 0xFFFFFFFF;
    RCC->AHBRSTR = 0;
    RCC->APBRSTR1 = 0;
    RCC->APBRSTR2 = 0;

    // disable clocks
    RCC->AHBENR = 0;
    RCC->APBENR1 = RCC_APBENR1_PWREN; // only PWR clock enabled
    RCC->APBENR2 = 0;
}

using AdcInfo = Info2<offsetof(RCC_TypeDef, APBENR2)>;
using I2cInfo = Info2<offsetof(RCC_TypeDef, APBENR1)>;
using SpiInfo = Info2<offsetof(RCC_TypeDef, APBENR2)>;
using UsartInfo = Info;

} // namespace rcc
} // namespace coco
