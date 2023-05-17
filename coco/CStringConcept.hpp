#pragma once

//#include "Array.hpp"
#include <iterator>
#include <cstddef>


namespace coco {

template <typename>
inline constexpr bool IsCStringPointerValue = false;

template <>
inline constexpr bool IsCStringPointerValue<char *> = true;

template <>
inline constexpr bool IsCStringPointerValue<const char *> = true;

template <typename>
inline constexpr bool IsCStringArrayValue = false;

template <size_t N>
inline constexpr bool IsCStringArrayValue<char[N]> = true;

template <size_t N>
inline constexpr bool IsCStringArrayValue<const char[N]> = true;


template <typename T>
concept CStringPointerConcept = IsCStringPointerValue<T>;

template <typename T>
concept CStringArrayConcept = IsCStringArrayValue<T>;


/**
 * C-string concept
 * Usage:
 * template <typename T> requires (CStringConcept<T>)
 * void foo(const T &str) {...}
 */
template <typename T>
concept CStringConcept = IsCStringPointerValue<T> || IsCStringArrayValue<T>;


/**
 * Determine the length of a c-string
 */
template <typename T> requires (CStringPointerConcept<T>)
constexpr int length(const T &str) {
    int l = 0;
	while (str[l] != 0)
		++l;
	return l;
}

template <typename T> requires (CStringArrayConcept<T>)
constexpr int length(const T &str) {
    int l = 0;
    int s = int(std::size(str));
	while (l < s && str[l] != 0)
		++l;
	return l;
}

} // namespace coco
