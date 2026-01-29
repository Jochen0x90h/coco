#pragma once

#include "ArrayBuffer.hpp"


namespace coco {


/// @brief String buffer with fixed maximum length.
/// Usage: StringBuffer<20> b;
/// Note that this is an ArrayBuffer of type char which is a partial specialization of ArrayBuffer.
/// @tparam N number of 8 bit chars in the buffer
template <int N>
using StringBuffer = ArrayBuffer<char, N>;

} // namespace coco
