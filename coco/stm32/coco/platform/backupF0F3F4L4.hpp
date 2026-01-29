#pragma once

// do not include directly, use #include <coco/platform/backup.hpp>

#include "platform.hpp"
#include <cstddef>


namespace coco {
namespace backup {

/// @brief Value type that can be stored.
///
using Value = uint32_t;

/// @brief Number of backup registers.
///
constexpr int COUNT = RTC_BKP_NUMBER;


/// @brief Unlock backup registers.
///
__STATIC_FORCEINLINE void unlock() {
    // enable RTC clock
#ifdef RCC_BDCR_RTCEN
    RCC->BDCR |= RCC_BDCR_RTCEN;
#endif

    // unlock backup domain
#ifdef PWR_CR_DBP
    PWR->CR = PWR->CR | PWR_CR_DBP;
#endif
#ifdef PWR_CR1_DBP
    PWR->CR1 = PWR->CR1 | PWR_CR1_DBP;
#endif
}

/// @brief Lock backup registers.
///
__STATIC_FORCEINLINE void lock() {
    // lock backup domain
#ifdef PWR_CR_DBP
    PWR->CR = PWR->CR & ~PWR_CR_DBP;
#endif
#ifdef PWR_CR1_DBP
    PWR->CR1 = PWR->CR1 & ~PWR_CR1_DBP;
#endif
}

/// @brief Set a backup register.
/// @param index Index of register
/// @param value Value to set
__STATIC_FORCEINLINE void set(int index, Value value) {
    // set value
    (&RTC->BKP0R)[index] = value;
}

/// @brief Get a backup register.
/// @param index Index of register
/// @return Value of register
__STATIC_FORCEINLINE Value get(int index) {
    return (&RTC->BKP0R)[index];
}

} // namespace backup
} // namespace coco
