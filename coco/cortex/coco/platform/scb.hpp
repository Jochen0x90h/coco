#pragma once

#include <coco/platform/platform.hpp>


namespace coco {

/// @brief SCB (System Control Block)
/// See SCB_Type in system/core_cm0.h or system/core_cm4.h
namespace scb {

/// @brief System reset
///
__NO_RETURN __STATIC_FORCEINLINE void reset() {
    NVIC_SystemReset();
}

enum class Priority : uint8_t {
    // low priority
    LOW = 192,

    // medium priority
    MEDIUM = 128,

    // high priority
    HIGH = 64,

    // maximum priority
    MAX = 0,
};

/// @brief Get system handler priority
/// @param n System handler number (MemoryManagement_IRQn = -12 to SysTick_IRQn = -1)
/// @return Priority
__STATIC_FORCEINLINE Priority getPriority(int n) {
#ifdef _SHP_IDX
    return Priority(SCB->SHPR[_SHP_IDX(n)] >> _BIT_SHIFT(n));
#else
    return Priority(SCB->SHPR[n + 12]);
#endif
}

/// @brief Set system handler priority
/// @param n System handler number (MemoryManagement_IRQn = -12 to SysTick_IRQn = -1)
/// @param priority Priority
__STATIC_FORCEINLINE void setPriority(int n, Priority priority) {
#ifdef _SHP_IDX
    auto &SHP = SCB->SHPR[_SHP_IDX(n)];
    SHP = (SHP & ~(0xff << _BIT_SHIFT(n))) | (int(priority) << _BIT_SHIFT(n));
#else
    SCB->SHPR[n + 12] = uint8_t(priority);
#endif
}

} // namespace scb
} // namespace coco
