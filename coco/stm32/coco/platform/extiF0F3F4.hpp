#pragma once

// do not include directly, use #include <coco/platform/exti.hpp>

#include "platform.hpp"


namespace coco {

/// @brief EXTI (external interrupt) helpers.
/// G4 https://www.st.com/en/microcontrollers-microprocessors/stm32g4-series/documentation.html Section 15
namespace exti {

// Device specific lines
enum class Line : uint8_t;


// functions for single EXTI line using Line enum
// ----------------------------------------------

/// @brief Detect rising edge on an EXTI line.
/// @param line EXTI line
inline void detectRisingEdge(Line line) {
    int l = int(line);
    uint32_t f = 1 << l;
    EXTI->RTSR = EXTI->RTSR | f;
    EXTI->FTSR = EXTI->FTSR & ~f;
}

inline void detectFallingEdge(Line line) {
    int l = int(line);
    uint32_t f = 1 << l;
    EXTI->RTSR = EXTI->RTSR & ~f;
    EXTI->FTSR = EXTI->FTSR | f;
}

inline void detectBothEdges(Line line) {
    int l = int(line);
    uint32_t f = 1 << l;
    EXTI->RTSR = EXTI->RTSR | f;
    EXTI->FTSR = EXTI->FTSR | f;
}

inline void enableInterrupt(Line line) {
    int l = int(line);
    uint32_t f = 1 << l;
    EXTI->PR = f; // clear pending flags
    EXTI->IMR = EXTI->IMR | f;
}

inline auto pending(Line line) {
    int l = int(line);
    uint32_t f = 1 << l;
    return (EXTI->PR & f) != 0;
}

inline void clearPending(Line line) {
    int l = int(line);
    uint32_t f = 1 << l;
    EXTI->PR = f;
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
    EXTI->RTSR = EXTI->RTSR | f;
    EXTI->FTSR = EXTI->FTSR & ~f;
}

inline void detectFallingEdge(int line) {
    uint32_t f = 1 << line;
    EXTI->RTSR = EXTI->RTSR & ~f;
    EXTI->FTSR = EXTI->FTSR | f;
}

inline void detectBothEdges(int line) {
    uint32_t f = 1 << line;
    EXTI->RTSR = EXTI->RTSR | f;
    EXTI->FTSR = EXTI->FTSR | f;
}

/// @brief Enable interrupt.
/// First call clearPending() to make sure that no interrupt is already pending.
/// @param line EXTI line to modify
inline void enableInterrupt(int line) {
    uint32_t f = 1 << line;
    EXTI->IMR = EXTI->IMR | f;
}

/// @brief Disable interrupt.
/// @param line EXTI line to modify
inline void disableInterrupt(int line) {
    uint32_t f = 1 << line;
    EXTI->IMR1 = EXTI->IMR1 & ~f;
}

inline auto pending(int line) {
    uint32_t f = 1 << line;
    return (EXTI->PR & f) != 0;
}

inline void clearPending(int line) {
    uint32_t f = 1 << line;
    EXTI->PR = f;
}


// functions for multiple GPIO EXTI lines (only lines 0 - 15)
// ----------------------------------------------------------

/// @brief Detect rising edge on multiple EXTI lines (only lines 0 - 15).
/// @param lineFlags Flags indicating which EXTI lines to modify
inline void detectRisingEdgeFlags(int lineFlags) {
    EXTI->RTSR = EXTI->RTSR | lineFlags;
    EXTI->FTSR = EXTI->FTSR & ~lineFlags;
}

inline void detectFallingEdgeFlags(int lineFlags) {
    EXTI->RTSR = EXTI->RTSR & ~lineFlags;
    EXTI->FTSR = EXTI->FTSR | lineFlags;
}

inline void detectBothEdgesFlags(int lineFlags) {
    EXTI->RTSR = EXTI->RTSR | lineFlags;
    EXTI->FTSR = EXTI->FTSR | lineFlags;
}

/// @brief Enable interrupt flags (only lines 0 - 15).
/// First call clearPendingFlags() to make sure that no interrupt is already pending.
/// @param lineFlags Flags indicating which EXTI lines to modify
inline void enableInterruptFlags(int lineFlags) {
    EXTI->IMR = EXTI->IMR | lineFlags;
}

/// @brief Disable interrupt flags (only lines 0 - 15).
/// @param lineFlags Flags indicating which EXTI lines to modify
inline void disableInterruptFlags(int lineFlags) {
    EXTI->IMR1 = EXTI->IMR1 & ~ lineFlags;
}

inline auto pendingFlags(int lineFlags) {
    return EXTI->PR & lineFlags;
}

inline void clearPendingFlags(int lineFlags) {
    EXTI->PR = lineFlags;
}

} // namespace exti
} // namespace coco
