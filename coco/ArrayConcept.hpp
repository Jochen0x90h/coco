#pragma once

#include <iterator>


namespace coco {

/**
 * Boolean that indicates if a variable is a C array, optionally of a given element type.
 * Similar to std::is_array_v
 */
template <typename A, typename E = void>
inline constexpr bool IsCArray = false;

template <typename A, size_t N>
inline constexpr bool IsCArray<A[N], void> = true;

template <typename A, typename E, size_t N>
inline constexpr bool IsCArray<A[N], E> = std::is_same_v<A, E>;


/**
 * Boolean that indicates if a variable is a std-compatible array, optionally of a given element type.
 * Similar to std::is_array_v
 */
template <typename A, typename E>
inline constexpr bool IsStdArray =
    requires(A array) {
        {std::data(array)} -> std::same_as<const E *>;
        {std::size(array)};
    }
    || requires(A array) {
        {std::data(array)} -> std::same_as<E *>;
        {std::size(array)};
    };

template <typename A>
inline constexpr bool IsStdArray<A, void> = requires(A array) {
    {std::data(array)};
    {std::size(array)};
};


/**
 * Array concept, any class that supports std::data() and std::size().
 * Applies to C arrays, coco::Array, coco::ArrayBuffer, coco::String, std::string, std::vector etc.
 *
 * Usage:
 * template <typename T> requires (ArrayConcept<T>) void foo(const T &array) {...}
 * template <typename T> requires (ArrayConcept<T, char>) void foo(const T &charArray) {...}
 */
template <typename A, typename E = void>
concept ArrayConcept = IsCArray<A, E> || IsStdArray<A, E>;

} // namespace coco
