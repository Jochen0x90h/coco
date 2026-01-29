#pragma once

#include "rccF0F1F3.hpp"


namespace coco {
namespace rcc {

/// @brief Disable clocks and reset all peripherals except for PWR.
/// Useful for bootloaders before jumping to the application.
__STATIC_FORCEINLINE void resetAll() {
    // disable clocks
    RCC->AHBENR = 0;
    RCC->APB1ENR = RCC_APB1ENR_PWREN;
    RCC->APB2ENR = 0;

    // reset
    RCC->AHBRSTR = 0xFFFFFFFF;
    RCC->APB1RSTR = ~RCC_APB1RSTR_PWRRST;
    RCC->APB2RSTR = 0xFFFFFFFF;
    RCC->AHBRSTR = 0;
    RCC->APB1RSTR = 0;
    RCC->APB2RSTR = 0;
}

#ifdef RCC_APB2ENR_ADC1EN
    using AdcInfo = Info2<offsetof(RCC_TypeDef, APB2RSTR), offsetof(RCC_TypeDef, APB2ENR)>; // some MCUs have a single ADC on APB2
#else
    using AdcInfo = Info2<offsetof(RCC_TypeDef, AHBRSTR), offsetof(RCC_TypeDef, AHBENR)>;
#endif
using DacInfo = Info2<offsetof(RCC_TypeDef, APB1RSTR), offsetof(RCC_TypeDef, APB1ENR)>;
using I2cInfo = Info2<offsetof(RCC_TypeDef, APB1RSTR), offsetof(RCC_TypeDef, APB1ENR)>;
using SpiInfo = Info<offsetof(RCC_TypeDef, APB1RSTR) - offsetof(RCC_TypeDef, APB1ENR)>;
using UsartInfo = Info<offsetof(RCC_TypeDef, APB1RSTR) - offsetof(RCC_TypeDef, APB1ENR)>;

} // namespace rcc
} // namespace coco
