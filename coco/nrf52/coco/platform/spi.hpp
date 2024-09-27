#pragma once

#include "platform.hpp"
#include <coco/enum.hpp>


namespace coco {

/**
 * SPI helpers
 */
namespace spi {

enum class Config : uint32_t {
    // speed
    SPEED_125K = 0x02000000, // 125 kbps
    SPEED_250K = 0x04000000, // 250 kbps
    SPEED_500K = 0x08000000, // 500 kbps
    SPEED_1M = 0x10000000, // 1 Mbps
    SPEED_2M = 0x20000000, // 2 Mbps
    SPEED_4M = 0x40000000, // 4 Mbps
    SPEED_8M = 0x80000000, // 8 Mbps
    SPEED_16M = 0x0A000000, // 16 Mbps
    SPEED_32M = 0x14000000, // 32 Mbps

    // clock phase
    PHASE_0 = 0, // default
    PHASE_1 = SPIM_CONFIG_CPHA_Trailing << SPIM_CONFIG_CPHA_Pos,

    // clock polarity
    POLARITY_0 = 0, // default
    POLARITY_1 = SPIM_CONFIG_CPOL_ActiveLow << SPIM_CONFIG_CPOL_Pos,

    // convenience for clock pahse/polarity
    PHA0_POL0 = 0, // default
    PHA0_POL1 = POLARITY_1,
    PHA1_POL0 = PHASE_1,
    PHA1_POL1 = PHASE_1 | POLARITY_1,

    // endianness
    MSB_FIRST = 0, // default
    LSB_FIRST = SPIM_CONFIG_ORDER_LsbFirst << SPIM_CONFIG_ORDER_Pos,

    // number of data bits
    DATA_8 = 0, // default

    // default configuration: 1mbit, phase 0, polarity 0, MSB first, 8 data bits, 8 bit alignment
    DEFAULT = SPEED_1M | PHA0_POL0 | MSB_FIRST | DATA_8,

    // masks
    SPEED_MASK = 0xffffff00,
    CONFIG_MASK = 0x000000ff,
};
COCO_ENUM(Config);

/**
 * SPIM info, SPI master with EasyDMA
 */
struct InfoM {
    NRF_SPIM_Type *spi;

    // interrupt index
    uint8_t irq;
};

static const InfoM SPIM0_INFO{NRF_SPIM0, uint8_t(SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn)};
#ifdef NRF_SPIM1
static const InfoM SPIM1_INFO{NRF_SPIM1, uint8_t(SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQn)};
#endif
#ifdef NRF_SPIM2
static const InfoM SPIM2_INFO{NRF_SPIM2, uint8_t(SPIM2_SPIS2_SPI2_IRQn)};
#endif
#ifdef NRF_SPIM3
static const InfoM SPIM3_INFO{NRF_SPIM3, uint8_t(SPIM3_IRQn)};
#endif

} // namespace spi
} // namespace coco
