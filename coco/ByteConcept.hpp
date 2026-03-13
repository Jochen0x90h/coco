#pragma once

#include <type_traits>


namespace coco {

template <typename T>
concept ByteConcept = std::is_trivial_v<T> && sizeof (T) == 1;

} // namespace coco
