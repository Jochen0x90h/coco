#pragma once

#include "platform.hpp"
#include "rcc.hpp"
#include <coco/enum.hpp>
#include <utility>


namespace coco {

/// @brief DMA helpers
/// Refernece manual: https://www.st.com/resource/en/reference_manual/rm0364-stm32f334xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
/// DMA: Section 11
namespace dma {

/// @brief Mode (CR register).
///
enum class Mode : uint32_t {
    // scope
    PERIPHERAL_TO_MEMORY = 0,
    MEMORY_TO_PERIPHERAL = DMA_SxCR_DIR,

    // circular mode flag
    CIRCULAR = DMA_SxCR_CIRC,

    // source
    SOURCE_WIDTH_8 = 0, // default
    SOURCE_WIDTH_16 = 1 << DMA_SxCR_PSIZE_Pos, // note source and destination fields need to be swapped when DIR is set
    SOURCE_WIDTH_32 = 2 << DMA_SxCR_PSIZE_Pos,
    SOURCE_INCREMENT = DMA_SxCR_PINC,
    SOURCE_DYNAMIC = 3 << DMA_SxCR_PSIZE_Pos, // width and increment can be configured at run-time

    // destination
    DESTINATION_WIDTH_8 = 0, // default
    DESTINATION_WIDTH_16 = 1 << DMA_SxCR_MSIZE_Pos,
    DESTINATION_WIDTH_32 = 2 << DMA_SxCR_MSIZE_Pos,
    DESTINATION_INCREMENT = DMA_SxCR_MINC,
    DESTINATION_DYNAMIC = 3 << DMA_SxCR_MSIZE_Pos, // width and increment can be configured at run-time

    // convenience
    RX8 = PERIPHERAL_TO_MEMORY | SOURCE_WIDTH_8 | DESTINATION_WIDTH_8 | DESTINATION_INCREMENT,
    TX8 = MEMORY_TO_PERIPHERAL | SOURCE_WIDTH_8 | DESTINATION_WIDTH_8 | SOURCE_INCREMENT,
    RX16 = PERIPHERAL_TO_MEMORY | SOURCE_WIDTH_16 | DESTINATION_WIDTH_16 | DESTINATION_INCREMENT,
    TX16 = MEMORY_TO_PERIPHERAL | SOURCE_WIDTH_16 | DESTINATION_WIDTH_16  | SOURCE_INCREMENT,
    RX32 = PERIPHERAL_TO_MEMORY | SOURCE_WIDTH_32 | DESTINATION_WIDTH_32 | DESTINATION_INCREMENT,
    TX32 = MEMORY_TO_PERIPHERAL | SOURCE_WIDTH_32 | DESTINATION_WIDTH_32 | SOURCE_INCREMENT,
};
COCO_ENUM(Mode);

// dynamic source/destination width
constexpr int WIDTH_8 = 0;
constexpr int WIDTH_16 = 1;
constexpr int WIDTH_32 = 2;

/// @brief Source configuration
///
enum class Source : uint32_t {
    NO_INCREMENT = 0,
    INCREMENT = DMA_SxCR_PINC,
};

/// @brief Destination configuration
///
enum class Destination : uint32_t {
    NO_INCREMENT = 0,
    INCREMENT = DMA_SxCR_MINC,
};

/// @brief DMA Channel configuration.
///
enum class Config : uint32_t {
    DEFAULT = 0,

    // optional circular mode flag
    //CIRCULAR = DMA_SxCR_CIRC,

    // priority
    PRIORITY_LOW = 0, // default
    PRIORITY_MEDIUM = DMA_SxCR_PL_0,
    PRIORITY_HIGH = DMA_SxCR_PL_1,
    PRIORITY_MAX = DMA_SxCR_PL_0 | DMA_SxCR_PL_1,

    // interrupts
    TRANSFER_COMPLETE_INTERRUPT = DMA_SxCR_TCIE,
    HALF_TRANSFER_INTERRUPT = DMA_SxCR_HTIE,
};
COCO_ENUM(Config);

/// @brief Status of DMA channel.
///
enum class Status : uint32_t {
    NONE = 0,
    FIFO_ERROR = DMA_LISR_FEIF0,
    DIRECT_MODE_ERROR = DMA_LISR_DMEIF0,
    TRANSFER_ERROR = DMA_LISR_TEIF0,
    HALF_TRANSFER = DMA_LISR_HTIF0,
    TRANSFER_COMPLETE = DMA_LISR_TCIF0,
    ALL = FIFO_ERROR | DIRECT_MODE_ERROR | TRANSFER_ERROR | HALF_TRANSFER | TRANSFER_COMPLETE
};
COCO_ENUM(Status);


/// @brief Feature flags.
///
enum class Feature {
    NONE = 0,

    // support for circular mode
    CIRCULAR = 1 << 0,
};
COCO_ENUM(Feature)

constexpr uint32_t PERIPHERAL_FIELDS = DMA_SxCR_PSIZE | DMA_SxCR_PINC;
constexpr uint32_t MEMORY_FIELDS = DMA_SxCR_MSIZE | DMA_SxCR_MINC;


/// @brief DMA channel, thin wrapper of DMA_Channel_TypeDef.
/// @tparam M Transfer mode
template <Mode M>
struct Channel {
    static constexpr auto MODE = M;

    DMA_Stream_TypeDef *channel;
    int statusPosition;


    DMA_Stream_TypeDef *operator ->() {return channel;}
    operator DMA_Stream_TypeDef *() {return channel;}

    /// @brief Configure transfer mode using compile-time source AND destination transfer mode.
    /// Note that this does nothing as compile-time transfer mode is set in enable(), but should be called for
    /// compatiblity with H5, U3, U5 etc.
    /// @return *this
    auto &configure()
        requires ((M & Mode::SOURCE_DYNAMIC) != Mode::SOURCE_DYNAMIC && (M & Mode::DESTINATION_DYNAMIC) != Mode::DESTINATION_DYNAMIC)
    {
        return *this;
    }

        /// @brief Configure transfer mode using run-time source and compile-time destination transfer mode.
    /// @param souceWidth Source width, 0: 8 bit, 1: 16 bit, 2: 32 bit
    /// @param sourceFlags Source configuration flags
    /// @return *this
    auto &configure(int souceWidth, Source sourceFlags)
        requires (((M & Mode::SOURCE_DYNAMIC) == Mode::SOURCE_DYNAMIC) && ((M & Mode::DESTINATION_DYNAMIC) != Mode::DESTINATION_DYNAMIC))
    {
        if constexpr ((uint32_t(M) & DMA_SxCR_DIR) == 0) {
            channel->CR = (uint32_t(M) & ~PERIPHERAL_FIELDS)
                | (souceWidth << DMA_SxCR_PSIZE_Pos)
                | uint32_t(sourceFlags);
        } else {
            channel->CR = (uint32_t(M) & ~(PERIPHERAL_FIELDS | MEMORY_FIELDS))
                | (souceWidth << DMA_SxCR_MSIZE_Pos)
                | (uint32_t(sourceFlags) << 1)
                | ((uint32_t(M) & DMA_SxCR_MSIZE) >> 2)
                | ((uint32_t(M) & DMA_SxCR_MINC) >> 1);
        }
        return *this;
    }

    /// @brief Configure transfer mode using compile-time source and run-time destination transfer mode.
    /// @param destinationWidth Destination width, 0: 8 bit, 1: 16 bit, 2: 32 bit
    /// @param destinationFlags Destination configuration flags
    /// @return *this
    auto &configure(int destinationWidth, Destination destinationFlags)
        requires (((M & Mode::SOURCE_DYNAMIC) != Mode::SOURCE_DYNAMIC) && ((M & Mode::DESTINATION_DYNAMIC) == Mode::DESTINATION_DYNAMIC))
    {
        if constexpr ((uint32_t(M) & DMA_SxCR_DIR) == 0) {
            channel->CR = (uint32_t(M) & ~MEMORY_FIELDS)
                | (destinationWidth << DMA_SxCR_MSIZE_Pos)
                | uint32_t(destinationFlags);
        } else {
            channel->CR = (uint32_t(M) & ~(PERIPHERAL_FIELDS | MEMORY_FIELDS))
                | ((uint32_t(M) & DMA_SxCR_PSIZE) << 2)
                | ((uint32_t(M) & DMA_SxCR_PINC) << 1)
                | (destinationWidth << DMA_SxCR_PSIZE_Pos)
                | (uint32_t(destinationFlags) >> 1);
        }
        return *this;
    }

    /// @brief Configure transfer mode.
    /// @param souceWidth Source width, 0: 8 bit, 1: 16 bit, 2: 32 bit
    /// @param soruceFlags Source configuration flags
    /// @param destinationWidth Destination width, 0: 8 bit, 1: 16 bit, 2: 32 bit
    /// @param destinationFlags Destination configuration flags
    /// @return *this
    auto &configure(int souceWidth, Source sourceFlags, int destinationSize, Destination destinationWidth)
        requires ((M & Mode::SOURCE_DYNAMIC) == Mode::SOURCE_DYNAMIC && (M & Mode::DESTINATION_DYNAMIC) == Mode::DESTINATION_DYNAMIC)
    {
        if constexpr ((uint32_t(M) & DMA_SxCR_DIR) == 0) {
            channel->CR = uint32_t(M) & ~(PERIPHERAL_FIELDS | MEMORY_FIELDS)
                | (souceWidth << DMA_SxCR_PSIZE_Pos)
                | uint32_t(sourceFlags)
                | (destinationSize << DMA_SxCR_MSIZE_Pos)
                | uint32_t(destinationWidth);
        } else {
            channel->CR = uint32_t(M) & ~(PERIPHERAL_FIELDS | MEMORY_FIELDS)
                | (souceWidth << DMA_SxCR_MSIZE_Pos)
                | (uint32_t(sourceFlags) << 1)
                | (destinationSize << DMA_SxCR_PSIZE_Pos)
                | (uint32_t(destinationWidth) >> 1);
        }
        return *this;
    }

    /// @brief Get the configured source width.
    /// @return Source width, 0: 8 bit, 1: 16 bit, 2: 32 bit
    int sourceWidth() const {
        if constexpr ((uint32_t(M) & DMA_SxCR_DIR) == 0) {
            if constexpr ((M & Mode::SOURCE_DYNAMIC) != Mode::SOURCE_DYNAMIC)
                return (uint32_t(M) >> DMA_SxCR_PSIZE_Pos) & 3;
            else
                return (channel->CR >> DMA_SxCR_PSIZE_Pos) & 3;
        } else {
            if constexpr ((M & Mode::SOURCE_DYNAMIC) != Mode::SOURCE_DYNAMIC)
                return (uint32_t(M) >> DMA_SxCR_MSIZE_Pos) & 3;
            else
                return (channel->CR >> DMA_SxCR_MSIZE_Pos) & 3;
        }
    }

    /// @brief Get the configured destination width.
    /// @return Destination width, 0: 8 bit, 1: 16 bit, 2: 32 bit
    int destinationWidth() const {
        if constexpr ((uint32_t(M) & DMA_SxCR_DIR) == 0) {
            if constexpr ((M & Mode::DESTINATION_DYNAMIC) != Mode::DESTINATION_DYNAMIC)
                return (uint32_t(M) >> DMA_SxCR_MSIZE_Pos) & 3;
            else
                return (channel->CR >> DMA_SxCR_MSIZE_Pos) & 3;
        } else {
            if constexpr ((M & Mode::DESTINATION_DYNAMIC) != Mode::DESTINATION_DYNAMIC)
                return (uint32_t(M) >> DMA_SxCR_PSIZE_Pos) & 3;
            else
                return (channel->CR >> DMA_SxCR_PSIZE_Pos) & 3;
        }
    }

    /// @brief Set number of items to transfer.
    /// @param count Number if 8, 16 or 32 bit items to transfer
    /// @return *this
    auto &setCount(int count) {
        channel->NDTR = count;
        return *this;
    }

    /// @brief Set source transfer size in bytes.
    /// @param size Number of bytes to transfer
    /// @return *this
    auto &setSourceSize(int size) {
        channel->NDTR = size >> sourceSize();
        return *this;
    }

    /// @brief Set destination transfer size in bytes.
    /// @param size Number of bytes to transfer
    /// @return *this
    auto &setDestinationSize(int size) {
        channel->NDTR = size >> destinationWidth();
        return *this;
    }

    /// @brief Get number of items to transfer.
    /// @return Number of items to transfer
    int count() const {
        return channel->NDTR;
    }

    /// @brief Get source transfer size in bytes.
    /// @return Source transfer size in bytes
    int sourceSize() const {
        return channel->NDTR << sourceWidth();
    }

    /// @brief Get destination transfer size in bytes.
    /// @return Destination transfer size in bytes
    int destinationSize() const {
        return channel->NDTR << destinationWidth();
    }

    /// @brief Set source address.
    /// @param address Source address
    /// @return *this
    template <typename T>
    auto &setSourceAddress(T *address) {
        static_assert(std::is_volatile_v<T>, "Source address must point to a volatile-qualified type");
        if constexpr ((uint32_t(M) & DMA_SxCR_DIR) == 0)
            channel->PAR = uint32_t(address);
        else
            channel->M0AR = uint32_t(address);
        return *this;
    }

    /// @brief Set destination address.
    /// @param address Destination address
    /// @return *this
    template <typename T>
    auto &setDestinationAddress(T *address) {
        static_assert(std::is_volatile_v<T>, "Destination address must point to a volatile-qualified type");
        static_assert(!std::is_const_v<T>, "Destination address must not point to a const-qualified type");
        if constexpr ((uint32_t(M) & DMA_SxCR_DIR) == 0)
            channel->M0AR = uint32_t(address);
        else
            channel->PAR = uint32_t(address);
        return *this;
    }

    /// @brief Enable the DMA channel.
    /// @param config Configuration
    /// @param increment Auto-increment for source or destination
    /// @return *this
    auto &enable(Config config = Config::DEFAULT) {
        if constexpr ((M & Mode::SOURCE_DYNAMIC) != Mode::SOURCE_DYNAMIC && (M & Mode::DESTINATION_DYNAMIC) != Mode::DESTINATION_DYNAMIC) {
            // transfer mode is defined at compile-time
            if constexpr ((uint32_t(M) & DMA_SxCR_DIR) == 0) {
                channel->CR = uint32_t(config)
                    | (channel->CR & DMA_SxCR_CHSEL)
                    | uint32_t(M)
                    | DMA_SxCR_EN;
            } else {
                channel->CR = uint32_t(config)
                    | (channel->CR & DMA_SxCR_CHSEL)
                    | (uint32_t(M) & (DMA_SxCR_DIR | DMA_SxCR_CIRC))
                    | ((uint32_t(M) & DMA_SxCR_PSIZE) << 2) | ((uint32_t(M) & DMA_SxCR_MSIZE) >> 2)
                    | ((uint32_t(M) & DMA_SxCR_PINC) << 1) | ((uint32_t(M) & DMA_SxCR_MINC) >> 1)
                    | DMA_SxCR_EN;
            }
        } else {
            // transfer mode is defined at run-time
            channel->CR = uint32_t(config)
                | (channel->CR & (DMA_SxCR_CHSEL | DMA_SxCR_DIR | DMA_SxCR_CIRC | PERIPHERAL_FIELDS | MEMORY_FIELDS))
                | DMA_SxCR_EN;
        }
        return *this;
    }

    auto &disable() {
        if constexpr ((M & Mode::SOURCE_DYNAMIC) != Mode::SOURCE_DYNAMIC && (M & Mode::DESTINATION_DYNAMIC) != Mode::DESTINATION_DYNAMIC) {
            // transfer mode is defined at compile-time
            channel->CR = channel->CR & DMA_SxCR_CHSEL;
        } else {
            // transfer mode is defined at run-time
            channel->CR = channel->CR
                & (DMA_SxCR_CHSEL | DMA_SxCR_DIR | DMA_SxCR_CIRC | PERIPHERAL_FIELDS | MEMORY_FIELDS);
        }
        return *this;
    }

    bool enabled() const {
        return (channel->CR & DMA_SxCR_EN) != 0;
    }

    /// @brief Get the status flags.
    /// @return Status flags
    Status status() const {
        auto dma = reinterpret_cast<DMA_TypeDef *>(intptr_t(channel) & 0xffffff00);
        return Status((&dma->LISR)[statusPosition >> 5] >> (statusPosition & 31)) & Status::ALL;
    }

    /// @brief Clear status flags.
    /// @return *this
    auto &clear(Status status) {
        auto dma = reinterpret_cast<DMA_TypeDef *>(intptr_t(channel) & 0xffffff00);
        (&dma->LIFCR)[statusPosition >> 5] = int(status) << (statusPosition & 31);
        return *this;
    }

    /// @brief Wait until all of the given status flags are true.
    /// Call clear() before or after wait() to clear active status flags
    /// @param status Status flags to wait for
    /// @return *this
    auto &wait(Status stat) {
        while ((status() & stat) != stat);
    }

    /// @brief Wait until at least one of the given status flags is true.
    /// Call clear() before or after wait() to clear active status flags
    /// @param status Status flags to wait for
    /// @return Active status flags
    Status waitOne(Status stat) {
        Status s;
        do {
            s = status();
        } while ((s & stat) == Status::NONE);
        return s;
    }
};


/// @brief DMA channel info
/// @tparam F Feature flags
template <Feature F = Feature::NONE>
struct Info {
    // registers
    DMA_TypeDef *dma;

    // reset and clock control
    rcc::Info2<offsetof(RCC_TypeDef, AHB1ENR)> rcc;

    // DMA channel index (0 for Channel1)
    uint8_t channelIndex;

    // DMA interrupt index
    uint8_t irq;


    /// @brief Cast to another info structure.
    /// Requires that all features are supported.
    /// @tparam F2 Feature set of info to cast to
    template <Feature F2> requires ((F2 & F) == F2)
    operator Info<F2> () const {
        return {dma, rcc, channelIndex, irq};
    }

    auto channelRegisters() const {return (DMA_Stream_TypeDef *)(intptr_t(dma) + 0x10 + channelIndex * 0x18);}

    /// @brief Enable clock and return a channel wrapper.
    /// @tparam M Mode of Channel
    /// @return Channel (wrapper for channel registers)
    template <Mode M>
    Channel<M> enableClock() const {
        rcc.enableClock();
        return {channelRegisters(), channelIndex * 8 - (channelIndex & 1) * 2};
    }

    /// @brief Get wrapper for the the DMA channel registers.
    /// @tparam M Mode of Channel
    /// @return Channel (wrapper for channel registers)
    template <Mode M>
    Channel<M> channel() const {
        return {channelRegisters(), channelIndex * 8 - (channelIndex & 1) * 2};
    }

    /// @brief Set peripheral DMA request index for the DMA channel (see reference manual for list)
    ///
    void setRequest(int index) const {channelRegisters()->CR = index << DMA_SxCR_CHSEL_Pos;}
};

/// @brief Dual DMA channel info
/// @tparam F Feature flags
template <Feature F = Feature::NONE>
struct DualInfo {
    // registers
    DMA_TypeDef *dma;

    // reset and clock control
    rcc::Info2<offsetof(RCC_TypeDef, AHB1ENR)> rcc;

    // DMA channel index (0 for Channel1)
    uint8_t channelIndex1;
    uint8_t channelIndex2;

    // DMA interrupt index
    uint8_t irq1;
    uint8_t irq2;


    /// @brief Cast to another info structure.
    /// Requires that all features are supported.
    /// @tparam F2 Feature set of info to cast to
    template <Feature F2> requires ((F2 & F) == F2)
    operator DualInfo<F2> () const {
        return {dma, rcc, channelIndex1, channelIndex2, irq1, irq2};
    }

    auto channelRegisters1() const {return (DMA_Stream_TypeDef *)(intptr_t(dma) + 0x10 + channelIndex1 * 0x18);}
    auto channelRegisters2() const {return (DMA_Stream_TypeDef *)(intptr_t(dma) + 0x10 + channelIndex2 * 0x18);}

    /// @brief Enable clock and return a channel wrappers.
    /// @tparam M1 Mode of first channel
    /// @tparam M2 Mode of second channel
    /// @return Channels (wrapper for channel registers)
    template <Mode M1, Mode M2>
    std::pair<Channel<M1>, Channel<M2>> enableClock() const {
        rcc.enableClock();
        return {
            {channelRegisters1(), channelIndex1 * 8 - (channelIndex1 & 1) * 2},
            {channelRegisters2(), channelIndex2 * 8 - (channelIndex2 & 1) * 2}
        };
    }

    /// @brief Get wrapper for the the first DMA channel registers.
    /// @tparam M1 Mode of first channel
    /// @return First channel (wrapper for channel registers of the first channel)
    template <Mode M1>
    Channel<M1> channel1() const {
        return {channelRegisters1(), channelIndex1 * 8 - (channelIndex1 & 1) * 2};
    }

    /// @brief Get wrapper for the the second DMA channel registers.
    /// @tparam M2 Mode of second channel
    /// @return Second channel (wrapper for channel registers of the second channel)
    template <Mode M2>
    Channel<M2> channel2() const {
        return {channelRegisters2(), channelIndex2 * 8 - (channelIndex2 & 1) * 2};
    }

    /// @brief Set peripheral DMA request index for the first DMA channel (see reference manual for list)
    ///
    void setRequest1(int index) const {channelRegisters1()->CR = index << DMA_SxCR_CHSEL_Pos;}

    /// @brief Set peripheral DMA request index for the second DMA channel (see reference manual for list)
    ///
    void setRequest2(int index) const {channelRegisters2()->CR = index << DMA_SxCR_CHSEL_Pos;}
};


using FullInfo = Info<Feature::CIRCULAR>;
using FullDualInfo = DualInfo<Feature::CIRCULAR>;

// single channel DMA infos
static const FullInfo DMA1_CH0_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 0, DMA1_Stream0_IRQn};
static const FullInfo DMA1_CH1_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 1, DMA1_Stream1_IRQn};
static const FullInfo DMA1_CH2_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 2, DMA1_Stream2_IRQn};
static const FullInfo DMA1_CH3_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 3, DMA1_Stream3_IRQn};
static const FullInfo DMA1_CH4_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 4, DMA1_Stream4_IRQn};
static const FullInfo DMA1_CH5_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 5, DMA1_Stream5_IRQn};
static const FullInfo DMA1_CH6_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 6, DMA1_Stream6_IRQn};
static const FullInfo DMA1_CH7_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 7, DMA1_Stream7_IRQn};

static const FullInfo DMA2_CH0_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 0, DMA2_Stream0_IRQn};
static const FullInfo DMA2_CH1_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 1, DMA2_Stream1_IRQn};
static const FullInfo DMA2_CH2_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 2, DMA2_Stream2_IRQn};
static const FullInfo DMA2_CH3_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 3, DMA2_Stream3_IRQn};
static const FullInfo DMA2_CH4_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 4, DMA2_Stream4_IRQn};
static const FullInfo DMA2_CH5_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 5, DMA2_Stream5_IRQn};
static const FullInfo DMA2_CH6_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 6, DMA2_Stream6_IRQn};
static const FullInfo DMA2_CH7_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 7, DMA2_Stream7_IRQn};

// dual channel DMA infos
static const FullDualInfo DMA1_CH0_CH5_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 0, 5, DMA1_Stream0_IRQn, DMA1_Stream5_IRQn};
static const FullDualInfo DMA1_CH0_CH7_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 0, 7, DMA1_Stream0_IRQn, DMA1_Stream7_IRQn};
static const FullDualInfo DMA1_CH1_CH3_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 1, 3, DMA1_Stream1_IRQn, DMA1_Stream3_IRQn};
static const FullDualInfo DMA1_CH2_CH4_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 2, 4, DMA1_Stream2_IRQn, DMA1_Stream4_IRQn};
static const FullDualInfo DMA1_CH2_CH5_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 2, 5, DMA1_Stream2_IRQn, DMA1_Stream5_IRQn};
static const FullDualInfo DMA1_CH2_CH7_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 2, 7, DMA1_Stream2_IRQn, DMA1_Stream7_IRQn};
static const FullDualInfo DMA1_CH3_CH1_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 3, 1, DMA1_Stream3_IRQn, DMA1_Stream1_IRQn};
static const FullDualInfo DMA1_CH3_CH4_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 3, 4, DMA1_Stream3_IRQn, DMA1_Stream4_IRQn};
static const FullDualInfo DMA1_CH3_CH7_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 3, 7, DMA1_Stream3_IRQn, DMA1_Stream7_IRQn};
static const FullDualInfo DMA1_CH5_CH6_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 5, 6, DMA1_Stream5_IRQn, DMA1_Stream6_IRQn};
static const FullDualInfo DMA1_CH6_CH0_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 6, 0, DMA1_Stream6_IRQn, DMA1_Stream0_IRQn};

static const FullDualInfo DMA2_CH0_CH2_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 0, 2, DMA2_Stream0_IRQn, DMA2_Stream2_IRQn};
static const FullDualInfo DMA2_CH0_CH1_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 0, 1, DMA2_Stream0_IRQn, DMA2_Stream1_IRQn};
static const FullDualInfo DMA2_CH0_CH3_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 0, 3, DMA2_Stream0_IRQn, DMA2_Stream3_IRQn};
static const FullDualInfo DMA2_CH0_CH5_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 0, 5, DMA2_Stream0_IRQn, DMA2_Stream5_IRQn};
static const FullDualInfo DMA2_CH1_CH6_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 1, 6, DMA2_Stream1_IRQn, DMA2_Stream6_IRQn};
static const FullDualInfo DMA2_CH1_CH7_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 1, 7, DMA2_Stream1_IRQn, DMA2_Stream7_IRQn};
static const FullDualInfo DMA2_CH2_CH3_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 2, 3, DMA2_Stream2_IRQn, DMA2_Stream3_IRQn};
static const FullDualInfo DMA2_CH2_CH5_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 2, 5, DMA2_Stream2_IRQn, DMA2_Stream5_IRQn};
static const FullDualInfo DMA2_CH2_CH6_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 2, 6, DMA2_Stream2_IRQn, DMA2_Stream6_IRQn};
static const FullDualInfo DMA2_CH2_CH7_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 2, 7, DMA2_Stream2_IRQn, DMA2_Stream7_IRQn};
static const FullDualInfo DMA2_CH3_CH4_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 3, 4, DMA2_Stream3_IRQn, DMA2_Stream4_IRQn};
static const FullDualInfo DMA2_CH5_CH6_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 5, 6, DMA2_Stream5_IRQn, DMA2_Stream6_IRQn};
static const FullDualInfo DMA2_CH5_CH7_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 5, 7, DMA2_Stream5_IRQn, DMA2_Stream7_IRQn};


} // namespace dma
} // namespace coco
