#pragma once

#include "String.hpp"


namespace coco {

template <typename>
inline constexpr bool IsCocoStringValue = false;

template <>
inline constexpr bool IsCocoStringValue<String> = true;


/**
 * String concept
 * Usage:
 * template <typename T> requires (CtringConcept<T>)
 * void foo(const T &str) {...}
 */
template <typename T>
concept StringConcept = IsCStringPointerValue<T> || IsCStringArrayValue<T> || IsCocoStringValue<T>;

} // namespace coco
