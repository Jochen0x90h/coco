#pragma once

#include <coco/StringBuffer.hpp>

namespace coco {

/// @brief Get system id.
/// @return System id
inline StringBuffer<16> id() {
    StringBuffer<16> result;

    // todo: obtain os name
    result << "native";
    return result;
}

} // namespace coco
