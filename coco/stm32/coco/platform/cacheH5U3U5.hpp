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
    // invalidate instruction cache
    ICACHE->CR = ICACHE->CR | ICACHE_CR_CACHEINV;

#ifdef DCACHE1
    // invalidate data cache
    DCACHE1->CR = DCACHE1->CR | DCACHE_CR_CACHEINV;
#endif
}

} // namespace cache
} // namespace coco
