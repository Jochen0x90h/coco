#pragma once

#ifdef NATIVE
#include <iostream>
#endif


namespace coco {

/**
 * Vector with two dimensions
 * @tparam T base type
 */
template <typename T>
struct Vector2 {
    T x;
    T y;

    Vector2() : x(), y() {}
    Vector2(T x, T y) : x(x), y(y) {}

    Vector2(const Vector2 &v) = default;

    /**
     * Cast from vector with different base type
     */
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

    /**
     * Swap x and y
     */
    Vector2 yx() {return {this->y, this->x};}

    /**
     * Boradcast x
     */
    Vector2 xx() {return {this->x, this->x};}

    /**
     * Boradcast y
     */
    Vector2 yy() {return {this->y, this->y};}
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

#ifdef NATIVE
template <typename T>
std::ostream &operator <<(std::ostream &s, const Vector2<T> &value) {
    s << value.x << ' ' << value.y;
    return s;
}
#endif


using int2 = Vector2<int>;
using float2 = Vector2<float>;
using double2 = Vector2<double>;

} // namespace coco
