#pragma once

#include <coco/platform/stm32f4.h> // generated by CMake


/**
 * Get device id
 */
inline uint32_t getDeviceId() {
    // read DBGMCU_IDCODE register
    return *(const uint32_t *)(0xE0042000) & 0xfff;
}

/**
 * Get variant id
 */
inline uint32_t getVariantId() {
    // read DBGMCU_IDCODE register
    return (*(const uint32_t *)(0xE0042000) >> 16) & 0xffff;
}
