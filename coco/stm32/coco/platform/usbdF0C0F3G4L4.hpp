#pragma once

// do not include directly, use #include <coco/platform/usbd.hpp>

#include "platform.hpp"
#include <coco/enum.hpp>


/*
    Defines:
    HAVE_USBD        USB device supported
*/

// check if USBD is implemented in the current device
#ifdef USB
#define HAVE_USBD

namespace coco {

/// @brief USB device helpers.
/// F0 https://www.st.com/en/microcontrollers-microprocessors/stm32f0-series/documentation.html Section 30
/// F37 https://www.st.com/en/microcontrollers-microprocessors/stm32f3-series/documentation.html Section 29
/// G4 https://www.st.com/en/microcontrollers-microprocessors/stm32g4-series/documentation.html Section 45
namespace usbd {

// unified defines for EP register

// endpoint address
#define USB_EP_ADDR USB_EPADDR_FIELD

// RX status
#define USB_EP_STAT_RX USB_EPRX_STAT

// TX status
#define USB_EP_STAT_TX USB_EPTX_STAT


struct BufferDescriptor {
    volatile uint16_t offset;
    volatile uint16_t size;
};

struct EndpointDescriptor {
    // rx offset (lower 16 bit), size and capacity
    BufferDescriptor tx;

    // tx offset (lower 16 bit) and size
    BufferDescriptor rx;
};

/// @brief RX buffer capacity.
///
enum class RxCapacity : uint16_t {
    // buffer is unused
    UNUSED = 0,

    _8 = 4 << 10,
    _16 = 8 << 10,
    _32 = 16 << 10,
    _64 = 0x8000 | (1 << 10),
    _128 = 0x8000 | (3 << 10),
    _256 = 0x8000 | (7 << 10),
    _512 = 0x8000 | (15 << 10),
    _1023 = 0x8000 | (31 << 10),
};

// calculate size from RxCapacity (returns 1024 for RxCapacity::SIZE1023)
inline int rxSize(RxCapacity size) {
    int s = (int(size) >> 10) & 0x1f;
    return (int(size) & 0x8000) == 0 ? s * 2 : (s + 1) * 32;
}

struct BufferCapacity {
    RxCapacity rx;
    uint16_t tx;
};

/// @brief Interrupt enable flags (CNTR register).
///
enum class Interrupt : uint32_t {
    // device was reset
    RESET = USB_CNTR_RESETM,

    // a correct tansfer occurred on any endpoint
    CORRECT_TRANSFER = USB_CNTR_CTRM,

    // error
    ERROR = USB_CNTR_ERRM,

    SUSPEND = USB_CNTR_SUSPM,
    WAKE_UP = USB_CNTR_WKUPM,

    ALL = RESET | CORRECT_TRANSFER
};
COCO_ENUM(Interrupt)

/// @brief Status (ISTR register)
///
enum class Status : uint32_t {
    // devie was reset
    RESET = USB_ISTR_RESET,

    SUSPEND = USB_ISTR_SUSP,
    WAKE_UP = USB_ISTR_WKUP,

    ALL = 0xffffffff
};
COCO_ENUM(Status)

/// @brief Endpoint type
///
enum class EndpointType : uint32_t {
    BULK = USB_EP_BULK,
    INTERRUPT = USB_EP_INTERRUPT
};

/// @brief Endpoint status (CHEPxR register)
///
enum class EndpointStatus : uint32_t {
    NONE = 0,
    RX = USB_EP_CTR_RX,
    TX = USB_EP_CTR_TX,
    SETUP = USB_EP_SETUP
};
COCO_ENUM(EndpointStatus)

struct Setup {
    uint16_t request;
    uint16_t value;
    uint16_t index;
    uint16_t length;
};


/// @brief USB device instance (wrapper for USB registers)
///
struct Instance {
    USB_TypeDef *usb;
    volatile uint16_t *ram;


    USB_TypeDef *operator ->() const {return usb;}
    operator USB_TypeDef *() const {return usb;}

    /// @brief Configure the usb device.
    /// @param bufferCapacities Capacities for rx and tx buffers of the endpoints
    /// @return *this
    auto &configure(Array<const BufferCapacity> bufferCapacities) {
        int endpointCount = bufferCapacities.size();
        int offset = endpointCount * sizeof(EndpointDescriptor);

        // setup buffers for all endpoints
        auto desc = reinterpret_cast<volatile EndpointDescriptor *>(ram);
        for (auto &capacity : bufferCapacities) {
            // set rx buffer offset and capacity
            desc->rx.offset = offset;
            desc->rx.size = int(capacity.rx);
            offset += rxSize(capacity.rx);

            // set tx buffer offset
            desc->tx.offset = offset;
            offset += capacity.tx;
            ++desc;
        }
        return *this;
    }

    /// @brief Reset the usb device.
    /// @return *this
    auto &reset() {
        usb->ISTR = ~USB_ISTR_RESET;
        //debug::setBlue();

        // setup control endpoint 0
        usb->EP0R = USB_EP_RX_VALID | USB_EP_TX_STALL | USB_EP_CONTROL | 0;

        // enable usb at usb address 0
        usb->DADDR = USB_DADDR_EF | 0;

        return *this;
    }

    /// @brief Apply address from current setup packet
    /// @return *this
    int applyAddress() {
        auto desc = reinterpret_cast<volatile EndpointDescriptor *>(ram);
        int offset = desc[0].rx.offset;
        auto data = (volatile uint16_t *)(intptr_t(ram) + offset);
        int address = data[1];
        usb->DADDR = USB_DADDR_EF | address;
        return address;
    }

    auto &endpointRegister(int ep) {
        return (&usb->EP0R)[ep * 2];
    }

    /// @brief Enable interrupts.
    /// @param interrupt Interrupts to enable
    /// @return *this
    auto &enable(Interrupt interrupt) {
        usb->CNTR = usb->CNTR | uint32_t(interrupt);
        return *this;
    }

    /// @brief Disable interrupts.
    /// @param interrupt Interrupts to disable
    /// @return *this
    auto &disable(Interrupt interrupt) {
        usb->CNTR = usb->CNTR & ~uint32_t(interrupt);
        return *this;
    }

    /// @brief Get the currently enabled Interrupts.
    /// @return Enabled interrupts
    Interrupt interrupts() const {
        return Interrupt(usb->CNTR) & Interrupt::ALL;
    }

    /// @brief Get status flags.
    /// @return Current status flags
    Status status() const {
        return Status(usb->ISTR);
    }

    /// @brief Clear status flags.
    /// @param status Status flags to clear
    /// @return *this
    auto &clear(Status status) {
        usb->ISTR = ~uint32_t(status);
        return *this;
    }

    /// @brief Get endpoint status (interrupt flags).
    /// @param ep Endponit index
    /// @return *this
    EndpointStatus endpointStatus(int ep) {
        return EndpointStatus(endpointRegister(ep));
    }

    /// @brief Prepare status in transfer for control endpoint 0 (send a ZLP).
    ///
    void controlStatusIn() {
        auto desc = reinterpret_cast<volatile EndpointDescriptor *>(ram);
        desc[0].tx.size = 0; // clear tx size of control endpoint

        // keep DTOG_RX DTOG_TX STAT_RX, cancel STAT_TX, clear CTR_RX CTR_TX, toggle STAT_TX.VALID
        usb->EP0R = ((usb->EP0R & ~(USB_EP_DTOG_RX | USB_EP_DTOG_TX | USB_EP_STAT_RX | USB_EP_CTR_RX | USB_EP_CTR_TX)) ) ^ USB_EP_TX_VALID;
    }

    /// @brief Prepare status out transfer for control endpoint 0 (expect a ZLP).
    ///
    void controlStatusOut() {
        // keep DTOG_RX DTOG_TX STAT_TX, cancel STAT_RX, clear CTR_RX CTR_TX, set EP_KIND (STATUS_OUT), toggle STAT_RX.VALID
        usb->EP0R = ((usb->EP0R & ~(USB_EP_DTOG_RX | USB_EP_DTOG_TX | USB_EP_STAT_TX | USB_EP_CTR_RX | USB_EP_CTR_TX)) | USB_EP_KIND) ^ USB_EP_RX_VALID;
    }

    /// @brief Stall both directions of control endpoint 0.
    ///
    void controlStall() {
        // keep DTOG_RX DTOG_TX, cancel STAT_RX STAT_TX, clear CTR_RX CTR_TX EP_KIND (STATUS_OUT), toggle STAT_RX.STALL STAT_TX.STALL
        usb->EP0R = ((usb->EP0R & ~(USB_EP_DTOG_RX | USB_EP_DTOG_TX | USB_EP_CTR_RX | USB_EP_CTR_TX | USB_EP_KIND)) ) ^ (USB_EP_RX_STALL | USB_EP_TX_STALL);
    }

    /// @brief Initialize endpoint rx.
    /// @param ep Endpoint index
    /// @param endpointType Endpont type
    void rxInit(int ep, EndpointType endpointType) {
        // configure rx (out) endpoint: ready to receive, clear other toggle bits
        auto &EPxR = endpointRegister(ep);
        EPxR = ((EPxR
                & ~(USB_EP_CTR_RX // clear RX correct transfer flag
                    | USB_EP_TYPE_MASK // clear endpoint type
                    | USB_EP_KIND // clear endpoint kind
                    | USB_EP_ADDR // clear endpoint index (address)
                    | USB_EP_DTOG_TX // keep TX toggle (0 = keep, 1 = toggle)
                    | USB_EP_STAT_TX)) // keep TX status (0 = keep, 1 = toggle)
                | int(endpointType) // set endpoint type
                | ep) // set endpoint index (address)
            ^ USB_EP_RX_VALID // set RX status to VALID (ready to receive)
            ^ 0; // clear RX toggle (toggle when set)
    }

    /// @brief Indicate that we want to receive data from the host.
    ///
    void rxStart(int ep) {
        // keep DTOG_RX DTOG_TX STAT_TX, cancel STAT_RX, clear CTR_RX, don't clear CTR_TX, toggle STAT_RX.STALL
        auto &EPxR = endpointRegister(ep);
        EPxR = ((EPxR & ~(USB_EP_DTOG_RX | USB_EP_DTOG_TX | USB_EP_STAT_TX | USB_EP_CTR_RX)) | USB_EP_CTR_TX) ^ USB_EP_RX_VALID;
    }

    /// @brief Get received data.
    /// @param ep Endpoint index
    /// @param data Data (must be 4 byte aligned)
    /// @param size Size of data buffer
    /// @return Copied size
    int rx(int ep, uint32_t *data, int size) {
        auto desc = reinterpret_cast<volatile EndpointDescriptor *>(ram);
        auto &os = desc[ep].rx;

        // get offset
        int offset = os.offset;

        // get number of received bytes
        size = std::min(int(os.size & 0x3ff), size);

        // copy data from rx buffer into memory
        auto src = (const volatile uint16_t *)(uintptr_t(ram) + offset);
        auto dst = (uint16_t *)data;
        auto end = dst + ((size + 1) >> 1);
        while (dst != end) {
            *dst = *src;
            ++src;
            ++dst;
        }
        return size;
    }

    /// @brief Acknowledge receive.
    /// @param ep Endpoint index
    void rxAck(int ep) {
        // keep DTOG_RX DTOG_TX STAT_RX STAT_TX, clear CTR_RX, don't clear CTR_TX
        auto &EPxR = endpointRegister(ep);
        EPxR = ((EPxR & ~(USB_EP_DTOG_RX | USB_EP_DTOG_TX | USB_EP_STAT_RX | USB_EP_STAT_TX | USB_EP_CTR_RX)) | USB_EP_CTR_TX);
    }

    /// @brief Initialize endpoint tx.
    /// @param ep Endpoint index
    /// @param endpointType Endpont type
    void txInit(int ep, EndpointType endpointType) {
        // configure tx (in) endpoint: stall send, clear other toggle bits
        auto &EPxR = endpointRegister(ep);
        EPxR = ((EPxR
                & ~(USB_EP_CTR_TX // clear TX correct transfer flag
                    | USB_EP_TYPE_MASK // clear endpoint type
                    | USB_EP_KIND // clear endpoint kind
                    | USB_EP_ADDR // clear endpoint index (address)
                    | USB_EP_DTOG_RX // keep RX toggle (0 = keep, 1 = toggle)
                    | USB_EP_STAT_RX)) // keep RX status (0 = keep, 1 = toggle)
                | int(endpointType) // set endpoint type
                | ep) // set endpoint index (address)
            ^ USB_EP_TX_NAK // set TX status to NAK (nothing to send)
            ^ 0; // clear TX toggle (toggle when set)
    }

    /// @brief Send data to the host.
    /// @param ep Endpoint index
    /// @param data Data (must be 4 byte aligned)
    /// @param size Size of data buffer
    void tx(int ep, const uint32_t *data, int size) {
        auto desc = reinterpret_cast<volatile EndpointDescriptor *>(ram);
        auto &os = desc[ep].tx;

        // get offset
        int offset = os.offset;

        // set number of bytes to transmit
        os.size = size;

        // copy data from memory into tx buffer
        auto src = (const uint16_t *)data;
        auto dst = (volatile uint16_t *)(uintptr_t(ram) + offset);
        auto end = dst + ((size + 1) >> 1);
        while (dst != end) {
            *dst = *src;
            ++src;
            ++dst;
        }

        // indicate that we are ready to send
        // keep DTOG_RX DTOG_TX STAT_RX, cancel STAT_TX, clear CTR_TX, don't clear CTR_RX, toggle STAT_TX.VALID
        auto &EPxR = endpointRegister(ep);
        EPxR = ((EPxR & ~(USB_EP_DTOG_RX | USB_EP_DTOG_TX | USB_EP_STAT_RX | USB_EP_CTR_TX)) | USB_EP_CTR_RX) ^ USB_EP_TX_VALID;
    }

    /// @brief Acknowledge send.
    ///
    void txAck(int ep) {
        // keep DTOG_RX DTOG_TX STAT_RX STAT_TX, clear CTR_TX, don't clear CTR_RX
        auto &EPxR = endpointRegister(ep);
        EPxR = ((EPxR & ~(USB_EP_DTOG_RX | USB_EP_DTOG_TX | USB_EP_STAT_RX | USB_EP_STAT_TX | USB_EP_CTR_TX)) | USB_EP_CTR_RX);
    }

    template <typename T>
    std::pair<T, uint8_t> getSetupRequest() {
        auto desc = reinterpret_cast<volatile EndpointDescriptor *>(ram);
        int offset = desc[0].rx.offset;
        auto data = (volatile uint16_t *)(uintptr_t(ram) + offset);
        int d = data[0];
        return {T(d), uint8_t(d >> 8)};
    }

    template <typename T, typename R>
    T getSetup() {
        auto desc = reinterpret_cast<volatile EndpointDescriptor *>(ram);
        int offset = desc[0].rx.offset;
        auto data = (volatile uint16_t *)(uintptr_t(this->ram) + offset);
        auto d0 = data[0];
        return {R(d0), uint8_t(d0 >> 8), data[1], data[2], data[3]};
    }
};

} // namespace usbd
} // namespace coco

#endif // HAVE_USBD
