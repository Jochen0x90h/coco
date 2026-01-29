#pragma once

// do not include directly, use #include <coco/platform/cache.hpp>

#include "platform.hpp"


namespace coco {

/// @brief Cache helpers.
///
namespace cache {

/// @brief Invalidate the instruction and data cache.
///
__STATIC_FORCEINLINE void flush() {
    // G4: https://github.com/zephyrproject-rtos/zephyr/pull/32218/files#diff-20dbbd4b4adb0bda3e1b8f32c0e8db33501947942184928d542446d260a2eaae

    // get ACR
    uint32_t acr = FLASH->ACR;

    // disable caches
#ifdef FLASH_ACR_DCEN
    uint32_t disabled = acr & ~(FLASH_ACR_ICEN | FLASH_ACR_DCEN);
#else
    uint32_t disabled = acr & ~FLASH_ACR_ICEN;
#endif
    FLASH->ACR = disabled;

    // reset caches while disabled
#ifdef FLASH_ACR_DCRST
    FLASH->ACR = disabled | (FLASH_ACR_ICRST | FLASH_ACR_DCRST);
#else
    FLASH->ACR = disabled | FLASH_ACR_ICRST;
#endif

    // restore ACR
    FLASH->ACR = acr;
}

} // namespace cache
} // namespace coco
