#pragma once

#include "rccL4G4.hpp"


namespace coco {
namespace rcc {

/// @brief Disable clocks and reset all peripherals except for PWR.
/// Useful for bootloaders before jumping to the application.
__STATIC_FORCEINLINE void resetAll() {
    // disable clocks
    RCC->AHB1ENR = 0;
    RCC->AHB2ENR = 0;
    RCC->AHB3ENR = 0;
    RCC->APB1ENR1 = RCC_APB1ENR1_PWREN;
    RCC->APB1ENR2 = 0;
    RCC->APB2ENR = 0;

    // reset
    RCC->AHB1RSTR = 0xFFFFFFFF;
    RCC->AHB2RSTR = 0xFFFFFFFF;
    RCC->AHB3RSTR = 0xFFFFFFFF;
    RCC->APB1RSTR1 = ~RCC_APB1RSTR1_PWRRST;
    RCC->APB1RSTR2 = 0xFFFFFFFF;
    RCC->APB2RSTR = 0xFFFFFFFF;
    RCC->AHB1RSTR = 0;
    RCC->AHB2RSTR = 0;
    RCC->AHB3RSTR = 0;
    RCC->APB1RSTR1 = 0;
    RCC->APB1RSTR2 = 0;
    RCC->APB2RSTR = 0;
}

using AdcInfo = Info2<offsetof(RCC_TypeDef, AHB2ENR)>;
using DacInfo = Info2<offsetof(RCC_TypeDef, AHB2ENR)>;
using FdCanInfo = Info2<offsetof(RCC_TypeDef, APB1ENR1)>;
using I2cInfo = Info;
using QuadSpiInfo = Info2<offsetof(RCC_TypeDef, AHB3ENR)>;
using SpiInfo = Info;
using UsartInfo = Info;

} // namespace rcc
} // namespace coco
