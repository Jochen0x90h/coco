#pragma once

#include "platform.hpp"
#include "dma.hpp"
#include <coco/enum.hpp>


namespace coco {
namespace usart {

constexpr int CR2_SHIFT = 12;

/**
 * Compressed configuration, contains bits form CR1, CR2 and CR3
 */
enum class Config : uint32_t {
    // number of data bits
    DATA_8 = 0, // default
    DATA_9 = USART_CR1_M,

    // parity
	PARITY_NONE = 0, // default
	PARITY_ODD = USART_CR1_PCE | USART_CR1_PS,
	PARITY_EVEN = USART_CR1_PCE,

    // number of stop bits
	STOP_1 = 0, // default
	STOP_0_5 = 1 << (USART_CR2_STOP_Pos - CR2_SHIFT),
	STOP_2 = 2 << (USART_CR2_STOP_Pos - CR2_SHIFT),
	STOP_1_5 = 3 << (USART_CR2_STOP_Pos - CR2_SHIFT),

    // default configuration: 8 data bits, no parity, 1 stop bit
    DEFAULT = DATA_8 | PARITY_NONE | STOP_1,

    // masks
    CR1_MASK = USART_CR1_M | USART_CR1_PCE | USART_CR1_PS,
    CR2_MASK = (USART_CR2_STOP) >> CR2_SHIFT,
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

/**
 * USART info
 */
struct Info {
    // registers
    USART_TypeDef *usart;

    // reset and clock control
    rcc::Info rcc;

    // interrupt index
    uint8_t irq;


    /**
     * Map a DMA channel to the RX channel of the USART
     * Check reference manual which mappings are possible
     */
    void mapRx(const dma::Info &dmaInfo) const {
        int channel = 4;
        if (false
#ifdef UART6
            || this->usart == USART6
#endif
#ifdef UART7
            || this->usart == UART7
#endif
#ifdef UART8
            || this->usart == UART8
#endif
        ) {
            channel = 5;
        }
        dmaInfo.channel().channel->CR = channel << DMA_SxCR_CHSEL_Pos;
    }

    /**
     * Map a DMA channel to the TX channel of the USART
     * Check reference manual which mappings are possible
     */
    void mapTx(const dma::Info &dmaInfo) const {
        int channel = 4;
        if (false
#ifdef UART6
            || this->usart == USART6
#endif
#ifdef UART7
            || this->usart == UART7
#endif
#ifdef UART8
            || this->usart == UART8
#endif
        ) {
            channel = 5;
        }
        dmaInfo.channel().channel->CR = channel << DMA_SxCR_CHSEL_Pos;
    }

    /**
     * Map DMA channels to the RX and TX channels of the USART
     * Check reference manual which mappings are possible
     */
    void map(const dma::Info2 &dmaInfo) const {
        int channel = 4;
        if (false
#ifdef UART6
            || this->usart == USART6
#endif
#ifdef UART7
            || this->usart == UART7
#endif
#ifdef UART8
            || this->usart == UART8
#endif
        ) {
            channel = 5;
        }
        dmaInfo.channel1().channel->CR = channel << DMA_SxCR_CHSEL_Pos;
        dmaInfo.channel2().channel->CR = channel << DMA_SxCR_CHSEL_Pos;
    }
};


static const Info USART1_INFO{USART1, {&RCC->APB2ENR, RCC_APB2ENR_USART1EN}, USART1_IRQn};
static const Info USART2_INFO{USART2, {&RCC->APB1ENR, RCC_APB1ENR_USART2EN}, USART2_IRQn};
#ifdef USART3
static const Info USART3_INFO{USART3, {&RCC->APB1ENR, RCC_APB1ENR_USART3EN}, USART3_IRQn};
#endif
#ifdef UART4
static const Info UART4_INFO{UART4, {&RCC->APB1ENR, RCC_APB1ENR_UART4EN}, UART4_IRQn};
#endif
#ifdef UART5
static const Info UART5_INFO{UART5, {&RCC->APB1ENR, RCC_APB1ENR_UART5EN}, UART5_IRQn};
#endif
#ifdef USART6
static const Info USART6_INFO{USART6, {&RCC->APB2ENR, RCC_APB2ENR_USART6EN}, USART6_IRQn};
#endif
#ifdef UART7
static const Info UART7_INFO{UART7, {&RCC->APB1ENR, RCC_APB1ENR_UART7EN}, UART7_IRQn};
#endif
#ifdef UART8
static const Info UART8_INFO{UART8, {&RCC->APB1ENR, RCC_APB1ENR_UART8EN}, UART8_IRQn};
#endif

} // namespace usart
} // namespace coco
