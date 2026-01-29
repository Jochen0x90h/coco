#pragma once

#include "platform.hpp"
#include <coco/bits.hpp>
#include <coco/enum.hpp>


namespace coco {

/// @brief GPIO (general purpose input/output) helpers.
/// F0 Section 8
///   https://www.st.com/resource/en/datasheet/stm32f042f6.pdf GPIO AC characteristics: Table 50
///   https://www.st.com/resource/en/datasheet/dm00039193.pdf GPIO AC characteristics: Table 50
/// F3 Section 9
///   https://www.st.com/resource/en/datasheet/stm32f334k4.pdf I/O AC characteristics: Table 52
/// G4 https://www.st.com/en/microcontrollers-microprocessors/stm32g4-series/documentation.html Section 9
///   https://www.st.com/resource/en/datasheet/stm32g431rb.pdf GPIO AC characteristics: Table 55
/// U3 https://www.st.com/en/microcontrollers-microprocessors/stm32u3-series/documentation.html Section 12
namespace gpio {


/// @brief GPIO configuration
/// GPIO configuration including pin, port, alternate function, speed, pull up/down and invert flag.
/// The invert flag is handled in software or can be respected in peripherals, e.g. the USART RXINV/TXINV flags
enum class Config : uint32_t {
    // no pin (needs to be checked for before calling gpio helper functions)
    NONE = 0xffffffff,

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
    PIN_MASK = 15,

    // port
    PORT_A = 0 << 4,
    PORT_B = 1 << 4,
    PORT_C = 2 << 4,
    PORT_D = 3 << 4,
    PORT_E = 4 << 4,
    PORT_F = 5 << 4,
    PORT_G = 6 << 4,
    PORT_H = 7 << 4,
    PORT_MASK = 7 << 4,

    // alternate function
    ALTERNATE_FUNCTION_0 = 0,
    ALTERNATE_FUNCTION_1 = 1 << 8,
    ALTERNATE_FUNCTION_2 = 2 << 8,
    ALTERNATE_FUNCTION_3 = 3 << 8,
    ALTERNATE_FUNCTION_4 = 4 << 8,
    ALTERNATE_FUNCTION_5 = 5 << 8,
    ALTERNATE_FUNCTION_6 = 6 << 8,
    ALTERNATE_FUNCTION_7 = 7 << 8,
    ALTERNATE_FUNCTION_8 = 8 << 8,
    ALTERNATE_FUNCTION_9 = 9 << 8,
    ALTERNATE_FUNCTION_10 = 10 << 8,
    ALTERNATE_FUNCTION_11 = 11 << 8,
    ALTERNATE_FUNCTION_12 = 12 << 8,
    ALTERNATE_FUNCTION_13 = 13 << 8,
    ALTERNATE_FUNCTION_14 = 14 << 8,
    ALTERNATE_FUNCTION_15 = 15 << 8,
    ALTERNATE_FUNCTION_MASK = 15 << 8,

    // pull up/down resistor on pin
    PULL_NONE = 0, // default
    PULL_UP = 1 << 12,
    PULL_DOWN = 2 << 12,
    PULL_MASK = 3 << 12,

    // drive
    DRIVE_BOTH = 0, // default
    DRIVE_DOWN = 1 << 14,
    DRIVE_MASK = 1 << 14,

    // speed
    SPEED_LOW = 0, // default, ~2 MHz
    SPEED_MEDIUM = 1 << 16, // ~10-25 MHz
#if defined(STM32F0) || defined(STM32F3)
    SPEED_HIGH = 3 << 16, // f0, f3: ~20-50 MHz
#else
    SPEED_HIGH = 2 << 16, // g4: ~25-100 MHz
#endif
    SPEED_MAX = 3 << 16, // g4: ~50-180 MHz
    SPEED_MASK = 3 << 16,

    // invert, gets handled in software
    INVERT = 1 << 24
};
COCO_ENUM(Config);

// convenience for pin and port
constexpr auto NONE = Config::NONE;

constexpr auto PA0 = Config::PORT_A | Config::PIN_0;
constexpr auto PA1 = Config::PORT_A | Config::PIN_1;
constexpr auto PA2 = Config::PORT_A | Config::PIN_2;
constexpr auto PA3 = Config::PORT_A | Config::PIN_3;
constexpr auto PA4 = Config::PORT_A | Config::PIN_4;
constexpr auto PA5 = Config::PORT_A | Config::PIN_5;
constexpr auto PA6 = Config::PORT_A | Config::PIN_6;
constexpr auto PA7 = Config::PORT_A | Config::PIN_7;
constexpr auto PA8 = Config::PORT_A | Config::PIN_8;
constexpr auto PA9 = Config::PORT_A | Config::PIN_9;
constexpr auto PA10 = Config::PORT_A | Config::PIN_10;
constexpr auto PA11 = Config::PORT_A | Config::PIN_11;
constexpr auto PA12 = Config::PORT_A | Config::PIN_12;
constexpr auto PA13 = Config::PORT_A | Config::PIN_13;
constexpr auto PA14 = Config::PORT_A | Config::PIN_14;
constexpr auto PA15 = Config::PORT_A | Config::PIN_15;

constexpr auto PB0 = Config::PORT_B | Config::PIN_0;
constexpr auto PB1 = Config::PORT_B | Config::PIN_1;
constexpr auto PB2 = Config::PORT_B | Config::PIN_2;
constexpr auto PB3 = Config::PORT_B | Config::PIN_3;
constexpr auto PB4 = Config::PORT_B | Config::PIN_4;
constexpr auto PB5 = Config::PORT_B | Config::PIN_5;
constexpr auto PB6 = Config::PORT_B | Config::PIN_6;
constexpr auto PB7 = Config::PORT_B | Config::PIN_7;
constexpr auto PB8 = Config::PORT_B | Config::PIN_8;
constexpr auto PB9 = Config::PORT_B | Config::PIN_9;
constexpr auto PB10 = Config::PORT_B | Config::PIN_10;
constexpr auto PB11 = Config::PORT_B | Config::PIN_11;
constexpr auto PB12 = Config::PORT_B | Config::PIN_12;
constexpr auto PB13 = Config::PORT_B | Config::PIN_13;
constexpr auto PB14 = Config::PORT_B | Config::PIN_14;
constexpr auto PB15 = Config::PORT_B | Config::PIN_15;

constexpr auto PC0 = Config::PORT_C | Config::PIN_0;
constexpr auto PC1 = Config::PORT_C | Config::PIN_1;
constexpr auto PC2 = Config::PORT_C | Config::PIN_2;
constexpr auto PC3 = Config::PORT_C | Config::PIN_3;
constexpr auto PC4 = Config::PORT_C | Config::PIN_4;
constexpr auto PC5 = Config::PORT_C | Config::PIN_5;
constexpr auto PC6 = Config::PORT_C | Config::PIN_6;
constexpr auto PC7 = Config::PORT_C | Config::PIN_7;
constexpr auto PC8 = Config::PORT_C | Config::PIN_8;
constexpr auto PC9 = Config::PORT_C | Config::PIN_9;
constexpr auto PC10 = Config::PORT_C | Config::PIN_10;
constexpr auto PC11 = Config::PORT_C | Config::PIN_11;
constexpr auto PC12 = Config::PORT_C | Config::PIN_12;
constexpr auto PC13 = Config::PORT_C | Config::PIN_13;
constexpr auto PC14 = Config::PORT_C | Config::PIN_14;
constexpr auto PC15 = Config::PORT_C | Config::PIN_15;

constexpr auto PD0 = Config::PORT_D | Config::PIN_0;
constexpr auto PD1 = Config::PORT_D | Config::PIN_1;
constexpr auto PD2 = Config::PORT_D | Config::PIN_2;
constexpr auto PD3 = Config::PORT_D | Config::PIN_3;
constexpr auto PD4 = Config::PORT_D | Config::PIN_4;
constexpr auto PD5 = Config::PORT_D | Config::PIN_5;
constexpr auto PD6 = Config::PORT_D | Config::PIN_6;
constexpr auto PD7 = Config::PORT_D | Config::PIN_7;
constexpr auto PD8 = Config::PORT_D | Config::PIN_8;
constexpr auto PD9 = Config::PORT_D | Config::PIN_9;
constexpr auto PD10 = Config::PORT_D | Config::PIN_10;
constexpr auto PD11 = Config::PORT_D | Config::PIN_11;
constexpr auto PD12 = Config::PORT_D | Config::PIN_12;
constexpr auto PD13 = Config::PORT_D | Config::PIN_13;
constexpr auto PD14 = Config::PORT_D | Config::PIN_14;
constexpr auto PD15 = Config::PORT_D | Config::PIN_15;

constexpr auto PE0 = Config::PORT_E | Config::PIN_0;
constexpr auto PE1 = Config::PORT_E | Config::PIN_1;
constexpr auto PE2 = Config::PORT_E | Config::PIN_2;
constexpr auto PE3 = Config::PORT_E | Config::PIN_3;
constexpr auto PE4 = Config::PORT_E | Config::PIN_4;
constexpr auto PE5 = Config::PORT_E | Config::PIN_5;
constexpr auto PE6 = Config::PORT_E | Config::PIN_6;
constexpr auto PE7 = Config::PORT_E | Config::PIN_7;
constexpr auto PE8 = Config::PORT_E | Config::PIN_8;
constexpr auto PE9 = Config::PORT_E | Config::PIN_9;
constexpr auto PE10 = Config::PORT_E | Config::PIN_10;
constexpr auto PE11 = Config::PORT_E | Config::PIN_11;
constexpr auto PE12 = Config::PORT_E | Config::PIN_12;
constexpr auto PE13 = Config::PORT_E | Config::PIN_13;
constexpr auto PE14 = Config::PORT_E | Config::PIN_14;
constexpr auto PE15 = Config::PORT_E | Config::PIN_15;

constexpr auto PF0 = Config::PORT_F | Config::PIN_0;
constexpr auto PF1 = Config::PORT_F | Config::PIN_1;
constexpr auto PF2 = Config::PORT_F | Config::PIN_2;
constexpr auto PF3 = Config::PORT_F | Config::PIN_3;
constexpr auto PF4 = Config::PORT_F | Config::PIN_4;
constexpr auto PF5 = Config::PORT_F | Config::PIN_5;
constexpr auto PF6 = Config::PORT_F | Config::PIN_6;
constexpr auto PF7 = Config::PORT_F | Config::PIN_7;
constexpr auto PF8 = Config::PORT_F | Config::PIN_8;
constexpr auto PF9 = Config::PORT_F | Config::PIN_9;
constexpr auto PF10 = Config::PORT_F | Config::PIN_10;
constexpr auto PF11 = Config::PORT_F | Config::PIN_11;
constexpr auto PF12 = Config::PORT_F | Config::PIN_12;
constexpr auto PF13 = Config::PORT_F | Config::PIN_13;
constexpr auto PF14 = Config::PORT_F | Config::PIN_14;
constexpr auto PF15 = Config::PORT_F | Config::PIN_15;

constexpr auto PG0 = Config::PORT_G | Config::PIN_0;
constexpr auto PG1 = Config::PORT_G | Config::PIN_1;
constexpr auto PG2 = Config::PORT_G | Config::PIN_2;
constexpr auto PG3 = Config::PORT_G | Config::PIN_3;
constexpr auto PG4 = Config::PORT_G | Config::PIN_4;
constexpr auto PG5 = Config::PORT_G | Config::PIN_5;
constexpr auto PG6 = Config::PORT_G | Config::PIN_6;
constexpr auto PG7 = Config::PORT_G | Config::PIN_7;
constexpr auto PG8 = Config::PORT_G | Config::PIN_8;
constexpr auto PG9 = Config::PORT_G | Config::PIN_9;
constexpr auto PG10 = Config::PORT_G | Config::PIN_10;
constexpr auto PG11 = Config::PORT_G | Config::PIN_11;
constexpr auto PG12 = Config::PORT_G | Config::PIN_12;
constexpr auto PG13 = Config::PORT_G | Config::PIN_13;
constexpr auto PG14 = Config::PORT_G | Config::PIN_14;
constexpr auto PG15 = Config::PORT_G | Config::PIN_15;

constexpr auto PH0 = Config::PORT_H | Config::PIN_0;
constexpr auto PH1 = Config::PORT_H | Config::PIN_1;
constexpr auto PH2 = Config::PORT_H | Config::PIN_2;
constexpr auto PH3 = Config::PORT_H | Config::PIN_3;
constexpr auto PH4 = Config::PORT_H | Config::PIN_4;
constexpr auto PH5 = Config::PORT_H | Config::PIN_5;
constexpr auto PH6 = Config::PORT_H | Config::PIN_6;
constexpr auto PH7 = Config::PORT_H | Config::PIN_7;
constexpr auto PH8 = Config::PORT_H | Config::PIN_8;
constexpr auto PH9 = Config::PORT_H | Config::PIN_9;
constexpr auto PH10 = Config::PORT_H | Config::PIN_10;
constexpr auto PH11 = Config::PORT_H | Config::PIN_11;
constexpr auto PH12 = Config::PORT_H | Config::PIN_12;
constexpr auto PH13 = Config::PORT_H | Config::PIN_13;
constexpr auto PH14 = Config::PORT_H | Config::PIN_14;
constexpr auto PH15 = Config::PORT_H | Config::PIN_15;

// convenience for alternate function
constexpr auto AF0 = Config::ALTERNATE_FUNCTION_0;
constexpr auto AF1 = Config::ALTERNATE_FUNCTION_1;
constexpr auto AF2 = Config::ALTERNATE_FUNCTION_2;
constexpr auto AF3 = Config::ALTERNATE_FUNCTION_3;
constexpr auto AF4 = Config::ALTERNATE_FUNCTION_4;
constexpr auto AF5 = Config::ALTERNATE_FUNCTION_5;
constexpr auto AF6 = Config::ALTERNATE_FUNCTION_6;
constexpr auto AF7 = Config::ALTERNATE_FUNCTION_7;
constexpr auto AF8 = Config::ALTERNATE_FUNCTION_8;
constexpr auto AF9 = Config::ALTERNATE_FUNCTION_9;
constexpr auto AF10 = Config::ALTERNATE_FUNCTION_10;
constexpr auto AF11 = Config::ALTERNATE_FUNCTION_11;
constexpr auto AF12 = Config::ALTERNATE_FUNCTION_12;
constexpr auto AF13 = Config::ALTERNATE_FUNCTION_13;
constexpr auto AF14 = Config::ALTERNATE_FUNCTION_14;
constexpr auto AF15 = Config::ALTERNATE_FUNCTION_15;


/// @brief Mode of aGPIO pin.
///
enum class Mode : uint8_t {
    // input
    INPUT = 0, // default

    // output (getInput() can read back the output value)
    OUTPUT = 1,

    // alternate function, controlled by peripheral
    ALTERNATE = 2,

    // analog
    ANALOG = 3,

    MODE_MASK = 3,
};

/// @brief Get pointer to GPIO port structure (e.g. GPIOA)
/// @param config Pin and port (configuration ignored)
inline GPIO_TypeDef *getPort(Config config) {return (GPIO_TypeDef *)(GPIOA_BASE + (int(config & Config::PORT_MASK) << 6));} // * 0x00000400UL

constexpr int getPinIndex2(Config config) {return int(config) & 0xf;}
constexpr int getPortIndex(Config config) {return (int(config) >> 4) & 0xf;}

/// @brief Index of pin including port, e.g. PB0 has index 16
/// @param config Pin and port (configuration ignored)
constexpr int getPinPortIndex(Config config) {return int(config) & 0xff;}

/// @brief Enable a GPIO pin as input.
/// Make sure the clock of the port is enabled in SystemInit()
/// @param config pin, port and configuration (only pull up/down used, alternate function is not set)
inline void enableInput(Config config) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    int pos2 = pin << 1;

    // set pull
    int pull = extract(config, Config::PULL_MASK);
    port->PUPDR = (port->PUPDR & ~(3 << pos2)) | (pull << pos2);

    // set mode
    int mode = int(Mode::INPUT);
    port->MODER = (port->MODER & ~(3 << pos2)) | (mode << pos2);
}

/// @brief Enable a GPIO pin as output.
/// Make sure the clock of the port is enabled in SystemInit()
/// @param config pin, port and configuration (alternate function is not set)
/// @param initialValue initial value of the output pin (may get inverted by INVERT flag)
inline void enableOutput(Config config, bool initialValue) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    int pos2 = pin << 1;

    // set initial value of output bit
    int flag = int(initialValue) ^ extract(config, Config::INVERT);
    port->BSRR = (0x10000 | flag) << pin;

    // set pull
    int pull = extract(config, Config::PULL_MASK);
    port->PUPDR = (port->PUPDR & ~(3 << pos2)) | (pull << pos2);

    // set drive
    int drive = extract(config, Config::DRIVE_MASK);
    port->OTYPER = (port->OTYPER & ~(1 << pin)) | (drive << pin);

    // set speed
    int speed = extract(config, Config::SPEED_MASK);
    port->OSPEEDR = (port->OSPEEDR & ~(3 << pos2)) | (speed << pos2);

    // set mode
    int mode = int(Mode::OUTPUT);
    port->MODER = (port->MODER & ~(3 << pos2)) | (mode << pos2);
}

/// @brief Enable a GPIO pin for alternate function.
/// Make sure the clock of the port is enabled in SystemInit().
/// Direction (input/output) is configured by the peripheral. Checks for gpio::Config::NONE.
/// @param config pin, port and configuration
inline void enableAlternate(Config config) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    int pos2 = pin << 1;
    int pos4 = (pin & 7) << 2;

    // set pull
    int pull = extract(config, Config::PULL_MASK);
    port->PUPDR = (port->PUPDR & ~(3 << pos2)) | (pull << pos2);

    // set drive
    int drive = extract(config, Config::DRIVE_MASK);
    port->OTYPER = (port->OTYPER & ~(1 << pin)) | (drive << pin);

    // set speed
    int speed = extract(config, Config::SPEED_MASK);
    port->OSPEEDR = (port->OSPEEDR & ~(3 << pos2)) | (speed << pos2);

    // set alternate function
    int function = extract(config, Config::ALTERNATE_FUNCTION_MASK);
    auto &AFR = port->AFR[pin >> 3];
    AFR = (AFR & ~(15 << pos4)) | (function << pos4);

    // set mode
    int mode = int(Mode::ALTERNATE);
    port->MODER = (port->MODER & ~(3 << pos2)) | (mode << pos2);
}

/// @brief Enable a GPIO pin as analog input.
/// Make sure the clock of the port is enabled in SystemInit()
/// @param config pin and port (pull up/down gets disabled, no additional configuraton gets applied)
inline void enableAnalog(Config config) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    int pos2 = pin << 1;

    // disable pull
    port->PUPDR = port->PUPDR & ~(3 << pos2);

    // set mode to analog
    int mode = int(Mode::ANALOG);
    port->MODER = (port->MODER & ~(3 << pos2)) | (mode << pos2);
}

/// @brief Enable a GPIO pin.
/// Make sure the clock of the port is enabled in SystemInit()
/// @param config Pin, port and configuration
/// @param mode Mode
inline void enable(Config config, Mode mode) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    int pos2 = pin << 1;
    int pos4 = (pin & 7) << 2;

    // set pull
    int pull = extract(config, Config::PULL_MASK);
    port->PUPDR = (port->PUPDR & ~(3 << pos2)) | (pull << pos2);

    // set drive
    int drive = extract(config, Config::DRIVE_MASK);
    port->OTYPER = (port->OTYPER & ~(1 << pin)) | (drive << pin);

    // set speed
    int speed = extract(config, Config::SPEED_MASK);
    port->OSPEEDR = (port->OSPEEDR & ~(3 << pos2)) | (speed << pos2);

    // set alternate function
    int function = extract(config, Config::ALTERNATE_FUNCTION_MASK);
    auto &AFR = port->AFR[pin >> 3];
    AFR = (AFR & ~(15 << pos4)) | (function << pos4);

    // set mode
    port->MODER = (port->MODER & ~(3 << pos2)) | (int(mode) << pos2);
}

/// @brief Set only the mode of a GPIO
/// @param pinPort pin and port (no additional configuraton gets applied)
/// @param mode mode
inline void setMode(Config pinPort, Mode mode) {
    auto port = getPort(pinPort);
    int pin = extract(pinPort, Config::PIN_MASK);
    int pos2 = pin << 1;

    port->MODER = (port->MODER & ~(3 << pos2)) | (int(mode) << pos2);
}


/// @brief Get input value
/// @param pinPort pin and port (no additional configuraton gets applied except for INVERT)
inline bool getInput(Config pinPort) {
    auto port = getPort(pinPort);
    int pin = extract(pinPort, Config::PIN_MASK);
    return bool(((port->IDR >> pin) & 1) ^ extract(pinPort, Config::INVERT));
}

/// @brief Set output value
/// @param pinPort pin and port (no additional configuraton gets applied except for INVERT)
inline void setOutput(Config pinPort, bool value) {
    auto port = getPort(pinPort);
    int pin = extract(pinPort, Config::PIN_MASK);
    int flag = int(value != 0) ^ extract(pinPort, Config::INVERT);
    port->BSRR = (0x10000 | flag) << pin;
}

/// @brief Toggle output value
/// @param pinPort pin and port (no additional configuraton gets applied)
inline void toggleOutput(Config pinPort) {
    auto port = getPort(pinPort);
    int pin = extract(pinPort, Config::PIN_MASK);
    port->BSRR = (0x00010001 << pin) & ~port->ODR;
}

/// @brief Set or toggle output value
/// @param pinPort pin and port (no additional configuraton gets applied except for INVERT)
/// @param value Value to set
/// @param function Function: false: toggle when value is set, true: set value to output
inline void setOutput(Config pinPort, bool value, bool function) {
    auto port = getPort(pinPort);
    int pin = extract(pinPort, Config::PIN_MASK);
    if (function) {
        // clear or set
        int flag = int(value != 0) ^ extract(pinPort, Config::INVERT);
        port->BSRR = (0x10000 | flag) << pin;
    } else if (value) {
        // toggle
        port->BSRR = (0x00010001 << pin) & ~port->ODR;
    }
}

} // namespace gpio
} // namespace coco
