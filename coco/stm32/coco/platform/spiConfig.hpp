#pragma once

#include <coco/enum.hpp>
#include <cstddef>

// do not include directly, use #include <coco/platform/spi.hpp>


namespace coco {
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
    DATA_4 = ((7 ^ 3) << SPI_CR2_DS_Pos) << CR2_SHIFT,
    DATA_5 = ((7 ^ 4) << SPI_CR2_DS_Pos) << CR2_SHIFT,
    DATA_6 = ((7 ^ 5) << SPI_CR2_DS_Pos) << CR2_SHIFT,
    DATA_7 = ((7 ^ 6) << SPI_CR2_DS_Pos) << CR2_SHIFT,
    DATA_8 = 0, // default
    DATA_9 = (((7 ^ 8) << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH) << CR2_SHIFT,
    DATA_10 = (((7 ^ 9) << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH) << CR2_SHIFT,
    DATA_11 = (((7 ^ 10) << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH) << CR2_SHIFT,
    DATA_12 = (((7 ^ 11) << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH) << CR2_SHIFT,
    DATA_13 = (((7 ^ 12) << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH) << CR2_SHIFT,
    DATA_14 = (((7 ^ 13) << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH) << CR2_SHIFT,
    DATA_15 = (((7 ^ 14) << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH) << CR2_SHIFT,
    DATA_16 = (((7 ^ 15) << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH) << CR2_SHIFT,

    // default configuration: prescaler div 2, phase 0, polarity 0, Motorola frame format, MSB first, 8 data bits, 8 bit alignment
    DEFAULT = CLOCK_DIV2 | PHA0_POL0 | MSB_FIRST | FRAME_MOTOROLA | DATA_8,

    // masks
    CR1_MASK = (SPI_CR1_BR | SPI_CR1_CPHA | SPI_CR1_CPOL | SPI_CR1_LSBFIRST),
    CR2_MASK = (SPI_CR2_FRF | SPI_CR2_DS | SPI_CR2_FRXTH) << CR2_SHIFT
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
    // bits 0-2 of SPI_CR2_DS and SPI_CR2_FRXTH are inverted to make Config::DATA_8 the default
    return (uint32_t(config & Config::CR2_MASK) >> CR2_SHIFT) ^ ((7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH);
}

} // namespace spi
} // namespace coco
