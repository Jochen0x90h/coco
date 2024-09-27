#pragma once

#include "Unit.hpp"


namespace coco {

// duration
template <typename T, int P>
using Duration = Unit<T, P, 1>;
template <typename T = int>
using Nanoseconds = Duration<T, -9>;
template <typename T = int>
using Microseconds = Duration<T, -6>;
template <typename T = int>
using Milliseconds = Duration<T, -3>;
template <typename T = int>
using Seconds = Duration<T, 0>;

// time
template <typename T, int P>
using Time = AbsoluteUnit<T, P, 1>;
template <typename T = int>
using TimeNanoseconds = Time<T, -9>;
template <typename T = int>
using TimeMicroseconds = Time<T, -6>;
template <typename T = int>
using TimeMilliseconds = Time<T, -3>;
template <typename T = int>
using TimeSeconds = Time<T, 0>;


namespace literals {

/**
	Suffix for nanoseconds, e.g. 10ns
*/
constexpr Nanoseconds<> operator "" ns(unsigned long long d) {
	return Nanoseconds(int(d));
}

/**
	Suffix for microseconds, e.g. 50us
*/
constexpr Microseconds<> operator "" us(unsigned long long d) {
	return Microseconds(int(d));
}

/**
	Suffix for milliseconds, e.g. 100ms
*/
constexpr Milliseconds<> operator "" ms(unsigned long long d) {
	return Milliseconds(int(d));
}

/**
	Suffix for seconds, e.g. 5s
*/
constexpr Seconds<> operator "" s(unsigned long long d) {
	return Seconds(int(d));
}

/**
	Suffix for minutes, e.g. 3min
*/
constexpr Seconds<> operator "" min(unsigned long long d) {
	return Seconds(int(d * 60));
}

/**
	Suffix for hours, e.g. 8h
*/
constexpr Seconds<> operator "" h(unsigned long long d) {
	return Seconds(int(d * 60 * 60));
}

} // namespace literals

using namespace literals;

} // namespace coco
