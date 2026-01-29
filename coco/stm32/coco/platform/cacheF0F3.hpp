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
    // has no cache
}

} // namespace cache
} // namespace coco
