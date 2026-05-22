#pragma once

#include <type_traits>


namespace coco {

/// @brief Concept for trivially copyable values, e.g. int or float
///
template <typename T>
concept TriviallyCopyable = std::is_trivially_copyable_v<T>;

} // namespace coco
