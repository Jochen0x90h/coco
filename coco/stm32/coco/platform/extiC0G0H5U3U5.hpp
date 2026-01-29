#pragma once

// do not include directly, use #include <coco/platform/exti.hpp>

#include "platform.hpp"


namespace coco {

/// @brief EXTI (external interrupt) helpers.
/// G0X0 https://www.st.com/en/microcontrollers-microprocessors/stm32g0-series/documentation.html Section 12
/// G0X1 https://www.st.com/en/microcontrollers-microprocessors/stm32g0-series/documentation.html Section 13
/// H503 https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series/documentation.html Section 17
/// H5 https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series/documentation.html Section 18
/// U3 https://www.st.com/en/microcontrollers-microprocessors/stm32u3-series/documentation.html Section 17
namespace exti {

// Device specific lines
enum class Line : uint8_t;


// functions for single EXTI line using Line enum
// ----------------------------------------------

/// @brief Detect rising edge on an EXTI line.
/// @param line EXTI line
inline void detectRisingEdge(Line line) {
    int l = int(line);
#if defined(EXTI_RTSR2_RT) || defined(EXTI_RTSR2_RT34)
    if (l >= 32) {
        uint32_t f = 1 << (l - 32);
        EXTI->RTSR2 = EXTI->RTSR2 | f;
        EXTI->FTSR2 = EXTI->FTSR2 & ~f;
    } else
#endif
    {
        uint32_t f = 1 << l;
        EXTI->RTSR1 = EXTI->RTSR1 | f;
        EXTI->FTSR1 = EXTI->FTSR1 & ~f;
    }
}

inline void detectFallingEdge(Line line) {
    int l = int(line);
#if defined(EXTI_RTSR2_RT) || defined(EXTI_RTSR2_RT34)
    if (l >= 32) {
        uint32_t f = 1 << (l - 32);
        EXTI->RTSR2 = EXTI->RTSR2 & ~f;
        EXTI->FTSR2 = EXTI->FTSR2 | f;
    } else
#endif
    {
        uint32_t f = 1 << l;
        EXTI->RTSR1 = EXTI->RTSR1 & ~f;
        EXTI->FTSR1 = EXTI->FTSR1 | f;
    }
}

inline void detectBothEdges(Line line) {
    int l = int(line);
#if defined(EXTI_RTSR2_RT) || defined(EXTI_RTSR2_RT34)
    if (l >= 32) {
        uint32_t f = 1 << (l - 32);
        EXTI->RTSR2 = EXTI->RTSR2 | f;
        EXTI->FTSR2 = EXTI->FTSR2 | f;
    } else
#endif
    {
        uint32_t f = 1 << l;
        EXTI->RTSR1 = EXTI->RTSR1 | f;
        EXTI->FTSR1 = EXTI->FTSR1 | f;
    }
}

inline void enableInterrupt(Line line) {
    int l = int(line);
#if defined(EXTI_RTSR2_RT) || defined(EXTI_RTSR2_RT34)
    if (l >= 32) {
        uint32_t f = 1 << (l - 32);
        EXTI->RPR2 = f; // clear pending flags
        EXTI->FPR2 = f; // clear pending flags
        EXTI->IMR2 = EXTI->IMR2 | f;
    } else
#endif
    {
        uint32_t f = 1 << l;
        EXTI->RPR1 = f; // clear pending flags
        EXTI->FPR1 = f; // clear pending flags
        EXTI->IMR1 = EXTI->IMR1 | f;
    }
}

inline auto pending(Line line) {
    int l = int(line);
#if defined(EXTI_RTSR2_RT) || defined(EXTI_RTSR2_RT34)
    if (l >= 32)
    {
        uint32_t f = 1 << (l - 32);
        return ((EXTI->RPR2 | EXTI->FPR2) & f) != 0;
    } else
#endif
    {
        uint32_t f = 1 << l;
        return ((EXTI->RPR1 | EXTI->FPR1) & f) != 0;
    }
}

inline void clearPending(Line line) {
    int l = int(line);
#if defined(EXTI_RTSR2_RT) || defined(EXTI_RTSR2_RT34)
    if (l >= 32)
    {
        uint32_t f = 1 << (l - 32);
        EXTI->RPR2 = f;
        EXTI->FPR2 = f;
    } else
#endif
    {
        uint32_t f = 1 << l;
        EXTI->RPR1 = f;
        EXTI->FPR1 = f;
    }
}


// functions for single GPIO EXTI line (only lines 0 - 15)
// -------------------------------------------------------

/// @brief Set the port for a given GPIO EXTI line (only lines 0 - 15).
/// @param line EXTI line number
/// @param port Port to set (0 = A, 1 = B, 2 = C, 3 = D, 4 = E, 5 = F, 6 = G, 7 = H)
inline void setPort(int line, int port) {
    int pos8 = (line & 3) * 8;
    auto &EXTICR = EXTI->EXTICR[line >> 2];
    EXTICR = (EXTICR & ~(0xff << pos8)) | (port << pos8);
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
    EXTI->RPR1 = f; // clear pending flags
    EXTI->FPR1 = f; // clear pending flags
    EXTI->IMR1 = EXTI->IMR1 | f;
}

inline auto pending(int line) {
    uint32_t f = 1 << line;
    return ((EXTI->RPR1 | EXTI->FPR1) & f) != 0;
}

inline void clearPending(int line) {
    uint32_t f = 1 << line;
    EXTI->RPR1 = f;
    EXTI->FPR1 = f;
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
    EXTI->RPR1 = lineFlags; // clear pending flags
    EXTI->FPR1 = lineFlags; // clear pending flags
    EXTI->IMR1 = EXTI->IMR1 | lineFlags;
}

inline auto pendingFlags(int lineFlags) {
    return (EXTI->RPR1 | EXTI->FPR1) & lineFlags;
}

inline void clearPendingFlags(int lineFlags) {
    EXTI->RPR1 = lineFlags;
    EXTI->FPR1 = lineFlags;
}

} // namespace exti
} // namespace coco
