#pragma once

#include "Array.hpp"
#include "Buffer.hpp"
#include "String.hpp"


namespace coco {

// similar to std::is_array_v but also takes coco::Array as array
template <typename>
inline constexpr bool IsArrayValue = false;

template <typename T, size_t N>
inline constexpr bool IsArrayValue<T[N]> = true;

template <typename T, int N>
inline constexpr bool IsArrayValue<Array<T, N>> = true;

template <typename T, int N>
inline constexpr bool IsArrayValue<Buffer<T, N>> = true;

template <>
inline constexpr bool IsArrayValue<String> = true;


/**
 * Array concept
 * Usage:
 * template <typename T> requires (ArrayConcept<T>)
 * void foo(T ptr) {...}
 */
template <typename T>
concept ArrayConcept = IsArrayValue<T>;

} // namespace coco
