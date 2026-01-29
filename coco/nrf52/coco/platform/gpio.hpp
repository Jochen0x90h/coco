#pragma once

#include <coco/bits.hpp>
#include <coco/enum.hpp>
#include "platform.hpp"


namespace coco {
namespace gpio {

// pin is disconnected
constexpr uint32_t DISCONNECTED = 0xffffffff;

/// @brief GPIO configuration
/// GPIO configuration including pin, port, drive, pull up/down and invert flag.
/// The invert flag is handled in software or can be respected in peripherals
enum class Config : uint32_t {
    // no pin (needs to be checked for before calling gpio helper functions)
    NONE = DISCONNECTED,

    // pin
    PIN_0 = 0,
    PIN_1 = 1,
    PIN_2 = 2,
    PIN_3 = 3,
    PIN_4 = 4,
    PIN_5 = 5,
    PIN_6 = 6,
    PIN_7 = 7,
    PIN_8 = 8,
    PIN_9 = 9,
    PIN_10 = 10,
    PIN_11 = 11,
    PIN_12 = 12,
    PIN_13 = 13,
    PIN_14 = 14,
    PIN_15 = 15,
    PIN_16 = 16,
    PIN_17 = 17,
    PIN_18 = 18,
    PIN_19 = 19,
    PIN_20 = 20,
    PIN_21 = 21,
    PIN_22 = 22,
    PIN_23 = 23,
    PIN_24 = 24,
    PIN_25 = 25,
    PIN_26 = 26,
    PIN_27 = 27,
    PIN_28 = 28,
    PIN_29 = 29,
    PIN_30 = 30,
    PIN_31 = 31,
    PIN_MASK = 31,

    // port
    PORT_0 = 0 << 5,
    PORT_1 = 1 << 5,
    PORT_MASK = 1 << 5,

    // pull up/down resistor on pin
    PULL_NONE = 0, // default
    PULL_UP = GPIO_PIN_CNF_PULL_Pullup << (GPIO_PIN_CNF_PULL_Pos + 8),
    PULL_DOWN = GPIO_PIN_CNF_PULL_Pulldown << (GPIO_PIN_CNF_PULL_Pos + 8),
    PULL_MASK = GPIO_PIN_CNF_PULL_Msk << 8,

    // drive
    DRIVE_S0S1 = 0, // default, standard '0', standard '1'
    DRIVE_H0S1 = GPIO_PIN_CNF_DRIVE_H0S1 << (GPIO_PIN_CNF_DRIVE_Pos + 8), // high drive '0', standard '1'
    DRIVE_S0H1 = GPIO_PIN_CNF_DRIVE_S0H1 << (GPIO_PIN_CNF_DRIVE_Pos + 8), // standard '0', high drive '1'
    DRIVE_H0H1 = GPIO_PIN_CNF_DRIVE_H0H1 << (GPIO_PIN_CNF_DRIVE_Pos + 8), // high drive '0', high drive '1'
    DRIVE_D0S1 = GPIO_PIN_CNF_DRIVE_D0S1 << (GPIO_PIN_CNF_DRIVE_Pos + 8), // disconnect '0' standard '1' (normally used for wired-or connections)
    DRIVE_D0H1 = GPIO_PIN_CNF_DRIVE_D0H1 << (GPIO_PIN_CNF_DRIVE_Pos + 8), // disconnect '0', high drive '1' (normally used for wired-or connections)
    DRIVE_S0D1 = GPIO_PIN_CNF_DRIVE_S0D1 << (GPIO_PIN_CNF_DRIVE_Pos + 8), // standard '0', disconnect '1' (normally used for wired-and connections)
    DRIVE_H0D1 = GPIO_PIN_CNF_DRIVE_H0D1 << (GPIO_PIN_CNF_DRIVE_Pos + 8),  // high drive '0', disconnect '1' (normally used for wired-and connections)
    DRIVE_MASK = GPIO_PIN_CNF_DRIVE_Msk << 8,

    // invert, gets handled in software
    INVERT = 1 << 24
};
COCO_ENUM(Config);

// convenience for pin and port
constexpr auto NONE = Config::NONE;

constexpr auto P0_0 = Config::PORT_0 | Config::PIN_0;
constexpr auto P0_1 = Config::PORT_0 | Config::PIN_1;
constexpr auto P0_2 = Config::PORT_0 | Config::PIN_2;
constexpr auto P0_3 = Config::PORT_0 | Config::PIN_3;
constexpr auto P0_4 = Config::PORT_0 | Config::PIN_4;
constexpr auto P0_5 = Config::PORT_0 | Config::PIN_5;
constexpr auto P0_6 = Config::PORT_0 | Config::PIN_6;
constexpr auto P0_7 = Config::PORT_0 | Config::PIN_7;
constexpr auto P0_8 = Config::PORT_0 | Config::PIN_8;
constexpr auto P0_9 = Config::PORT_0 | Config::PIN_9;
constexpr auto P0_10 = Config::PORT_0 | Config::PIN_10;
constexpr auto P0_11 = Config::PORT_0 | Config::PIN_11;
constexpr auto P0_12 = Config::PORT_0 | Config::PIN_12;
constexpr auto P0_13 = Config::PORT_0 | Config::PIN_13;
constexpr auto P0_14 = Config::PORT_0 | Config::PIN_14;
constexpr auto P0_15 = Config::PORT_0 | Config::PIN_15;
constexpr auto P0_16 = Config::PORT_0 | Config::PIN_16;
constexpr auto P0_17 = Config::PORT_0 | Config::PIN_17;
constexpr auto P0_18 = Config::PORT_0 | Config::PIN_18;
constexpr auto P0_19 = Config::PORT_0 | Config::PIN_19;
constexpr auto P0_20 = Config::PORT_0 | Config::PIN_20;
constexpr auto P0_21 = Config::PORT_0 | Config::PIN_21;
constexpr auto P0_22 = Config::PORT_0 | Config::PIN_22;
constexpr auto P0_23 = Config::PORT_0 | Config::PIN_23;
constexpr auto P0_24 = Config::PORT_0 | Config::PIN_24;
constexpr auto P0_25 = Config::PORT_0 | Config::PIN_25;
constexpr auto P0_26 = Config::PORT_0 | Config::PIN_26;
constexpr auto P0_27 = Config::PORT_0 | Config::PIN_27;
constexpr auto P0_28 = Config::PORT_0 | Config::PIN_28;
constexpr auto P0_29 = Config::PORT_0 | Config::PIN_29;
constexpr auto P0_30 = Config::PORT_0 | Config::PIN_30;
constexpr auto P0_31 = Config::PORT_0 | Config::PIN_31;

constexpr auto P1_0 = Config::PORT_1 | Config::PIN_0;
constexpr auto P1_1 = Config::PORT_1 | Config::PIN_1;
constexpr auto P1_2 = Config::PORT_1 | Config::PIN_2;
constexpr auto P1_3 = Config::PORT_1 | Config::PIN_3;
constexpr auto P1_4 = Config::PORT_1 | Config::PIN_4;
constexpr auto P1_5 = Config::PORT_1 | Config::PIN_5;
constexpr auto P1_6 = Config::PORT_1 | Config::PIN_6;
constexpr auto P1_7 = Config::PORT_1 | Config::PIN_7;
constexpr auto P1_8 = Config::PORT_1 | Config::PIN_8;
constexpr auto P1_9 = Config::PORT_1 | Config::PIN_9;
constexpr auto P1_10 = Config::PORT_1 | Config::PIN_10;
constexpr auto P1_11 = Config::PORT_1 | Config::PIN_11;
constexpr auto P1_12 = Config::PORT_1 | Config::PIN_12;
constexpr auto P1_13 = Config::PORT_1 | Config::PIN_13;
constexpr auto P1_14 = Config::PORT_1 | Config::PIN_14;
constexpr auto P1_15 = Config::PORT_1 | Config::PIN_15;


/// @brief Mode of aGPIO pin.
///
enum class Mode : uint8_t {
    // input
    INPUT = 0, // default

    // output (getInput() can read back the output value)
    OUTPUT = (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos) | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos),

    // analog
    ANALOG = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) | (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos),
};

/// @brief Get pointer to GPIO port structure (e.g. NRF_P0)
/// @param config Pin and port (configuration ignored)
inline NRF_GPIO_Type *getPort(Config config) {return (NRF_GPIO_Type *) ((config & Config::PORT_MASK) == 0 ? NRF_P0_BASE : NRF_P1_BASE);}

constexpr int getPinIndex2(Config config) {return int(config) & 0x1f;}
constexpr int getPortIndex(Config config) {return (int(config) >> 5) & 0x7;}

/// @brief Index of pin including port, e.g. P1_0 has index 32
/// @param config Pin, port and configuration
constexpr int getPinPortIndex(Config config) {return int(config) & 0xff;}

/// @brief Enable a GPIO pin as input.
/// Make sure the clock enable bit for the port set.
/// @param config Pin, port and configuration (only pull up/down used)
inline void enableInput(Config config) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    port->PIN_CNF[pin] = int(Mode::INPUT) | (int(config) >> 8);
}

/// @brief Enable a GPIO pin as output.
/// Make sure the clock enable bit for the port set.
/// @param config pin, port and configuration (alternate function is not set)
/// @param initialValue initial level of the output pin
inline void enableOutput(Config config, bool initialValue) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    uint32_t bit = 1 << pin;
    if (int(initialValue) ^ extract(config, Config::INVERT))
        port->OUTSET = bit;
    else
        port->OUTCLR = bit;
    port->PIN_CNF[pin] = int(Mode::OUTPUT) | (int(config) >> 8);
}

/// @brief Enable a GPIO pin for alternate function.
/// Direction (input/output) is configured by the peripheral. Is similar to enableInput().
/// @param config pin, port and configuration
inline void enableAlternate(Config config) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    port->PIN_CNF[pin] = int(config) >> 8;
}

/// @brief Enable a GPIO pin as analog input.
/// @param config pin and port (pull up/down gets disabled, no additional configuraton gets applied)
inline void enableAnalog(Config config) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    port->PIN_CNF[pin] = int(Mode::ANALOG);
}

/// @brief Enable a GPIO pin in the given mode.
/// Make sure the clock enable bit for the port set.
/// @param config Pin, port and configuration
/// @param mode Mode
inline void enable(Config config, Mode mode) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    port->PIN_CNF[pin] = int(mode) | (int(config) >> 8);
}

/// @brief Set only the mode of a GPIO
/// @param pinPort pin and port (no additional configuraton gets applied)
/// @param mode mode
inline void setMode(Config pinPort, Mode mode) {
    auto port = getPort(pinPort);
    int pin = extract(pinPort, Config::PIN_MASK);
    uint32_t cnf = port->PIN_CNF[pin];
    port->PIN_CNF[pin] = int(mode) | (cnf & ~3);
}


/// @brief Get input value
/// @param pinPort pin and port (no additional configuraton gets applied except for INVERT)
inline bool getInput(Config pinPort) {
    auto port = getPort(pinPort);
    int pin = extract(pinPort, Config::PIN_MASK);
    return bool(((port->IN >> pin) & 1) ^ extract(pinPort, Config::INVERT));
}

/// @brief Set output value
/// @param pinPort pin and port (no additional configuraton gets applied except for INVERT)
inline void setOutput(Config pinPort, bool value) {
    auto port = getPort(pinPort);
    int pin = extract(pinPort, Config::PIN_MASK);
    uint32_t bit = 1 << pin;

    // either write to OUTSET or OUTCLR
    int index = int(value == 0) ^ extract(pinPort, Config::INVERT);
    (&port->OUTSET)[index] = bit;
}

/// @brief Toggle output value
/// @param pinPort pin and port (no additional configuraton gets applied)
inline void toggleOutput(Config pinPort) {
    auto port = getPort(pinPort);
    int pin = extract(pinPort, Config::PIN_MASK);
    uint32_t bit = 1 << pin;

    // either write to OUTSET or OUTCLR
    int index = (port->OUT >> pin) & 1;
    (&port->OUTSET)[index] = bit;
}

/// @brief Set or toggle output value
/// @param pinPort pin and port (no additional configuraton gets applied except for INVERT)
/// @param value Value to set
/// @param function Function: false: toggle when value is set, true: set value to output
inline void setOutput(Config pinPort, bool value, bool function) {
    auto port = getPort(pinPort);
    int pin = extract(pinPort, Config::PIN_MASK);
    uint32_t bit = 1 << pin;
    if (function) {
        // clear or set
        int index = int(value == 0) ^ extract(pinPort, Config::INVERT);
        (&port->OUTSET)[index] = bit;
    } else if (value) {
        // toggle
        int index = (port->OUT >> pin) & 1;
        (&port->OUTSET)[index] = bit;
    }
}

} // namespace gpio
} // namespace coco
