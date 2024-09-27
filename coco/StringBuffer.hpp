#pragma once

#include "ArrayBuffer.hpp"


namespace coco {

/**
    String buffer with fixed maximum length (is a Buffer<char, N>)
    Usage: StringBuffer<20> b;
    @tparam N number of 8 bit chars in the buffer
*/

template <int N>
using StringBuffer = ArrayBuffer<char, N>;

} // namespace coco
