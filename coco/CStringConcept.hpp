#pragma once

#include <iterator>
#include <cstddef>


namespace coco {

template <typename>
inline constexpr bool IsCStringPointer = false;

template <>
inline constexpr bool IsCStringPointer<char *> = true;

template <>
inline constexpr bool IsCStringPointer<const char *> = true;

template <typename>
inline constexpr bool IsCStringArray = false;

template <size_t N>
inline constexpr bool IsCStringArray<char[N]> = true;

template <size_t N>
inline constexpr bool IsCStringArray<const char[N]> = true;


template <typename T>
concept CStringPointerConcept = IsCStringPointer<T>;

template <typename T>
concept CStringArrayConcept = IsCStringArray<T>;


/// @brief C-string concept, is either (const) char * or C array of (const) char
/// @code
/// template <typename T> requires (CStringConcept<T>) void foo(const T &str) {...}
/// @endcode
template <typename T>
concept CStringConcept = IsCStringPointer<T> || IsCStringArray<T>;


/// @brief Determine the length of a c-string.
/// @tparam T C-string pointer type, e.g. char *, const char *
/// @param str C-string
/// @return Length of the c-string, excluding the null terminator.
template <typename T> requires (CStringPointerConcept<T>)
constexpr int length(const T &str) {
    int l = 0;
	while (str[l] != 0)
		++l;
	return l;
}
/// @brief Determine the length of a C-array of characters.
/// @tparam T C-array of characters, e.g. char[10], const char[10]
/// @param str C-array of characters
/// @return Length of the C-array of characters, excluding the null terminator if present.
template <typename T> requires (CStringArrayConcept<T>)
constexpr int length(const T &str) {
    int l = 0;
    int s = int(std::size(str));
	while (l < s && str[l] != 0)
		++l;
	return l;
}

/**
	Char array concept, any class that supports std::data() and std::size().
	Applies to C arrays of char, coco::ArrayBuffer<char>, coco::String, std::string, std::vector<char> etc.
*/
/*template <typename T>
concept CharArrayConcept = IsCStringArray<T>
	|| requires(T t) {
		{std::data(t)} -> std::same_as<const char *>;
		{std::size(t)};
	}
	|| requires(T t) {
		{std::data(t)} -> std::same_as<char *>;
		{std::size(t)};
	};
*/

} // namespace coco
