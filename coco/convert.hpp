#pragma once

#include "Array.hpp"
#include "String.hpp"
#include <optional>


namespace coco {

/**
 * Convert string to integer
 * @param str string to convert
 * @return optional int, defined if conversion was successful
 */
std::optional<int> parseInt(String str);

/**
 * Convert string to floating point number in the form x.y without support for exponential notation
 * @param str string to convert
 * @return optional floating point number, defined if conversion was successful
 */
std::optional<float> parseFloat(String str);

/**
 * Convert a 64 bit integer to string
 * @param buffer buffer in which conversion takes place
 * @param value value to convert
 * @param digitCount minimum number of digits to convert, pad smaller numbers with leading zeros, maximum is 15
 * @return string containing the number, references the input buffer
 */
String toString(Array<char, 16> buffer, int64_t value, int digitCount = 1);

/**
 * Convert a 64 bit unsigned integer to hex string
 * @param buffer buffer in which conversion takes place
 * @param value value to convert
 * @param digitCount number of hex digits to convert
 * @return string containing the number, references the input buffer
 */
String toHexString(Array<char, 16> buffer, uint64_t value, int digitCount);

/**
 * Convert a float to string
 * @param buffer buffer in which conversion takes place
 * @param value value to convert
 * @param digitCount minimum number of digits to convert before the decimal point
 * @param decimalCount minimum number of digits to convert after the decimal point, negative to keep trailing zeros
 * @return string containing the number, references the input buffer
 */
String toString(Array<char, 21> buffer, float value, int digitCount, int decimalCount);

} // namespace coco
