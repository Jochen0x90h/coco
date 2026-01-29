#pragma once

// do not include directly, use #include <coco/platform/dma.hpp>

#include "platform.hpp"
#include "rcc.hpp"
#include <coco/enum.hpp>
#include <utility>
#include <type_traits>


namespace coco {

/// @brief DMA helpers.
/// F0, L0, F1, F3, L1, L4 use SYSCFG to associate peripherals to DMA
/// C0, G0, G4 use DMAMUX to associate peripherals to DMA
///
/// G0X0 https://www.st.com/en/microcontrollers-microprocessors/stm32g0-series/documentation.html Sections 9 + 10
/// G0X1 https://www.st.com/en/microcontrollers-microprocessors/stm32g0-series/documentation.html Sections 10 + 11
/// G4 https://www.st.com/en/microcontrollers-microprocessors/stm32g4-series/documentation.html Sections 12 + 13
namespace dma {

/// @brief Mode (CCR register).
///
enum class Mode : uint32_t {
    // scope
    PERIPHERAL_TO_MEMORY = 0,
    MEMORY_TO_PERIPHERAL = DMA_CCR_DIR,
    MEMORY_TO_MEMORY = DMA_CCR_MEM2MEM,

    // circular mode flag
    CIRCULAR = DMA_CCR_CIRC,

    // source
    SOURCE_WIDTH_8 = 0, // default
    SOURCE_WIDTH_16 = 1 << DMA_CCR_PSIZE_Pos, // note source and destination fields need to be swapped when DIR is set
    SOURCE_WIDTH_32 = 2 << DMA_CCR_PSIZE_Pos,
    SOURCE_INCREMENT = DMA_CCR_PINC,
    SOURCE_DYNAMIC = 3 << DMA_CCR_PSIZE_Pos, // width and increment can be configured at run-time

    // destination
    DESTINATION_WIDTH_8 = 0, // default
    DESTINATION_WIDTH_16 = 1 << DMA_CCR_MSIZE_Pos,
    DESTINATION_WIDTH_32 = 2 << DMA_CCR_MSIZE_Pos,
    DESTINATION_INCREMENT = DMA_CCR_MINC,
    DESTINATION_DYNAMIC = 3 << DMA_CCR_MSIZE_Pos, // width and increment can be configured at run-time

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
    INCREMENT = DMA_CCR_PINC,
};

/// @brief Destination configuration
///
enum class Destination : uint32_t {
    NO_INCREMENT = 0,
    INCREMENT = DMA_CCR_MINC,
};

/// @brief DMA Channel configuration.
///
enum class Config : uint32_t {
    DEFAULT = 0,

    // optional circular mode flag
    //CIRCULAR = DMA_CCR_CIRC,

    // priority
    PRIORITY_LOW = 0, // default
    PRIORITY_MEDIUM = DMA_CCR_PL_0,
    PRIORITY_HIGH = DMA_CCR_PL_1,
    PRIORITY_MAX = DMA_CCR_PL_0 | DMA_CCR_PL_1,

    // interrupts
    TRANSFER_COMPLETE_INTERRUPT = DMA_CCR_TCIE,
    HALF_TRANSFER_INTERRUPT = DMA_CCR_HTIE,
};
COCO_ENUM(Config);

/// @brief Status of DMA channel (ISR register).
///
enum class Status : uint32_t {
    NONE = 0,
    GLOBAL = DMA_ISR_GIF1,
    TRANSFER_COMPLETE = DMA_ISR_TCIF1,
    HALF_TRANSFER = DMA_ISR_HTIF1,
    TRANSFER_ERROR = DMA_ISR_TEIF1,
    ALL = GLOBAL | TRANSFER_COMPLETE | HALF_TRANSFER | TRANSFER_ERROR
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

constexpr uint32_t PERIPHERAL_FIELDS = DMA_CCR_PSIZE | DMA_CCR_PINC;
constexpr uint32_t MEMORY_FIELDS = DMA_CCR_MSIZE | DMA_CCR_MINC;


/// @brief DMA channel, thin wrapper of DMA_Channel_TypeDef.
/// @tparam M Transfer mode
template <Mode M>
struct Channel {
    static constexpr auto MODE = M;

    DMA_Channel_TypeDef *channel;
    int statusPosition;


    DMA_Channel_TypeDef *operator ->() const {return channel;}
    operator DMA_Channel_TypeDef *() const {return channel;}

    /// @brief Configure transfer mode using compile-time source and destination transfer mode.
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
        if constexpr ((uint32_t(M) & DMA_CCR_DIR) == 0) {
            channel->CCR = (uint32_t(M) & ~PERIPHERAL_FIELDS)
                | (souceWidth << DMA_CCR_PSIZE_Pos)
                | uint32_t(sourceFlags);
        } else {
            channel->CCR = (uint32_t(M) & ~(PERIPHERAL_FIELDS | MEMORY_FIELDS))
                | (souceWidth << DMA_CCR_MSIZE_Pos)
                | (uint32_t(sourceFlags) << 1)
                | ((uint32_t(M) & DMA_CCR_MSIZE) >> 2)
                | ((uint32_t(M) & DMA_CCR_MINC) >> 1);
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
        if constexpr ((uint32_t(M) & DMA_CCR_DIR) == 0) {
            channel->CCR = (uint32_t(M) & ~MEMORY_FIELDS)
                | (destinationWidth << DMA_CCR_MSIZE_Pos)
                | uint32_t(destinationFlags);
        } else {
            channel->CCR = (uint32_t(M) & ~(PERIPHERAL_FIELDS | MEMORY_FIELDS))
                | ((uint32_t(M) & DMA_CCR_PSIZE) << 2)
                | ((uint32_t(M) & DMA_CCR_PINC) << 1)
                | (destinationWidth << DMA_CCR_PSIZE_Pos)
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
    auto &configure(int souceWidth, Source sourceFlags, int destinationWidth, Destination destinationFlags)
        requires ((M & Mode::SOURCE_DYNAMIC) == Mode::SOURCE_DYNAMIC && (M & Mode::DESTINATION_DYNAMIC) == Mode::DESTINATION_DYNAMIC)
    {
        if constexpr ((uint32_t(M) & DMA_CCR_DIR) == 0) {
            channel->CCR = uint32_t(M) & ~(PERIPHERAL_FIELDS | MEMORY_FIELDS)
                | (souceWidth << DMA_CCR_PSIZE_Pos)
                | uint32_t(sourceFlags)
                | (destinationWidth << DMA_CCR_MSIZE_Pos)
                | uint32_t(destinationFlags);
        } else {
            channel->CCR = uint32_t(M) & ~(PERIPHERAL_FIELDS | MEMORY_FIELDS)
                | (souceWidth << DMA_CCR_MSIZE_Pos)
                | (uint32_t(sourceFlags) << 1)
                | (destinationWidth << DMA_CCR_PSIZE_Pos)
                | (uint32_t(destinationFlags) >> 1);
        }
        return *this;
    }

    /// @brief Get the configured source width.
    /// @return Source width, 0: 8 bit, 1: 16 bit, 2: 32 bit
    int sourceWidth() const {
        if constexpr ((uint32_t(M) & DMA_CCR_DIR) == 0) {
            if constexpr ((M & Mode::SOURCE_DYNAMIC) != Mode::SOURCE_DYNAMIC)
                return (uint32_t(M) >> DMA_CCR_PSIZE_Pos) & 3;
            else
                return (channel->CCR >> DMA_CCR_PSIZE_Pos) & 3;
        } else {
            if constexpr ((M & Mode::SOURCE_DYNAMIC) != Mode::SOURCE_DYNAMIC)
                return (uint32_t(M) >> DMA_CCR_MSIZE_Pos) & 3;
            else
                return (channel->CCR >> DMA_CCR_MSIZE_Pos) & 3;
        }
    }

    /// @brief Get the configured destination width.
    /// @return Destination width, 0: 8 bit, 1: 16 bit, 2: 32 bit
    int destinationWidth() const {
        if constexpr ((uint32_t(M) & DMA_CCR_DIR) == 0) {
            if constexpr ((M & Mode::DESTINATION_DYNAMIC) != Mode::DESTINATION_DYNAMIC)
                return (uint32_t(M) >> DMA_CCR_MSIZE_Pos) & 3;
            else
                return (channel->CCR >> DMA_CCR_MSIZE_Pos) & 3;
        } else {
            if constexpr ((M & Mode::DESTINATION_DYNAMIC) != Mode::DESTINATION_DYNAMIC)
                return (uint32_t(M) >> DMA_CCR_PSIZE_Pos) & 3;
            else
                return (channel->CCR >> DMA_CCR_PSIZE_Pos) & 3;
        }
    }

    /// @brief Set number of items to transfer.
    /// @param count Number if 8, 16 or 32 bit items to transfer
    /// @return *this
    auto &setCount(int count) {
        channel->CNDTR = count;
        return *this;
    }

    /// @brief Set source transfer size in bytes.
    /// @param size Number of bytes to transfer
    /// @return *this
    auto &setSourceSize(int size) {
        channel->CNDTR = size >> sourceWidth();
        return *this;
    }

    /// @brief Set destination transfer size in bytes.
    /// @param size Number of bytes to transfer
    /// @return *this
    auto &setDestinationSize(int size) {
        channel->CNDTR = size >> destinationWidth();
        return *this;
    }

    /// @brief Get number of items to transfer.
    /// @return Number of items to transfer
    int count() const {
        return channel->CNDTR;
    }

    /// @brief Get source transfer size in bytes.
    /// @return Source transfer size in bytes
    int sourceSize() const {
        return channel->CNDTR << sourceWidth();
    }

    /// @brief Get destination transfer size in bytes.
    /// @return Destination transfer size in bytes
    int destinationSize() const {
        return channel->CNDTR << destinationWidth();
    }

    /// @brief Set source address.
    /// @param address Source address, must be volatile-qualified
    /// @return *this
    template <typename T>
    auto &setSourceAddress(T *address) {
        static_assert(std::is_volatile_v<T>, "Source address must point to a volatile-qualified type");
        if constexpr ((uint32_t(M) & DMA_CCR_DIR) == 0)
            channel->CPAR = uint32_t(address);
        else
            channel->CMAR = uint32_t(address);
        return *this;
    }

    /// @brief Set destination address.
    /// @param address Destination address, must be volatile-qualified
    /// @return *this
    template <typename T>
    auto &setDestinationAddress(T *address) {
        static_assert(std::is_volatile_v<T>, "Destination address must point to a volatile-qualified type");
        static_assert(!std::is_const_v<T>, "Destination address must not point to a const-qualified type");
        if constexpr ((uint32_t(M) & DMA_CCR_DIR) == 0)
            channel->CMAR = uint32_t(address);
        else
            channel->CPAR = uint32_t(address);
        return *this;
    }

    /// @brief Enable the DMA channel.
    /// @param config Configuration
    /// @param increment Auto-increment for source or destination
    /// @return *this
    auto &enable(Config config = Config::DEFAULT) {
        if constexpr ((M & Mode::SOURCE_DYNAMIC) != Mode::SOURCE_DYNAMIC && (M & Mode::DESTINATION_DYNAMIC) != Mode::DESTINATION_DYNAMIC) {
            // transfer mode is defined at compile-time
            if constexpr ((uint32_t(M) & DMA_CCR_DIR) == 0) {
                channel->CCR = uint32_t(config)
                    | uint32_t(M)
                    | DMA_CCR_EN;
            } else {
                channel->CCR = uint32_t(config)
                    | (uint32_t(M) & (DMA_CCR_DIR | DMA_CCR_MEM2MEM | DMA_CCR_CIRC))
                    | ((uint32_t(M) & DMA_CCR_PSIZE) << 2) | ((uint32_t(M) & DMA_CCR_MSIZE) >> 2)
                    | ((uint32_t(M) & DMA_CCR_PINC) << 1) | ((uint32_t(M) & DMA_CCR_MINC) >> 1)
                    | DMA_CCR_EN;
            }
        } else {
            // transfer mode is defined at run-time
            channel->CCR = uint32_t(config)
                | (channel->CCR & (DMA_CCR_DIR | DMA_CCR_MEM2MEM | DMA_CCR_CIRC | PERIPHERAL_FIELDS | MEMORY_FIELDS))
                | DMA_CCR_EN;
        }
        return *this;
    }

    auto &disable() {
        if constexpr ((M & Mode::SOURCE_DYNAMIC) != Mode::SOURCE_DYNAMIC && (M & Mode::DESTINATION_DYNAMIC) != Mode::DESTINATION_DYNAMIC) {
            // transfer mode is defined at compile-time
            channel->CCR = 0;
        } else {
            // transfer mode is defined at run-time
            channel->CCR = channel->CCR
                & (DMA_CCR_DIR | DMA_CCR_MEM2MEM | DMA_CCR_CIRC | PERIPHERAL_FIELDS | MEMORY_FIELDS);
        }
        return *this;
    }

    bool enabled() const {
        return (channel->CCR & DMA_CCR_EN) != 0;
    }

    /// @brief Get the status flags.
    /// @return Status flags
    Status status() const {
        auto dma = reinterpret_cast<DMA_TypeDef *>(intptr_t(channel) & 0xffffff00);
        return Status(dma->ISR >> statusPosition) & Status::ALL;
    }

    /// @brief Clear status flags.
    /// @param status Status flags to clear
    /// @return *this
    auto &clear(Status status) {
        auto dma = reinterpret_cast<DMA_TypeDef *>(intptr_t(channel) & 0xffffff00);
        dma->IFCR = int(status) << statusPosition;
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

} // namespace dma
} // namespace coco
