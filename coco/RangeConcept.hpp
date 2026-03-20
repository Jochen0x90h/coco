#pragma once

#include <ranges>
#include <concepts>


namespace coco {

/// @brief Range concept, any range of elements with size 1 byte, e.g. std::vector<char>, std::string_view, coco::String etc.
/// tparam T Range type
template<class T>
concept RangeConcept = std::ranges::range<T>;

/// @brief Byte range concept, any range of elements with size 1 byte, e.g. std::vector<char>, std::string_view, coco::String etc.
/// tparam T Range type
template <class T>
concept ByteRangeConcept = std::ranges::range<T> &&
    requires {
        requires sizeof(std::ranges::range_reference_t<T>) == 1;
    };



} // namespace coco
