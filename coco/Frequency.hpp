#pragma once

#include <cstdint>


// Duration and Time
namespace coco {

struct Frequency {
	uint32_t value;

	/**
		Comparison
	*/
	constexpr bool operator ==(Frequency b) const {
		return this->value == b.value;
	}

	/**
		Multiply
	*/
	constexpr Frequency operator *(uint32_t factor) const {
		return {this->value * factor};
	}

	/**
		Divide by integer divisor with rounding
	*/
	constexpr Frequency operator /(uint32_t divisor) const {
		return {(this->value + (divisor >> 1)) / divisor};
	}

	/**
		Divide by frequency divisor with rounding
	*/
	constexpr uint32_t operator /(Frequency divisor) const {
		return (this->value + (divisor.value >> 1)) / divisor.value;
	}
};


namespace literals {

/**
   Suffix for megahertz, e.g. 100MHz
*/
constexpr Frequency operator "" MHz(unsigned long long f) {
	return {uint32_t(f * 1000000)};
}

/**
	Suffix for kilohertz, e.g. 5kHz
*/
constexpr Frequency operator "" kHz(unsigned long long f) {
	return {uint32_t(f * 1000)};
}

/**
	Suffix for hertz, e.g. 3Hz
*/
constexpr Frequency operator "" Hz(unsigned long long f) {
	return {uint32_t(f)};
}

}

using namespace literals;

} // namespace coco
