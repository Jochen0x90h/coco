#pragma once

#include "rccU3U5.hpp"


namespace coco {
namespace rcc {

/// @brief Disable clocks and reset all peripherals except for PWR.
/// Useful for bootloaders before jumping to the application.
__STATIC_FORCEINLINE void resetAll() {
    // disable clocks
    RCC->AHB1ENR1 = 0;
    RCC->AHB2ENR1 = 0;
    RCC->AHB2ENR2 = 0;
    //RCC->AHB1ENR2 = RCC_AHB1ENR2_PWREN;
    RCC->APB1ENR1 = 0;
    RCC->APB1ENR2 = 0;
    RCC->APB2ENR = 0;
    RCC->APB3ENR = 0;

    // reset
    RCC->AHB1RSTR1 = 0xFFFFFFFF;
    RCC->AHB2RSTR1 = 0xFFFFFFFF;
    RCC->AHB2RSTR2 = 0xFFFFFFFF;
    RCC->APB1RSTR1 = 0xFFFFFFFF;
    RCC->APB1RSTR2 = 0xFFFFFFFF;
    RCC->APB2RSTR = 0xFFFFFFFF;
    RCC->APB3RSTR = 0xFFFFFFFF;
    RCC->AHB1RSTR1 = 0;
    RCC->AHB2RSTR1 = 0;
    RCC->AHB2RSTR2 = 0;
    RCC->APB1RSTR1 = 0;
    RCC->APB1RSTR2 = 0;
    RCC->APB2RSTR = 0;
    RCC->APB3RSTR = 0;
}

using AdcInfo = Info2<offsetof(RCC_TypeDef, AHB2ENR1)>;
using DacInfo = Info2<offsetof(RCC_TypeDef, AHB2ENR1)>;
using DmaInfo = Info2<offsetof(RCC_TypeDef, AHB1ENR1)>;
using FdCanInfo = Info2<offsetof(RCC_TypeDef, APB1ENR2)>;
using I2cInfo = Info;
using SpiInfo = Info;
using UsartInfo = Info;

} // namespace rcc
} // namespace coco
