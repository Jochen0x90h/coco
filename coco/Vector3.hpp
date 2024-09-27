#pragma once

#ifdef NATIVE
#include <iostream>
#endif


namespace coco {

/**
 * Vector with three dimensions
 * @tparam T base type
 */
template <typename T>
struct Vector3 {
    T x;
    T y;
    T z;

    Vector3() : x(), y(), z() {}
    Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

    Vector3(const Vector3 &v) = default;

    /**
     * Cast from vector with different base type
     */
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

    /**
     * Left rotate x, y and z
     */
    Vector3 yzx() {return {this->y, this->z, this->x};}

    /**
     * Right rotate x, y and z
     */
    Vector3 zxy() {return {this->z, this->x, this->y};}

    /**
     * Boradcast x
     */
    Vector3 xxx() {return {this->x, this->x, this->x};}

    /**
     * Boradcast y
     */
    Vector3 yyy() {return {this->y, this->y, this->y};}

    /**
     * Boradcast z
     */
    Vector3 zzz() {return {this->z, this->z, this->z};}
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

#ifdef NATIVE
template <typename T>
std::ostream &operator <<(std::ostream &s, const Vector3<T> &value) {
    s << value.x << ' ' << value.y << ' ' << value.z;
    return s;
}
#endif


using int3 = Vector3<int>;
using float3 = Vector3<float>;
using double3 = Vector3<double>;

} // namespace coco
