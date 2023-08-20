#pragma once

#include "platform.hpp"


namespace coco {

/**
	NVIC (nested vectored interrupt controller)
	see NVIC_Type in system/core_cm0.h or system/core_cm4.h
*/
namespace nvic {

/**
	Get interrupt priority
*/
inline uint8_t getPriority(int n) {
	return NVIC->IP[n];
}

/**
	Set interrupt priority
*/
inline void setPriority(int n, uint8_t priority) {
	NVIC->IP[n] = priority;
}

/**
	Enable interrupt
*/
inline void enable(int n) {
	NVIC->ISER[n >> 5] = 1 << (n & 31);
}

/**
	Disable interrupt
*/
inline void disable(int n) {
	NVIC->ICER[n >> 5] = 1 << (n & 31);
}

/**
	Check if an interrupt is pending
*/
inline bool pending(int n) {
	return (NVIC->ISPR[n >> 5] & (1 << (n & 31))) != 0;
}

/**
	Set interrupt pending (interrupt vector gets called if interrupt is enabled)
*/
inline void set(int n) {
	NVIC->ISPR[n >> 5] = 1 << (n & 31);
}

/**
	Clear interrupt
*/
inline void clear(int n) {
	NVIC->ICPR[n >> 5] = 1 << (n & 31);
}

} // namespace nvic
} // namespace coco
