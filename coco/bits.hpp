#pragma once

#include <type_traits>


namespace coco {

/**
 * get the lowest set bit in a bitfield
 * @param bitfield bitfield to extract from
 * @return lowest set bit
 */
template <typename T>
constexpr int firstBit(T bitfield) {
	return bitfield & ~(bitfield - 1);
}


/**
 * Exract a value from a bitfield using a mask
 * @tparam type of bitfield (e.g. int)
 * @param bitfield bitfield to extract from
 * @param mask mask
 * @return masked value shifted down
 */
template <typename T>
constexpr auto extract(T bitfield, T mask) {
	auto b = std::make_unsigned_t<T>(bitfield);
	auto m = std::make_unsigned_t<T>(mask);
	return (b & m) / (m & ~(m - 1));
}

}
