#pragma once

#include "platform.hpp"
#include "dma.hpp"
#include <coco/enum.hpp>


namespace coco {

/**
 * SPI helpers
 */
namespace spi {

constexpr int CR2_SHIFT = 16;

enum class Config : uint32_t {
    // clock prescaler
    CLOCK_DIV2 = 0, // default
    CLOCK_DIV4 = 1 << SPI_CR1_BR_Pos,
    CLOCK_DIV8 = 2 << SPI_CR1_BR_Pos,
    CLOCK_DIV16 = 3 << SPI_CR1_BR_Pos,
    CLOCK_DIV32 = 4 << SPI_CR1_BR_Pos,
    CLOCK_DIV64 = 5 << SPI_CR1_BR_Pos,
    CLOCK_DIV128 = 6 << SPI_CR1_BR_Pos,
    CLOCK_DIV256 = 7 << SPI_CR1_BR_Pos,

    // clock phase
    PHASE_0 = 0, // default
    PHASE_1 = SPI_CR1_CPHA,

    // clock polarity
    POLARITY_0 = 0, // default
    POLARITY_1 = SPI_CR1_CPOL,

    // convenience for clock pahse/polarity
    PHA0_POL0 = 0, // default
    PHA0_POL1 = POLARITY_1,
    PHA1_POL0 = PHASE_1,
    PHA1_POL1 = PHASE_1 | POLARITY_1,

    // endianness
    MSB_FIRST = 0, // default
    LSB_FIRST = SPI_CR1_LSBFIRST,

    // frame format
    FRAME_MOTOROLA = 0, // default
    FRAME_TI = SPI_CR2_FRF << CR2_SHIFT,

    // number of data bits
    DATA_8 = 0, // default

    // default configuration: prescaler div 2, phase 0, polarity 0, Motorola frame format, MSB first, 8 data bits, 8 bit alignment
    DEFAULT = CLOCK_DIV2 | PHA0_POL0 | MSB_FIRST | FRAME_MOTOROLA | DATA_8,

    // masks
    CR1_MASK = (SPI_CR1_BR | SPI_CR1_CPHA | SPI_CR1_CPOL | SPI_CR1_LSBFIRST),
    CR2_MASK = (SPI_CR2_FRF) << CR2_SHIFT
};
COCO_ENUM(Config)

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
    return uint32_t(config & Config::CR2_MASK) >> CR2_SHIFT;
}

/**
 * SPI info
 */
struct Info {
    // registers
    SPI_TypeDef *spi;

    // reset and clock control
    rcc::Info rcc;

    // interrupt index
    uint8_t irq;

    // DMA channel selection
    uint8_t sel;


    /**
     * Map a DMA channel to the RX channel of the SPI
     * Check reference manual which mappings are possible
     */
    void mapRx(const dma::Info &dmaInfo) const {
        dmaInfo.setSel(this->sel);
    }

    /**
     * Map a DMA channel to the TX channel of the SPI
     * Check reference manual which mappings are possible
     */
    void mapTx(const dma::Info &dmaInfo) const {
        dmaInfo.setSel(this->sel);
    }

    /**
     * Map DMA channels to the RX and TX channels of the SPI
     * Check reference manual which mappings are possible
     */
    void map(const dma::Info2 &dmaInfo) const {
        dmaInfo.setSel1(this->sel); // rx
        dmaInfo.setSel2(this->sel); // tx
    }
};


static const Info SPI1_INFO{SPI1, {&RCC->APB2ENR, RCC_APB2ENR_SPI1EN}, SPI1_IRQn, 3};
#ifdef SPI2
static const Info SPI2_INFO{SPI2, {&RCC->APB1ENR, RCC_APB1ENR_SPI2EN}, SPI2_IRQn, 0};
#endif
#ifdef SPI3
static const Info SPI3_INFO{SPI3, {&RCC->APB1ENR, RCC_APB1ENR_SPI3EN}, SPI3_IRQn, 0};
#endif
#ifdef SPI4
static const Info SPI4_INFO{SPI4, {&RCC->APB2ENR, RCC_APB2ENR_SPI4EN}, SPI4_IRQn, 4};
#endif
#ifdef SPI5
static const Info SPI5_INFO{SPI5, {&RCC->APB2ENR, RCC_APB2ENR_SPI5EN}, SPI5_IRQn, 7};
#endif
#ifdef SPI6
static const Info SPI6_INFO{SPI6, {&RCC->APB2ENR, RCC_APB2ENR_SPI6EN}, SPI6_IRQn, 1};
#endif

} // namespace spi
} // namespace coco
