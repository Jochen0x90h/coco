#pragma once

#include "CStringConcept.hpp"


namespace coco {

/// @brief String concept.
/// C-string (char *, const char *), C-array of char or any class that is an array of char and supports
/// std::data() and std::size() such as coco::String, coco::ArrayBuffer<char>, std::string, std::vector<char>
/// Usage:
/// template <typename T> requires (StringConcept<T>)
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


template <typename S, typename T> requires (StringConcept<T>)
inline S &operator <<(S &s, const T &str) {
    //String str2(str);
    //s.write(str2.data(), str2.size());
    s << String(str);
    return s;
}


} // namespace coco
