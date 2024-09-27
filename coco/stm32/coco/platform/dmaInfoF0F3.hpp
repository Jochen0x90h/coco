#pragma once

#include "rccInfoF0F3.hpp"
#include <coco/enum.hpp>

// do not include directly, use #include <coco/platform/dma.hpp>

namespace coco {

/**
 * DMA helpers for STM32F0 and STM32F3
 * DMA channel is DMA_Channel_TypeDef
 */
namespace dma {

/**
 * DMA status flags for one channel
*/
struct Status {
    __IO uint32_t *registers;

    int position;

    enum class Flags : uint32_t {
        GLOBAL = DMA_ISR_GIF1,
        TRANSFER_COMPLETE = DMA_ISR_TCIF1,
        HALF_TRANSFER = DMA_ISR_HTIF1,
        TRANSFER_ERROR = DMA_ISR_TEIF1,
        ALL = GLOBAL | TRANSFER_COMPLETE | HALF_TRANSFER | TRANSFER_ERROR
    };

    /**
     * Get the status flags. Note that it may contain flags from other channels, therefore use &-operator
     */
    Flags get() {return Flags(this->registers[0] >> position);}

    /**
     * Clear status flags
     */
    void clear(Flags flags) {this->registers[1] = int(flags) << this->position;}
};
COCO_ENUM(Status::Flags);

/**
 * DMA channel, thin wrapper of DMA_Channel_TypeDef
 */
struct Channel {
    DMA_Channel_TypeDef *channel;

    enum class Config : uint32_t {
        // auto increment
        INCREMENT_MEMORY = DMA_CCR_MINC,
        INCREMENT_PERIPHERAL = DMA_CCR_PINC,

        // direction
        PERIPHERAL_TO_MEMORY = 0,
        MEMORY_TO_PERIPHERAL = DMA_CCR_DIR,
        MEMORY_TO_MEMORY = DMA_CCR_MEM2MEM,

        // convenience
        RX = PERIPHERAL_TO_MEMORY | INCREMENT_MEMORY,
        TX = MEMORY_TO_PERIPHERAL | INCREMENT_MEMORY,

        // size
        PERIPHERAL_SIZE_8 = 0 << DMA_CCR_PSIZE_Pos,
        PERIPHERAL_SIZE_16 = 1 << DMA_CCR_PSIZE_Pos,
        PERIPHERAL_SIZE_32 = 2 << DMA_CCR_PSIZE_Pos,
        MEMORY_SIZE_8 = 0 << DMA_CCR_MSIZE_Pos,
        MEMORY_SIZE_16 = 1 << DMA_CCR_MSIZE_Pos,
        MEMORY_SIZE_32 = 2 << DMA_CCR_MSIZE_Pos,

        // optional circular mode flag
        CIRCULAR = DMA_CCR_CIRC,

        // priority
        PRIORITY_LOW = 0, // default
        PRIORITY_MEDIUM = DMA_CCR_PL_0,
        PRIORITY_HIGH = DMA_CCR_PL_1,
        PRIORITY_MAX = DMA_CCR_PL_0 | DMA_CCR_PL_1,

        // interrupts
        TRANSFER_COMPLETE_INTERRUPT = DMA_CCR_TCIE,
        HALF_TRANSFER_INTERRUPT = DMA_CCR_HTIE,
    };

    void setPeripheralAddress(__IO void *address) {this->channel->CPAR = uint32_t(address);}
    void setMemoryAddress(void *address) {this->channel->CMAR = uint32_t(address);}
    void setCount(int count) {this->channel->CNDTR = count;}

    void enable(Config config) {this->channel->CCR = DMA_CCR_EN | uint32_t(config);}
    void enable(Config config, int memorySize) {
        this->channel->CCR = DMA_CCR_EN | uint32_t(config) | (memorySize << DMA_CCR_MSIZE_Pos);
    }
    void disable() {this->channel->CCR = 0;}
    bool enabled() {return (this->channel->CCR & DMA_CCR_EN) != 0;}

    int count() {return this->channel->CNDTR;}
};
COCO_ENUM(Channel::Config)

/**
 * DMA channel info
 */
struct Info {
    // registers
    DMA_TypeDef *dma;

    // reset and clock control
#ifdef RCC_AHB1ENR_DMA1EN
    rcc::Info2<offsetof(RCC_TypeDef, AHB1ENR)> rcc;
#else
    rcc::Info2<offsetof(RCC_TypeDef, AHBENR)> rcc;
#endif

    // DMA channel index (0 for Channel1)
    uint8_t channelIndex;

    // DMA interrupt index
    uint8_t irq;


    /**
     * Get DMA status
     */
    Status status() const {return {&this->dma->ISR, this->channelIndex * 4};}

    /**
     * Get the DMA channel structure for the DMA channel
     */
    Channel channel() const {return {(DMA_Channel_TypeDef *)(intptr_t(this->dma) + 8 + this->channelIndex * 0x14)};}
};

/**
 * Dual DMA channel info
 */
struct Info2 {
    // registers
    DMA_TypeDef *dma;

    // reset and clock control
#ifdef RCC_AHB1ENR_DMA1EN
    rcc::Info2<offsetof(RCC_TypeDef, AHB1ENR)> rcc;
#else
    rcc::Info2<offsetof(RCC_TypeDef, AHBENR)> rcc;
#endif

    // DMA channel index (0 for Channel1)
    uint8_t channelIndex1;
    uint8_t channelIndex2;

    // DMA interrupt index
    uint8_t irq1;
    uint8_t irq2;


    /**
     * Get DMA status for the first channel
     */
    Status status1() const {return {&this->dma->ISR, this->channelIndex1 * 4};}

    /**
     * Get DMA status for the second channel
     */
    Status status2() const {return {&this->dma->ISR, this->channelIndex2 * 4};}

    /**
     * Get the DMA channel structure for the first DMA channel
     */
    Channel channel1() const {return {(DMA_Channel_TypeDef *)(intptr_t(this->dma) + 8 + this->channelIndex1 * 0x14)};}

    /**
     * Get the DMA channel structure for the second DMA channel
     */
    Channel channel2() const {return {(DMA_Channel_TypeDef *)(intptr_t(this->dma) + 8 + this->channelIndex2 * 0x14)};}
};

} // namespace dma
} // namespace coco
