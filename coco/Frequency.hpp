#pragma once

#include "Unit.hpp"


namespace coco {

// frequency
template <typename T, int P>
using Frequency = Unit<T, P, -1>;
template <typename T = int>
using Hertz = Frequency<T, 0>;
template <typename T = int>
using Kilohertz = Frequency<T, 3>;
template <typename T = int>
using Megahertz = Frequency<T, 6>;
template <typename T = int>
using Gigahertz = Frequency<T, 9>;


namespace literals {

/**
	Suffix for hertz, e.g. 3Hz
*/
constexpr Hertz<> operator "" Hz(unsigned long long f) {
	return Hertz(int(f));
}

/**
	Suffix for kilohertz, e.g. 5kHz
*/
constexpr Kilohertz<> operator "" kHz(unsigned long long f) {
	return Kilohertz(int(f));
}

/**
   Suffix for megahertz, e.g. 100MHz
*/
constexpr Megahertz<> operator "" MHz(unsigned long long f) {
	return Megahertz(int(f));
}

/**
   Suffix for gigiahertz, e.g. 5GHz
*/
constexpr Gigahertz<> operator "" GHz(unsigned long long f) {
	return Gigahertz(int(f));
}

} // namespace literals

using namespace literals;

} // namespace coco
