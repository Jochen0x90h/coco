#pragma once

#include "ArrayConcept.hpp"
#include "CStringConcept.hpp"
#include <limits>
#include <algorithm>
#include <cstdint>
#ifdef NATIVE
#include <ostream>
#endif


namespace coco {

inline bool isWhiteSpace(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == 0;
}


/// @brief String, only references the data.
/// Is similar to std::string_view
struct String {
    friend bool operator ==(String a, String b);
    friend std::strong_ordering operator <=>(String a, String b);
public:
    constexpr String() noexcept
        : buffer(), length(0)
    {}

    String(const String &str) = default;

    /// @brief Construct from c-string (const char *) or char array (char[])
    ///
    template <typename T> requires (CStringConcept<T>)
    constexpr String(const T &str) noexcept
        : buffer(str), length(coco::length(str))
    {}

    /// @brief Construct from container supporting std::data() and std::size() with compatible data type (e.g. coco::Array, std::string)
    ///
    template <typename T> requires (ArrayConcept<T, char> && !CStringConcept<T>)
    constexpr String(const T &array) noexcept : buffer(std::data(array)), length(std::size(array)) {}

    /// @brief Construct String from any type of data
    ///
    constexpr String(const char *data, int length)
        : buffer(data), length(int(length))
    {}

    /// @brief Construct/cast String from any type of data
    ///
    explicit String(const void *data, int length)
        : buffer(reinterpret_cast<char const*>(data)), length(length)
    {}

    /// @brief Default assignment operator
    ///
    String &operator =(String const &str) = default;

    bool empty() const {return this->length <= 0;}

    /// @brief Get a substring
    /// @param startIndex start of substring
    /// @return string that references the a part of this string
    String substring(int startIndex) const {
        return {this->buffer + startIndex, std::max(this->length - startIndex, 0)};
    }

    /// @brief Get a substring
    /// @param startIndex start of substring
    /// @param endIndex end of substring (not included)
    /// @return string that references a part of this string
    String substring(int startIndex, int endIndex) const {
        return {this->buffer + startIndex, std::max(std::min(endIndex, this->length) - startIndex, 0)};
    }

    /// @brief Checks if a string starts with a prefix string.
    /// @param prefix Prefix
    /// @return True if string starts with prefix
    bool startsWith(String prefix) const {
        if (prefix.length > this->length)
            return false;
        auto it = prefix.buffer;
        auto end = it + prefix.length;
        auto it2 = this->buffer;
        for (; it != end; ++it, ++it2) {
            if (*it != *it2)
                return false;
        }
        return true;
    }

    /// @brief Checks if a string ends with a prefix string.
    /// @param suffix Suffix
    /// @return True if string ends with suffix
    bool endsWith(String suffix) const {
        if (suffix.length > this->length)
            return false;
        auto it = suffix.buffer;
        auto end = it + suffix.length;
        auto it2 = this->buffer + this->length - suffix.length;
        for (; it != end; ++it, ++it2) {
            if (*it != *it2)
                return false;
        }
        return true;
    }

    /// @brief Remove whitespaces at the beginning and end of the string
    ///
    String trim() const {
        // trim at beginning
        int s = 0;
        while (s < this->length && isWhiteSpace(buffer[s]))
            ++s;

        // trim at end
        int e = this->length;
        while (e > s && isWhiteSpace(buffer[e - 1]))
            --e;
        return {this->buffer + s, e - s};
    }

    /// @brief Return the index of the first occurrence of a character or a default value (-1) when not found
    /// @param ch character to search
    /// @param startIndex index where to start searching
    /// @param defaultValue value to return when the character is not found
    /// @return index of first occurrence of the character
    int indexOf(char ch, int startIndex = 0, int defaultValue = -1) const {
        for (int i = startIndex; i < this->length; ++i) {
            if (this->buffer[i] == ch)
                return i;
        }
        return defaultValue;
    }

    /// @brief Return the index of the first occurrence of a string or a default value (-1) when not found
    /// @param str string to search
    /// @param startIndex index where to start searching
    /// @param defaultValue value to return when the character is not found
    /// @return index of first occurrence of the character
    int indexOf(String str, int startIndex = 0, int defaultValue = -1) const {
        char ch = str[0];
        for (int i = startIndex; i < this->length; ++i) {
            if (this->buffer[i] == ch) {
                auto b = &this->buffer[i];
                for (int j = 1; j < str.size(); ++j) {
                    if (b[j] != str[j]) {
                        goto skip;
                    }
                }
                return i;
            }
    skip:
            ;
        }
        return defaultValue;
    }

    /// @brief Return the index of the last occurrence of a character
    /// @param ch character to search
    /// @param startIndex index where to start searching
    /// @param defaultValue value to return when the character is not found
    /// @return index of first occurrence of the character
    int lastIndexOf(char ch, int startIndex = std::numeric_limits<int>::max(), int defaultValue = -1) const {
        int i = std::min(startIndex, this->length);
        while (i > 0) {
            --i;
            if (this->buffer[i] == ch)
                return i;
        }
        return defaultValue;
    }

    /// @brief Index operator
    ///
    constexpr char const operator [](int index) const {return this->buffer[index];}

    /// @brief Convert to std::string_view
    /// @return string_view that references this buffer
    operator std::string_view() const {
        return {this->buffer, size_t(this->length)};
    }

    /// @brief Calculate a hash of the string
    /// http://www.cse.yorku.ca/~oz/hash.html
    /// @return dj2 hash of string
    uint32_t hash() const {
        uint32_t h = 5381;
        for (char c : *this) {
            h = (h << 5) + h + uint8_t(c); // hash	33 + c
        }
        return h;
    }

    /// @brief Array access data() and size() which is O(1)
    ///
    const char *data() const {return this->buffer;}
    int size() const {return this->length;}

    /// @brief Iterators begin() and end()
    ///
    const char *begin() const {return this->buffer;}
    const char *end() const {return this->buffer + this->length;}

protected:

    const char *buffer;
    int length;
};

/// @brief Equality operator
///
inline bool operator ==(String a, String b) {
    if (a.length != b.length)
        return false;
    auto it = a.buffer;
    auto end = it + a.length;
    auto it2 = b.buffer;
    for (; it != end; ++it, ++it2) {
        if (*it != *it2)
            return false;
    }
    /*for (int i = 0; i < a.length; ++i) {
        if (a.buffer[i] != b.buffer[i])
            return false;
    }*/
    return true;
}

/// @brief Spaceship operator
///
inline std::strong_ordering operator <=>(String a, String b) {
    int length = std::min(a.length, b.length);
    for (int i = 0; i < length; ++i) {
        auto c = (unsigned char)a.buffer[i];
        auto d = (unsigned char)b.buffer[i];
        auto res = c <=> d;
        if (res != 0)
            return res;
    }
    return a.length <=> b.length;
}

/// @brief Assign a string to a fixed size c-string
/// @param str fixed size c-string of type char[N]
/// @param s source string
template <int N>
inline void assign(char (&str)[N], String const &s) {
    // copy string
    int count = std::min(N, s.size());
    for (int i = 0; i < count; ++i) {
        str[i] = s[i];
    }

    // pad with zeros
    for (int i = count; i < N; ++i) {
        str[i] = 0;
    }
}

#ifdef NATIVE
inline std::ostream &operator <<(std::ostream &s, const String &str) {
    s.write(str.data(), str.size());
    return s;
}
#endif

} // namespace coco
