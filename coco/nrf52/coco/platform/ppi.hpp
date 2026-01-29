#pragma once

#include "platform.hpp"


namespace coco {

/**
 * Refernece manual:
 *   https://infocenter.nordicsemi.com/topic/ps_nrf52840/ppi.html?cp=5_0_0_5_15
 */
namespace ppi {

enum Channel {
    PPI_CH0 = 0,
    PPI_CH1 = 1,
    PPI_CH2 = 2,
    PPI_CH3 = 3,
    PPI_CH4 = 4,
    PPI_CH5 = 5,
    PPI_CH6 = 6,
    PPI_CH7 = 7,
    PPI_CH8 = 8,
    PPI_CH9 = 9,
    PPI_CH10 = 10,
    PPI_CH11 = 11,
    PPI_CH12 = 12,
    PPI_CH13 = 13,
    PPI_CH14 = 14,
    PPI_CH15 = 15,
    PPI_CH16 = 16,
    PPI_CH17 = 17,
    PPI_CH18 = 18,
    PPI_CH19 = 19,
};

enum DualChannel {
    PPI_CH0_CH1 = 0 | (1 << 8),
    PPI_CH1_CH2 = 1 | (2 << 8),
    PPI_CH2_CH3 = 2 | (3 << 8),
    PPI_CH3_CH4 = 3 | (4 << 8),
    PPI_CH4_CH5 = 4 | (5 << 8),
    PPI_CH5_CH6 = 5 | (6 << 8),
    PPI_CH6_CH7 = 6 | (7 << 8),
    PPI_CH7_CH8 = 7 | (8 << 8),
    PPI_CH8_CH9 = 8 | (9 << 8),
    PPI_CH9_CH10 = 9 | (10 << 8),
    PPI_CH10_CH11 = 10 | (11 << 8),
    PPI_CH11_CH12 = 11 | (12 << 8),
    PPI_CH12_CH13 = 12 | (13 << 8),
    PPI_CH13_CH14 = 13 | (14 << 8),
    PPI_CH14_CH15 = 14 | (15 << 8),
    PPI_CH15_CH16 = 15 | (16 << 8),
    PPI_CH16_CH17 = 16 | (17 << 8),
    PPI_CH17_CH18 = 17 | (18 << 8),
    PPI_CH18_CH19 = 18 | (19 << 8),
};

} // namespace ppi
} // namespace coco
