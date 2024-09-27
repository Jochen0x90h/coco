#pragma once

#include <coco/enum.hpp>
#include <cstddef>

// do not include directly, use #include <coco/platform/usart.hpp>

namespace coco {
namespace usart {

constexpr int CR2_SHIFT = 12;

/**
 * Compressed configuration, contains bits form CR1, CR2 and CR3
 */
enum class Config : uint32_t {
    // number of data bits
    DATA_8 = 0, // default
#ifdef USART_CR1_M1
    DATA_7 = USART_CR1_M1,
    DATA_9 = USART_CR1_M0,
#else
    DATA_9 = USART_CR1_M,
#endif

    // parity
	PARITY_NONE = 0, // default
	PARITY_ODD = USART_CR1_PCE | USART_CR1_PS,
	PARITY_EVEN = USART_CR1_PCE,

    // number of stop bits
	STOP_1 = 0, // default
	STOP_0_5 = 1 << (USART_CR2_STOP_Pos - CR2_SHIFT),
	STOP_2 = 2 << (USART_CR2_STOP_Pos - CR2_SHIFT),
	STOP_1_5 = 3 << (USART_CR2_STOP_Pos - CR2_SHIFT),

    // endianness
    LSB_FIRST = 0, // default
    MSB_FIRST = 1 << (USART_CR2_MSBFIRST_Pos - CR2_SHIFT),

    // swap rx and tx
    SWAP_RX_TX = 1 << (USART_CR2_SWAP_Pos - CR2_SHIFT),

    // invert data
    INVERT_DATA = 1 << (USART_CR2_DATAINV_Pos - CR2_SHIFT),

    // default configuration: 8 data bits, no parity, 1 stop bit
    DEFAULT = DATA_8 | PARITY_NONE | STOP_1,

    // masks
    CR1_MASK = USART_CR1_M | USART_CR1_PCE | USART_CR1_PS,
    CR2_MASK = (USART_CR2_STOP | USART_CR2_MSBFIRST | USART_CR2_SWAP | USART_CR2_DATAINV) >> CR2_SHIFT,
};
COCO_ENUM(Config);

/**
 * Get CR1 register contents from Config
 */
constexpr uint32_t CR1(Config config) {
    return uint32_t(config & Config::CR1_MASK);
}

/**
 * Get CR2 register contents from Config
 */
constexpr uint32_t CR2(Config config) {
    return uint32_t(config & Config::CR2_MASK) << CR2_SHIFT;
}

} // namespace usart
} // namespace coco
