#pragma once

#include <coco/platform/compiler.hpp>
#include <bit>
#include <cstdint>


namespace coco {

/// @brief Packed data type with optional endianness.
/// Useful for constructing structures to be serialized.
/// @tparam Base type (e.g. uint32_t)
COCO_PACK_BEGIN
template <typename T, std::endian E = std::endian::native>
struct PackedValue {
    std::make_unsigned_t<T> value;

    PackedValue() = default;

    constexpr PackedValue(T v)
        : value(std::endian::native == E ? std::make_unsigned_t<T>(v) : std::byteswap(std::make_unsigned_t<T>(v)))
    {
    }

    constexpr PackedValue(const PackedValue &v)
        : value(v.value)
    {
    }

    PackedValue &operator =(T v) {
        value = std::endian::native == E ? std::make_unsigned_t<T>(v) : std::byteswap(std::make_unsigned_t<T>(v));
        return *this;
    }

    operator T () const {
        return T(std::endian::native == E ? value : std::byteswap(value));
    }
} COCO_PACK_END

using U8 = uint8_t;
using U16 = PackedValue<uint16_t>;
using U16L = PackedValue<uint16_t, std::endian::little>;
using U16B = PackedValue<uint16_t, std::endian::big>;
using U32 = PackedValue<uint32_t>;
using U32L = PackedValue<uint32_t, std::endian::little>;
using U32B = PackedValue<uint32_t, std::endian::big>;
using U64 = PackedValue<uint64_t>;
using U64L = PackedValue<uint64_t, std::endian::little>;
using U64B = PackedValue<uint64_t, std::endian::big>;

// enums
template <typename T> requires (sizeof(T) == 1)
using E8 = T;
template <typename T> requires (sizeof(T) == 2)
using E16 = PackedValue<T>;
template <typename T> requires (sizeof(T) == 2)
using E16L = PackedValue<T, std::endian::little>;
template <typename T> requires (sizeof(T) == 2)
using E16B = PackedValue<T, std::endian::big>;
template <typename T> requires (sizeof(T) == 4)
using E32 = PackedValue<T>;
template <typename T> requires (sizeof(T) == 4)
using E32L = PackedValue<T, std::endian::little>;
template <typename T> requires (sizeof(T) == 4)
using E32B = PackedValue<T, std::endian::big>;

} // namespace coco
