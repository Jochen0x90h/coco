#pragma once

#include <coco/StringBuffer.hpp>

namespace coco {
namespace system {

/// @brief Get system name.
/// @return System name (Windows, Darwin, Linux)
inline StringBuffer<16> name() {
#ifdef _WIN32
    return "Windows";
#endif
#ifdef __APPLE__
    return "Darwin";
#endif
#ifdef __linux__
    return "Linux";
#endif
}

/// @brief Get system version.
/// @return System version (e.g. 10 22H2)
inline StringBuffer<16> version() {
    return {};
}

/// Get system id.
/// @return System id, 1: Windows, 2: Darwin, 3: Linux
inline int id() {
#ifdef _WIN32
    return 1;
#endif
#ifdef __APPLE__
    return 2;
#endif
#ifdef __linux__
    return 3;
#endif
}

/// Get system build.
/// @return System build
inline int build() {
    return 0;
}

} // namespace system
} // namespace coco
