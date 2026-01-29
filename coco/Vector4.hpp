#pragma once

#include "String.hpp"


namespace coco {

/// @brief Vector with four dimensions.
/// @tparam T base type
template <typename T>
struct Vector4 {
    T x;
    T y;
    T z;
    T w;

    constexpr Vector4() : x(), y(), z(), w() {}
    constexpr Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

    Vector4(const Vector4 &v) = default;

    /// @brief Cast from vector with different base type
    ///
    template <typename T2>
    explicit Vector4(const Vector4<T2> &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

    Vector4 &operator +=(const T &a) {
        this->x += a;
        this->y += a;
        this->z += a;
        this->w += a;
        return *this;
    }

    Vector4 &operator +=(const Vector4 &a) {
        this->x += a.x;
        this->y += a.y;
        this->z += a.z;
        this->w += a.w;
        return *this;
    }

    Vector4 &operator -=(const T &a) {
        this->x -= a;
        this->y -= a;
        this->z -= a;
        this->w -= a;
        return *this;
    }

    Vector4 &operator -=(const Vector4 &a) {
        this->x -= a.x;
        this->y -= a.y;
        this->z -= a.z;
        this->w -= a.w;
        return *this;
    }

    Vector4 &operator *=(const T &a) {
        this->x *= a;
        this->y *= a;
        this->z *= a;
        this->w *= a;
        return *this;
    }

    Vector4 &operator *=(const Vector4 &a) {
        this->x *= a.x;
        this->y *= a.y;
        this->z *= a.z;
        this->w *= a.w;
        return *this;
    }

    Vector4 &operator /=(const T &a) {
        this->x /= a;
        this->y /= a;
        this->z /= a;
        this->w /= a;
        return *this;
    }

    Vector4 &operator /=(const Vector4 &a) {
        this->x /= a.x;
        this->y /= a.y;
        this->z /= a.z;
        this->w /= a.w;
        return *this;
    }

    /// @brief Boradcast x
    ///
    Vector4 xxxx() {return {this->x, this->x, this->x, this->x};}

    /// @brief Boradcast y
    ///
    Vector4 yyyy() {return {this->y, this->y, this->y, this->y};}

    /// @brief Boradcast z
    ///
    Vector4 zzzz() {return {this->z, this->z, this->z, this->z};}

    /// @brief Boradcast w
    ///
    Vector4 wwww() {return {this->w, this->w, this->w, this->w};}

    int size() {return 4;}
    T *data() {return &this->x;}
    const T *data() const {return &this->x;}

    T *begin() {return &this->x;}
    const T *begin() const {return &this->x;}
    T *end() {return &this->x + 4;}
    const T *end() const {return &this->x + 4;}
};

template <typename T>
inline bool operator ==(const Vector4<T> &a, const Vector4<T> &b) {
	return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

template <typename T>
inline auto operator +(const Vector4<T> &a) {
    return Vector4<decltype(+a.x)>(+a.x, +a.y, +a.z, +a.w);
}

template <typename T>
inline auto operator -(const Vector4<T> &a) {
    return Vector4<decltype(-a.x)>(-a.x, -a.y, -a.z, -a.w);
}

template <typename T1, typename T2>
inline auto operator +(const Vector4<T1> &a, T2 b) {
    return Vector4<decltype(a.x + b)>(a.x + b, a.y + b, a.z + b, a.w + b);
}

template <typename T1, typename T2>
inline auto operator +(T1 a, const Vector4<T2> &b) {
    return Vector4<decltype(a + b.x)>(a + b.x, a + b.y, a + b.z, a + b.w);
}

template <typename T1, typename T2>
inline auto operator +(const Vector4<T1> &a, const Vector4<T2> &b) {
    return Vector4<decltype(a.x + b.x)>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

template <typename T1, typename T2>
inline auto operator -(const Vector4<T1> &a, T2 b) {
    return Vector4<decltype(a.x - b)>(a.x - b, a.y - b, a.z - b, a.w - b);
}

template <typename T1, typename T2>
inline auto operator -(T1 a, const Vector4<T2> &b) {
    return Vector4<decltype(a - b.x)>(a - b.x, a - b.y, a - b.z, a - b.w);
}

template <typename T1, typename T2>
inline auto operator -(const Vector4<T1> &a, const Vector4<T2> &b) {
    return Vector4<decltype(a.x - b.x)>(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

template <typename T1, typename T2>
inline auto operator *(const Vector4<T1> &a, T2 b) {
    return Vector4<decltype(a.x * b)>(a.x * b, a.y * b, a.z * b, a.w * b);
}

template <typename T1, typename T2>
inline auto operator *(T1 a, const Vector4<T2> & b) {
    return Vector4<decltype(a * b.x)>(a * b.x, a * b.y, a * b.z, a * b.w);
}

template <typename T1, typename T2>
inline auto operator *(const Vector4<T1> &a, const Vector4<T2> &b) {
    return Vector4<decltype(a.x * b.x)>(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

template <typename T1, typename T2>
inline auto operator /(const Vector4<T1> &a, T2 b) {
    return Vector4<decltype(a.x / b)>(a.x / b, a.y / b, a.z / b, a.w / b);
}

template <typename T1, typename T2>
inline auto operator /(const Vector4<T1> &a, const Vector4<T2> &b) {
    return Vector4<decltype(a.x / b.x)>(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}
/*
#ifdef NATIVE
template <typename T>
std::ostream &operator <<(std::ostream &s, const Vector4<T> &value) {
    s << value.x << ' ' << value.y << ' ' << value.z << ' ' << value.w;
    return s;
}
#endif
*/

template <typename T, typename F>
struct Vector4Serializer {
    // vector to serialize
    const Vector4<T> &v;

    // function that does the serialization
    const F &f;

    // delimiter between vector elements
    String d;
};

template <typename S, typename T, typename F>
S &operator <<(S &stream, Vector4Serializer<T, F> s) {
    s.f(stream, s.v.x);
    stream << s.d;
    s.f(stream, s.v.y);
    stream << s.d;
    s.f(stream, s.v.z);
    stream << s.d;
    s.f(stream, s.v.w);
    return stream;
}

template <typename T, typename F>
Vector4Serializer<T, F> vector(const Vector4<T> &vector, const F &function, String delimiter) {
    return {vector, function, delimiter};
}


using int4 = Vector4<int>;
using float4 = Vector4<float>;
using double4 = Vector4<double>;

} // namespace coco
