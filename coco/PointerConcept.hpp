#pragma once

#include <type_traits>


namespace coco {

/// @brief Pointer concept.
/// Usage:
/// template <typename T> requires (PointerConcept<T>)
/// void foo(T ptr) {...}
template <typename T>
concept PointerConcept = std::is_pointer_v<T>;

} // namespace coco
