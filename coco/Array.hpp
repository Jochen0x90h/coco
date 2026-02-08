#pragma once

#include <cassert>
#include <cstdint>
#include <iterator>


namespace coco {

/// @brief Array with known size that only references the data.
/// @tparam T type of buffer elements
/// @tparam N size of buffer
template <typename T, int N = -1>
class Array {
public:
    using Element = T;


    /// @brief Default copy constructor.
    ///
    Array(Array const &array) = default;

    /// @brief Construct from C-array or C-string.
    ///
    constexpr Array(T (&array)[N]) noexcept : buffer(array) {}

    /// @brief Copy construct from array with compatible data type (e.g. non-const to const).
    ///
    template <typename T2>
    Array(Array<T2, N> array) noexcept : buffer(array.data()) {}

    /// @brief Explicitly construct from any data using reinterpret_cast.
    ///
    template <typename T2>
    explicit Array(T2 *data) noexcept : buffer(reinterpret_cast<T2 *>(data)) {}

    /// @brief Default assignment operator.
    ///
    Array &operator =(Array const &array) = default;

    /// @brief Assignment operator from buffer with different data type (e.g. non-const to const).
    ///
    template <typename T2>
    Array &operator =(Array<T2, N> array) {
        this->buffer = array.data();
        return *this;
    }

    /// @brief Check if the array is empty.
    /// @return true when empty
    static bool empty() {return N == 0;}

    /// @brief Fill whole buffer with a value.
    /// @param value fill value
    void fill(T const &value) {
        for (auto &element : *this) {
            element = value;
        }
    }

    /// @brief Assign from an iterator, also cast each value to destination type.
    /// @param src source iterator
    template <typename InputIt>
    void assign(InputIt src) {
        auto it = begin();
        auto e = end();
        for (; it < e; ++it, ++src) {
            *it = T(*src);
        }
    }

    /// @brief Index operator.
    ///
    T &operator [](int index) const {
        assert(uint32_t(index) < N);
        return this->buffer[index];
    }

    /// @brief Get an array staring at the given index.
    ///
    template <int M>
    Array<T, M> array(int index) const {
        assert(uint32_t(index) <= N - M);
        return Array<T, M>(this->buffer + index);
    }

    /// @brief Get a sub-array.
    /// @param startIndex start of sub-array
    /// @return array that references the a part of this array
    Array<T, -1> subarray(int startIndex) const {
        return {this->buffer + startIndex, std::max(N - startIndex, 0)};
    }

    /// @brief Get a sub-array.
    /// @param startIndex start of sub-array
    /// @param endIndex end of sub-array (not included)
    /// @return array that references the a part of this array
    Array<T, -1> subarray(int startIndex, int endIndex) const {
        return {this->buffer + startIndex, std::max(std::min(endIndex, N) - startIndex, 0)};
    }

    /// @brief Array access data() and size() which is O(1).
    ///
    T *data() const {return this->buffer;}

    /// @brief Get size of array.
    /// @return Size
    static int size() {return N;}

    /// @brief Iterators begin() and end().
    ///
    T *begin() const {return this->buffer;}
    T *end() const {return this->buffer + N;}

protected:

    T *buffer;
};


/// @brief Array with variable size that only references the data, similar to std::span.
/// @tparam T type of array element, e.g. Array<const int> for an array of constant integers
template <typename T>
class Array<T, -1> {
public:
    using Element = T;

    /// @brief Default constructor.
    ////
    constexpr Array() noexcept : buffer(nullptr), length(0) {}

    /// @brief Default copy constructor.
    ///
    Array(Array const &) = default;

    /// @brief Construct from container supporting std::data() and std::size() with compatible data type (e.g. std::vector).
    ///
    template <typename T2>
    constexpr Array(T2 &container) noexcept : buffer(std::data(container)), length(std::size(container)) {}

    /// @brief Construct from begin and end.
    ///
    Array(T *begin, T *end) noexcept : buffer(begin), length(end - begin) {}

    /// @brief Construct from data and lenth.
    ///
    Array(T *data, int length) noexcept : buffer(data), length(length) {}

    /// @brief Explicitly construct from any data and length using reinterpret_cast.
    ///
    template <typename T2>
    explicit Array(T2 *data, int length) noexcept : buffer(reinterpret_cast<T2 *>(data)), length(length) {}

    /// @brief Default assignment operator.
    ///
    Array &operator =(Array const &) = default;

    /// @brief Check if the array is empty.
    /// @return true when empty
    bool empty() const {return this->length <= 0;}


    /// @brief Fill whole buffer with a value.
    /// @param value fill value
    void fill(T const &value) {
        for (auto &element : *this) {
            element = value;
        }
    }

    /// @brief Assign from an iterator, also cast each value to destination type.
    /// @param src source iterator
    template <typename InputIt>
    void assign(InputIt src) {
        auto it = begin();
        auto e = end();
        for (; it < e; ++it, ++src) {
            *it = T(*src);
        }
    }

    /// @brief Index operator. Note that the returned reference to the element is non-const.
    /// @param index index of element
    /// @return reference to element
    T &operator [](int index) const {
        assert(uint32_t(index) < uint32_t(this->length));
        return this->buffer[index];
    }

    /// @brief Get a sub-array.
    /// @param startIndex start of sub-array
    /// @return array that references the a part of this array
    Array subarray(int startIndex) const {
        return {this->buffer + startIndex, std::max(this->length - startIndex, 0)};
    }

    /// @brief Get a sub-array.
    /// @param startIndex start of sub-array
    /// @param endIndex end of sub-array (not included)
    /// @return array that references the a part of this array
    Array subarray(int startIndex, int endIndex) const {
        return {this->buffer + startIndex, std::max(std::min(endIndex, this->length) - startIndex, 0)};
    }

    /// @brief Array access data() and size() which is O(1).
    ///
    T *data() const {return this->buffer;}

    /// @brief Get size of array.
    /// @return Size
    int size() const {return this->length;}

    /// @brief Iterators begin() and end().
    ///
    T *begin() const {return this->buffer;}
    T *end() const {return this->buffer + this->length;}

protected:

    T *buffer;
    int length;
};


/// @brief Array comparison operator.
/// @param array first array to compare
/// @param container second array of any type that supports std::data() and std::size()
template <typename T, int N, typename T2>
bool operator ==(const Array<T, N> &a, const T2 &b) {
    if (a.size() != std::size(b))
        return false;

    auto da = a.data();
    auto db = std::data(b);
    for (int i = 0; i < a.size(); ++i) {
        if (da[i] != db[i])
            return false;
    }
    return true;
}

} // namespace coco
