#pragma once

#include <coco/platform/compiler.hpp>
#include <bit>
#include <cstdint>


namespace coco {

/// @brief Packed data type with optional endianness.
/// Useful for constructing structures to be serialized.
/// @tparam T Base type (e.g. uint32_t)
/// @tparam E Endianness
COCO_PACK_BEGIN
template <typename T, std::endian E = std::endian::native>
struct PackedValue {
    // use make_unsigned to make it work for enum types
    using value_type = std::make_unsigned_t<T>;
    value_type value;

    PackedValue() = default;

    constexpr PackedValue(T v)
        : value(E == std::endian::native ? value_type(v) : std::byteswap(value_type(v)))
    {
    }

    PackedValue(const PackedValue &) = default;
    PackedValue(PackedValue &&) = default;

    PackedValue &operator =(T v) {
        value = E == std::endian::native ? value_type(v) : std::byteswap(value_type(v));
        return *this;
    }

    PackedValue& operator=(const PackedValue &) = default;
    PackedValue& operator=(PackedValue &&) = default;

    operator T () const {
        return T(E == std::endian::native ? value : std::byteswap(value));
    }
} COCO_PACK_END

// only equality can be optimized, less/greater have to be converted to native byte order via operator T ()
template <typename T, std::endian E>
bool operator ==(PackedValue<T, E> a, PackedValue<T, E> b) {
    return a.value == b.value;
}

/// @brief Packed data type with optional endianness for non power of two sizes.
/// @tparam T Base type (e.g. uint32_t)
/// @tparam T1 First component type (e.g. uint16_t for 24 bit value)
/// @tparam T2 Second component type (e.g. uint8_t for 24 bit value)
/// @tparam E Endianness
template <typename T, typename T1, typename T2, std::endian E = std::endian::native>
struct PackedValue2 {
    PackedValue<T1, E> v1;
    PackedValue<T2, E> v2;

    PackedValue2() = default;

    constexpr PackedValue2(T v)
        : v1(E == std::endian::little ? v : v >> sizeof(T2) * 8)
        , v2(E == std::endian::little ? v >> sizeof(T1) * 8 : v)
    {
    }

    PackedValue2(const PackedValue2 &) = default;
    PackedValue2(PackedValue2 &&) = default;

    PackedValue2 &operator =(T v) {
        v1 = (E == std::endian::little ? v : v >> sizeof(T2) * 8);
        v2 = (E == std::endian::little ? v >> sizeof(T1) * 8 : v);
        return *this;
    }

    PackedValue2& operator=(const PackedValue2 &) = default;
    PackedValue2& operator=(PackedValue2 &&) = default;

    operator T () const {
        //return (E == std::endian::little ? T(v1) : T(v1) << sizeof(T2) * 8)
        //    | (E == std::endian::little ? T(v2) << sizeof(T1) * 8 : T(v2));

        // with sign extension:
        return (E == std::endian::little ? T(v1) : T(T(v1) << (sizeof(T) - sizeof(T1)) * 8) >> (sizeof(T) - sizeof(T1) - sizeof(T2)) * 8)
            | (E == std::endian::little ? T(T(v2) << (sizeof(T) - sizeof(T2)) * 8) >> (sizeof(T) - sizeof(T1) - sizeof(T2)) * 8 : T(v2));
    }
};

// only equality can be optimized, less/greater have to be converted to native byte order via operator T ()
template <typename T, typename T1, typename T2, std::endian E>
bool operator ==(PackedValue2<T, T1, T2, E> a, PackedValue2<T, T1, T2, E> b) {
    return a.v1.value == b.v1.value && a.v2.value == b.v2.value;
}

// packed unsigned ingeger
using U8 = uint8_t;
using U16 = PackedValue<uint16_t>;
using U16L = PackedValue<uint16_t, std::endian::little>;
using U16B = PackedValue<uint16_t, std::endian::big>;
using U24 = PackedValue2<uint32_t, uint16_t, uint8_t>;
using U24L = PackedValue2<uint32_t, uint16_t, uint8_t, std::endian::little>;
using U24B = PackedValue2<uint32_t, uint16_t, uint8_t, std::endian::big>;
using U24o = PackedValue2<uint32_t, uint8_t, uint16_t>; // odd alignment, e.g. struct {U8 u8, U24o u24;}
using U24Lo = PackedValue2<uint32_t, uint8_t, uint16_t, std::endian::little>; // odd alignment
using U24Bo = PackedValue2<uint32_t, uint8_t, uint16_t, std::endian::big>; // odd alignment
using U32 = PackedValue<uint32_t>;
using U32L = PackedValue<uint32_t, std::endian::little>;
using U32B = PackedValue<uint32_t, std::endian::big>;
using U48 = PackedValue2<uint64_t, uint32_t, uint16_t>;
using U48L = PackedValue2<uint64_t, uint32_t, uint16_t, std::endian::little>;
using U48B = PackedValue2<uint64_t, uint32_t, uint16_t, std::endian::big>;
using U64 = PackedValue<uint64_t>;
using U64L = PackedValue<uint64_t, std::endian::little>;
using U64B = PackedValue<uint64_t, std::endian::big>;

// packed signed ingeger
using I8 = int8_t;
using I16 = PackedValue<int16_t>;
using I16L = PackedValue<int16_t, std::endian::little>;
using I16B = PackedValue<int16_t, std::endian::big>;
using I24 = PackedValue2<int32_t, uint16_t, uint8_t>;
using I24L = PackedValue2<int32_t, uint16_t, uint8_t, std::endian::little>;
using I24B = PackedValue2<int32_t, uint16_t, uint8_t, std::endian::big>;
using I24o = PackedValue2<int32_t, uint8_t, uint16_t>; // odd alignment, e.g. struct {U8 u8, I24o i24;}
using I24Lo = PackedValue2<int32_t, uint8_t, uint16_t, std::endian::little>; // odd alignment
using I24Bo = PackedValue2<int32_t, uint8_t, uint16_t, std::endian::big>; // odd alignment
using I32 = PackedValue<int32_t>;
using I32L = PackedValue<int32_t, std::endian::little>;
using I32B = PackedValue<int32_t, std::endian::big>;
using I48 = PackedValue2<int64_t, uint32_t, uint16_t>;
using I48L = PackedValue2<int64_t, uint32_t, uint16_t, std::endian::little>;
using I48B = PackedValue2<int64_t, uint32_t, uint16_t, std::endian::big>;
using I64 = PackedValue<int64_t>;
using I64L = PackedValue<int64_t, std::endian::little>;
using I64B = PackedValue<int64_t, std::endian::big>;

// packed enum
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
