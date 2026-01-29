#pragma once

#include "String.hpp"


namespace coco {

/// @brief Vector with three dimensions.
/// @tparam T base type
template <typename T>
struct Vector3 {
    T x;
    T y;
    T z;

    constexpr Vector3() : x(), y(), z() {}
    constexpr Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

    Vector3(const Vector3 &v) = default;

    /// @brief Cast from vector with different base type
    ///
    template <typename T2>
    explicit Vector3(const Vector3<T2> &v) : x(v.x), y(v.y), z(v.z) {}

    Vector3 &operator +=(const T &a) {
        this->x += a;
        this->y += a;
        this->z += a;
        return *this;
    }

    Vector3 &operator +=(const Vector3 &a) {
        this->x += a.x;
        this->y += a.y;
        this->z += a.z;
        return *this;
    }

    Vector3 &operator -=(const T &a) {
        this->x -= a;
        this->y -= a;
        this->z -= a;
        return *this;
    }

    Vector3 &operator -=(const Vector3 &a) {
        this->x -= a.x;
        this->y -= a.y;
        this->z -= a.z;
        return *this;
    }

    Vector3 &operator *=(const T &a) {
        this->x *= a;
        this->y *= a;
        this->z *= a;
        return *this;
    }

    Vector3 &operator *=(const Vector3 &a) {
        this->x *= a.x;
        this->y *= a.y;
        this->z *= a.z;
        return *this;
    }

    Vector3 &operator /=(const T &a) {
        this->x /= a;
        this->y /= a;
        this->z /= a;
        return *this;
    }

    Vector3 &operator /=(const Vector3 &a) {
        this->x /= a.x;
        this->y /= a.y;
        this->z /= a.z;
        return *this;
    }

    /// @brief Left rotate x, y and z
    ///
    Vector3 yzx() {return {this->y, this->z, this->x};}

    /// @brief Right rotate x, y and z
    ///
    Vector3 zxy() {return {this->z, this->x, this->y};}

    /// @brief Boradcast x
    ///
    Vector3 xxx() {return {this->x, this->x, this->x};}

    /// @brief Boradcast y
    ///
    Vector3 yyy() {return {this->y, this->y, this->y};}

    /// @brief Boradcast z
    ///
    Vector3 zzz() {return {this->z, this->z, this->z};}

    int size() {return 3;}
    T *data() {return &this->x;}
    const T *data() const {return &this->x;}

    T *begin() {return &this->x;}
    const T *begin() const {return &this->x;}
    T *end() {return &this->x + 3;}
    const T *end() const {return &this->x + 3;}
};

template <typename T>
inline bool operator ==(const Vector3<T> &a, const Vector3<T> &b) {
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

template <typename T>
inline auto operator +(const Vector3<T> &a) {
    return Vector3<decltype(+a.x)>(+a.x, +a.y, +a.z);
}

template <typename T>
inline auto operator -(const Vector3<T> &a) {
    return Vector3<decltype(-a.x)>(-a.x, -a.y, -a.z);
}

template <typename T1, typename T2>
inline auto operator +(const Vector3<T1> &a, T2 b) {
    return Vector3<decltype(a.x + b)>(a.x + b, a.y + b, a.z + b);
}

template <typename T1, typename T2>
inline auto operator +(T1 a, const Vector3<T2> &b) {
    return Vector3<decltype(a + b.x)>(a + b.x, a + b.y, a + b.z);
}

template <typename T1, typename T2>
inline auto operator +(const Vector3<T1> &a, const Vector3<T2> &b) {
    return Vector3<decltype(a.x + b.x)>(a.x + b.x, a.y + b.y, a.z + b.z);
}

template <typename T1, typename T2>
inline auto operator -(const Vector3<T1> &a, T2 b) {
    return Vector3<decltype(a.x - b)>(a.x - b, a.y - b, a.z - b);
}

template <typename T1, typename T2>
inline auto operator -(T1 a, const Vector3<T2> &b) {
    return Vector3<decltype(a - b.x)>(a - b.x, a - b.y, a - b.z);
}

template <typename T1, typename T2>
inline auto operator -(const Vector3<T1> &a, const Vector3<T2> &b) {
    return Vector3<decltype(a.x - b.x)>(a.x - b.x, a.y - b.y, a.z - b.z);
}

template <typename T1, typename T2>
inline auto operator *(const Vector3<T1> &a, T2 b) {
    return Vector3<decltype(a.x * b)>(a.x * b, a.y * b, a.z * b);
}

template <typename T1, typename T2>
inline auto operator *(T1 a, const Vector3<T2> & b) {
    return Vector3<decltype(a * b.x)>(a * b.x, a * b.y, a * b.z);
}

template <typename T1, typename T2>
inline auto operator *(const Vector3<T1> &a, const Vector3<T2> &b) {
    return Vector3<decltype(a.x * b.x)>(a.x * b.x, a.y * b.y, a.z * b.z);
}

template <typename T1, typename T2>
inline auto operator /(const Vector3<T1> &a, T2 b) {
    return Vector3<decltype(a.x / b)>(a.x / b, a.y / b, a.z / b);
}

template <typename T1, typename T2>
inline auto operator /(const Vector3<T1> &a, const Vector3<T2> &b) {
    return Vector3<decltype(a.x / b.x)>(a.x / b.x, a.y / b.y, a.z / b.z);
}

/*
#ifdef NATIVE
template <typename T>
std::ostream &operator <<(std::ostream &s, const Vector3<T> &value) {
    s << value.x << ' ' << value.y << ' ' << value.z;
    return s;
}
#endif
*/

template <typename T, typename F>
struct Vector3Serializer {
    // vector to serialize
    const Vector3<T> &v;

    // function that does the serialization
    const F &f;

    // delimiter between vector elements
    String d;
};

template <typename S, typename T, typename F>
S &operator <<(S &stream, Vector3Serializer<T, F> s) {
    s.f(stream, s.v.x);
    stream << s.d;
    s.f(stream, s.v.y);
    stream << s.d;
    s.f(stream, s.v.z);
    return stream;
}

template <typename T, typename F>
Vector3Serializer<T, F> vector(const Vector3<T> &vector, const F &function, String delimiter) {
    return {vector, function, delimiter};
}


using int3 = Vector3<int>;
using float3 = Vector3<float>;
using double3 = Vector3<double>;

} // namespace coco
