#pragma once

#include <coco/IntrusiveMpscQueue.hpp>
//#include "platform.hpp" // platform.hpp is next to this file in the conan package
#include <coco/platform/platform.hpp> // platform.hpp is next to this file in the conan package


namespace coco {

/**
 * NVIC (Nested Vectored Interrupt Controller)
 * See NVIC_Type in system/core_cm0.h or system/core_cm4.h
 */
namespace nvic {

enum class Priority : uint8_t {
    // low priority
    LOW = 192,

    // medium priority
    MEDIUM = 128,

    // high priority
    HIGH = 64,

    // maximum priority
    MAX = 0,
};

/**
 * Get interrupt priority (use scb::getPriority for system handlers such as SysTick_IRQn)
 */
inline Priority getPriority(int n) {
#ifdef _IP_IDX
    return Priority(NVIC->IP[_IP_IDX(n)] >> _BIT_SHIFT(n));
#else
    return Priority(NVIC->IP[n]);
#endif
}

/**
 * Set interrupt priority (use scb::setPriority for system handlers such as SysTick_IRQn)
 */
inline void setPriority(int n, Priority priority) {
#ifdef _IP_IDX
    auto &IP = NVIC->IP[_IP_IDX(n)];
    IP = (IP & ~(0xff << _BIT_SHIFT(n))) | (int(priority) << _BIT_SHIFT(n));
#else
    NVIC->IP[n] = int(priority);
#endif
}

/**
 * Enable interrupt
 */
inline void enable(int n) {
    NVIC->ISER[n >> 5] = 1 << (n & 31);
}

/**
 * Disable interrupt
 */
inline void disable(int n) {
    NVIC->ICER[n >> 5] = 1 << (n & 31);
}

/**
 * Check if an interrupt is pending
 */
inline bool pending(int n) {
    return (NVIC->ISPR[n >> 5] & (1 << (n & 31))) != 0;
}

/**
 * Set interrupt pending (interrupt vector gets called if interrupt is enabled)
 */
inline void set(int n) {
    NVIC->ISPR[n >> 5] = 1 << (n & 31);
}

/**
 * Clear interrupt
 */
inline void clear(int n) {
    NVIC->ICPR[n >> 5] = 1 << (n & 31);
}


struct Guard {
    [[nodiscard]] explicit Guard(int n) : n(n) {disable(n);}
    Guard(const Guard&) = delete;
    Guard &operator=(const Guard &) = delete;
    ~Guard() {enable(n);}

    int n;
};

struct Guard2 {
    [[nodiscard]] explicit Guard2(int n1, int n2) : n1(n1), n2(n2) {disable(n1); disable(n2);}
    Guard2(const Guard2&) = delete;
    Guard2 &operator=(const Guard2 &) = delete;
    ~Guard2() {enable(n2); enable(n1);}

    int n1;
    int n2;
};

struct Guard3 {
    [[nodiscard]] explicit Guard3(int n1, int n2, int n3) : n1(n1), n2(n2), n3(n3) {disable(n1); disable(n2); disable(n3);}
    Guard3(const Guard2&) = delete;
    Guard3 &operator=(const Guard3 &) = delete;
    ~Guard3() {enable(n3); enable(n2); enable(n1);}

    int n1;
    int n2;
    int n3;
};

template<typename F>
auto guarded(int n, F function) {
    Guard guard(n);
    return function();
}

template<typename F>
auto guarded(int n1, int n2, F function) {
    Guard2 guard(n1, n2);
    return function();
}

template<typename F>
auto guarded(int n1, int n2, int n3, F function) {
    Guard2 guard(n1, n2, n3);
    return function();
}

} // namespace nvic
} // namespace coco
