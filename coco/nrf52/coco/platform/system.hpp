#pragma once

#include <coco/convert.hpp>
#include <coco/StringBuffer.hpp>
#include <coco/platform/nvic.hpp>


namespace coco {

/// @brief System functions.
/// System functions such as id(), reset(), jump()
namespace system {

/// @brief Get system name.
/// @return System name (nrf52xxx)
StringBuffer<16> name();

/// @brief Get system version.
/// @return Device version (e.g. D0)
inline StringBuffer<16> version() {
    StringBuffer<16> result;

    // https://docs.nordicsemi.com/bundle/ps_nrf52840/page/ficr.html
    uint32_t variant = NRF_FICR->INFO.VARIANT;
    result << char(variant >> 8);
    result << char(variant);
    return result;
}

/// Get system id.
/// nRF52840: 0x52840
/// @return Device id
inline int id() {
    return NRF_FICR->INFO.PART;
}

/// Get system build.
/// @return System build
inline int build() {
    // https://docs.nordicsemi.com/bundle/ps_nrf52840/page/ficr.html
    return NRF_FICR->INFO.VARIANT & 0xffff;
}

/*
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
*/

/// @brief Reset the microcontroller.
///
__NO_RETURN __STATIC_FORCEINLINE void reset() {
    NVIC_SystemReset();
}

} // namespace system
} // namespace coco
