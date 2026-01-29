#pragma once

#include "Array.hpp"
#include "String.hpp"
#include "StringConcept.hpp"


namespace coco {

/// @brief Array with fixed maximum size (similar to std::inplace_vector of C++26)
/// @tparam T element type
/// @tparam N maximum size
template <typename T, int N>
class ArrayBuffer {
public:
    static constexpr int MAX_SIZE = N;

    /// @brief Default constructor
    ///
    ArrayBuffer() : length() {}

    /// @brief Destructor
    ///
    ~ArrayBuffer() {
        auto buffer = reinterpret_cast<T *>(this->buffer);
        for (auto it = buffer; it < buffer + this->length; ++it) {
            it->~T();
        }
    }

    /// @brief Construct from any container supporting std::begin() and std::end()
    /// @tparam T2 container type
    /// @param container container used to initialize the buffer
    template <typename T2>
    ArrayBuffer(const T2 &container) {
        auto buffer = reinterpret_cast<T *>(this->buffer);
        auto it1 = buffer;
        auto end1 = buffer + N;
        auto it2 = std::begin(container);
        auto end2 = std::end(container);
        int l = 0;
        for (; it1 != end1 && it2 != end2; ++it1, ++it2, ++l)
            new (it1) T(*it2);
        this->length = l;
    }

    /// @brief Assign from any container supporting std::begin() and std::end()
    /// @tparam T2 container type
    /// @param container container that is assigned to the buffer
    /// @return *this
    template <typename T2>
    ArrayBuffer &operator =(const T2 &container) {
        auto buffer = reinterpret_cast<T *>(this->buffer);
        for (auto it = buffer; it < buffer + this->length; ++it) {
            it->~T();
        }
        auto it1 = buffer;
        auto end1 = buffer + N;
        auto it2 = std::begin(container);
        auto end2 = std::end(container);
        int l = 0;
        for (; it1 != end1 && it2 != end2; ++it1, ++it2, ++l)
            new (it1) T(*it2);
        this->length = l;
        return *this;
    }

    /// @brief Check if the array is empty
    /// @return true when empty
    bool empty() const {return this->length == 0;}

    /// @brief Get the number of elements in the array which is O(1)
    /// @return number of elements
    int size() const {return this->length;}

    /// @brief Get the capacity of the array, number of pre-allocated elements
    /// @return capacity
    static int capacity() {return N;}

    /// @brief Clear the array
    ///
    void clear() {
        auto buffer = reinterpret_cast<T *>(this->buffer);
        for (auto it = buffer; it < buffer + this->length; ++it) {
            it->~T();
        }
        this->length = 0;
    }

    /// @brief Resize the array
    /// @param size new size, gets clamped to the maximum size of the array
    void resize(int size) {
        auto buffer = reinterpret_cast<T *>(this->buffer);
        int newLength = std::clamp(size, 0, N);

        auto a = buffer + newLength;
        auto b = buffer + this->length;

        // destruct excess elements
        for (auto it = a; it < b; ++it) {
            it->~T();
        }

        // construct new elements
        for (auto it = b; it < a; ++it) {
            new (it) T();
        }

        this->length = newLength;
    }

    /// @brief Fill array with a value while keeping the size
    /// @param value fill value
    void fill(const T &value) {
        for (auto &element : *this) {
            element = value;
        }
    }

    /// @brief Index operator
    /// @param index index of the element
    /// @return element at the index
    const T &operator [](int index) const {
        assert(uint32_t(index) < this->length);
        auto buffer = reinterpret_cast<const T *>(this->buffer);
        return buffer[index];
    }
    T &operator [](int index) {
        assert(uint32_t(index) < this->length);
        auto buffer = reinterpret_cast<T *>(this->buffer);
        return buffer[index];
    }

    /// @brief Append an element to the array
    /// @param element element to append
    void push_back(const T &element) {
        auto buffer = reinterpret_cast<T *>(this->buffer);
        if (this->length < N) {
            new (buffer + this->length) T(element);
            ++this->length;
        }
    }

    /// @brief Construct an element at the end of the array in-place
    /// @tparam Args argument types
    template<typename... Args>
    T &emplace_back(Args &&... args) {
        assert(this->length < N);
        auto ptr = reinterpret_cast<T *>(this->buffer) + this->length;
        new (ptr) T(std::forward<Args>(args)...);
        ++this->length;
        return *ptr;
    }

    /// Append any range or container supporting std::begin() and std::end() to the array
    /// @tparam T2 container type
    /// @param container container whose contents are appended
    template <typename T2>
    void append_range(const T2 &container) {
        auto buffer = reinterpret_cast<T *>(this->buffer);
        auto it1 = buffer + this->length;
        auto end1 = buffer + N;
        auto it2 = std::begin(container);
        auto end2 = std::end(container);
        int l = this->length;
        for (; it1 != end1 && it2 != end2; ++it1, ++it2, ++l)
            new (it1) T(*it2);
        this->length = l;
    }

    ///  @briefGet pointer to data
    ///
    T *data() {return reinterpret_cast<T *>(this->buffer);}
    const T *data() const {return reinterpret_cast<const T *>(this->buffer);}

    ///  @brief Iterators
    ///
    T *begin() {return reinterpret_cast<T* >(this->buffer);}
    T *end() {return reinterpret_cast<T *>(this->buffer) + this->length;}
    const T *begin() const {return reinterpret_cast<const T *>(this->buffer);}
    const T *end() const {return reinterpret_cast<const T *>(this->buffer) + this->length;}


    // buffer elements as byte array to avoid default construction of T
    alignas(T) uint8_t buffer[N * sizeof(T)];

    // length after buffer so that alignas(A) ArrayBuffer<T, N> works correctly
    int length;
};


/// @brief Partial specialization of ArrayBuffer for char to be used as StringBuffer<N>.
/// @tparam N maximum size
template <int N>
class ArrayBuffer<char, N> {
public:
    static constexpr int CAPACITY = N;

    /// @brief Default constructor
    ///
    ArrayBuffer() : length() {}

    /// @brief Construct from any container supporting std::begin() and std::end()
    /// @tparam T2 container type
    /// @param container container used to initialize the buffer
    template <typename T2>
    ArrayBuffer(const T2 &container) {
        auto buffer = this->buffer;
        auto it1 = buffer;
        auto end1 = buffer + N;
        auto it2 = std::begin(container);
        auto end2 = std::end(container);
        int l = 0;
        for (; it1 != end1 && it2 != end2; ++it1, ++it2, ++l)
            *it1 = *it2;
        this->length = l;
    }

    /// @brief Assign from any container supporting std::begin() and std::end()
    /// @tparam T2 container type
    /// @param container container that is assigned to the buffer
    /// @return *this
    template <typename T2> requires (!StringConcept<T2>)
    ArrayBuffer &operator =(const T2 &container) {
        auto buffer = this->buffer;
        auto it1 = buffer;
        auto end1 = buffer + N;
        auto it2 = std::begin(container);
        auto end2 = std::end(container);
        for (; it1 != end1 && it2 != end2; ++it1, ++it2)
            *it1 = *it2;
        this->length = it1 - buffer;
#ifdef DEBUG
        this->buffer[this->length] = 0;
#endif
        return *this;
    }

    /// @brief Assign a string
    /// @tparam T2 string type
    /// @param string string that is assigned to the buffer
    /// @return *this
    template <typename T2> requires (StringConcept<T2>)
    ArrayBuffer &operator =(const T2 &string) {
        auto buffer = this->buffer;
        auto capacity = N;

        String s(string);
        int l = std::min(capacity, s.size());
        std::copy(s.begin(), s.begin() + l, buffer);
        this->length = l;
#ifdef DEBUG
        buffer[this->length] = 0;
#endif
        return *this;
    }

    /// @brief Check if the array is empty
    /// @return true when empty
    bool empty() const {return this->length == 0;}

    /// @brief Get the number of elements in the array which is O(1)
    /// @return number of elements
    int size() const {return this->length;}

    /// @brief Get the capacity of the array, number of pre-allocated elements
    /// @return capacity
    static int capacity() {return N;}

    /// @brief Clear the buffer
    ///
    void clear() {
        this->length = 0;
#ifdef DEBUG
        this->buffer[0] = 0;
#endif
    }

    /// Resize the array
    /// @param size new size, gets clamped to the maximum size of the array
    void resize(int size) {
        auto buffer = this->buffer;
        int newLength = std::clamp(size, 0, N);

        auto a = buffer + newLength;
        auto b = buffer + this->length;

        // clear new elements
        for (auto it = b; it < a; ++it) {
            *it = 0;
        }

        this->length = newLength;
#ifdef DEBUG
        this->buffer[newLength] = 0;
#endif
    }

    /// @brief Fill array with a value while keeping the size
    /// @param value fill value
    void fill(const char value) {
        for (auto &element : *this) {
            element = value;
        }
    }

    /// @brief Index operator
    /// @param index index of the element
    /// @return element at the index
    const char &operator [](int index) const {
        assert(uint32_t(index) < this->length);
        return this->buffer[index];
    }
    char &operator [](int index) {
        assert(uint32_t(index) < this->length);
        return this->buffer[index];
    }

    /// @brief Append a single character to the array
    /// @param ch char to append
    void push_back(char ch) {
        auto buffer = this->buffer;
        auto capacity = N;

        if (this->length < capacity)
            buffer[this->length++] = ch;
#ifdef DEBUG
        buffer[this->length] = 0;
#endif
    }

    /// @brief Construct an element at the end of the array in-place
    char &emplace_back(char ch = 0) {
        assert(this->length < N);
        auto ptr = this->buffer + this->length;
        *ptr = ch;
        ++this->length;
        return *ptr;
    }

    /// Append any range or container supporting std::begin() and std::end() to the array
    /// @tparam T2 container type
    /// @param container container to append
    template <typename T2> requires (!StringConcept<T2>)
    void append_range(const T2 &container) {
        auto buffer = this->buffer;
        auto capacity = N;

        auto it1 = buffer + this->length;
        auto it2 = std::begin(container);
        auto end1 = buffer + capacity;
        auto end2 = std::end(container);
        int l = this->length;
        for (; it1 != end1 && it2 != end2; ++it1, ++it2, ++l)
            *it1 = *it2;
        this->length = l;
#ifdef DEBUG
        buffer[this->length] = 0;
#endif
    }

    /// Append any string to the array
    /// @tparam T2 container type
    /// @param container container to append
    template <typename T2> requires (StringConcept<T2>)
    void append_range(const T2 &string) {
        auto buffer = this->buffer;
        auto capacity = N;

        String s(string);
        int l = std::min(capacity - this->length, s.size());
        std::copy(s.begin(), s.begin() + l, buffer + this->length);
        this->length += l;
#ifdef DEBUG
        buffer[this->length] = 0;
#endif
    }

    /// @brief Append a single character.
    ///
    ArrayBuffer &operator <<(char ch) {
        push_back(ch);
        return *this;
    }

    /// @brief Append a string.
    ///
    ArrayBuffer &operator <<(const String &string) {
        append_range(string);
        return *this;
    }

    /// @brief Stream a string concept (C-string, std::string)
    ///
    /*template <typename T> requires (StringConcept<T>)
    ArrayBuffer &operator <<(T const &string) {
        append_range(String(string));
        return *this;
    }*/

    /// @brief Get as coco::String
    /// @return string that references this buffer
    String string() const {
        return {this->buffer, this->length};
    }

    /// @brief Convert operator to coco::String
    /// @return String that references this buffer
    operator String() const {
        return {this->buffer, this->length};
    }

    /// @brief Convert to std::string_view
    /// @return string_view that references this buffer
    operator std::string_view() const {
        return {this->buffer, size_t(this->length)};
    }

    /// @brief Get a substring
    /// @param startIndex start of substring
    /// @return string that references the a part of this buffer
    String substring(int startIndex) const {
        return {this->buffer + startIndex, std::max(this->length - startIndex, 0)};
    }

    /// @brief Get a substring
    /// @param startIndex start of substring
    /// @param endIndex end of substring (not included)
    /// @return string that references a part of this buffer
    String substring(int startIndex, int endIndex) const {
        return {this->buffer + startIndex, std::max(std::min(endIndex, this->length) - startIndex, 0)};
    }

    /// @brief Get pointer to data
    ///
    char *data() {return this->buffer;}
    const char *data() const {return this->buffer;}

    /// @brief Get as C-string
    /// @return C-string
    const char *c_str() {
        this->buffer[this->length] = 0;
        return this->buffer;
    }

    /// @brief Iterators
    ///
    char *begin() {return this->buffer;}
    char *end() {return this->buffer + this->length;}
    const char *begin() const {return this->buffer;}
    const char *end() const {return this->buffer + this->length;}


    char buffer[N + 1];

    // length after buffer so that alignas(A) StringBuffer<N> works correctly
    int length;
};

} // namespace coco
