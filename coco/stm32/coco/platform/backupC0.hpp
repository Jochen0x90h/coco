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
namespace backup {

/// @brief Value type that can be stored.
///
using Value = uint16_t;

/// @brief Number of backup registers.
///
constexpr int COUNT = 4;


/// @brief Unlock backup registers.
///
__STATIC_FORCEINLINE void unlock() {
}

/// @brief Lock backup registers.
///
__STATIC_FORCEINLINE void lock() {
}

/// @brief Set a backup register.
/// @param index Index of register
/// @param value Value to set
__STATIC_FORCEINLINE void set(int index, Value value) {
    // set value
    (&PWR->BKP0R)[index] = value;
}

/// @brief Get a backup register.
/// @param index Index of register
/// @return Value of register
__STATIC_FORCEINLINE Value get(int index) {
    return (&PWR->BKP0R)[index];
}

} // namespace backup
} // namespace coco
