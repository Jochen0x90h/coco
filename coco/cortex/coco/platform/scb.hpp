#pragma once

#include "platform.hpp" // platform.hpp is next to this file in the conan package


namespace coco {

/**
 * SCB (System Control Block)
 * See SCB_Type in system/core_cm0.h or system/core_cm4.h
 */
namespace scb {

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

/**
 * Get system handler priority
 */
inline Priority getPriority(int n) {
#ifdef _SHP_IDX
	return Priority(SCB->SHP[_SHP_IDX(n)] >> _BIT_SHIFT(n));
#else
	return Priority(SCB->SHP[n + 12]);
#endif
}

/**
 * Set system handler priority
 */
inline void setPriority(int n, Priority priority) {
#ifdef _SHP_IDX
	auto &SHP = SCB->SHP[_SHP_IDX(n)];
	SHP = (SHP & ~(0xff << _BIT_SHIFT(n))) | (int(priority) << _BIT_SHIFT(n));
#else
	SCB->SHP[n + 12] = uint8_t(priority);
#endif
}

} // namespace scb
} // namespace coco
