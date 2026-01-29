#pragma once

#include <algorithm>
#include <concepts>
#include <limits>


namespace coco {

/// @brief Unit class that supports powers of ten (e.g. 10^-3 for milli), powers of units such as seconds (e.g. s^2 or s^-1 for Hz)
/// and relative/absolute flag to be able to distinguish between absolute time and duration.
/// Also see https://github.com/nholthaus/units/blob/master/include/units.h
///
/// @tparam T Base type such as int
/// @tparam P Factor exponent of ten (3 for 1000/kilo, 6 for mega, -3 for milli, -6 for mico etc.)
/// @tparam U1 Uit exponent (1 for seconds, -1 for Hz)
/// @tparam A Absolute value as opposed to relative value (Time is absolute, Duration is relative)
template <typename T, int P, int U1, bool A = false>
struct Unit {
    static constexpr int UNIT1 = U1;
    T value;

    Unit() = default;

    /// @brief Constructor from value
    /// @param value Value to trat as unit
    constexpr explicit Unit(T value) : value(value)
    {
    }

    /// @brief Constructor from other decimal power with same type and greater or equal power of ten
    ///
    template <typename T2, int P2, bool A2> requires (std::same_as<T2, T> && P2 >= P && A2 == A)
    constexpr Unit(const Unit<T2, P2, U1, A2> &b) : value(b.value * pow10<P2 - P>())
    {
    }

    /// @brief Explicit constructor from other decimal power with possible loss of precision
    ///
    template <typename T2, int P2, bool A2> requires (!std::same_as<T2, T> || P2 < P || A2 != A)
    constexpr explicit Unit(const Unit<T2, P2, U1, A2> &b) : value(b.value / pow10<P - P2>())
    {
    }

    /// @brief Add and assign
    ///
    template <int P2> requires (P2 >= P)
    auto &operator +=(const Unit<T, P2, U1, false> &b) {
        this->value += b.value * pow10<P2 - P>();
        return *this;
    }

    /// @brief Subtract and assign
    ///
    template <int P2> requires (P2 >= P)
    auto &operator -=(const Unit<T, P2, U1, false> &b) {
        this->value -= b.value * pow10<P2 - P>();
        return *this;
    }

    /// @brief Multiply assign with factor of base type
    ///
    auto &operator *=(T b) {
        this->value *= b;
        return *this;
    }

    /// @brief Multiply and assign
    ///
    template <int P2>
    auto &operator *=(const Unit<T, P2, U1> &b) {
        this->value *= b.value * pow10<P2>();
        this->value /= pow10<-P2>();
        return *this;
    }

    /// @brief Divide assign with divisor of base type
    ///
    auto &operator /=(T b) {
        this->value /= b;
        return *this;
    }

    /// @brief Divide and assign
    ///
    template <int P2>
    auto &operator /=(const Unit<T, P2, U1> &b) {
        this->value *= pow10<-P2>();
        this->value /= b.value * pow10<P2>();
        return *this;
    }

    /*template <int P2>
    constexpr T operator /(Unit<T, P2> b) const {
        uint32_t x = this->value * pow10<P - P2>() / pow10<P2 - P>();
        uint32_t y = b.value;
        return x / y;
    }*/

    /// @brief Convert to int with decimal power
    ///
    constexpr explicit operator short() const requires (!(P == 0 && U1 == 0 && std::signed_integral<T> && sizeof(T) <= sizeof(short))) {return this->value * pow10<P>() / pow10<-P>();}
    constexpr explicit operator int() const requires (!(P == 0 && U1 == 0 && std::signed_integral<T> && sizeof(T) <= sizeof(int))) {return this->value * pow10<P>() / pow10<-P>();}
    constexpr explicit operator long() const requires (!(P == 0 && U1 == 0 && std::signed_integral<T> && sizeof(T) <= sizeof(long))) {return this->value * pow10<P>() / pow10<-P>();}
    constexpr explicit operator long long() const requires (!(P == 0 && U1 == 0 && std::signed_integral<T> && sizeof(T) <= sizeof(long long))) {return this->value * pow10<P>() / pow10<-P>();}

    constexpr operator short() const requires (P == 0 && U1 == 0 && std::signed_integral<T> && sizeof(T) <= sizeof(short)) {return this->value;}
    constexpr operator int() const requires (P == 0 && U1 == 0 && std::signed_integral<T> && sizeof(T) <= sizeof(int)) {return this->value;}
    constexpr operator long() const requires (P == 0 && U1 == 0 && std::signed_integral<T> && sizeof(T) <= sizeof(long)) {return this->value;}
    constexpr operator long long() const requires (P == 0 && U1 == 0 && std::signed_integral<T> && sizeof(T) <= sizeof(long long)) {return this->value;}

    /// @brief Convert to float with decimal power
    ///
    constexpr explicit operator float() const {return float(this->value) * float(pow10<P>()) / float(pow10<-P>());}
    constexpr explicit operator double() const {return double(this->value) * double(pow10<P>()) / double(pow10<-P>());}

    /// @brief Maximum value
    ///
    static constexpr auto max() {return Unit(std::numeric_limits<T>::max());}

    // helper function
    template <int P2>
    static constexpr int pow10() {
        int p = 1;
        for (int i = 0; i < P2; ++i)
            p *= 10;
        return p;
    }
};

/// @brief Equality operator
///
template <typename T, int P1, int P2, int U1, bool A1, bool A2> requires (A1 == A2)
bool operator ==(const Unit<T, P1, U1, A1> &a, const Unit<T, P2, U1, A2> &b) {
    return a.value * Unit<int, 0, 0>::pow10<P1 - P2>() == b.value * Unit<int, 0, 0>::pow10<P2 - P1>();
}

/// @brief "Starship" comparison operator for relative values
///
template <typename T, int P1, int P2, int U1>
auto operator <=>(const Unit<T, P1, U1, false> &a, const Unit<T, P2, U1, false> &b) {
    return a.value * Unit<int, 0, 0>::pow10<P1 - P2>() <=> b.value * Unit<int, 0, 0>::pow10<P2 - P1>();
}

/// @brief "Starship" comparison operator for absolute values
///
template <typename T, int P1, int P2, int U1>
auto operator <=>(const Unit<T, P1, U1, true> &a, const Unit<T, P2, U1, true> &b) {
    return std::make_signed_t<T>(a.value * Unit<int, 0, 0>::pow10<P1 - P2>() - b.value * Unit<int, 0, 0>::pow10<P2 - P1>()) <=> 0;
}

/// @brief Unary plus
///
template <typename T, int P, int U1>
constexpr auto operator +(const Unit<T, P, U1, false> &a) {
    return Unit<decltype(+a.value), P, U1, false>(+a.value);
}

/// @brief Unary minus
///
template <typename T, int P, int U1>
constexpr auto operator -(const Unit<T, P, U1, false> &a) {
    return Unit<decltype(-a.value), P, U1, false>(-a.value);
}

/// @brief Unary star to convert from relative from absolute
///
template <typename T, int P, int U1>
constexpr auto operator *(const Unit<T, P, U1, false> &a) {
    return Unit<T, P, U1, true>(a.value);
}

/// @brief Add
///
template <typename T1, typename T2, int P1, int P2, int U1, bool A1, bool A2> requires (!(A1 && A2))
constexpr auto operator +(const Unit<T1, P1, U1, A1> &a, const Unit<T2, P2, U1, A2> &b) {
    return Unit<decltype(a.value + b.value), P1 < P2 ? P1 : P2, U1, A1 | A2>(a.value * Unit<int, 0, 0>::pow10<P1 - P2>() + b.value * Unit<int, 0, 0>::pow10<P2 - P1>());
}

/// @brief Subtract
///
template <typename T1, typename T2, int P1, int P2, int U1, bool A1, bool A2> requires (A1 || !A2)
constexpr auto operator -(const Unit<T1, P1, U1, A1> &a, const Unit<T2, P2, U1, A2> &b) {
    return Unit<decltype(a.value - b.value), P1 < P2 ? P1 : P2, U1, A1 ^ A2>(a.value * Unit<int, 0, 0>::pow10<P1 - P2>() - b.value * Unit<int, 0, 0>::pow10<P2 - P1>());
}

/// @brief Multiply with factor of base type
///
template <typename T1, typename T2, int P, int U1>
constexpr auto operator *(T1 a, const Unit<T2, P, U1, false> &b) {
    return Unit<decltype(a * b.value), P, U1, false>(a * b.value);
}

/// @brief Multiply with factor of base type
///
template <typename T1, typename T2, int P, int U1>
constexpr auto operator *(const Unit<T1, P, U1, false> &a, T2 b) {
    return Unit<decltype(a.value * b), P, U1, false>(a.value * b);
}

/// @brief Multiply
///
template <typename T1, typename T2, int P1, int P2, int U11, int U12>
constexpr auto operator *(const Unit<T1, P1, U11, false> &a, const Unit<T2, P2, U12, false> &b) {
    return Unit<decltype(a.value * b.value), P1 + P2, U11 + U12, false>(a.value * b.value);
}

/// @brief Divide by factor of base type
///
template <typename T1, typename T2, int P, int U1>
constexpr auto operator /(const Unit<T1, P, U1, false> &a, T2 b) {
    return Unit<decltype(a.value / b), P, U1, false>(a.value / b);
}

/// @brief Divide
///
template <typename T1, typename T2, int P1, int P2, int U11, int U12>
constexpr auto operator /(const Unit<T1, P1, U11, false> &a, const Unit<T2, P2, U12, false> &b) {
    return Unit<decltype(a.value / b.value), std::min(P1, P2) - P2, U11 - U12, false>(a.value * Unit<int, 0, 0>::pow10<P1 - P2>() / b.value);
}

/// @brief Modulus
///
template <typename T1, typename T2, int P1, int P2, int U11, int U12>
constexpr auto operator %(const Unit<T1, P1, U11, false> &a, const Unit<T2, P2, U12, false> &b) {
    return Unit<decltype(a.value % b.value), P1, U11, false>(a.value % (b.value / Unit<int, 0, 0>::pow10<P1 - P2>()));
}

/// @brief Minimum of two durations
///
template <typename T, int P, int U1, bool A>
inline Unit<T, P, U1, A> min(const Unit<T, P, U1, A> &a, const Unit<T, P, U1, A> &b) {return {a < b ? a : b};}

/// @brief Maximum of two durations
///
template <typename T, int P, int U1, bool A>
inline Unit<T, P, U1, A> max(const Unit<T, P, U1, A> &a, const Unit<T, P, U1, A> &b) {return {a > b ? a : b};}



template <typename T, int P, int U1>
using AbsoluteUnit = Unit<T, P, U1, true>;


} // namespace coco
