#pragma once

#ifdef NATIVE
#include <iostream>
#endif


namespace coco {

/**
 * Vector with four dimensions
 * @tparam T base type
 */
template <typename T>
struct Vector4 {
    T x;
    T y;
    T z;
    T w;

    Vector4() : x(), y(), z(), w() {}
    Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

    Vector4(const Vector4 &v) = default;

    /**
     * Cast from vector with different base type
     */
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

    /**
     * Boradcast x
     */
    Vector4 xxxx() {return {this->x, this->x, this->x, this->x};}

    /**
     * Boradcast y
     */
    Vector4 yyyy() {return {this->y, this->y, this->y, this->y};}

    /**
     * Boradcast z
     */
    Vector4 zzzz() {return {this->z, this->z, this->z, this->z};}
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
    return Vector4<decltype(a.x + b)>(a.x + b, a.y + b, a.z + b);
}

template <typename T1, typename T2>
inline auto operator +(T1 a, const Vector4<T2> &b) {
    return Vector4<decltype(a + b.x)>(a + b.x, a + b.y, a + b.z);
}

template <typename T1, typename T2>
inline auto operator +(const Vector4<T1> &a, const Vector4<T2> &b) {
    return Vector4<decltype(a.x + b.x)>(a.x + b.x, a.y + b.y, a.z + b.z);
}

template <typename T1, typename T2>
inline auto operator -(const Vector4<T1> &a, T2 b) {
    return Vector4<decltype(a.x - b)>(a.x - b, a.y - b, a.z - b);
}

template <typename T1, typename T2>
inline auto operator -(T1 a, const Vector4<T2> &b) {
    return Vector4<decltype(a - b.x)>(a - b.x, a - b.y, a - b.z);
}

template <typename T1, typename T2>
inline auto operator -(const Vector4<T1> &a, const Vector4<T2> &b) {
    return Vector4<decltype(a.x - b.x)>(a.x - b.x, a.y - b.y, a.z - b.z);
}

template <typename T1, typename T2>
inline auto operator *(const Vector4<T1> &a, T2 b) {
    return Vector4<decltype(a.x * b)>(a.x * b, a.y * b, a.z * b);
}

template <typename T1, typename T2>
inline auto operator *(T1 a, const Vector4<T2> & b) {
    return Vector4<decltype(a * b.x)>(a * b.x, a * b.y, a * b.z);
}

template <typename T1, typename T2>
inline auto operator *(const Vector4<T1> &a, const Vector4<T2> &b) {
    return Vector4<decltype(a.x * b.x)>(a.x * b.x, a.y * b.y, a.z * b.z);
}

template <typename T1, typename T2>
inline auto operator /(const Vector4<T1> &a, T2 b) {
    return Vector4<decltype(a.x / b)>(a.x / b, a.y / b, a.z / b);
}

template <typename T1, typename T2>
inline auto operator /(const Vector4<T1> &a, const Vector4<T2> &b) {
    return Vector4<decltype(a.x / b.x)>(a.x / b.x, a.y / b.y, a.z / b.z);
}

#ifdef NATIVE
template <typename T>
std::ostream &operator <<(std::ostream &s, const Vector4<T> &value) {
    s << value.x << ' ' << value.y << ' ' << value.z << ' ' << value.w;
    return s;
}
#endif


using int4 = Vector4<int>;
using float4 = Vector4<float>;
using double4 = Vector4<double>;

} // namespace coco
