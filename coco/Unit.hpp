#pragma once

#include <algorithm>
#include <concepts>
#include <limits>


namespace coco {

/**
	Unit class that supports powers of ten (e.g. 10^-3 for milli), powers of units such as seconds (e.g. s^2 or s^-1 for Hz)
	and relative/absolute flag to be able to distinguish between absolute time and duration.
	Also see https://github.com/nholthaus/units/blob/master/include/units.h

	@tparam T base type such as int
	@tparam P factor given as power of ten such as 3 for 1000
	@tparam U1 unit exponent (3 for kilo, 6 for mega, -3 for milli, -6 for mico etc.)
	@tparam A absolute value as opposed to relative value (Time is absolute, Duration is relative)
*/
template <typename T, int P, int U1, bool A = false>
struct Unit {
	T value;

	Unit() = default;

	/**
		Constructor from value
	*/
	constexpr explicit Unit(T value) : value(value)
	{
	}

	/**
		Constructor from other decimal power with same type and greater or equal power of ten
	*/
	template <typename T2, int P2, bool A2> requires (std::same_as<T2, T> && P2 >= P && A2 == A)
	constexpr Unit(const Unit<T2, P2, U1, A2> &b) : value(b.value * pow10<P2 - P>())
	{
	}

	/**
		Explicit constructor from other decimal power with possible loss of precision
	*/
	template <typename T2, int P2, bool A2> requires (!std::same_as<T2, T> || P2 < P || A2 != A)
	constexpr explicit Unit(const Unit<T2, P2, U1, A2> &b) : value(b.value / pow10<P - P2>())
	{
	}

	/**
		Add and assign
	*/
	template <int P2> requires (P2 >= P)
	auto &operator +=(const Unit<T, P2, U1, false> &b) {
		this->value += b.value * pow10<P2 - P>();
		return *this;
	}

	/**
		Subtract and assign
	*/
	template <int P2> requires (P2 >= P)
	auto &operator -=(const Unit<T, P2, U1, false> &b) {
		this->value -= b.value * pow10<P2 - P>();
		return *this;
	}

	/**
		Multiply assign with factor of base type
	*/
	auto &operator *=(T b) {
		this->value *= b;
		return *this;
	}

	/**
		Multiply and assign
	*/
	template <int P2>
	auto &operator *=(const Unit<T, P2, U1> &b) {
		this->value *= b.value * pow10<P2>();
		this->value /= pow10<-P2>();
		return *this;
	}

	/**
		Divide assign with divisor of base type
	*/
	auto &operator /=(T b) {
		this->value /= b;
		return *this;
	}

	/**
		Divide and assign
	*/
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

	/**
		Convert to int with decimal power
	*/
	constexpr explicit operator int() const {return this->value * pow10<P>() / pow10<-P>();}
	constexpr explicit operator short() const {return this->value * pow10<P>() / pow10<-P>();}
	constexpr explicit operator long() const {return this->value * pow10<P>() / pow10<-P>();}

	/**
		Convert to float with decimal power
	*/
	constexpr explicit operator float() const {return float(this->value) * float(pow10<P>()) / float(pow10<-P>());}
	constexpr explicit operator double() const {return double(this->value) * double(pow10<P>()) / double(pow10<-P>());}

	/**
		Maximum value
	*/
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

/**
	Equality operator
*/
template <typename T, int P1, int P2, int U1, bool A1, bool A2> requires (A1 == A2)
bool operator ==(const Unit<T, P1, U1, A1> &a, const Unit<T, P2, U1, A2> &b) {
	return a.value * Unit<int, 0, 0>::pow10<P1 - P2>() == b.value * Unit<int, 0, 0>::pow10<P2 - P1>();
}

/**
	"Starship" comparison operator for relative values
*/
template <typename T, int P1, int P2, int U1>
auto operator <=>(const Unit<T, P1, U1, false> &a, const Unit<T, P2, U1, false> &b) {
	return a.value * Unit<int, 0, 0>::pow10<P1 - P2>() <=> b.value * Unit<int, 0, 0>::pow10<P2 - P1>();
}

/**
	"Starship" comparison operator for absolute values
*/
template <typename T, int P1, int P2, int U1>
auto operator <=>(const Unit<T, P1, U1, true> &a, const Unit<T, P2, U1, true> &b) {
	return std::make_signed_t<T>(a.value * Unit<int, 0, 0>::pow10<P1 - P2>() - b.value * Unit<int, 0, 0>::pow10<P2 - P1>()) <=> 0;
}

/**
	Unary plus
*/
template <typename T, int P, int U1>
constexpr auto operator +(const Unit<T, P, U1, false> &a) {
	return Unit<decltype(+a.value), P, U1, false>(+a.value);
}

/**
	Unary minus
*/
template <typename T, int P, int U1>
constexpr auto operator -(const Unit<T, P, U1, false> &a) {
	return Unit<decltype(-a.value), P, U1, false>(-a.value);
}

/**
	Unary star to convert from relative from absolute
*/
template <typename T, int P, int U1>
constexpr auto operator *(const Unit<T, P, U1, false> &a) {
	return Unit<T, P, U1, true>(a.value);
}

/**
	Add
*/
template <typename T1, typename T2, int P1, int P2, int U1, bool A1, bool A2> requires (!(A1 && A2))
constexpr auto operator +(const Unit<T1, P1, U1, A1> &a, const Unit<T2, P2, U1, A2> &b) {
	return Unit<decltype(a.value + b.value), P1 < P2 ? P1 : P2, U1, A1 | A2>(a.value * Unit<int, 0, 0>::pow10<P1 - P2>() + b.value * Unit<int, 0, 0>::pow10<P2 - P1>());
}

/**
	Subtract
*/
template <typename T1, typename T2, int P1, int P2, int U1, bool A1, bool A2> requires (A1 || !A2)
constexpr auto operator -(const Unit<T1, P1, U1, A1> &a, const Unit<T2, P2, U1, A2> &b) {
	return Unit<decltype(a.value - b.value), P1 < P2 ? P1 : P2, U1, A1 ^ A2>(a.value * Unit<int, 0, 0>::pow10<P1 - P2>() - b.value * Unit<int, 0, 0>::pow10<P2 - P1>());
}

/**
	Multiply with factor of base type
*/
template <typename T1, typename T2, int P, int U1>
constexpr auto operator *(T1 a, const Unit<T2, P, U1, false> &b) {
	return Unit<decltype(a * b.value), P, U1, false>(a * b.value);
}

/**
	Multiply with factor of base type
*/
template <typename T1, typename T2, int P, int U1>
constexpr auto operator *(const Unit<T1, P, U1, false> &a, T2 b) {
	return Unit<decltype(a.value * b), P, U1, false>(a.value * b);
}

/**
	Multiply
*/
template <typename T1, typename T2, int P1, int P2, int U11, int U12>
constexpr auto operator *(const Unit<T1, P1, U11, false> &a, const Unit<T2, P2, U12, false> &b) {
	return Unit<decltype(a.value * b.value), P1 + P2, U11 + U12, false>(a.value * b.value);
}

/**
	Divide by factor of base type
*/
template <typename T1, typename T2, int P, int U1>
constexpr auto operator /(const Unit<T1, P, U1, false> &a, T2 b) {
	return Unit<decltype(a.value / b), P, U1, false>(a.value / b);
}

/**
	Divide
*/
template <typename T1, typename T2, int P1, int P2, int U11, int U12>
constexpr auto operator /(const Unit<T1, P1, U11, false> &a, const Unit<T2, P2, U12, false> &b) {
	return Unit<decltype(a.value / b.value), std::min(P1, P2) - P2, U11 - U12, false>(a.value * Unit<int, 0, 0>::pow10<P1 - P2>() / b.value);
}

/**
	Modulus
*/
template <typename T1, typename T2, int P1, int P2, int U11, int U12>
constexpr auto operator %(const Unit<T1, P1, U11, false> &a, const Unit<T2, P2, U12, false> &b) {
	return Unit<decltype(a.value % b.value), P1, U11, false>(a.value % (b.value / Unit<int, 0, 0>::pow10<P1 - P2>()));
}

/**
	Minimum of two durations
*/
template <typename T, int P, int U1, bool A>
inline Unit<T, P, U1, A> min(const Unit<T, P, U1, A> &a, const Unit<T, P, U1, A> &b) {return {a < b ? a : b};}

/**
	Maximum of two durations
*/
template <typename T, int P, int U1, bool A>
inline Unit<T, P, U1, A> max(const Unit<T, P, U1, A> &a, const Unit<T, P, U1, A> &b) {return {a > b ? a : b};}



template <typename T, int P, int U1>
using AbsoluteUnit = Unit<T, P, U1, true>;


} // namespace coco
