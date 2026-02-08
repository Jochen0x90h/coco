#pragma once

// do not include directly, use #include <coco/platform/backup.hpp>

#include "platform.hpp"
#include <cstddef>


/*
    Defines:
    HAVE_BACKUP              Backup registers are supported (are always supported)
*/

#define HAVE_BACKUP

namespace coco {

/// @brief Backup helpers.
/// G4 https://www.st.com/en/microcontrollers-microprocessors/stm32g4-series/documentation.html Section 38
/// U3 https://www.st.com/en/microcontrollers-microprocessors/stm32u3-series/documentation.html Section 44
namespace backup {

/// @brief Value type that can be stored.
///
using Value = uint32_t;

/// @brief Number of backup registers.
///
constexpr int COUNT = (sizeof(TAMP_TypeDef) - offsetof(TAMP_TypeDef, BKP0R)) / 4;


/// @brief Unlock backup registers.
///
__STATIC_FORCEINLINE void unlock() {
    // enable RTC clock
#ifdef RCC_APB1ENR1_RTCAPBEN
    RCC->APB1ENR1 |= RCC_APB1ENR1_RTCAPBEN;
#endif
#ifdef RCC_APB3ENR_RTCAPBEN
    RCC->APB3ENR |= RCC_APB3ENR_RTCAPBEN;
#endif

    // unlock backup domain
#ifdef PWR_CR1_DBP
    PWR->CR1 = PWR->CR1 | PWR_CR1_DBP;
#endif
#ifdef PWR_DBPCR_DBP
    PWR->DBPCR = PWR->DBPCR | PWR_DBPCR_DBP;
#endif
#ifdef PWR_DBPR_DBP
    PWR->DBPR = PWR->DBPR | PWR_DBPR_DBP;
#endif
}

/// @brief Lock backup registers.
///
__STATIC_FORCEINLINE void lock() {
    // lock backup domain
#ifdef PWR_CR1_DBP
    PWR->CR1 = PWR->CR1 & ~PWR_CR1_DBP;
#endif
#ifdef PWR_DBPCR_DBP
    PWR->DBPCR = PWR->DBPCR & ~PWR_DBPCR_DBP;
#endif
#ifdef PWR_DBPR_DBP
    PWR->DBPR = PWR->DBPR & ~PWR_DBPR_DBP;
#endif
}

/// @brief Set a backup register.
/// @param index Index of register
/// @param value Value to set
__STATIC_FORCEINLINE void set(int index, Value value) {
    // set value
    (&TAMP->BKP0R)[index] = value;
}

/// @brief Get a backup register.
/// @param index Index of register
/// @return Value of register
__STATIC_FORCEINLINE Value get(int index) {
    return (&TAMP->BKP0R)[index];
}

} // namespace backup
} // namespace coco
