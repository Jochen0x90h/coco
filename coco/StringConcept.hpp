#pragma once

#include "CStringConcept.hpp"


namespace coco {

/// @brief String concept.
/// C-string (char *, const char *), C-array of char or any class that is an array of char and supports
/// std::data() and std::size()
/// Usage:
/// template <typename T> requires (CtringConcept<T>)
/// void foo(const T &str) {...}
template <typename T>
concept StringConcept = IsCStringPointer<T> || IsCStringArray<T>
    || requires(T t) {
        {std::data(t)} -> std::same_as<const char *>;
        {std::size(t)};
    }
    || requires(T t) {
        {std::data(t)} -> std::same_as<char *>;
        {std::size(t)};
    };

} // namespace coco
