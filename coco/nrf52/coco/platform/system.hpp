#pragma once

#include "nvic.hpp"
#include <coco/convert.hpp>
#include <coco/StringBuffer.hpp>


namespace coco {

/// @brief System functions.
/// System functions such as reset, intent, jump
namespace system {

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
