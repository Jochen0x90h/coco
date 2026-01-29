#pragma once

#include "Array.hpp"
#include "String.hpp"
#include <atomic>
#include <optional>


namespace coco {

template <typename T>
struct ConvertedValue {
    // converted value
    T value;

    // length of string that was converted
    int length;


    operator bool () const {
        return this->length > 0;
    }

    T operator * () const {
        return this->value;
    }
};

template <int N>
struct ConvertedBuffer {
    // data
    char data[N];

    char *begin;
    int length;


    ConvertedBuffer() = default;

    ConvertedBuffer(const ConvertedBuffer &other)
        : begin(std::begin(this->data) + (other.begin - std::begin(other.data))), length(other.length)
    {
        std::ranges::copy(other.data, std::begin(this->data));
    }

    operator String () const {
        return {this->begin, this->length};
    }
};


namespace detail {
    char *dec(char *end, uint32_t value, int digitCount);
    char *dec(char *end, uint64_t value, int digitCount);
    char *dec(char *end, double value, int digitCount, int decimalCount);

    char *hex(char *end, uint32_t value, int digitCount);
    char *hex(char *end, uint64_t value, int digitCount);

    char *utf8(char *end, uint32_t code);
} // namespace detail

/// @brief Convert a decimal string to an integer.
/// @tparam T Integer type
/// @param str String
/// @return Converted integer and number of characters used
template <typename T>
ConvertedValue<T> dec(String str) {
    int i = 0;

    // check for sign
    bool minus = false;
    if constexpr (std::is_signed_v<T>) {
        // signed
        if (str.size() > 0) {
            minus = str[0] == '-';
            if (minus || str[0] == '+') {
                i = 1;
            }
        }
    }

    // parse integer
    T value = 0;
    for (; i < str.size(); ++i) {
        uint8_t ch = str[i];
        if (ch >= '0' && ch <= '9') {
            value = value * 10 + ch - '0';
        } else {
            // invalid character
            return {};
        }
    }

    return {minus ? -value : value, i};
}

/// @brief Convert an integer value to a decimal string.
/// use is_value_preserving_convertible when available https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p2509r0.html
/// @param value Value
/// @return Buffer that can convert itself to coco::String (via operator String)
template <typename T> requires (std::is_integral_v<T>)
auto dec(const T &value, int digitCount = 1, int minWidth = 0) {
    if constexpr (sizeof(T) <= 4) {
        // 32 bit
        ConvertedBuffer<12> buffer;
        char *end = std::end(buffer.data);
        char *it;
        if constexpr (!std::is_signed_v<T>) {
            // unsigned
            it = detail::dec(end, uint32_t(value), std::clamp(digitCount, 0, 12));
        } else {
            // signed
            it = detail::dec(end, uint32_t(std::abs(value)), std::clamp(digitCount, 0, 11));
            if (value < 0) {
                --it;
                *it = '-';
            }
        }
        if (minWidth > 0) {
            auto begin = std::min(it, end - std::min(minWidth, 12));
            while (it > begin) {
                --it;
                *it = ' ';
            }
        }
        buffer.begin = it;
        buffer.length = end - it;
        return buffer;
    } else {
        // 64 bit
        ConvertedBuffer<24> buffer;
        char *end = std::end(buffer.data);
        char *it;
        if constexpr (!std::is_signed_v<T>) {
            // unsigned
            it = detail::dec(end, uint64_t(value), std::clamp(digitCount, 0, 24));
        } else {
            // signed
            it = detail::dec(end, uint64_t(std::abs(value)), std::clamp(digitCount, 0, 23));
            if (value < 0) {
                --it;
                *it = '-';
            }
        }
        if (minWidth > 0) {
            auto begin = std::min(it, end - std::min(minWidth, 12));
            while (it > begin) {
                --it;
                *it = ' ';
            }
        }
        buffer.begin = it;
        buffer.length = end - it;
        return buffer;
    }
}

/// @brief Convert an enum to a decimal string.
/// @param value Value
/// @return Buffer that can convert itself to coco::String (via operator String)
template <typename T> requires (std::is_enum_v<T>)
auto dec(const T &value, int digitCount = 1, int minWidth = 0) {
    return dec(std::underlying_type_t<T>(value), digitCount, minWidth);
}

/// @brief Convert a floating point value to a decimal string.
/// @param value Value
/// @return Buffer that can convert itself to coco::String (via operator String)
template <typename T> requires (std::is_floating_point_v<T>)
auto dec(const T &value, int digitCount, int decimalCount, int minWidth = 0) {
    ConvertedBuffer<24> buffer;
    char *end = std::end(buffer.data);
    char *it = detail::dec(end, double(value), std::clamp(digitCount, 0, 11), std::clamp(decimalCount, -11, 11));
    if (minWidth > 0) {
        auto begin = std::min(it, end - std::min(minWidth, 12));
        while (it > begin) {
            --it;
            *it = ' ';
        }
    }
    buffer.begin = it;
    buffer.length = end - it;
    return buffer;
}

/// @brief Convert a floating point value to a decimal string.
/// @param value Value
/// @return Buffer that can convert itself to coco::String (via operator String)
template <typename T> requires (std::is_floating_point_v<T>)
auto dec(const T &value, int decimalCount = 3) {
    return dec(value, 1, decimalCount, 0);
}


/// @brief Convert an atomic integer or enum to a decimal string.
/// @param value Value
/// @return Buffer that can convert itself to coco::String (via operator String)
template <typename T> requires (std::is_integral_v<T> || std::is_enum_v<T>)
auto dec(const std::atomic<T> &value, int digitCount = 1, int minWidth = 0) {
    return dec(typename std::atomic<T>::value_type(value), digitCount, minWidth);
}

/// @brief Convert an atomic floating point value to a decimal string.
/// @param value Value
/// @return Buffer that can convert itself to coco::String (via operator String)
template <typename T> requires (std::is_floating_point_v<T>)
auto dec(const std::atomic<T> &value, int digitCount, int decimalCount, int minWidth = 0) {
    return dec(typename std::atomic<T>::value_type(value), digitCount, decimalCount, minWidth);
}

/// @brief Convert an atomic floating point value to a decimal string.
/// @param value Value
/// @return Buffer that can convert itself to coco::String (via operator String)
template <typename T> requires (std::is_floating_point_v<T>)
auto dec(const std::atomic<T> &value, int decimalCount = 3) {
    return dec(typename std::atomic<T>::value_type(value), 1, decimalCount, 0);
}


/// @brief Convert a value to a hex string.
/// @param value Value
/// @return Buffer that has an operator String
template <typename T> requires (std::is_integral_v<T>)
auto hex(const T &value, int digitCount = sizeof(T) * 2, int minWidth = 0) {
    if constexpr (sizeof(T) <= 4) {
        // 32 bit
        ConvertedBuffer<8> buffer;
        auto end = std::end(buffer.data);
        auto it = detail::hex(end, uint32_t(value), std::clamp(digitCount, 0, 8));
        if (minWidth > 0) {
            auto begin = std::min(it, end - std::min(minWidth, 12));
            while (it > begin) {
                --it;
                *it = ' ';
            }
        }
        buffer.begin = it;
        buffer.length = end - it;
        return buffer;
    } else {
        // 64 bit
        ConvertedBuffer<16> buffer;
        auto end = std::end(buffer.data);
        auto it = detail::hex(end, uint64_t(value), std::clamp(digitCount, 0, 16));
        if (minWidth > 0) {
            auto begin = std::min(it, end - std::min(minWidth, 12));
            while (it > begin) {
                --it;
                *it = ' ';
            }
        }
        buffer.begin = it;
        buffer.length = end - it;
        return buffer;
    }
}

/// @brief Convert an enum to a hex string.
/// @param value Value
/// @return Buffer that has an operator String
template <typename T> requires (std::is_enum_v<T>)
auto hex(const T &value, int digitCount = sizeof(T) * 2, int minWidth = 0) {
    return hex(std::underlying_type_t<T>(value), digitCount, minWidth);
}

/// @brief Convert an atomic value to a hex string.
/// @param value Value
/// @return Buffer that has an operator String
template <typename T>
auto hex(const std::atomic<T> &value, int digitCount = sizeof(T) * 2, int minWidth = 0) {
    return hex(typename std::atomic<T>::value_type(value), digitCount, minWidth);
}


/// @brief Convert an UTF-8 string to a character code point.
/// 1 byte: 0xxxxxxx
/// 2 byte: 110xxxxx 10xxxxxx
/// 3 byte: 1110xxxx 10xxxxxx 10xxxxxx
/// 4 byte: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
/// @param str String to convert
/// @return Code and length
ConvertedValue<int> utf8(String str);

/// @brief Convert a character code point to an UTF-8 string.
/// @param code Code point to convert
/// @return Buffer that has an operator String
inline auto utf8(int code) {
    ConvertedBuffer<7> buffer;
    auto end = std::end(buffer.data);
    auto begin = detail::utf8(end, uint32_t(code));
    buffer.begin = begin;
    buffer.length = end - begin;
    return buffer;
}









//

/**
 * Convert string to integer
 * @param str string to convert
 * @return optional int, defined if conversion was successful
 */
std::optional<int> parseInt(String str);

/**
 * Convert string to floating point number in the form x.y without support for exponential notation
 * @param str string to convert
 * @return optional floating point number, defined if conversion was successful
 */
std::optional<float> parseFloat(String str);

/**
 * Convert a 64 bit integer to string
 * @param buffer buffer in which conversion takes place
 * @param value value to convert
 * @param digitCount minimum number of digits to convert, pad smaller numbers with leading zeros, maximum is 15
 * @return string containing the number, references the input buffer
 */
//String toString(Array<char, 16> buffer, int64_t value, int digitCount = 1);

/**
 * Convert a 64 bit unsigned integer to hex string
 * @param buffer buffer in which conversion takes place
 * @param value value to convert
 * @param digitCount number of hex digits to convert
 * @return string containing the number, references the input buffer
 */
//String toHexString(Array<char, 16> buffer, uint64_t value, int digitCount);

/**
 * Convert a float to string
 * @param buffer buffer in which conversion takes place
 * @param value value to convert
 * @param digitCount minimum number of digits to convert before the decimal point
 * @param decimalCount minimum number of digits to convert after the decimal point, negative to keep trailing zeros
 * @return string containing the number, references the input buffer
 */
//String toString(Array<char, 21> buffer, float value, int digitCount, int decimalCount);

} // namespace coco
