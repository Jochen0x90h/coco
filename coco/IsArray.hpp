#pragma once


namespace coco {

// helper classes for distinguishing between fixed size array and c-string
struct False {};
struct True {};

/**
 * Check if a type is a C-style array, e.g. IsArray<Foo>::value
 * @tparam T type to check if it is an array
 */
template <typename T>
struct IsArray : False {
	static constexpr bool value = false;
};
template <typename T, int N>
struct IsArray<T[N]> : True {
	static constexpr bool value = true;
};

} // namespace coco
