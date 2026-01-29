#pragma once

#include "String.hpp"


namespace coco {

/// @brief Vector with two dimensions.
/// @tparam T base type
template <typename T>
struct Vector2 {
    T x;
    T y;

    constexpr Vector2() : x(), y() {}
    constexpr Vector2(T x, T y) : x(x), y(y) {}

    Vector2(const Vector2 &v) = default;

    /// @brief Cast from vector with different base type
    ///
    template <typename T2>
    explicit Vector2(const Vector2<T2> &v) : x(v.x), y(v.y) {}

    Vector2 &operator +=(const T &a) {
        this->x += a;
        this->y += a;
        return *this;
    }

    Vector2 &operator +=(const Vector2 &a) {
        this->x += a.x;
        this->y += a.y;
        return *this;
    }

    Vector2 &operator -=(const T &a) {
        this->x -= a;
        this->y -= a;
        return *this;
    }

    Vector2 &operator -=(const Vector2 &a) {
        this->x -= a.x;
        this->y -= a.y;
        return *this;
    }

    Vector2 &operator *=(const T &a) {
        this->x *= a;
        this->y *= a;
        return *this;
    }

    Vector2 &operator *=(const Vector2 &a) {
        this->x *= a.x;
        this->y *= a.y;
        return *this;
    }

    Vector2 &operator /=(const T &a) {
        this->x /= a;
        this->y /= a;
        return *this;
    }

    Vector2 &operator /=(const Vector2 &a) {
        this->x /= a.x;
        this->y /= a.y;
        return *this;
    }

    /// @brief Swap x and y
    ///
    Vector2 yx() {return {this->y, this->x};}

    /// @brief Boradcast x
    ///
    Vector2 xx() {return {this->x, this->x};}

    /// @brief Boradcast y
    ///
    Vector2 yy() {return {this->y, this->y};}

    int size() {return 2;}
    T *data() {return &this->x;}
    const T *data() const {return &this->x;}

    T *begin() {return &this->x;}
    const T *begin() const {return &this->x;}
    T *end() {return &this->x + 2;}
    const T *end() const {return &this->x + 2;}
};

template <typename T>
inline bool operator ==(const Vector2<T> &a, const Vector2<T> &b) {
	return a.x == b.x && a.y == b.y;
}

template <typename T>
inline auto operator +(const Vector2<T> &a) {
    return Vector2<decltype(+a.x)>(+a.x, +a.y);
}

template <typename T>
inline auto operator -(const Vector2<T> &a) {
    return Vector2<decltype(-a.x)>(-a.x, -a.y);
}

template <typename T1, typename T2>
inline auto operator +(const Vector2<T1> &a, T2 b) {
    return Vector2<decltype(a.x + b)>(a.x + b, a.y + b);
}

template <typename T1, typename T2>
inline auto operator +(T1 a, const Vector2<T2> &b) {
    return Vector2<decltype(a + b.x)>(a + b.x, a + b.y);
}

template <typename T1, typename T2>
inline auto operator +(const Vector2<T1> &a, const Vector2<T2> &b) {
    return Vector2<decltype(a.x + b.x)>(a.x + b.x, a.y + b.y);
}

template <typename T1, typename T2>
inline auto operator -(const Vector2<T1> &a, T2 b) {
    return Vector2<decltype(a.x - b)>(a.x - b, a.y - b);
}

template <typename T1, typename T2>
inline auto operator -(T1 a, const Vector2<T2> &b) {
    return Vector2<decltype(a - b.x)>(a - b.x, a - b.y);
}

template <typename T1, typename T2>
inline auto operator -(const Vector2<T1> &a, const Vector2<T2> &b) {
    return Vector2<decltype(a.x - b.x)>(a.x - b.x, a.y - b.y);
}

template <typename T1, typename T2>
inline auto operator *(const Vector2<T1> &a, T2 b) {
    return Vector2<decltype(a.x * b)>(a.x * b, a.y * b);
}

template <typename T1, typename T2>
inline auto operator *(T1 a, const Vector2<T2> & b) {
    return Vector2<decltype(a * b.x)>(a * b.x, a * b.y);
}

template <typename T1, typename T2>
inline auto operator *(const Vector2<T1> &a, const Vector2<T2> &b) {
    return Vector2<decltype(a.x * b.x)>(a.x * b.x, a.y * b.y);
}

template <typename T1, typename T2>
inline auto operator /(const Vector2<T1> &a, T2 b) {
    return Vector2<decltype(a.x / b)>(a.x / b, a.y / b);
}

template <typename T1, typename T2>
inline auto operator /(const Vector2<T1> &a, const Vector2<T2> &b) {
    return Vector2<decltype(a.x / b.x)>(a.x / b.x, a.y / b.y);
}
/*
#ifdef NATIVE
template <typename T>
std::ostream &operator <<(std::ostream &s, const Vector2<T> &value) {
    s << value.x << ' ' << value.y;
    return s;
}
#endif
*/

template <typename T, typename F>
struct Vector2Serializer {
    // vector to serialize
    const Vector2<T> &v;

    // function that does the serialization
    const F &f;

    // delimiter between vector elements
    String d;
};

template <typename S, typename T, typename F>
S &operator <<(S &stream, Vector2Serializer<T, F> s) {
    s.f(stream, s.v.x);
    stream << s.d;
    s.f(stream, s.v.y);
    return stream;
}

template <typename T, typename F>
Vector2Serializer<T, F> vector(const Vector2<T> &vector, const F &function, String delimiter) {
    return {vector, function, delimiter};
}


using int2 = Vector2<int>;
using float2 = Vector2<float>;
using double2 = Vector2<double>;

} // namespace coco
