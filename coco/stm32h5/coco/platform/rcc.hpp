#pragma once

#include "rccH5.hpp"


namespace coco {
namespace rcc {

/// @brief Disable clocks and reset all peripherals except for PWR.
/// Useful for bootloaders before jumping to the application.
__STATIC_FORCEINLINE void resetAll() {
    // disable clocks
    RCC->AHB1ENR = 0;
    RCC->AHB2ENR = 0;
#ifdef RCC_AHB4ENR_SDMMC1EN
    RCC->AHB4ENR = 0;
#endif
    RCC->APB1LENR = 0;
    RCC->APB1HENR = 0;
    RCC->APB2ENR  = 0;
    RCC->APB3ENR  = 0;

    // reset
    RCC->AHB1RSTR = 0xFFFFFFFF;
    RCC->AHB2RSTR = 0xFFFFFFFF;
#ifdef RCC_AHB4ENR_SDMMC1EN
    RCC->AHB4RSTR  = 0xFFFFFFFF;
#endif
    RCC->APB1LRSTR = 0xFFFFFFFF;
    RCC->APB1HRSTR = 0xFFFFFFFF;
    RCC->APB2RSTR  = 0xFFFFFFFF;
    RCC->APB3RSTR  = 0xFFFFFFFF;
    RCC->AHB1RSTR = 0;
    RCC->AHB2RSTR = 0;
#ifdef RCC_AHB4ENR_SDMMC1EN
    RCC->AHB4RSTR  = 0;
#endif
    RCC->APB1LRSTR = 0;
    RCC->APB1HRSTR = 0;
    RCC->APB2RSTR  = 0;
    RCC->APB3RSTR  = 0;
}

using AdcInfo = Info2<offsetof(RCC_TypeDef, AHB2ENR)>;
using DacInfo = Info2<offsetof(RCC_TypeDef, AHB2ENR)>;
using DmaInfo = Info2<offsetof(RCC_TypeDef, AHB1ENR)>;
using FdCanInfo = Info2<offsetof(RCC_TypeDef, APB1HENR)>;
using I2cInfo = Info;
using SpiInfo = Info;
using UsartInfo = Info;

} // namespace rcc
} // namespace coco
