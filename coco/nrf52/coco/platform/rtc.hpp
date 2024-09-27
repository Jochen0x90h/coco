#pragma once

#include "platform.hpp"


namespace coco {

/**
 * Refernece manual:
 *   https://infocenter.nordicsemi.com/index.jsp?topic=%2Fps_nrf52840%2Frtc.html&cp=5_0_0_5_21
 */
namespace rtc {

/**
 * TIMER info
 */
struct Info {
    NRF_RTC_Type *rtc;

    // interrupt index
    uint8_t irq;
};


static const Info RTC0_INFO{NRF_RTC0, uint8_t(RTC0_IRQn)}; // CC[0..2] implemented
static const Info RTC1_INFO{NRF_RTC1, uint8_t(RTC1_IRQn)}; // CC[0..3] implemented
#ifdef NRF_RTC2
static const Info RTC2_INFO{NRF_RTC2, uint8_t(RTC2_IRQn)}; // CC[0..3] implemented
#endif

} // namespace rtc
} // namespace coco
