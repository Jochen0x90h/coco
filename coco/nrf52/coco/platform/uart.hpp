#pragma once

#include "platform.hpp"
#include <coco/enum.hpp>


namespace coco {

/**
 * UART helpers
 */
namespace uart {

/**
 * Configuration
 */
enum class Config : uint32_t {
    // 8 data bits, no parity, 1 stop bit
    DEFAULT = 0,

    // number of data bits
    DATA_8 = 0, // default

    // parity
	PARITY_NONE = 0, // default
	PARITY_EVEN = UARTE_CONFIG_PARITY_Included << UARTE_CONFIG_PARITY_Pos,

    // number of stop bits
	STOP_1 = 0, // default
   	STOP_2 = UARTE_CONFIG_STOP_Two << UARTE_CONFIG_STOP_Pos,

    // control signals
    ENABLE_RTS_CTS = UARTE_CONFIG_HWFC_Enabled << UARTE_CONFIG_HWFC_Pos,
};
COCO_ENUM(Config);


/**
 * UART info
 */
struct Info {
    NRF_UART_Type *uart;

    // interrupt index
    uint8_t irq;
};

/**
 * UARTE info, UART with EasyDMA
 */
struct InfoE {
    NRF_UARTE_Type *uart;

    // interrupt index
    uint8_t irq;
};

static const Info UART0_INFO{NRF_UART0, uint8_t(UARTE0_UART0_IRQn)};
static const InfoE UARTE0_INFO{NRF_UARTE0, uint8_t(UARTE0_UART0_IRQn)};
static const InfoE UARTE1_INFO{NRF_UARTE1, uint8_t(UARTE1_IRQn)};

} // namespace uart
} // namespace coco
