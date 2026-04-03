#pragma once

#include "nvic.hpp"
#include <coco/convert.hpp>
#include <coco/StringBuffer.hpp>


namespace coco {

/// @brief System functions.
/// System functions such as id(), reset(), jump()
namespace system {

/// Get device id.
/// @return Device id, e.g. 0x52840 for nrf52840
inline int device() {
    return NRF_FICR->INFO.PART;
}

/// @brief Get revision id.
/// @return Revision id
inline int revision() {
    return NRF_FICR->INFO.VARIANT & 0xffff;
}

/// @brief Get system id.
/// @return System id in the format nrf-device-variant
inline StringBuffer<16> id() {
    StringBuffer<16> result;

    // https://docs.nordicsemi.com/bundle/ps_nrf52840/page/ficr.html
    result << "nrf-" << hex(NRF_FICR->INFO.PART, 5) << '-';
    uint32_t variant = NRF_FICR->INFO.VARIANT;
    result << char(variant >> 8);
    result << char(variant);
    return result;
}

/// @brief Reset the microcontroller.
///
__NO_RETURN __STATIC_FORCEINLINE void reset() {
    NVIC_SystemReset();
}

} // namespace system
} // namespace coco
