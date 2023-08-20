#pragma once

#include "convert.hpp"


/*
 * Number formats with stream operators, useful in conjuncton with streams:
 * s << "foo" << dec(50) << hex(0xbeef) << flt(3.14f, 1, 2);
 */
namespace coco {

/*
This is how a stream class should look like:

class Stream {
	// write a single character, this can be a virtual method
	(virtual) void write(char ch);

	// write a string, this can be a virtual method
	(virtual) void write(const String &str);

	// stream a single character using write()
	Stream &operator <<(char ch) {
		write(ch);
		return *this;
	}

	// stream a string using write(), either C-string or coco::String
	template <typename T> requires StringConcept<T>
	Stream &operator <<(T const &str) {
		write(str);
		return *this;
	}
};
*/


// decimal number
template <typename T>
struct Dec {
	T value;
	int digitCount;
};

template <typename S, typename T>
S &operator <<(S &s, Dec<T> dec) {
	char buffer[16];
	return s << toString(buffer, int64_t(dec.value), dec.digitCount);
}

/**
 * Decimal number for stream operators
 * Use like s << dec(50) << "foo"
 * @param value decimal value
 * @param digitCount number of digits for the decimal value
 */
template <typename T>
Dec<T> dec(T value, int digitCount = 1) {
	return {value, digitCount};
}


// hexadecimal number
template <typename T>
struct Hex {
	T value;
	int digitCount;
};

template <typename S, typename T>
S &operator <<(S &s, Hex<T> hex) {
	char buffer[16];
	return s << toHexString(buffer, uint64_t(hex.value), hex.digitCount);
}

/**
 * Hex number for stream operators
 * Use like s << hex(0xbeef) << "foo"
 * @param value hex value
 * @param digitCount number of digits for the hex value
 */
template <typename T>
Hex<T> hex(T value, int digitCount = sizeof(T) * 2) {
	return {value, digitCount};
}


// floating point number
struct Flt {
	float value;

	// number of digits before the decimal point (e.g. 2 for 00.0)
	int digitCount;

	// maximum number of decimals (e.g. 3 for 0.123). Negative value for fixed number of decimals (e.g. -3 for 0.000)
	int decimalCount;
};

template <typename S>
inline S &operator <<(S &s, Flt flt) {
	char buffer[21];
	return s << toString(buffer, flt.value, flt.digitCount, flt.decimalCount);
}

/**
 * Create a float value wrapper with parameters for printing
 * @param value float value
 * @param decimalCount minimum number of digits to convert after the decimal point, negative to keep trailing zeros
 * @return float value wrapper
 */
constexpr Flt flt(float value, int decimalCount = 3) {
	return {value, 1, decimalCount};
}

/**
 * Create a float value wrapper with parameters for printing
 * @param value value
 * @param digitCount minimum number of digits to convert before the decimal point
 * @param decimalCount minimum number of digits to convert after the decimal point, negative to keep trailing zeros
 * @return float value wrapper
 */
constexpr Flt flt(float value, int digitCount, int decimalCount) {
	return {value, digitCount, decimalCount};
}


} // namespace coco
