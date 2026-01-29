#pragma once

// do not include directly, use #include <coco/platform/dma.hpp>

#include "platform.hpp"
#include "rcc.hpp"
#include <coco/enum.hpp>
#include <utility>


namespace coco {

/// @brief DMA helpers.
/// Uses TRIGSEL in CxTR2 register to associate peripherals to DMA
///
/// H503 https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series/documentation.html Section 15
/// H5 https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series/documentation.html Section 16
/// U3 Refernece manual: https://www.st.com/resource/en/reference_manual/rm0487-stm32u3-series-armbased-32bit-mcus-stmicroelectronics.pdf
///   Section 15
namespace dma {

/// @brief Mode (CTR1 register).
///
enum class Mode : uint32_t {
    // scope
    PERIPHERAL_TO_MEMORY = DMA_CTR1_DAP,
    MEMORY_TO_PERIPHERAL = DMA_CTR1_SAP,
    MEMORY_TO_MEMORY = DMA_CTR1_SAP | DMA_CTR1_DAP,

    // circular mode, only supported by channels that suppord 2D transfers
    CIRCULAR = 1u << 31,

    // source
    SOURCE_WIDTH_8 = 0, // default
    SOURCE_WIDTH_16 = 1 << DMA_CTR1_SDW_LOG2_Pos,
    SOURCE_WIDTH_32 = 2 << DMA_CTR1_SDW_LOG2_Pos,
    SOURCE_INCREMENT = DMA_CTR1_SINC,
    SOURCE_DYNAMIC = 3 << DMA_CTR1_SDW_LOG2_Pos, // width and increment can be configured at run-time

    // destination
    DESTINATION_WIDTH_8 = 0, // default
    DESTINATION_WIDTH_16 = 1 << DMA_CTR1_DDW_LOG2_Pos,
    DESTINATION_WIDTH_32 = 2 << DMA_CTR1_DDW_LOG2_Pos,
    DESTINATION_INCREMENT = DMA_CTR1_DINC,
    DESTINATION_DYNAMIC = 3 << DMA_CTR1_DDW_LOG2_Pos, // width and increment can be configured at run-time

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
    INCREMENT = DMA_CTR1_SINC,
};

/// @brief Destination configuration
///
enum class Destination : uint32_t {
    NO_INCREMENT = 0,
    INCREMENT = DMA_CTR1_DINC,
};

/// @brief DMA Channel configuration.
///
enum class Config : uint32_t {
    DEFAULT = 0,

    // priority
    PRIORITY_LOW = 0, // default
    PRIORITY_MEDIUM = DMA_CCR_PRIO_0,
    PRIORITY_HIGH = DMA_CCR_PRIO_1,
    PRIORITY_MAX = DMA_CCR_PRIO_0 | DMA_CCR_PRIO_1,

    // interrupts
    TRANSFER_COMPLETE_INTERRUPT = DMA_CCR_TCIE,
    HALF_TRANSFER_INTERRUPT = DMA_CCR_HTIE,

    // link step mode
    LINK_STEP = DMA_CCR_LSM,
};
COCO_ENUM(Config);

/// @brief Status of DMA channel (CSR register).
///
enum class Status : uint32_t {
    NONE = 0,
    IDLE = DMA_CSR_IDLEF,
    TRANSFER_COMPLETE = DMA_CSR_TCF,
    HALF_TRANSFER = DMA_CSR_HTF,
    TRANSFER_ERROR = DMA_CSR_DTEF,
    ALL = TRANSFER_COMPLETE | HALF_TRANSFER | TRANSFER_ERROR
};
COCO_ENUM(Status);


/// @brief Feature flags.
///
enum class Feature {
    NONE = 0,

    // support for circular mode
    CIRCULAR = 1 << 0,

    // support for 2D block transfers
    BLOCK_2D = 1 << 1,
};
COCO_ENUM(Feature)

constexpr uint32_t SOURCE_FIELDS = DMA_CTR1_SDW_LOG2 | DMA_CTR1_SINC | DMA_CTR1_SBL_1 | DMA_CTR1_PAM | DMA_CTR1_SBX;
constexpr uint32_t DESTINATION_FIELDS = DMA_CTR1_DDW_LOG2 | DMA_CTR1_DINC | DMA_CTR1_DBL_1 | DMA_CTR1_DBX | DMA_CTR1_DHX;


/// @brief DMA channel, thin wrapper of DMA_Channel_TypeDef.
/// @tparam M Transfer mode
template <Mode M>
struct Channel {
    static constexpr auto MODE = M;

    DMA_Channel_TypeDef *channel;


    DMA_Channel_TypeDef *operator ->() const {return channel;}
    operator DMA_Channel_TypeDef *() const {return channel;}

    /// @brief Configure transfer mode using compile-time source and destination transfer mode.
    /// @return *this
    auto &configure()
        requires ((M & Mode::SOURCE_DYNAMIC) != Mode::SOURCE_DYNAMIC && (M & Mode::DESTINATION_DYNAMIC) != Mode::DESTINATION_DYNAMIC)
    {
        channel->CTR1 = uint32_t(M & ~Mode::CIRCULAR);
        return *this;
    }

    /// @brief Configure transfer mode using run-time source and compile-time destination transfer mode.
    /// @param souceWidth Source width, 0: 8 bit, 1: 16 bit, 2: 32 bit
    /// @param sourceFlags Source configuration flags
    /// @param sourceBurstLength Source burst Length
    /// @return *this
    auto &configure(int souceWidth, Source sourceFlags, int sourceBurstLength = 1)
        requires (((M & Mode::SOURCE_DYNAMIC) == Mode::SOURCE_DYNAMIC) && ((M & Mode::DESTINATION_DYNAMIC) != Mode::DESTINATION_DYNAMIC))
    {
        channel->CTR1 = (uint32_t(M & ~Mode::CIRCULAR) & ~SOURCE_FIELDS)
            | (souceWidth << DMA_CTR1_SDW_LOG2_Pos)
            | uint32_t(sourceFlags)
            | ((sourceBurstLength - 1) << DMA_CTR1_SBL_1_Pos);
        return *this;
    }

    /// @brief Configure transfer mode using compile-time source and run-time destination transfer mode.
    /// @param destinationWidth Destination width, 0: 8 bit, 1: 16 bit, 2: 32 bit
    /// @param destinationFlags Destination configuration flags
    /// @param destinationBurstLength Destination burst Length
    /// @return *this
    auto &configure(int destinationWidth, Destination destinationFlags, int destinationBurstLength = 1)
        requires (((M & Mode::SOURCE_DYNAMIC) != Mode::SOURCE_DYNAMIC) && ((M & Mode::DESTINATION_DYNAMIC) == Mode::DESTINATION_DYNAMIC))
    {
        channel->CTR1 = (uint32_t(M & ~Mode::CIRCULAR) & ~DESTINATION_FIELDS)
            | (destinationWidth << DMA_CTR1_DDW_LOG2_Pos)
            | uint32_t(destinationFlags)
            | ((destinationBurstLength - 1) << DMA_CTR1_DBL_1_Pos);
        return *this;
    }

    /// @brief Configure transfer mode.
    /// @param souceWidth Source width, 0: 8 bit, 1: 16 bit, 2: 32 bit
    /// @param soruceFlags Source configuration flags
    /// @param destinationWidth Destination width, 0: 8 bit, 1: 16 bit, 2: 32 bit
    /// @param destinationFlags Destination configuration flags
    /// @param destinationBurstLength Destination burst Length
    /// @return *this
    auto &configure(int souceWidth, Source sourceFlags, int destinationWidth, Destination destinationFlags, int destinationBurstLength = 1)
        requires ((M & Mode::SOURCE_DYNAMIC) == Mode::SOURCE_DYNAMIC && (M & Mode::DESTINATION_DYNAMIC) == Mode::DESTINATION_DYNAMIC)
    {
        channel->CTR1 = uint32_t(M & ~Mode::CIRCULAR) & ~(SOURCE_FIELDS | DESTINATION_FIELDS)
            | (souceWidth << DMA_CTR1_SDW_LOG2_Pos)
            | uint32_t(sourceFlags)
            | (destinationWidth << DMA_CTR1_DDW_LOG2_Pos)
            | uint32_t(destinationFlags)
            | ((destinationBurstLength - 1) << DMA_CTR1_DBL_1_Pos);
        return *this;
    }

    /// @brief Configure transfer mode.
    /// @param souceWidth Source width, 0: 8 bit, 1: 16 bit, 2: 32 bit
    /// @param soruceFlags Source configuration flags
    /// @param sourceBurstLength Source burst Length
    /// @param destinationWidth Destination width, 0: 8 bit, 1: 16 bit, 2: 32 bit
    /// @param destinationFlags Destination configuration flags
    /// @param destinationBurstLength Destination burst Length
    /// @return *this
    auto &configure(int souceWidth, Source sourceFlags, int sourceBurstLength, int destinationWidth, Destination destinationFlags, int destinationBurstLength = 1)
        requires ((M & Mode::SOURCE_DYNAMIC) == Mode::SOURCE_DYNAMIC && (M & Mode::DESTINATION_DYNAMIC) == Mode::DESTINATION_DYNAMIC)
    {
        channel->CTR1 = uint32_t(M & ~Mode::CIRCULAR) & ~(SOURCE_FIELDS | DESTINATION_FIELDS)
            | (souceWidth << DMA_CTR1_SDW_LOG2_Pos)
            | uint32_t(sourceFlags)
            | ((sourceBurstLength - 1) << DMA_CTR1_SBL_1_Pos)
            | (destinationWidth << DMA_CTR1_DDW_LOG2_Pos)
            | uint32_t(destinationFlags)
            | ((destinationBurstLength - 1) << DMA_CTR1_DBL_1_Pos);
        return *this;
    }

    /// @brief Get the configured source width.
    /// @return Source width, 0: 8 bit, 1: 16 bit, 2: 32 bit
    int sourceWidth() const {
        if constexpr ((M & Mode::SOURCE_DYNAMIC) == Mode::SOURCE_DYNAMIC)
            return (channel->CTR1 >> DMA_CTR1_SDW_LOG2_Pos) & 3;
        else
            return (int(M) >> DMA_CTR1_SDW_LOG2_Pos) & 3;
    }

    /// @brief Get the configured destination width.
    /// @return Destination width, 0: 8 bit, 1: 16 bit, 2: 32 bit
    int destinationWidth() const {
        if constexpr ((M & Mode::DESTINATION_DYNAMIC) == Mode::DESTINATION_DYNAMIC)
            return (channel->CTR1 >> DMA_CTR1_DDW_LOG2_Pos) & 3;
        else
            return (int(M) >> DMA_CTR1_DDW_LOG2_Pos) & 3;
    }

    /// @brief Set number of items to transfer.
    /// @param count Number of 8, 16 or 32 bit items to transfer
    /// @return *this
    auto &setCount(int count) {
        setSourceSize(count << sourceWidth());
        return *this;
    }

    /// @brief Set source transfer size in bytes.
    /// @param size Number of bytes to transfer from the source
    /// @return *this
    auto &setSourceSize(int size) {
        if constexpr ((M & Mode::CIRCULAR) == 0) {
            channel->CBR1 = size;
        } else {
            channel->CBR1 = size
                | (1 << DMA_CBR1_BRC_Pos) // one block
                | DMA_CBR1_BRSDEC // subtract size at end of transfer
                | DMA_CBR1_BRDDEC;

            // size to subtract at end of transfer
            uint32_t ssize = size << DMA_CBR2_BRSAO_Pos;
            uint32_t dsize = size << DMA_CBR2_BRDAO_Pos;
            if constexpr ((M & Mode::SOURCE_DYNAMIC) != Mode::SOURCE_DYNAMIC && (M & Mode::DESTINATION_DYNAMIC) != Mode::DESTINATION_DYNAMIC) {
                // compile-time source and destination transfer mode
                channel->CBR2 = (((uint32_t(M) & DMA_CTR1_SINC) != 0) ? ssize : 0)
                    | (((uint32_t(M) & DMA_CTR1_DINC) != 0) ? dsize : 0);
            } else {
                auto ctr1 = channel->CTR1;
                if constexpr ((M & Mode::DESTINATION_DYNAMIC) != Mode::DESTINATION_DYNAMIC) {
                    // run-time source and compile-time destination transfer mode
                    channel->CBR2 = (((ctr1 & DMA_CTR1_SINC) != 0) ? ssize : 0)
                        | (((uint32_t(M) & DMA_CTR1_DINC) != 0) ? dsize : 0);
                } else if constexpr ((M & Mode::SOURCE_DYNAMIC) != Mode::SOURCE_DYNAMIC) {
                    // compile-time source and run-time destination transfer mode
                    channel->CBR2 = (((uint32_t(M) & DMA_CTR1_SINC) != 0) ? ssize : 0)
                        | (((ctr1 & DMA_CTR1_DINC) != 0) ? dsize : 0);
                } else {
                    // run-time source and destination transfer mode
                    channel->CBR2 = (((ctr1 & DMA_CTR1_SINC) != 0) ? ssize : 0)
                        | (((ctr1 & DMA_CTR1_DINC) != 0) ? dsize : 0);
                }
            }

            // auto-restart
            channel->CLLR = 1 << DMA_CLLR_LA_Pos;
        }
        return *this;
    }

    /// @brief Set destination transfer size in bytes.
    /// @param size Number of bytes to transfer
    /// @return *this
    auto &setDestinationSize(int size) {
        setSourceSize((size >> destinationWidth()) << sourceWidth());
        return *this;
    }

    /// @brief Get number of items to transfer.
    /// @return Number of items to transfer
    int count() const {
        return channel->CBR1 >> sourceWidth();
    }

    /// @brief Get source transfer size in bytes.
    /// @return Source transfer size in bytes
    int sourceSize() const {
        return channel->CBR1;
    }

    /// @brief Get destination transfer size in bytes.
    /// @return Destination transfer size in bytes
    int destinationSize() const {
        return (channel->CBR1 >> sourceWidth()) << destinationWidth();
    }

    /// @brief Set source address.
    /// @param address Source address, must be volatile-qualified
    /// @return *this
    template <typename T>
    auto &setSourceAddress(T *address) {
        static_assert(std::is_volatile_v<T>, "Source address must point to a volatile-qualified type");
        channel->CSAR = uint32_t(address);
        return *this;
    }

    /// @brief Set destination address.
    /// @param address Destination address, must be volatile-qualified
    /// @return *this
    template <typename T>
    auto &setDestinationAddress(T *address) {
        static_assert(std::is_volatile_v<T>, "Destination address must point to a volatile-qualified type");
        static_assert(!std::is_const_v<T>, "Destination address must not point to a const-qualified type");
        channel->CDAR = uint32_t(address);
        return *this;
    }

    /// @brief Enable the DMA channel.
    /// @param config Configuration
    /// @param increment Auto-increment for source or destination
    /// @return *this
    auto &enable(Config config = Config::DEFAULT) {
        channel->CCR = uint32_t(config) | DMA_CCR_EN;
        return *this;
    }

    /// @brief Disable DMA channel with blocking wait for idle.
    /// @return *this
    auto &disable() {
        channel->CCR = DMA_CCR_SUSP;
        while ((channel->CSR & DMA_CSR_IDLEF) == 0);
        channel->CCR = DMA_CCR_RESET;
        return *this;
    }

    /// @brief Get enabled state.
    /// @return True if enabled, false if disabled
    bool enabled() const {
        return (channel->CCR & DMA_CCR_EN) != 0;
    }

    /// @brief Get the status flags.
    /// @return Status flags
    Status status() {
        return Status(channel->CSR);
    }

    /// @brief Clear status flags.
    /// @return *this
    auto &clear(Status status) {
        channel->CFCR = int(status);
        return *this;
    }

    /// @brief Wait until all of the given status flags are true.
    /// Call clear() before or after wait() to clear active status flags
    /// @param status Status flags to wait for
    /// @return *this
    auto &wait(Status stat) {
        while ((status() & stat) != stat);
        return *this;
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

/// @brief Node for linked lists
///
struct ListNode {
    // transfer registers
    volatile uint32_t TR1;
    volatile uint32_t TR2;

    // block register 1 (source transfer size in bytes)
    volatile uint32_t BR1;

    // source address register
    volatile uint32_t SAR;

    // destination address register
    volatile uint32_t DAR;

    // linked-list address register
    volatile uint32_t LLR;

    /// @brief Set source address.
    /// @param address Source address
    /// @return *this
    ListNode &setSourceAddress(volatile void *address) {
        SAR = uint32_t(address);
        return *this;
    }

    /// @brief Set destination address.
    /// @param address Destination address
    /// @return *this
    ListNode &setDestinationAddress(volatile void *address) {
        DAR = uint32_t(address);
        return *this;
    }

    /// @brief Set source transfer size in bytes.
    /// @param size Number of bytes to transfer from the source
    /// @return *this
    auto &setSize(int size) {
        BR1 = size;
        return *this;
    }
};


/// @brief DMA channel info
/// @tparam F Feature flags
template <Feature F = Feature::NONE>
struct Info {
    // registers
    DMA_TypeDef *dma;

    // reset and clock control
    rcc::DmaInfo rcc;

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

    auto channelRegisters() const {return (DMA_Channel_TypeDef *)(intptr_t(dma) + 0x50 + channelIndex * 0x80);}

    /// @brief Enable clock and return a channel wrapper.
    /// @tparam M Mode of Channel
    /// @return Channel (wrapper for channel registers)
    template <Mode M>
    Channel<M> enableClock() const {
        static_assert(!((F & Feature::CIRCULAR) == 0 && (M & Mode::CIRCULAR) != 0), "Circular mode not supported");
        rcc.enableClock();
        return {channelRegisters()};
    }

    /// @brief Get wrapper for the the DMA channel registers.
    /// @tparam M Mode of Channel
    /// @return Channel (wrapper for channel registers)
    template <Mode M>
    Channel<M> channel() const {
        static_assert(!((F & Feature::CIRCULAR) == 0 && (M & Mode::CIRCULAR) != 0), "Circular mode not supported");
        return {channelRegisters()};
    }

    /// @brief Set RX peripheral DMA request index for the DMA channel (see reference manual for list)
    ///
    void setRxRequest(int index) const {channelRegisters()->CTR2 = index << DMA_CTR2_REQSEL_Pos;}

    /// @brief Set TX peripheral DMA request index for the DMA channel (see reference manual for list)
    ///
    void setTxRequest(int index) const {channelRegisters()->CTR2 = DMA_CTR2_DREQ | (index << DMA_CTR2_REQSEL_Pos);}
};

/// @brief Dual DMA channel info
/// @tparam F Feature flags
template <Feature F = Feature::NONE>
struct DualInfo {
    // registers
    DMA_TypeDef *dma;

    // reset and clock control
    rcc::DmaInfo rcc;

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

    auto channel1Registers() const {return (DMA_Channel_TypeDef *)(intptr_t(dma) + 0x50 + channelIndex1 * 0x80);}
    auto channel2Registers() const {return (DMA_Channel_TypeDef *)(intptr_t(dma) + 0x50 + channelIndex2 * 0x80);}

    /// @brief Enable clock and return a channel wrappers.
    /// @tparam M1 Mode of first channel
    /// @tparam M2 Mode of second channel
    /// @return Channels (wrapper for channel registers)
    template <Mode M1, Mode M2>
    std::pair<Channel<M1>, Channel<M2>> enableClock() const {
        static_assert(!((F & Feature::CIRCULAR) == 0 && ((M1 | M2) & Mode::CIRCULAR) != 0), "Circular mode not supported");
        rcc.enableClock();
        return {{channel1Registers()}, {channel2Registers()}};
    }

    /// @brief Get wrapper for the the first DMA channel registers.
    /// @tparam M1 Mode of first channel
    /// @return First channel (wrapper for channel registers of the first channel)
    template <Mode M1>
    Channel<M1> channel1() const {
        static_assert(!((F & Feature::CIRCULAR) == 0 && (M1 & Mode::CIRCULAR) != 0), "Circular mode not supported");
        return {channel1Registers()};
    }

    /// @brief Get wrapper for the the second DMA channel registers.
    /// @tparam M2 Mode of second channel
    /// @return Second channel (wrapper for channel registers of the second channel)
    template <Mode M2>
    Channel<M2> channel2() const {
        static_assert(!((F & Feature::CIRCULAR) == 0 && (M2 & Mode::CIRCULAR) != 0), "Circular mode not supported");
        return {channel2Registers()};
    }

    /// @brief Set RX peripheral DMA request index for the first DMA channel (see reference manual for list)
    ///
    void setRxRequest1(int index) const {channel1Registers()->CTR2 = index << DMA_CTR2_REQSEL_Pos;}

    /// @brief Set TX peripheral DMA request index for the second DMA channel (see reference manual for list)
    ///
    void setTxRequest2(int index) const {channel2Registers()->CTR2 = DMA_CTR2_DREQ | (index << DMA_CTR2_REQSEL_Pos);}
};

} // namespace dma
} // namespace coco
