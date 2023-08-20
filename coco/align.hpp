#pragma once


namespace coco {

template <typename T>
constexpr T align2(T x) {return ((x + 1) & ~T(1));}

template <typename T>
constexpr T align4(T x) {return ((x + 3) & ~T(3));}

template <typename T>
constexpr T align8(T x) {return ((x + 7) & ~T(7));}

template <typename T>
constexpr T align16(T x) {return ((x + 15) & ~T(15));}

template <typename T>
constexpr T align32(T x) {return ((x + 31) & ~T(31));}

template <typename T>
constexpr T align(T x, int n) {return ((x + (n - 1)) & ~T(n - 1));}

} // namespace coco
