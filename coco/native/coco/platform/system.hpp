#pragma once

#include <coco/StringBuffer.hpp>

namespace coco {
namespace system {

inline int device() {
    return 0;
}

inline int revision() {
    return 0;
}

/// @brief Get system id.
/// @return System id
inline StringBuffer<16> id() {
    StringBuffer<16> result;

    // todo: obtain os name
    result << "native";
    return result;
}

} // namespace system
} // namespace coco
