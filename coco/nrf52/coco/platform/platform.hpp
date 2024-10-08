#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvolatile"
#include <coco/platform/nrf52.h> // generated by CMake
#pragma GCC diagnostic pop


// construct bitfield from value
#define V(field, value) ((value) << field##_Pos)

// construct bitfield from named value
#define N(field, value) (field##_##value << field##_Pos)

// get bitfield
#define GET(reg, field) (((reg) & field##_Msk) >> field##_Pos)

// test if a bitfield has a given named value
#define TEST(reg, field, value) (((reg) & field##_Msk) == (field##_##value << field##_Pos))

// trigger a task
constexpr int TRIGGER = 1;

// indicator that an event was generated
constexpr int GENERATED = 1;


/**
 * Get device id
 */
inline uint32_t getDeviceId() {
    return NRF_FICR->INFO.PART;
}

/**
 * Get variant id
 */
inline uint32_t getVariantId() {
    return NRF_FICR->INFO.VARIANT;
}
