#pragma once

#include <type_traits>


namespace coco {

template <typename T>
concept TriviallyCopyable = std::is_trivially_copyable_v<T>;

} // namespace coco
