#pragma once

#include <coco/bits.hpp>
#include <coco/enum.hpp>
#include "platform.hpp"


namespace coco {
namespace gpio {

// pin is disconnected
constexpr int DISCONNECTED = 0xffffffff;

/**
 * GPIO configuration
 */
enum class Config : uint32_t {
    // no pin (has to be checked before calling one of the configure functions)
    NONE = 0xff,

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
    PORT_MASK = 7 << 5,

    // convenience for pin and port
    P0_0 = PORT_0 | PIN_0,
    P0_1 = PORT_0 | PIN_1,
    P0_2 = PORT_0 | PIN_2,
    P0_3 = PORT_0 | PIN_3,
    P0_4 = PORT_0 | PIN_4,
    P0_5 = PORT_0 | PIN_5,
    P0_6 = PORT_0 | PIN_6,
    P0_7 = PORT_0 | PIN_7,
    P0_8 = PORT_0 | PIN_8,
    P0_9 = PORT_0 | PIN_9,
    P0_10 = PORT_0 | PIN_10,
    P0_11 = PORT_0 | PIN_11,
    P0_12 = PORT_0 | PIN_12,
    P0_13 = PORT_0 | PIN_13,
    P0_14 = PORT_0 | PIN_14,
    P0_15 = PORT_0 | PIN_15,
    P0_16 = PORT_0 | PIN_16,
    P0_17 = PORT_0 | PIN_17,
    P0_18 = PORT_0 | PIN_18,
    P0_19 = PORT_0 | PIN_19,
    P0_20 = PORT_0 | PIN_20,
    P0_21 = PORT_0 | PIN_21,
    P0_22 = PORT_0 | PIN_22,
    P0_23 = PORT_0 | PIN_23,
    P0_24 = PORT_0 | PIN_24,
    P0_25 = PORT_0 | PIN_25,
    P0_26 = PORT_0 | PIN_26,
    P0_27 = PORT_0 | PIN_27,
    P0_28 = PORT_0 | PIN_28,
    P0_29 = PORT_0 | PIN_29,
    P0_30 = PORT_0 | PIN_30,
    P0_31 = PORT_0 | PIN_31,

    P1_0 = PORT_1 | PIN_0,
    P1_1 = PORT_1 | PIN_1,
    P1_2 = PORT_1 | PIN_2,
    P1_3 = PORT_1 | PIN_3,
    P1_4 = PORT_1 | PIN_4,
    P1_5 = PORT_1 | PIN_5,
    P1_6 = PORT_1 | PIN_6,
    P1_7 = PORT_1 | PIN_7,
    P1_8 = PORT_1 | PIN_8,
    P1_9 = PORT_1 | PIN_9,
    P1_10 = PORT_1 | PIN_10,
    P1_11 = PORT_1 | PIN_11,
    P1_12 = PORT_1 | PIN_12,
    P1_13 = PORT_1 | PIN_13,
    P1_14 = PORT_1 | PIN_14,
    P1_15 = PORT_1 | PIN_15,

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

enum class Mode : uint8_t {
    INPUT = 0, // default
    OUTPUT = (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos) | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos),
    ANALOG = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) | (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos),
};


/**
 * Get pointer to GPIO port structure (e.g. NRF_P0)
 */
inline NRF_GPIO_Type *getPort(Config config) {return (NRF_GPIO_Type *) ((config & Config::PORT_MASK) == 0 ? NRF_P0_BASE : NRF_P1_BASE);}

/**
 * Index of pin including port, e.g. P1_0 has index 32
 */
constexpr uint8_t getPinIndex(Config config) {return int(config) & 0xff;}


/**
 * Configure a GPIO pin.
 * Make sure the clock enable bit for the port set.
 * @param mode mode
 * @param config configuration
 */
inline void configure(Mode mode, Config config) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    port->PIN_CNF[pin] = int(mode) | (int(config) >> 8);
}

/**
 * Configure a GPIO pin as input.
 * Make sure the clock enable bit for the port set.
 * @param config pin, port and configuration (only pull up/down used)
 */
inline void configureInput(Config config) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    port->PIN_CNF[pin] = int(Mode::INPUT) | (int(config) >> 8);
}

/**
 * Configure a GPIO pin as output.
 * Make sure the clock enable bit for the port set.
 * @param config pin, port and configuration (alternate function is not set)
 * @param initialValue initial level of the output pin
 */
inline void configureOutput(Config config, bool initialValue) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    uint32_t bit = 1 << pin;
    if (int(initialValue) ^ extract(config, Config::INVERT))
        port->OUTSET = bit;
    else
        port->OUTCLR = bit;
    port->PIN_CNF[pin] = int(Mode::OUTPUT) | (int(config) >> 8);
}

/**
 * Configure a GPIO pin for alternate function.
 * Make sure the clock enable bit for the port set.
 * @param config pin, port and configuration
 */
inline void configureAlternate(Config config) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    port->PIN_CNF[pin] = int(config) >> 8;
}

/**
 * Configure a GPIO pin as analog input.
 * Make sure the clock enable bit for the port set.
 * @param config pin and port (pull up/down gets disabled, no additional configuraton gets applied)
 */
inline void configureAnalog(Config config) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    port->PIN_CNF[pin] = int(Mode::ANALOG);
}


/**
 * Get input value
 */
inline bool getInput(Config config) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    return bool(((port->IN >> pin) & 1) ^ extract(config, Config::INVERT));
}

/**
 * Enable ouput of a GPIO, use to switch between input and output after calling configure()
 * @param enable true for output, false for input
 */
inline void enableOutput(Config config, bool enable) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    uint32_t cnf = port->PIN_CNF[pin];
    port->PIN_CNF[pin] = int(enable ? Mode::OUTPUT : Mode::INPUT) | (cnf & ~3);
}

/**
 * Set output value
 */
inline void setOutput(Config config, bool value) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    uint32_t bit = 1 << pin;

    // either write to OUTSET or OUTCLR
    int index = int(value == 0) ^ extract(config, Config::INVERT);
    (&port->OUTSET)[index] = bit;
}

/**
 * Toggle output value
 */
inline void toggleOutput(Config config) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    uint32_t bit = 1 << pin;

    // either write to OUTSET or OUTCLR
    int index = (port->OUT >> pin) & 1;
    (&port->OUTSET)[index] = bit;
}

/**
 * Set or toggle output value
 */
inline void setOutput(Config config, bool value, bool function) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    uint32_t bit = 1 << pin;
    if (function) {
        // clear or set
        int index = int(value == 0) ^ extract(config, Config::INVERT);
        (&port->OUTSET)[index] = bit;
    } else if (value) {
        // toggle
        int index = (port->OUT >> pin) & 1;
        (&port->OUTSET)[index] = bit;
    }
}



/*

// pin, port
constexpr int P0(int pin) {return pin;}
constexpr int P1(int pin) {return 32 + pin;}

// get pin index
constexpr int getPin(int pinPort) {return pinPort & 0x1f;}

// get port index
constexpr int getPort(int pinPort) {return pinPort >> 5;}

// get pointer to GPIO port (e.g. NRF_P0)
constexpr NRF_GPIO_Type *GPIO(int pinPort) {return (NRF_GPIO_Type *) (pinPort < 32 ? NRF_P0_BASE : NRF_P1_BASE);}


/ **
 * GPIO configuration
 * /
enum class Config : uint16_t {
    // default configuratoin: input, no pull up/down, drive in both directions, slow speed
    DEFAULT = 0,

    // mode
    INPUT = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos), // default
    OUTPUT = (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos) | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos),
    ANALOG = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) | (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos),
    MODE_MASK = GPIO_PIN_CNF_DIR_Msk | GPIO_PIN_CNF_INPUT_Msk,

    // pull up/down resistor on pin
    PULL_NONE = GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos, // default
    PULL_UP = GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos,
    PULL_DOWN = GPIO_PIN_CNF_PULL_Pulldown << GPIO_PIN_CNF_PULL_Pos,
    PULL_MASK = GPIO_PIN_CNF_PULL_Msk,

    // drive
    DRIVE_S0S1 = GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos, // default, standard '0', standard '1'
    DRIVE_H0S1 = GPIO_PIN_CNF_DRIVE_H0S1 << GPIO_PIN_CNF_DRIVE_Pos, // high drive '0', standard '1'
    DRIVE_S0H1 = GPIO_PIN_CNF_DRIVE_S0H1 << GPIO_PIN_CNF_DRIVE_Pos, // standard '0', high drive '1'
    DRIVE_H0H1 = GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos, // high drive '0', high drive '1'
    DRIVE_D0S1 = GPIO_PIN_CNF_DRIVE_D0S1 << GPIO_PIN_CNF_DRIVE_Pos, // disconnect '0' standard '1' (normally used for wired-or connections)
    DRIVE_D0H1 = GPIO_PIN_CNF_DRIVE_D0H1 << GPIO_PIN_CNF_DRIVE_Pos, // disconnect '0', high drive '1' (normally used for wired-or connections)
    DRIVE_S0D1 = GPIO_PIN_CNF_DRIVE_S0D1 << GPIO_PIN_CNF_DRIVE_Pos, // standard '0', disconnect '1' (normally used for wired-and connections)
    DRIVE_H0D1 = GPIO_PIN_CNF_DRIVE_H0D1 << GPIO_PIN_CNF_DRIVE_Pos,  // high drive '0', disconnect '1' (normally used for wired-and connections)
    DRIVE_MASK = GPIO_PIN_CNF_DRIVE_Msk
};
COCO_ENUM(Config);


/ **
 * Configure a GPIO pin.
 * @param pinPort pin and port
 * @param config configuration
 * /
inline void configure(int pinPort, Config config) {
    auto port = GPIO(pinPort);
    int pin = getPin(pinPort);
    port->PIN_CNF[pin] = int(config);
}

/ **
 * Configure a GPIO pin.
 * @param pinPort pin and port
 * @param initialValue initial value of the output bit
 * @param config configuration
 * /
inline void configure(int pinPort, bool initialValue, Config config) {
    auto port = GPIO(pinPort);
    int pin = getPin(pinPort);
    uint32_t bit = 1 << pin;
    if (initialValue)
        port->OUTSET = bit;
    else
        port->OUTCLR = bit;
    port->PIN_CNF[pin] = int(config);
}

/ **
 * Configure a GPIO pin as input.
 * @param pinPort pin and port
 * @param config configuration
 * /
inline void configureInput(int pinPort, Config config = Config::DEFAULT) {
    auto port = GPIO(pinPort);
    int pin = getPin(pinPort);
    port->PIN_CNF[pin] = int(Config::INPUT | (~Config::MODE_MASK & config));
}

/ **
 * Configure a GPIO pin as output.
 * @param pinPort pin and port
 * @param initialValue initial value of the output bit
 * @param config configuration
 * /
inline void configureOutput(int pinPort, bool initialValue, Config config = Config::DEFAULT) {
    auto port = GPIO(pinPort);
    int pin = getPin(pinPort);
    uint32_t bit = 1 << pin;
    if (initialValue)
        port->OUTSET = bit;
    else
        port->OUTCLR = bit;
    port->PIN_CNF[pin] = int(Config::OUTPUT | (~Config::MODE_MASK & config));
}

/ **
 * Configure a GPIO pin as analog input.
 * @param pinPort pin and port
 * /
inline void configureAnalog(int pinPort) {
    auto port = GPIO(pinPort);
    int pin = getPin(pinPort);
    port->PIN_CNF[pin] = int(Config::ANALOG);
}

/ **
 * Get input value
 * /
inline bool getInput(int pinPort) {
    auto port = GPIO(pinPort);
    int pin = getPin(pinPort);
    return (port->IN & (1 << pin)) != 0;
}

/ **
 * Enable ouput of a GPIO, use to switch between input and output after calling configure()
 * @param enable true for output, false for input
 * /
inline void enableOutput(int pinPort, bool enable) {
    auto port = GPIO(pinPort);
    int pin = getPin(pinPort);
    auto config = Config(port->PIN_CNF[pin]);
    port->PIN_CNF[pin] = int((enable ? Config::OUTPUT : Config::INPUT) | (~Config::MODE_MASK & config));
}

/ **
 * Set output value
 * /
inline void setOutput(int pinPort, bool value) {
auto port = GPIO(pinPort);
    int pin = getPin(pinPort);
    uint32_t bit = 1 << pin;
    if (value)
        port->OUTSET = bit;
    else
        port->OUTCLR = bit;
}

/ **
 * Toggle output value
 * /
inline void toggleOutput(int pinPort) {
    auto port = GPIO(pinPort);
    int pin = getPin(pinPort);
    uint32_t bit = 1 << pin;
    bool value = (port->OUT & bit) == 0;
    if (value)
        port->OUTSET = bit;
    else
        port->OUTCLR = bit;
}

/ **
 * Set or toggle output value
 * /
inline void setOutput(int pinPort, bool value, bool function) {
    auto port = GPIO(pinPort);
    int pin = getPin(pinPort);
    uint32_t bit = 1 << pin;
    if (!function) {
        if (value)
            value = (port->OUT & bit) == 0;
        else
            return;
    }
    if (value)
        port->OUTSET = bit;
    else
        port->OUTCLR = bit;
}
*/

} // namespace gpio
} // namespace coco
