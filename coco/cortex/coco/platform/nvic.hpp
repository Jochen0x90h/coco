#pragma once

#include <coco/IntrusiveMpscQueue.hpp>
#include <coco/platform/platform.hpp>


namespace coco {

/// @brief NVIC (Nested Vectored Interrupt Controller)
/// See NVIC_Type in system/core_cm0.h or system/core_cm4.h
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

/// @brief Get interrupt priority (use scb::getPriority for system handlers such as SysTick_IRQn)
/// @param n Interrupt number
/// @return Priority
__STATIC_FORCEINLINE Priority getPriority(int n) {
#ifdef _IP_IDX
    return Priority(NVIC->IPR[_IP_IDX(n)] >> _BIT_SHIFT(n));
#else
    return Priority(NVIC->IPR[n]);
#endif
}

/// @brief Set interrupt priority (use scb::setPriority for system handlers such as SysTick_IRQn)
/// @param n Interrupt number
/// @param priority Priority
__STATIC_FORCEINLINE void setPriority(int n, Priority priority) {
#ifdef _IP_IDX
    auto &IPR = NVIC->IPR[_IP_IDX(n)];
    IPR = (IPR & ~(0xff << _BIT_SHIFT(n))) | (int(priority) << _BIT_SHIFT(n));
#else
    NVIC->IPR[n] = int(priority);
#endif
}

/// @brief Enable interrupt.
///
__STATIC_FORCEINLINE void enable(int n) {
    NVIC->ISER[n >> 5] = 1 << (n & 31);
}

/// @brief Disable interrupt.
///
__STATIC_FORCEINLINE void disable(int n) {
    NVIC->ICER[n >> 5] = 1 << (n & 31);
}

/// @brief Disable all interrupts.
///
__STATIC_FORCEINLINE void disableAll() {
    for (auto &ICERi : NVIC->ICER) {
        ICERi = 0xFFFFFFFF;
    }
}

/// @brief Check if an interrupt is pending.
///
__STATIC_FORCEINLINE bool pending(int n) {
    return (NVIC->ISPR[n >> 5] & (1 << (n & 31))) != 0;
}

/// @brief Set interrupt pending flag (interrupt vector gets called if interrupt is enabled).
///
__STATIC_FORCEINLINE void set(int n) {
    NVIC->ISPR[n >> 5] = 1 << (n & 31);
}

/// @brief Clear interrupt pending flag.
///
__STATIC_FORCEINLINE void clear(int n) {
    NVIC->ICPR[n >> 5] = 1 << (n & 31);
}

/// @brief Clear all interrupt pending flags.
///
__STATIC_FORCEINLINE void clearAll() {
    for (auto &ICPRi : NVIC->ICPR) {
        ICPRi = 0xFFFFFFFF;
    }
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

/// @brief Execute function with interrupt disabled.
/// @tparam F Function type
/// @param n Interrupt index
/// @param function Function to execute with interrupt disabled
/// @return Result of function
template<typename F>
auto guarded(int n, F function) {
    Guard guard(n);
    return function();
}

/// @brief Execute function with two interrupts disabled.
/// @tparam F Function type
/// @param n1 Interrupt index
/// @param n2 Interrupt index
/// @param function Function to execute with interrupts disabled
/// @return Result of function
template<typename F>
auto guarded(int n1, int n2, F function) {
    Guard2 guard(n1, n2);
    return function();
}

/// @brief Execute function with three interrupts disabled.
/// @tparam F Function type
/// @param n1 Interrupt index
/// @param n2 Interrupt index
/// @param n3 Interrupt index
/// @param function Function to execute with interrupts disabled
/// @return Result of function
template<typename F>
auto guarded(int n1, int n2, int n3, F function) {
    Guard2 guard(n1, n2, n3);
    return function();
}

} // namespace nvic
} // namespace coco
