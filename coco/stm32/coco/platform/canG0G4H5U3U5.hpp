#pragma once

// do not include directly, use #include <coco/platform/can.hpp>

#include "platform.hpp"
#include "rcc.hpp"
#include <coco/enum.hpp>


/*
    Defines:
    HAVE_CAN         Classic CAN supported (is not supported by this file)
    HAVE_FDCAN       FDCAN supported
*/

// check if FDCAN is implemented in the current device
#ifdef SRAMCAN_BASE
#define HAVE_FDCAN


namespace coco {

/// @brief FDCAN helpers.
/// G0X0 https://www.st.com/en/microcontrollers-microprocessors/stm32g0-series/documentation.html Not supported
/// G0X1 https://www.st.com/en/microcontrollers-microprocessors/stm32g0-series/documentation.html Section 36
/// G4 https://www.st.com/en/microcontrollers-microprocessors/stm32g4-series/documentation.html Section 44
/// H503 https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series/documentation.html Section 39
/// H5 https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series/documentation.html Sections 54
/// U3 Reference manual: https://www.st.com/en/microcontrollers-microprocessors/stm32u3-series/documentation.html
///   Section 51
namespace can {

//static const auto RAM = reinterpret_cast<volatile uint32_t *>(SRAMCAN_BASE);

/// @brief Clock config (register CKDIV)
/// Note: Only the first instance (FDCAN1) has clock config
enum class ClockConfig : uint32_t {
    // clock prescaler
    DIV_1 = 0,
    DIV_2 = 1,
    DIV_4 = 2,
    DIV_6 = 3,
    DIV_8 = 4,
    DIV_10 = 5,
    DIV_12 = 6,
    DIV_14 = 7,
    DIV_16 = 8,
    DIV_18 = 9,
    DIV_20 = 10,
    DIV_22 = 11,
    DIV_24 = 12,
    DIV_26 = 13,
    DIV_28 = 14,
    DIV_30 = 15,
};


/// @brief Config
enum class Config : uint32_t {
    DEFAULT = 0,

    // FD mode
    FD = FDCAN_CCCR_FDOE,

    // fast mode (bit rate switching)
    FAST = FDCAN_CCCR_BRSE,

    // monitoring mode (disable tx)
    MONITORING = FDCAN_CCCR_MON,

    // restricted operation mode
    RESTRICTED_MODE = FDCAN_CCCR_ASM,
};
COCO_ENUM(Config)

/// @brief Create nominal bit timing (NBTP register)
/// @param prescaler Prescaler for bit time quantum, range [1, 512]
/// @param segment1 Length of bit segment 1 in time quanta, range [1, 256]
/// @param segment2 Length of bit segment 1 in time quanta, range [1, 128]
/// @param syncJumpWidth Synchronization jump width defines an upper bound for the amount of lengthening or shortening of the bit segments, range [1, 128]
/// @return Configuration of NBTP register
constexpr uint32_t makeNominalBitTiming(int prescaler, int segment1, int segment2, int syncJumpWidth) {
    return ((prescaler - 1) << FDCAN_NBTP_NBRP_Pos)
        | ((segment1 - 1) << FDCAN_NBTP_NTSEG1_Pos)
        | ((segment2 - 1) << FDCAN_NBTP_NTSEG2_Pos)
        | ((syncJumpWidth - 1) << FDCAN_NBTP_NSJW_Pos);
}

/// @brief Create data bit timing for CAN FD high speed data phase (DBTP register)
/// @param prescaler Prescaler for bit time quantum, range [1, 32]
/// @param segment1 Length of bit segment 1 in time quanta, range [1, 32]
/// @param segment2 Length of bit segment 1 in time quanta, range [1, 16]
/// @param syncJumpWidth Synchronization jump width defines an upper bound for the amount of lengthening or shortening of the bit segments, range [1, 16]
/// @param delayCompensation Transceiver delay compensation
/// @return Configuration of DBTP register
constexpr uint32_t makeDataBitTiming(int prescaler, int segment1, int segment2, int syncJumpWidth, bool delayCompensation) {
    return ((prescaler - 1) << FDCAN_DBTP_DBRP_Pos)
        | ((segment1 - 1) << FDCAN_DBTP_DTSEG1_Pos)
        | ((segment2 - 1) << FDCAN_DBTP_DTSEG2_Pos)
        | ((syncJumpWidth - 1) << FDCAN_DBTP_DSJW_Pos)
        | (delayCompensation ? FDCAN_DBTP_TDC : 0);
}


/// @brief RX config (register RXGFC)
///
enum class RxConfig : uint32_t {
    DEFAULT = 0,

    // fifo operating mode
    FIFO0_BLOCKING = 0, // default
    FIFO0_OVERWRITE = 1 << FDCAN_RXGFC_F0OM_Pos,
    FIFO1_BLOCKING = 0, // default
    FIFO1_OVERWRITE = 1 << FDCAN_RXGFC_F1OM_Pos,

    // default action for standard frames after filtering
    STANDARD_ACCEPT_FIFO0 = 0, // default
    STANDARD_ACCEPT_FIFO1 = 1 << FDCAN_RXGFC_ANFS_Pos,
    STANDARD_REJECT = 2 << FDCAN_RXGFC_ANFS_Pos,

    // default action for extended frames after filtering
    EXTENDED_ACCEPT_FIFO0 = 0, // default
    EXTENDED_ACCEPT_FIFO1 = 1 << FDCAN_RXGFC_ANFS_Pos,
    EXTENDED_REJECT = 2 << FDCAN_RXGFC_ANFS_Pos,

    // handling of remote frames
    REMOTE_FILTER = 0, // default
    REMOTE_REJECT = 1 << FDCAN_RXGFC_RRFS,
};
COCO_ENUM(RxConfig)

/// @brief When a filter matches, an action is executed
/// Part of FDCAN standard/extended message ID filter element
enum class FilterAction {
    // filter is disabled
    DISABLED = 0,

    // store message in Rx FIFO 0 if filter matches
    ACCEPT_FIFO0 = 1,

    // store message in Rx FIFO 1 if filter matches
    ACCEPT_FIFO1 = 2,

    // reject message if filter matches
    REJECT = 3,

    // set priority if filter matches
    SET_PRIORITY = 4,

    // set priority and store message in FIFO 0 if filter matches
    SET_PRIORITY_ACCEPT_FIFO0 = 5,

    // set priority and store message in FIFO 1 if filter matches
    SET_PRIORITY_ACCEPT_FIFO1 = 6,
};

using StandardFilter = uint32_t;

/// @brief Create a classic mask filer for standard 11-bit ids.
/// @param action Filter action
/// @param id Id to filter for (comparison is message.id & mask == id)
/// @param mask AND-mask on id, zero bits are not relevant for comparison, range [0, 2047]
/// @return StandardFilter
constexpr StandardFilter makeStandardMaskFilter(FilterAction action, int id, int mask = 0x7ff) {
    return (2 << 30) | (uint32_t(action) << 27) | (id << 16) | mask;
}

/// @brief Create a dual filter for standard 11-bit ids.
/// @param action Filter action
/// @param id1 First id
/// @param id2 Second id
/// @return StandardFilter
constexpr StandardFilter makeStandardDualFilter(FilterAction action, int id1, int id2) {
    return (1 << 30) | (uint32_t(action) << 27) | (id1 << 16) | id2;
}

/// @brief Crate a range filter for standard 11-bit ids.
/// @param action Filter action
/// @param minId Minimum id
/// @param maxId Maximum id
/// @return StandardFilter
constexpr StandardFilter makeStandardRangeFilter(FilterAction action, int minId, int maxId) {
    return (0 << 30) | (uint32_t(action) << 27) | (minId << 16) | maxId;
}

struct ExtendedFilter {
    uint32_t F0;
    uint32_t F1;
};

/// @brief Create a classic mask filer for extended 29-bit ids.
/// @param action Filter action
/// @param id Id to filter for (comparison is message.id & mask == id)
/// @param mask AND-mask on id, zero bits are not relevant for comparison
/// @return ExtendedFilter
constexpr ExtendedFilter makeExtendedMaskFilter(FilterAction action, int id, int mask = 0x1fffffff) {
    return {(uint32_t(action) << 29) | id, uint32_t((2 << 30) | mask)};
}

/// @brief Create a dual filter for extended 29-bit ids.
/// @param action Filter action
/// @param id1 First id
/// @param id2 Second id
/// @return ExtendedFilter
constexpr ExtendedFilter makeExtendedDualFilter(FilterAction action, int id1, int id2) {
    return {(uint32_t(action) << 29) | id1, uint32_t((1 << 30) | id2)};
}

/// @brief Crate a range filter for extended 29-bit ids.
/// @param action Filter action
/// @param minId Minimum id
/// @param maxId Maximum id
/// @return ExtendedFilter
constexpr ExtendedFilter makeExtendedRangeFilter(FilterAction action, int minId, int maxId) {
    return {(uint32_t(action) << 29) | minId, uint32_t((0 << 30) | maxId)};
}


/// @brief TX config (register RXGFC)
///
enum class TxConfig : uint32_t {
    DEFAULT = 0,

    // TX FIFO/queue mode
    TX_FIFO = 0, // default
    TX_QUEUE = FDCAN_TXBC_TFQM,
};

/// @brief Message configuration.
///
enum class MessageConfig : uint32_t {
    NONE = 0,

    //
    ERROR_STATE_INDICATOR = 1u << 31,

    // 29-bit id instead of 11-bit id
    EXTENDED_ID = 1 << 30,

    REMOTE_FRAME = 1 << 29,

    STORE_TX_EVENTS = 1 << 23,

    // CAN FD frame format
    FD = 1 << 21,

    // fast mode (bit rate switching)
    FAST = 1 << 20,

    LENGTH0 = 0,
    LENGTH1 = 1 << 16,
    LENGTH2 = 2 << 16,
    LENGTH3 = 3 << 16,
    LENGTH4 = 4 << 16,
    LENGTH5 = 5 << 16,
    LENGTH6 = 6 << 16,
    LENGTH7 = 7 << 16,
    LENGTH8 = 8 << 16,
    LENGTH12 = 9 << 16,
    LENGTH16 = 10 << 16,
    LENGTH20 = 11 << 16,
    LENGTH24 = 12 << 16,
    LENGTH32 = 13 << 16,
    LENGTH48 = 14 << 16,
    LENGTH64 = 15 << 16,
};
COCO_ENUM(MessageConfig)

// message lengths
static const uint8_t lengths[] = {
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    12,
    16,
    20,
    24,
    32,
    48,
    64
};

struct MessageHeader {
    uint32_t a;
    uint32_t b;


    MessageHeader() = default;

    MessageHeader(int id, MessageConfig config, int marker) {
        a = (uint32_t(config) & 0xe0000000) | id;
        b = (marker << 24) | (uint32_t(config) & 0x00ff0000);
    }

    // message id
    int id() const {return a & 0x1fffffff;}

    // message flags
    MessageConfig config() const {return MessageConfig((a & 0xe0000000) | (b & 0x00f00000));}

    // length of message
    int length() const {return lengths[(b >> 16) & 0xf];}

    // message marker, RX: Index of matched filter or a value >= 0x80 when accepted by default, TX: sequence number
    int marker() const {return b >> 24;}
};

/// @brief Interrupt enable flags (IE register).
///
enum class Interrupt : uint32_t {
    NONE = 0,

    // Rx FIFO 0 new message
    RX_FIFO0_NEW = FDCAN_IE_RF0NE,

    // Rx FIFO 0 full
    RX_FIFO0_FULL = FDCAN_IE_RF0FE,

    // Rx FIFO 0 message lost
    RX_FIFO0_LOST = FDCAN_IE_RF0LE,

    // Rx FIFO 1 new message
    RX_FIFO1_NEW = FDCAN_IE_RF0NE,

    // Rx FIFO 1 full
    RX_FIFO1_FULL = FDCAN_IE_RF0FE,

    // Rx FIFO 1 message lost
    RX_FIFO1_LOST = FDCAN_IE_RF0LE,

    // High-priority message
    HIGH_PRIORITY_MESSAGE = FDCAN_IE_HPME,

    // Transmission completed
    TRANSMISSION_COMPLETED = FDCAN_IE_TCE,

    // Transmission cancellation finished
    TRANSMISSION_CANCELLED = FDCAN_IE_TCFE,

    // Tx FIFO empty
    TX_FIFO_EMPTY = FDCAN_IE_TFEE,

    // Tx event FIFO new entry
    TX_EVENT_FIFO_NEW = FDCAN_IE_TEFNE,

    // Tx event FIFO full
    TX_EVENT_FIFO_FULL = FDCAN_IE_TEFNE,

    // Tx event FIFO element lost
    TX_EVENT_FIFO_LOST = FDCAN_IE_TEFLE,

    // Timestamp wraparound
    TIMESTAMP_WRAPAROUND = FDCAN_IE_TSWE,

    // Message RAM access failure
    RAM_ACCESS_FAILURE = FDCAN_IE_MRAFE,

    // Timeout occurred
    TIMEOUT_OCCURRED = FDCAN_IE_TOOE,

    // Error logging overflow
    ERROR_LOGGING_OVERFLOW = FDCAN_IE_ELOE,

    // Error passive
    ERROR_PASSIVE = FDCAN_IE_EPE,

    // Warning status
    WARNING_STATUS = FDCAN_IE_EWE,

    // Bus-off status
    BUS_OFF_STATUS = FDCAN_IE_BOE,

    // Watchdog interrupt
    WATCHDOG = FDCAN_IE_WDIE,

    // Protocol error in arbitration phase
    PROTOCOL_ERROR_ARBITRATION = FDCAN_IE_PEAE,

    // Protocol error in data phase
    PROTOCOL_ERROR_DATA = FDCAN_IE_PEDE,

    // Access to reserved address
    ACCESS_TO_RESERVED_ADDRESS = FDCAN_IE_ARAE,
};
COCO_ENUM(Interrupt)

/// @brief Interrupt select (ILS register).
///
enum class InterruptSelect : uint32_t {
    DEFAULT = 0,

    RX_FIFO0_LINE0 = 0,
    RX_FIFO0_LINE1 = FDCAN_ILS_RXFIFO0,

    RX_FIFO1_LINE0 = 0,
    RX_FIFO1_LINE1 = FDCAN_ILS_RXFIFO1,


    TX_FIFO_LINE0 = 0,
    TX_FIFO_LINE1 = FDCAN_ILS_TFERR,
};
COCO_ENUM(InterruptSelect)

/// @brief Interrupt line enable flags (ILE register).
///
enum class InterruptLine : uint32_t {
    NONE = 0,

    LINE0 = FDCAN_ILE_EINT0,
    LINE1 = FDCAN_ILE_EINT1,

    ALL = LINE0 | LINE1
};
COCO_ENUM(InterruptLine)

/// @brief Status flags (IR register).
///
enum class Status : uint32_t {
    NONE = 0,

    // Rx FIFO 0 new message
    RX_FIFO0_NEW = FDCAN_IR_RF0N,

    // Rx FIFO 0 full
    RX_FIFO0_FULL = FDCAN_IR_RF0F,

    // Rx FIFO 0 message lost
    RX_FIFO0_LOST = FDCAN_IR_RF0L,

    // Rx FIFO 1 new message
    RX_FIFO1_NEW = FDCAN_IR_RF0N,

    // Rx FIFO 1 full
    RX_FIFO1_FULL = FDCAN_IR_RF0F,

    // Rx FIFO 1 message lost
    RX_FIFO1_LOST = FDCAN_IR_RF0L,

    // High-priority message
    HIGH_PRIORITY_MESSAGE = FDCAN_IR_HPM,

    // Transmission completed
    TRANSMISSION_COMPLETED = FDCAN_IR_TC,

    // Transmission cancellation finished
    TRANSMISSION_CANCELLED = FDCAN_IR_TCF,

    // Tx FIFO empty
    TX_FIFO_EMPTY = FDCAN_IR_TFE,

    // Tx event FIFO new entry
    TX_EVENT_FIFO_NEW = FDCAN_IR_TEFN,

    // Tx event FIFO full
    TX_EVENT_FIFO_FULL = FDCAN_IR_TEFN,

    // Tx event FIFO element lost
    TX_EVENT_FIFO_LOST = FDCAN_IR_TEFL,

    // Timestamp wraparound
    TIMESTAMP_WRAPAROUND = FDCAN_IR_TSW,

    // Message RAM access failure
    RAM_ACCESS_FAILURE = FDCAN_IR_MRAF,

    // Timeout occurred
    TIMEOUT_OCCURRED = FDCAN_IR_TOO,

    // Error logging overflow
    ERROR_LOGGING_OVERFLOW = FDCAN_IR_ELO,

    // Error passive
    ERROR_PASSIVE = FDCAN_IR_EP,

    // Warning status
    WARNING_STATUS = FDCAN_IR_EW,

    // Bus-off status
    BUS_OFF_STATUS = FDCAN_IR_BO,

    // Watchdog interrupt
    WATCHDOG = FDCAN_IR_WDI,

    // Protocol error in arbitration phase
    PROTOCOL_ERROR_ARBITRATION = FDCAN_IR_PEA,

    // Protocol error in data phase
    PROTOCOL_ERROR_DATA = FDCAN_IR_PED,

    // Access to reserved address
    ACCESS_TO_RESERVED_ADDRESS = FDCAN_IR_ARA,
};
COCO_ENUM(Status)


/// @brief FDCAN instance (wrapper for FDCAN registers)
///
struct Instance {
    // counts
    static constexpr int MAX_STANDARD_FILTER_COUNT = 28;
    static constexpr int MAX_EXTENDED_FILTER_COUNT = 8;
    static constexpr int MAX_RX_FIFO0_ELEMENT_COUNT = 3;
    static constexpr int MAX_RX_FIFO1_ELEMENT_COUNT = 3;
    static constexpr int MAX_TX_EVENT_FIFO_ELEMENT_COUNT = 3;
    static constexpr int MAX_TX_FIFO_ELEMENT_COUNT = 3;

    // maxium message size counted in 32-bit words
    static constexpr int MAX_MESSAGE_SIZE = 18;

    // offsets in 32-bit words
    static constexpr int STANDARD_FILTER_OFFSET = 0;
    static constexpr int EXTENDED_FILTER_OFFSET = STANDARD_FILTER_OFFSET + MAX_STANDARD_FILTER_COUNT;
    static constexpr int RX_FIFO0_OFFSET = EXTENDED_FILTER_OFFSET + MAX_EXTENDED_FILTER_COUNT * 2;
    static constexpr int RX_FIFO1_OFFSET = RX_FIFO0_OFFSET + MAX_RX_FIFO0_ELEMENT_COUNT * MAX_MESSAGE_SIZE;
    static constexpr int TX_EVENT_FIFO_OFFSET = RX_FIFO1_OFFSET + MAX_RX_FIFO1_ELEMENT_COUNT * MAX_MESSAGE_SIZE;
    static constexpr int TX_FIFO_OFFSET = TX_EVENT_FIFO_OFFSET + MAX_TX_EVENT_FIFO_ELEMENT_COUNT * 2;

    FDCAN_GlobalTypeDef *can;
    volatile uint32_t *ram;


    FDCAN_GlobalTypeDef *operator ->() const {return can;}
    operator FDCAN_GlobalTypeDef *() const {return can;}

    /// @brief Configure the FDCAN instance
    /// @param config
    /// @param nominalBitTiming Nominal bit timing, create with makeNominalBitTiming()
    /// @param dataBitTiming Data bit timing, create with makeDataBitTiming()
    /// @param rxConfig
    /// @param standardFilters
    /// @param extendedFilters
    /// @param txConfig
    /// @return *this
    auto &configure(Config config, uint32_t nominalBitTiming, uint32_t dataBitTiming,
        RxConfig rxConfig, Array<const StandardFilter> standardFilters, Array<const ExtendedFilter> extendedFilters,
        TxConfig txConfig, Interrupt interrupts = Interrupt::NONE,
        InterruptSelect interruptSelect = InterruptSelect::DEFAULT, InterruptLine interruptLines = InterruptLine::ALL)
    {
        // switch to init mode
        while ((can->CCCR & (FDCAN_CCCR_CCE | FDCAN_CCCR_INIT)) != (FDCAN_CCCR_CCE | FDCAN_CCCR_INIT))
            can->CCCR = FDCAN_CCCR_CCE | FDCAN_CCCR_INIT;

        // set config
        can->CCCR = uint32_t(config) | FDCAN_CCCR_CCE | FDCAN_CCCR_INIT;

        // set bit timings
        can->NBTP = nominalBitTiming;
        can->DBTP = dataBitTiming;

        // set RX config
        can->RXGFC = uint32_t(rxConfig)
            | (standardFilters.size() << FDCAN_RXGFC_LSS_Pos)
            | (extendedFilters.size() << FDCAN_RXGFC_LSE_Pos);

        // configure standard message filters
        auto standardFilter = ram + STANDARD_FILTER_OFFSET;
        for (auto &filter : standardFilters) {
            *standardFilter++ = filter;
        }

        // configure extended message filters
        auto extendedFilter = ram + EXTENDED_FILTER_OFFSET;
        for (auto &filter : extendedFilters) {
            *extendedFilter++ = filter.F0;
            *extendedFilter++ = filter.F1;
        }

        // set tx config
        can->TXBC = uint32_t(txConfig);

        // set interrupts
        can->IE = uint32_t(interrupts);
        can->ILS = uint32_t(interruptSelect);
        can->ILE = uint32_t(interruptLines);

        // switch to normal operation
        can->CCCR = uint32_t(config);

        return *this;
    }

    /// @brief Enable interrupts.
    /// @param interrupts Interrupt flags to set
    /// @return *this
    auto &enable(Interrupt interrupts) {
        can->IE = can->IE | uint32_t(interrupts);
        return *this;
    }

    /// @brief Disable interrupts.
    /// @param interrupts Interrupt flags to clear
    /// @return *this
    auto &disable(Interrupt interrupts) {
        can->IE = can->IE & ~uint32_t(interrupts);
        return *this;
    }

    /// @brief Get the currently enabled Interrupts.
    /// @return Enabled interrupts
    Interrupt interrupts() {
        return Interrupt(can->IE);
    }

    /// @brief Get status flags.
    /// @return Current status flags
    Status status() const {
        return Status(can->IR);
    }

    /// @brief Clear status flags.
    /// Note that not all flags can be cleared
    /// @return *this
    auto &clearStatus(Status status) const {
        can->IR = uint32_t(status);
        return *this;
    }

    /// @brief Get rx fifo fill level
    /// @return Fill level
    int rxFillLevel(int index) {
        auto &RXFxS = (&can->RXF0S)[index * 2];
        return (RXFxS & FDCAN_RXF0S_F0FL_Msk) >> FDCAN_RXF0S_F0FL_Pos;
    }

    /// @brief Check if rx fifo is full
    /// @return True if rx fifo is full
    bool rxFull(int index) {
        auto &RXFxS = (&can->RXF0S)[index * 2];
        return (RXFxS & FDCAN_RXF0S_F0F) != 0;
    }

    /// @brief Receive a message
    /// @param header Message header
    /// @param data Message data, needs to be aligned to 4 bytes
    void rx(int index, MessageHeader &header, uint32_t *data) {
        auto &RXFxS = (&can->RXF0S)[index * 2];
        int getIndex = (RXFxS & FDCAN_RXF0S_F0GI_Msk) >> FDCAN_RXF0S_F0GI_Pos;
        auto src = ram + RX_FIFO0_OFFSET + getIndex * MAX_MESSAGE_SIZE;
        auto dst = data;

        // header
        header.a = src[0];
        header.b = src[1];
        src += 2;

        // data;
        int wordCount = (header.length() + 3) >> 2; //wordCounts[(uint32_t(config)) >> 16 & 15];
        for (int i = 0; i < wordCount; ++i) {
            *dst = *src;
            ++src;
            ++dst;
        }

        auto &RXFxA = (&can->RXF0A)[index * 2];
        RXFxA = getIndex << FDCAN_RXF0A_F0AI_Pos;
    }

    /// @brief Send a message
    /// @param header Message header
    /// @param data Message data, needs to be aligned to 4 bytes
    void tx(const MessageHeader &header, const uint32_t *data) {
        int putIndex = (can->TXFQS & FDCAN_TXFQS_TFQPI_Msk) >> FDCAN_TXFQS_TFQPI_Pos;
        auto src = data;
        auto dst = ram + TX_FIFO_OFFSET + putIndex * MAX_MESSAGE_SIZE;

        // header
        dst[0] = header.a;
        dst[1] = header.b;
        dst += 2;

        // data
        int wordCount = (header.length() + 3) >> 2;
        for (int i = 0; i < wordCount; ++i) {
            *dst = *src;
            ++src;
            ++dst;
        }

        // add write request
        can->TXBAR = 1 << (putIndex + FDCAN_TXBAR_AR_Pos);
    }

    /// @brief Get tx fifo/queue free level
    /// @return Free level
    int txFreeLevel() {
        return (can->RXF0S & FDCAN_TXFQS_TFFL_Msk) >> FDCAN_TXFQS_TFFL_Pos;
    }

    /// @brief Check if the TX FIFO or queue is full
    /// @return True when full
    bool txFull() {
        return (can->TXFQS & FDCAN_TXFQS_TFQF) != 0;
    }
};


/// @brief FDCAN instance info
/// Contains pointer to the SPI instance, interrupt index and methods to map DMA channels
struct Info {
    // registers
    FDCAN_GlobalTypeDef *can;

    // reset and clock control
    rcc::FdCanInfo rcc;

    // first interrupt request index (irq for line 0, irq + 1 for line 1)
    uint8_t irq;


    /// @brief Enable clock and configure prescaler for all FDCAN instances.
    /// @return Instance (wrapper for registers)
    Instance enableClock(ClockConfig clockConfig) const {
        rcc.enableClock();
        __NOP();
        __NOP();

        can->CCCR = FDCAN_CCCR_CCE | FDCAN_CCCR_INIT;
        FDCAN_CONFIG->CKDIV = uint32_t(clockConfig);

        return {can, ram()};
    }

    /// @brief Get the instance
    /// @return Instance (wrapper for registers)
    Instance instance() const {
        return {can, ram()};
    }

    volatile uint32_t *ram() const {
        int index = ((uintptr_t(can) - FDCAN1_BASE) >> 10) & 3;
        return (volatile uint32_t *)(SRAMCAN_BASE + index * 0x0350);
    }
};

} // namespace can
} // namespace coco

#endif // #ifdef SRAMCAN_BASE
