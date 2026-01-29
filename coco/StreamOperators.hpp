#pragma once

#include "convert.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"


/// Number formats with stream operators, useful in conjuncton with streams:
/// s << "foo" << dec(50) << hex(0xbeef) << flt(3.14f, 1, 2);
namespace coco {

/*
This is how a stream class should look like:

class Stream {
    // write a single character, this can be a virtual method
    (virtual) void write(char ch);

    // write a string, this can be a virtual method
    (virtual) void write(const String &str);

    // stream a single character using write()
    Stream &operator <<(char ch) {
        write(ch);
        return *this;
    }

    // stream a string using write(), either C-string or coco::String
    template <typename T> requires StringConcept<T>
    Stream &operator <<(T const &str) {
        write(str);
        return *this;
    }
};
*/

namespace detail {
template <typename S, typename T, typename F>
void stream(S &s, const T& value, const F &function) {
    function(s, value);
}

template <typename S, typename T, typename F>
void stream(S &s, const Vector2<T>& value, const F &function) {
    function(s, value.x);
    s << " ";
    function(s, value.y);
}

template <typename S, typename T, typename F>
void stream(S &s, const Vector3<T>& value, const F &function) {
    function(s, value.x);
    s << " ";
    function(s, value.y);
    s << " ";
    function(s, value.z);
}

template <typename S, typename T, typename F>
void stream(S &s, const Vector4<T>& value, const F &function) {
    function(s, value.x);
    s << " ";
    function(s, value.y);
    s << " ";
    function(s, value.z);
    s << " ";
    function(s, value.w);
}
}
/*
// decimal number
template <typename T>
struct Dec {
    T value;
    int digitCount;
};

template <typename S, typename T>
S &operator <<(S &s, Dec<T> dec) {
    //char buffer[16];
    //return s << toString(buffer, int64_t(dec.value), dec.digitCount);
    int digitCount = dec.digitCount;

    // dec.value can be scalar or vector
    detail::stream(s, dec.value, [digitCount](S &s, int64_t value) {
        char buffer[16];
        s << toString(buffer, value, digitCount);
    });
    return s;
}

/// @brief Decimal number for stream operators.
/// Use like s << dec(50) << "foo"
/// @param value decimal value
/// @param digitCount minimum number of digits for the decimal value, adds leading zeros if necessary
/// @return decimal value wrapper
template <typename T>
Dec<T> dec(T value, int digitCount = 1) {
    return {value, digitCount};
}
*/
/*
// hexadecimal number
template <typename T>
struct Hex {
    T value;
    int digitCount;
};

template <typename S, typename T>
S &operator <<(S &s, Hex<T> hex) {
    //char buffer[16];
    //return s << toHexString(buffer, uint64_t(hex.value), hex.digitCount);
    int digitCount = hex.digitCount;
    detail::stream(s, hex.value, [digitCount](S &s, uint64_t value) {
        char buffer[16];
        s << toHexString(buffer, value, digitCount);
    });
    return s;
}

/// @brief Hex number for stream operators.
/// Use like s << hex(0xbeef) << "foo"
/// @param value hex value
/// @param digitCount number of digits for the hex value
/// @return hex value wrapper
template <typename T>
Hex<T> hex(T value, int digitCount = sizeof(T) * 2) {
    return {value, digitCount};
}
*/
/*
// floating point number
template <typename T>
struct Flt {
    T value;

    // number of digits before the decimal point (e.g. 2 for 00.0)
    int digitCount;

    // maximum number of decimals (e.g. 3 for 0.123). Negative value for fixed number of decimals (e.g. -3 for 0.000)
    int decimalCount;
};

template <typename S, typename T>
inline S &operator <<(S &s, Flt<T> flt) {
    //char buffer[21];
    //return s << toString(buffer, flt.value, flt.digitCount, flt.decimalCount);
    int digitCount = flt.digitCount;
    int decimalCount = flt.decimalCount;
    detail::stream(s, flt.value, [digitCount, decimalCount](S &s, float value) {
        char buffer[21];
        s << toString(buffer, value, digitCount, decimalCount);
    });
    return s;
}

/// @brief Create a float value wrapper with parameters for printing.
/// @param value float value
/// @param decimalCount minimum number of digits to convert after the decimal point, negative to keep trailing zeros
/// @return float value wrapper
template <typename T>
constexpr Flt<T> flt(T value, int decimalCount = 3) {
    return {value, 1, decimalCount};
}

/// @brief Create a float value wrapper with parameters for printing.
/// @param value value
/// @param digitCount minimum number of digits to convert before the decimal point
/// @param decimalCount minimum number of digits to convert after the decimal point, negative to keep trailing zeros
/// @return float value wrapper
template <typename T>
constexpr Flt<T> flt(T value, int digitCount, int decimalCount) {
    return {value, digitCount, decimalCount};
}
*/

} // namespace coco
