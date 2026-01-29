#pragma once

#include <type_traits>
#include <bit>


namespace coco {

/// @brief Get the lowest set bit in a bitfield.
/// @tparam T Type of bitfield (e.g. int)
/// @param bitfield Bitfield to extract from
/// @return Lowest set bit
template <typename T>
constexpr int firstBit(T bitfield) {
    return bitfield & ~(bitfield - 1);
}

/// @brief Exract a value from a bitfield using a mask.
/// @tparam T Type of bitfield (e.g. int)
/// @param bitfield Bitfield to extract from
/// @param mask Mask
/// @return Masked value shifted down
template <typename T>
constexpr auto extract(T bitfield, T mask) {
    auto b = std::make_unsigned_t<T>(bitfield);
    auto m = std::make_unsigned_t<T>(mask);
    return (b & m) / (m & ~(m - 1));
}

/// @brief Calculate parity.
/// https://graphics.stanford.edu/~seander/bithacks.html#ParityParallel
/// @tparam T Type of value
/// @param value Value for which the parity is calculated
/// @return 0 for even parity, 1 for odd parity
template <typename T>
constexpr int parity(T value) {
    auto v = std::make_unsigned_t<T>(value);
    if constexpr (sizeof(T) >= 8)
        v ^= v >> 32;
    if constexpr (sizeof(T) >= 4)
        v ^= v >> 16;
    if constexpr (sizeof(T) >= 2)
        v ^= v >> 8;
    v ^= v >> 4;
    v &= 0xf;
    return (0x6996 >> v) & 1;
}

/// @brief Count number of set bits.
/// Uses std::popcount
template <typename T>
constexpr int popcount(T value) {
    return std::popcount(std::make_unsigned_t<T>(value));
}

/// @brief Count number of set bits before a given bits starting at LSB.
/// @tparam T Type of value
/// @param value Value to count the bits in
/// @param bit Bit up to which the set bits are counted. Must not be zero.
/// @return Number of bits set before given bit
template <typename T, typename T2>
constexpr int popcountBefore(T value, T2 bit) {
    int count = 0;
    for (std::make_unsigned_t<T2> i = 1; i < std::make_unsigned_t<T2>(bit); i <<= 1) {
        if (std::make_unsigned_t<T>(value) & i)
            ++count;
    }
    return count;
}


}
