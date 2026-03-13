#pragma once

#include <ranges>
#include <concepts>


namespace coco {

template<class R>
concept RangeConcept = std::ranges::range<R>;

/// @brief Byte range concept, any range of elements with size 1 byte, e.g. std::vector<char>, std::string_view, coco::String etc.
template<class R>
concept ByteRangeConcept = std::ranges::range<R> &&
    requires(R& r) {
        requires sizeof(std::ranges::range_reference_t<R>) == 1;
    };



} // namespace coco
