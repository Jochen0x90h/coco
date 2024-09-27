#pragma once

#include "platform.hpp"


namespace coco {

/**
 * Refernece manual:
 *   https://infocenter.nordicsemi.com/topic/ps_nrf52840/timer.html?cp=5_0_0_5_29
 */
namespace timer {

/**
 * TIMER info
 */
struct Info {
    NRF_TIMER_Type *timer;

    // interrupt index
    uint8_t irq;
};

static const Info TIMER0_INFO{NRF_TIMER0, uint8_t(TIMER0_IRQn)};
static const Info TIMER1_INFO{NRF_TIMER1, uint8_t(TIMER1_IRQn)};
static const Info TIMER2_INFO{NRF_TIMER2, uint8_t(TIMER2_IRQn)};

} // namespace timer
} // namespace coco
