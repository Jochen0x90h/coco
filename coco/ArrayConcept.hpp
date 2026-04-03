#pragma once

#include <iterator>


namespace coco {
/*
/// @brief Boolean that indicates if a variable is a C array, optionally of a given element type.
/// @tparam A Array type
/// @tparam E Optional element type
/// @return True if the variable is a C array of the specified element type, false otherwise.
template <typename A, typename E = void>
inline constexpr bool IsCArray = false;

template <typename A, size_t N>
inline constexpr bool IsCArray<A[N], void> = true;

template <typename A, typename E, size_t N>
inline constexpr bool IsCArray<A[N], E> = std::is_same_v<std::remove_cv_t<A>, E>;


/// @brief Boolean that indicates if a variable is a std-compatible array, optionally of a given element type.
/// Similar to std::is_array_v
/// @tparam A Array type
/// @tparam E Element type
/// @return True if the variable is a std-compatible array of the specified element type, false otherwise.
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
inline constexpr bool IsStdArray<A, void> =
    requires(A array) {
        {std::data(array)};
        {std::size(array)};
    };


/// @brief Array concept, any class that supports std::data() and std::size().
/// Applies to C arrays, coco::Array, coco::ArrayBuffer, coco::String, std::string, std::vector etc.
/// @tparam A Array type
/// @tparam E Element type
/// @return True if the variable is an array of the specified type, false otherwise.
/// @code
/// template <typename T> requires (ArrayConcept<T>) void foo(const T &array) {...}
/// template <typename T> requires (ArrayConcept<T, char>) void foo(const T &charArray) {...}
/// @endcode
template <typename A, typename E = void>
concept ArrayConcept = IsCArray<A, E> || IsStdArray<A, E>;


/// @brief Byte array concept.
/// Any array (continuous range) of elements with size 1 byte, e.g. std::vector<char>, std::string_view, coco::String etc.
template<class R>
concept ByteArrayConcept = std::ranges::contiguous_range<R> &&
    requires(R &r) {
        requires sizeof(std::ranges::range_reference_t<R>) == 1;
    };
*/



/// @brief Array concept, any class that supports std::data() and std::size() and is contiguous in memory.
/// Applies to C arrays, coco::Array, coco::ArrayBuffer, coco::String, std::string, std::string_view, std::vector, std::span etc.
/// @tparam T Array type
/// @tparam E Element type
/// @return True if the variable is an array of the specified type, false otherwise.
/// @code
/// template <typename T> requires (ArrayConcept<T>) void foo(const T &array) {...}
/// template <typename T> requires (ArrayConcept<T, char>) void foo(const T &charArray) {...}
/// @endcode
template <typename T, typename E = void>
concept ArrayConcept = std::ranges::contiguous_range<T>
    && std::ranges::sized_range<T>
    && std::ranges::random_access_range<T>
    && (std::same_as<E, void> || std::same_as<std::ranges::range_value_t<T>, E>);

/// @brief Byte Array concept.
/// Any continuous range of elements with size 1 byte, e.g. std::vector<char>, std::string_view, coco::Array, coco::String etc.
template <class T>
concept ByteArrayConcept = std::ranges::contiguous_range<T>
    && std::ranges::sized_range<T>
    && std::ranges::random_access_range<T>
    && requires {
        requires sizeof(std::ranges::range_reference_t<T>) == 1;
    };

/// @brief Concept of type that is constructible like an array from data pointer and size.
/// Applies to coco::Array, std::string, std::string_view, std::span etc.
template <typename T>
concept ArrayConstructible =
    requires(typename std::ranges::range_value_t<T>* ptr, int n) {
        T(ptr, n); // check if constructor exists
    };

} // namespace coco
