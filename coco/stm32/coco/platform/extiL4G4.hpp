#pragma once

// do not include directly, use #include <coco/platform/exti.hpp>

#include "platform.hpp"


namespace coco {

/// @brief External interrupt helpers.
///
namespace exti {

// Device specific lines
enum class Line : uint8_t;


// functions for single EXTI line using Line enum
// ----------------------------------------------

/// @brief Detect rising edge on an EXTI line.
/// @param line EXTI line
inline void detectRisingEdge(Line line) {
    int l = int(line);
    if (l < 32) {
        uint32_t f = 1 << l;
        EXTI->RTSR1 = EXTI->RTSR1 | f;
        EXTI->FTSR1 = EXTI->FTSR1 & ~f;
    } else {
        uint32_t f = 1 << (l - 32);
        EXTI->RTSR2 = EXTI->RTSR2 | f;
        EXTI->FTSR2 = EXTI->FTSR2 & ~f;
    }
}

inline void detectFallingEdge(Line line) {
    int l = int(line);
    if (l < 32) {
        uint32_t f = 1 << l;
        EXTI->RTSR1 = EXTI->RTSR1 & ~f;
        EXTI->FTSR1 = EXTI->FTSR1 | f;
    } else {
        uint32_t f = 1 << (l - 32);
        EXTI->RTSR2 = EXTI->RTSR2 & ~f;
        EXTI->FTSR2 = EXTI->FTSR2 | f;
    }
}

inline void detectBothEdges(Line line) {
    int l = int(line);
    if (l < 32) {
        uint32_t f = 1 << l;
        EXTI->RTSR1 = EXTI->RTSR1 | f;
        EXTI->FTSR1 = EXTI->FTSR1 | f;
    } else {
        uint32_t f = 1 << (l - 32);
        EXTI->RTSR2 = EXTI->RTSR2 | f;
        EXTI->FTSR2 = EXTI->FTSR2 | f;
    }
}

inline void enableInterrupt(Line line) {
    int l = int(line);
    if (l < 32) {
        uint32_t f = 1 << l;
        EXTI->PR1 = f; // clear pending flags
        EXTI->IMR1 = EXTI->IMR1 | f;
    } else {
        uint32_t f = 1 << (l - 32);
        EXTI->PR2 = f; // clear pending flags
        EXTI->IMR2 = EXTI->IMR2 | f;
    }
}

inline auto pending(Line line) {
    int l = int(line);
    if (l < 32) {
        uint32_t f = 1 << l;
        return (EXTI->PR1 & f) != 0;
    } else {
        uint32_t f = 1 << (l - 32);
        return (EXTI->PR2 & f) != 0;
    }
}

inline void clearPending(Line line) {
    int l = int(line);
    if (l < 32) {
        uint32_t f = 1 << l;
        EXTI->PR1 = f;
    } else {
        uint32_t f = 1 << (l - 32);
        EXTI->PR2 = f;
    }
}


// functions for single GPIO EXTI line (only lines 0 - 15)
// -------------------------------------------------------

/// @brief Set the port for a given GPIO EXTI line (only lines 0 - 15).
/// @param line EXTI line number
/// @param port Port to set (0 = A, 1 = B, 2 = C, 3 = D, 4 = E, 5 = F, 6 = G, 7 = H)
inline void setPort(int line, int port) {
    int pos4 = (line & 3) * 4;
    auto &EXTICR = SYSCFG->EXTICR[line >> 2];
    EXTICR = (EXTICR & ~(0xf << pos4)) | (port << pos4);
}

/// @brief Detect rising edge on a single EXTI line (only lines 0 - 15).
/// @param line EXTI line
inline void detectRisingEdge(int line) {
    uint32_t f = 1 << line;
    EXTI->RTSR1 = EXTI->RTSR1 | f;
    EXTI->FTSR1 = EXTI->FTSR1 & ~f;
}

inline void detectFallingEdge(int line) {
    uint32_t f = 1 << line;
    EXTI->RTSR1 = EXTI->RTSR1 & ~f;
    EXTI->FTSR1 = EXTI->FTSR1 | f;
}

inline void detectBothEdges(int line) {
    uint32_t f = 1 << line;
    EXTI->RTSR1 = EXTI->RTSR1 | f;
    EXTI->FTSR1 = EXTI->FTSR1 | f;
}

inline void enableInterrupt(int line) {
    uint32_t f = 1 << line;
    EXTI->PR1 = f; // clear pending flags
    EXTI->IMR1 = EXTI->IMR1 | f;
}

inline auto pending(int line) {
    uint32_t f = 1 << line;
    return (EXTI->PR1 & f) != 0;
}

inline void clearPending(int line) {
    uint32_t f = 1 << line;
    EXTI->PR1 = f;
}


// functions for multiple GPIO EXTI lines (only lines 0 - 15)
// ----------------------------------------------------------

/// @brief Detect rising edge on multiple EXTI lines (only lines 0 - 15).
/// @param lineFlags Flags indicating for which EXTI lines to use
inline void detectRisingEdgeFlags(int lineFlags) {
    EXTI->RTSR1 = EXTI->RTSR1 | lineFlags;
    EXTI->FTSR1 = EXTI->FTSR1 & ~lineFlags;
}

inline void detectFallingEdgeFlags(int lineFlags) {
    EXTI->RTSR1 = EXTI->RTSR1 & ~lineFlags;
    EXTI->FTSR1 = EXTI->FTSR1 | lineFlags;
}

inline void detectBothEdgesFlags(int lineFlags) {
    EXTI->RTSR1 = EXTI->RTSR1 | lineFlags;
    EXTI->FTSR1 = EXTI->FTSR1 | lineFlags;
}

inline void enableInterruptFlags(int lineFlags) {
    EXTI->PR1 = lineFlags; // clear pending flags
    EXTI->IMR1 = EXTI->IMR1 | lineFlags;
}

inline auto pendingFlags(int lineFlags) {
    return EXTI->PR1 & lineFlags;
}

inline void clearPendingFlags(int lineFlags) {
    EXTI->PR1 = lineFlags;
}


/*
/// @brief Detect rising edge on EXTIs given by the lineFlags.
/// @tparam F Maximum set of flags to be used.
/// @param lineFlags Flags indicating for which EXTIs to set the rising edge detection.
template <LineFlags F>
inline void detectRisingEdge(LineFlags lineFlags = F) {
    if constexpr ((uint64_t(F) & 0xFFFFFFFF) != 0) {
        uint32_t f = uint32_t(lineFlags);
        EXTI->RTSR1 = EXTI->RTSR1 | f;
        EXTI->FTSR1 = EXTI->FTSR1 & ~f;
    }
    if constexpr ((uint64_t(F) >> 32) != 0) {
        uint32_t f = std::make_unsigned_t<LineFlags>(lineFlags) >> 32;
        EXTI->RTSR2 = EXTI->RTSR2 | f;
        EXTI->FTSR2 = EXTI->FTSR2 & ~f;
    }
}

template <LineFlags F>
inline void detectFallingEdge(LineFlags lineFlags = F) {
    if constexpr ((uint64_t(F) & 0xFFFFFFFF) != 0) {
        uint32_t f = uint32_t(lineFlags);
        EXTI->RTSR1 = EXTI->RTSR1 & ~f;
        EXTI->FTSR1 = EXTI->FTSR1 | f;
    }
    if constexpr ((uint64_t(F) >> 32) != 0) {
        uint32_t f = std::make_unsigned_t<LineFlags>(lineFlags) >> 32;
        EXTI->RTSR2 = EXTI->RTSR2 & ~f;
        EXTI->FTSR2 = EXTI->FTSR2 | f;
    }
}

template <LineFlags F>
inline void detectBothEdges(LineFlags lineFlags = F) {
    if constexpr ((uint64_t(F) & 0xFFFFFFFF) != 0) {
        uint32_t f = uint32_t(lineFlags);
        EXTI->RTSR1 = EXTI->RTSR1 | f;
        EXTI->FTSR1 = EXTI->FTSR1 | f;
    }
    if constexpr ((uint64_t(F) >> 32) != 0) {
        uint32_t f = std::make_unsigned_t<LineFlags>(lineFlags) >> 32;
        EXTI->RTSR2 = EXTI->RTSR2 | f;
        EXTI->FTSR2 = EXTI->FTSR2 | f;
    }
}

template <LineFlags F>
inline void enableInterrupt(LineFlags lineFlags = F) {
    if constexpr ((uint64_t(F) & 0xFFFFFFFF) != 0) {
        uint32_t f = uint32_t(lineFlags);
        EXTI->PR1 = f; // clear pending flags
        EXTI->IMR1 = EXTI->IMR1 | f;
    }
    if constexpr ((uint64_t(F) >> 32) != 0) {
        uint32_t f = std::make_unsigned_t<LineFlags>(lineFlags) >> 32;
        EXTI->PR2 = f; // clear pending flags
        EXTI->IMR2 = EXTI->IMR2 | f;
    }
}

template <LineFlags F>
inline auto pending(LineFlags lineFlags = F) {
    std::make_unsigned_t<LineFlags> p = 0;
    if constexpr ((uint64_t(F) & 0xFFFFFFFF) != 0) {
        uint32_t f = uint32_t(lineFlags);
        p |= EXTI->PR1 & f;
    }
    if constexpr ((uint64_t(F) >> 32) != 0) {
        uint32_t f = std::make_unsigned_t<LineFlags>(lineFlags) >> 32;
        p |= std::make_unsigned_t<LineFlags>(EXTI->PR2 & f) << 32;
    }
    return p;
}

template <LineFlags F>
inline void clearPending(LineFlags lineFlags = F) {
    if constexpr ((uint64_t(F) & 0xFFFFFFFF) != 0) {
        uint32_t f = uint32_t(lineFlags);
        EXTI->PR1 = f; // clear pending flags
    }
    if constexpr ((uint64_t(F) >> 32) != 0) {
        uint32_t f = std::make_unsigned_t<LineFlags>(lineFlags) >> 32;
        EXTI->PR2 = f; // clear pending flags
    }
}
*/

} // namespace exti
} // namespace coco
