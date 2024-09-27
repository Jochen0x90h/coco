#pragma once

#include "rccInfoF4.hpp"
#include <coco/enum.hpp>

// do not include directly, use #include <coco/platform/dma.hpp>

namespace coco {

/**
 * DMA helpers for STM32F4
 * DMA channel is DMA_Stream_TypeDef
 */
namespace dma {

/**
 * DMA status flags for one channel
 */
struct Status {
    __IO uint32_t *registers;

    int position;

    enum class Flags : uint32_t {
        FIFO_ERROR = DMA_LISR_FEIF0,
        DIRECT_MODE_ERROR = DMA_LISR_DMEIF0,
        TRANSFER_ERROR = DMA_LISR_TEIF0,
        HALF_TRANSFER = DMA_LISR_HTIF0,
        TRANSFER_COMPLETE = DMA_LISR_TCIF0,
        ALL = FIFO_ERROR | DIRECT_MODE_ERROR | TRANSFER_ERROR | HALF_TRANSFER | TRANSFER_COMPLETE
    };

    /**
     * Get the status flags. Note that it may contain flags from other channels, therefore use &-operator
     */
    Flags get() {return Flags(this->registers[0] >> position);}

    /**
     * Clear status flags
     */
    void clear(Flags flags) {this->registers[2] = int(flags) << this->position;}
};
COCO_ENUM(Status::Flags);

/**
 * DMA channel, thin wrapper of DMA_Stream_TypeDef
 */
struct Channel {
    DMA_Stream_TypeDef *channel;

    enum class Config : uint32_t {
        // auto increment
        INCREMENT_MEMORY = DMA_SxCR_MINC,
        INCREMENT_PERIPHERAL = DMA_SxCR_PINC,

        // direction
        PERIPHERAL_TO_MEMORY = 0,
        MEMORY_TO_PERIPHERAL = DMA_SxCR_DIR_0,
        MEMORY_TO_MEMORY = DMA_SxCR_DIR_1,

        // convenience
        RX = PERIPHERAL_TO_MEMORY | INCREMENT_MEMORY,
        TX = MEMORY_TO_PERIPHERAL | INCREMENT_MEMORY,

        // size
        PERIPHERAL_SIZE_8 = 0 << DMA_SxCR_PSIZE_Pos,
        PERIPHERAL_SIZE_16 = 1 << DMA_SxCR_PSIZE_Pos,
        PERIPHERAL_SIZE_32 = 2 << DMA_SxCR_PSIZE_Pos,
        MEMORY_SIZE_8 = 0 << DMA_SxCR_MSIZE_Pos,
        MEMORY_SIZE_16 = 1 << DMA_SxCR_MSIZE_Pos,
        MEMORY_SIZE_32 = 2 << DMA_SxCR_MSIZE_Pos,

        // optional circular mode flag
        CIRCULAR = DMA_SxCR_CIRC,

        // priority
        PRIORITY_LOW = 0, // default
        PRIORITY_MEDIUM = DMA_SxCR_PL_0,
        PRIORITY_HIGH = DMA_SxCR_PL_1,
        PRIORITY_MAX = DMA_SxCR_PL_0 | DMA_SxCR_PL_1,

        // interrupts
        TRANSFER_COMPLETE_INTERRUPT = DMA_SxCR_TCIE,
        HALF_TRANSFER_INTERRUPT = DMA_SxCR_HTIE,
    };

    void setPeripheralAddress(__IO void *address) {this->channel->PAR = uint32_t(address);}
    void setMemoryAddress(void *address) {this->channel->M0AR = uint32_t(address);}
    void setCount(int count) {this->channel->NDTR = count;}

    void enable(Config config) {
        this->channel->CR = (this->channel->CR & DMA_SxCR_CHSEL_Msk)
            | DMA_SxCR_EN
            | uint32_t(config);}
    void enable(Config config, int memorySize) {
        this->channel->CR = (this->channel->CR & DMA_SxCR_CHSEL_Msk)
            | DMA_SxCR_EN
            | uint32_t(config)
            | (memorySize << DMA_SxCR_MSIZE_Pos);
    }
    void disable() {this->channel->CR = this->channel->CR & DMA_SxCR_CHSEL_Msk;}
    bool enabled() {return (this->channel->CR & DMA_SxCR_EN) != 0;}

    int count() {return this->channel->NDTR;}
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
    Status status() const {return {&this->dma->LISR + (this->channelIndex >> 2), (this->channelIndex & 3) * 6 + (this->channelIndex & 2) * 2};}

    /**
     * Get the DMA channel structure for the DMA channel
     */
    Channel channel() const {return {(DMA_Stream_TypeDef *)(intptr_t(this->dma) + 0x10 + this->channelIndex * 0x18)};}

    /**
     * Set channel selection for the DMA channel (stream in F4)
     */
    void setSel(int index) const {channel().channel->CR = index << DMA_SxCR_CHSEL_Pos;}
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
    Status status1() const {return {&this->dma->LISR + (this->channelIndex1 >> 2), (this->channelIndex1 & 3) * 6 + (this->channelIndex1 & 2) * 2};}

    /**
     * Get DMA status for the second channel
     */
    Status status2() const {return {&this->dma->LISR + (this->channelIndex2 >> 2), (this->channelIndex2 & 3) * 6 + (this->channelIndex2 & 2) * 2};}

    /**
     * Get the DMA channel structure for the first DMA channel
     */
    Channel channel1() const {return {(DMA_Stream_TypeDef *)(intptr_t(this->dma) + 0x10 + this->channelIndex1 * 0x18)};}

    /**
     * Get the DMA channel structure for the second DMA channel
     */
    Channel channel2() const {return {(DMA_Stream_TypeDef *)(intptr_t(this->dma) + 0x10 + this->channelIndex2 * 0x18)};}

    /**
     * Set channel selection for the first DMA channel (stream in F4)
     */
    void setSel1(int index) const {channel1().channel->CR = index << DMA_SxCR_CHSEL_Pos;}

    /**
     * Set channel selection for the first DMA second (stream in F4)
     */
    void setSel2(int index) const {channel2().channel->CR = index << DMA_SxCR_CHSEL_Pos;}
};

} // namespace dma
} // namespace coco
