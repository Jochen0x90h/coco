#pragma once

#include <coco/platform/stm32f0.h> // generated by CMake


/**
 * Get device id
 */
inline uint32_t getDeviceId() {
    return (DBGMCU->IDCODE & DBGMCU_IDCODE_DEV_ID_Msk) >> DBGMCU_IDCODE_DEV_ID_Pos;
}

/**
 * Get variant id
 */
inline uint32_t getVariantId() {
    return (DBGMCU->IDCODE & DBGMCU_IDCODE_REV_ID_Msk) >> DBGMCU_IDCODE_REV_ID_Pos;
}
