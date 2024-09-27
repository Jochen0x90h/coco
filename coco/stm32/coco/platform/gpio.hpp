#pragma once

#include <coco/platform/platform.hpp>
#include <coco/bits.hpp>
#include <coco/enum.hpp>


namespace coco {

/**
    Refernece manual:
        f0:
            https://www.st.com/resource/en/reference_manual/dm00031936-stm32f0x1stm32f0x2stm32f0x8-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
                GPIO: Section 8
        f3:
            https://www.st.com/resource/en/reference_manual/rm0364-stm32f334xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
                GPIO: Section 9
        g4:
            https://www.st.com/resource/en/reference_manual/rm0440-stm32g4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
                GPIO: Section 9
    Data sheet:
        f0:
            https://www.st.com/resource/en/datasheet/stm32f042f6.pdf
            https://www.st.com/resource/en/datasheet/dm00039193.pdf
                GPIO AC characteristics: Table 50
        f3:
            https://www.st.com/resource/en/datasheet/stm32f334k4.pdf
                I/O AC characteristics: Table 52
        g4:
            https://www.st.com/resource/en/datasheet/stm32g431rb.pdf
                GPIO AC characteristics: Table 55
*/
namespace gpio {


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
    PORT_MASK = 15 << 4,

    // convenience for pin and port
    PA0 = PORT_A | PIN_0,
    PA1 = PORT_A | PIN_1,
    PA2 = PORT_A | PIN_2,
    PA3 = PORT_A | PIN_3,
    PA4 = PORT_A | PIN_4,
    PA5 = PORT_A | PIN_5,
    PA6 = PORT_A | PIN_6,
    PA7 = PORT_A | PIN_7,
    PA8 = PORT_A | PIN_8,
    PA9 = PORT_A | PIN_9,
    PA10 = PORT_A | PIN_10,
    PA11 = PORT_A | PIN_11,
    PA12 = PORT_A | PIN_12,
    PA13 = PORT_A | PIN_13,
    PA14 = PORT_A | PIN_14,
    PA15 = PORT_A | PIN_15,

    PB0 = PORT_B | PIN_0,
    PB1 = PORT_B | PIN_1,
    PB2 = PORT_B | PIN_2,
    PB3 = PORT_B | PIN_3,
    PB4 = PORT_B | PIN_4,
    PB5 = PORT_B | PIN_5,
    PB6 = PORT_B | PIN_6,
    PB7 = PORT_B | PIN_7,
    PB8 = PORT_B | PIN_8,
    PB9 = PORT_B | PIN_9,
    PB10 = PORT_B | PIN_10,
    PB11 = PORT_B | PIN_11,
    PB12 = PORT_B | PIN_12,
    PB13 = PORT_B | PIN_13,
    PB14 = PORT_B | PIN_14,
    PB15 = PORT_B | PIN_15,

    PC0 = PORT_C | PIN_0,
    PC1 = PORT_C | PIN_1,
    PC2 = PORT_C | PIN_2,
    PC3 = PORT_C | PIN_3,
    PC4 = PORT_C | PIN_4,
    PC5 = PORT_C | PIN_5,
    PC6 = PORT_C | PIN_6,
    PC7 = PORT_C | PIN_7,
    PC8 = PORT_C | PIN_8,
    PC9 = PORT_C | PIN_9,
    PC10 = PORT_C | PIN_10,
    PC11 = PORT_C | PIN_11,
    PC12 = PORT_C | PIN_12,
    PC13 = PORT_C | PIN_13,
    PC14 = PORT_C | PIN_14,
    PC15 = PORT_C | PIN_15,

    PD0 = PORT_D | PIN_0,
    PD1 = PORT_D | PIN_1,
    PD2 = PORT_D | PIN_2,
    PD3 = PORT_D | PIN_3,
    PD4 = PORT_D | PIN_4,
    PD5 = PORT_D | PIN_5,
    PD6 = PORT_D | PIN_6,
    PD7 = PORT_D | PIN_7,
    PD8 = PORT_D | PIN_8,
    PD9 = PORT_D | PIN_9,
    PD10 = PORT_D | PIN_10,
    PD11 = PORT_D | PIN_11,
    PD12 = PORT_D | PIN_12,
    PD13 = PORT_D | PIN_13,
    PD14 = PORT_D | PIN_14,
    PD15 = PORT_D | PIN_15,

    PE0 = PORT_E | PIN_0,
    PE1 = PORT_E | PIN_1,
    PE2 = PORT_E | PIN_2,
    PE3 = PORT_E | PIN_3,
    PE4 = PORT_E | PIN_4,
    PE5 = PORT_E | PIN_5,
    PE6 = PORT_E | PIN_6,
    PE7 = PORT_E | PIN_7,
    PE8 = PORT_E | PIN_8,
    PE9 = PORT_E | PIN_9,
    PE10 = PORT_E | PIN_10,
    PE11 = PORT_E | PIN_11,
    PE12 = PORT_E | PIN_12,
    PE13 = PORT_E | PIN_13,
    PE14 = PORT_E | PIN_14,
    PE15 = PORT_E | PIN_15,

    PF0 = PORT_F | PIN_0,
    PF1 = PORT_F | PIN_1,
    PF2 = PORT_F | PIN_2,
    PF3 = PORT_F | PIN_3,
    PF4 = PORT_F | PIN_4,
    PF5 = PORT_F | PIN_5,
    PF6 = PORT_F | PIN_6,
    PF7 = PORT_F | PIN_7,
    PF8 = PORT_F | PIN_8,
    PF9 = PORT_F | PIN_9,
    PF10 = PORT_F | PIN_10,
    PF11 = PORT_F | PIN_11,
    PF12 = PORT_F | PIN_12,
    PF13 = PORT_F | PIN_13,
    PF14 = PORT_F | PIN_14,
    PF15 = PORT_F | PIN_15,

    PG0 = PORT_G | PIN_0,
    PG1 = PORT_G | PIN_1,
    PG2 = PORT_G | PIN_2,
    PG3 = PORT_G | PIN_3,
    PG4 = PORT_G | PIN_4,
    PG5 = PORT_G | PIN_5,
    PG6 = PORT_G | PIN_6,
    PG7 = PORT_G | PIN_7,
    PG8 = PORT_G | PIN_8,
    PG9 = PORT_G | PIN_9,
    PG10 = PORT_G | PIN_10,
    PG11 = PORT_G | PIN_11,
    PG12 = PORT_G | PIN_12,
    PG13 = PORT_G | PIN_13,
    PG14 = PORT_G | PIN_14,
    PG15 = PORT_G | PIN_15,

    PH0 = PORT_H | PIN_0,
    PH1 = PORT_H | PIN_1,
    PH2 = PORT_H | PIN_2,
    PH3 = PORT_H | PIN_3,
    PH4 = PORT_H | PIN_4,
    PH5 = PORT_H | PIN_5,
    PH6 = PORT_H | PIN_6,
    PH7 = PORT_H | PIN_7,
    PH8 = PORT_H | PIN_8,
    PH9 = PORT_H | PIN_9,
    PH10 = PORT_H | PIN_10,
    PH11 = PORT_H | PIN_11,
    PH12 = PORT_H | PIN_12,
    PH13 = PORT_H | PIN_13,
    PH14 = PORT_H | PIN_14,
    PH15 = PORT_H | PIN_15,

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

    // convenience for alternate function
    AF0 = ALTERNATE_FUNCTION_0,
    AF1 = ALTERNATE_FUNCTION_1,
    AF2 = ALTERNATE_FUNCTION_2,
    AF3 = ALTERNATE_FUNCTION_3,
    AF4 = ALTERNATE_FUNCTION_4,
    AF5 = ALTERNATE_FUNCTION_5,
    AF6 = ALTERNATE_FUNCTION_6,
    AF7 = ALTERNATE_FUNCTION_7,
    AF8 = ALTERNATE_FUNCTION_8,
    AF9 = ALTERNATE_FUNCTION_9,
    AF10 = ALTERNATE_FUNCTION_10,
    AF11 = ALTERNATE_FUNCTION_11,
    AF12 = ALTERNATE_FUNCTION_12,
    AF13 = ALTERNATE_FUNCTION_13,
    AF14 = ALTERNATE_FUNCTION_14,
    AF15 = ALTERNATE_FUNCTION_15,

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

enum class Mode : uint8_t {
    INPUT = 0, // default
    OUTPUT = 1,
    ALTERNATE = 2,
    ANALOG = 3,
    MODE_MASK = 3,
};


/**
 * Get pointer to GPIO port structure (e.g. GPIOA)
 */
inline GPIO_TypeDef *getPort(Config config) {return (GPIO_TypeDef *)(GPIOA_BASE + (int(config & Config::PORT_MASK) << 6));} // * 0x00000400UL

/**
 * Index of pin including port, e.g. PB0 has index 16
 */
constexpr uint8_t getPinIndex(Config config) {return int(config) & 0xff;}


/**
 * Configure a GPIO pin.
 * Make sure the clock of the port is enabled in SystemInit()
 * @param mode mode
 * @param config configuration
 */
inline void configure(Mode mode, Config config) {
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

/**
 * Configure a GPIO pin as input.
 * Make sure the clock of the port is enabled in SystemInit()
 * @param config pin, port and configuration (only pull up/down used, alternate function is not set)
 */
inline void configureInput(Config config) {
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

/**
 * Configure a GPIO pin as output.
 * Make sure the clock of the port is enabled in SystemInit()
 * @param config pin, port and configuration (alternate function is not set)
 * @param initialValue initial value of the output pin (may get inverted by INVERT flag)
 */
inline void configureOutput(Config config, bool initialValue) {
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

/**
 * Configure a GPIO pin for alternate function.
 * Make sure the clock of the port is enabled in SystemInit()
 * @param config pin, port and configuration
 */
inline void configureAlternate(Config config) {
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

/**
 * Configure a GPIO pin for alternate function input.
 * Make sure the clock of the port is enabled in SystemInit()
 * @param config pin, port and configuration (only pull up/down and alternate function used)
 */
inline void configureAlternateInput(Config config) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    int pos2 = pin << 1;
    int pos4 = (pin & 7) << 2;

    // set pull
    int pull = extract(config, Config::PULL_MASK);
    port->PUPDR = (port->PUPDR & ~(3 << pos2)) | (pull << pos2);

    // set alternate function
    int function = extract(config, Config::ALTERNATE_FUNCTION_MASK);
    auto &AFR = port->AFR[pin >> 3];
    AFR = (AFR & ~(15 << pos4)) | (function << pos4);

    // set mode
    int mode = int(Mode::ALTERNATE);
    port->MODER = (port->MODER & ~(3 << pos2)) | (mode << pos2);
}

/**
 * Configure a GPIO pin as analog input.
 * Make sure the clock of the port is enabled in SystemInit()
 * @param config pin and port (pull up/down gets disabled, no additional configuraton gets applied)
 */
inline void configureAnalog(Config config) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    int pos2 = pin << 1;

    // disable pull
    port->PUPDR = port->PUPDR & ~(3 << pos2);

    // set mode to analog
    int mode = int(Mode::ANALOG);
    port->MODER = (port->MODER & ~(3 << pos2)) | (mode << pos2);
}


/**
 * Set the mode of a GPIO
 * @param config pin and port (no additional configuraton gets applied)
 * @param mode mode
 */
inline void setMode(Config pinPort, Mode mode) {
    auto port = getPort(pinPort);
    int pin = extract(pinPort, Config::PIN_MASK);
    int pos2 = pin << 1;

    port->MODER = (port->MODER & ~(3 << pos2)) | (int(mode) << pos2);
}

/**
 * Get input value
 */
inline bool getInput(Config config) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    return bool(((port->IDR >> pin) & 1) ^ extract(config, Config::INVERT));
}

/**
 * Enable ouput of a GPIO, use to switch between input and output after calling configure()
 * @param enable true for output, false for input
 */
inline void enableOutput(Config config, bool enable) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    int pos2 = pin << 1;
    port->MODER = (port->MODER & ~(3 << pos2)) | (int(enable) << pos2);
}

/**
 * Set output value
 */
inline void setOutput(Config config, bool value) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    int flag = int(value != 0) ^ extract(config, Config::INVERT);
    port->BSRR = (0x10000 | flag) << pin;
}

/**
 * Toggle output value
 */
inline void toggleOutput(Config config) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    port->BSRR = (0x00010001 << pin) & ~port->ODR;
}

/**
 * Set or toggle output value
 */
inline void setOutput(Config config, bool value, bool function) {
    auto port = getPort(config);
    int pin = extract(config, Config::PIN_MASK);
    if (function) {
        // clear or set
        int flag = int(value != 0) ^ extract(config, Config::INVERT);
        port->BSRR = (0x10000 | flag) << pin;
    } else if (value) {
        // toggle
        port->BSRR = (0x00010001 << pin) & ~port->ODR;
    }
}





/*

// pin, port and optional alternative function
constexpr int PA(int pin, int function = 0) {return (function << 8) + pin;}
constexpr int PB(int pin, int function = 0) {return (function << 8) + 16 + pin;}
constexpr int PC(int pin, int function = 0) {return (function << 8) + 32 + pin;}
constexpr int PD(int pin, int function = 0) {return (function << 8) + 48 + pin;}
#ifdef GPIOE
constexpr int PE(int pin, int function = 0) {return (function << 8) + 64 + pin;}
#endif
constexpr int PF(int pin, int function = 0) {return (function << 8) + 80 + pin;}
#ifdef GPIOG
constexpr int PG(int pin, int function = 0) {return (function << 8) + 96 + pin;}
#endif
#ifdef GPIOH
constexpr int PH(int pin, int function = 0) {return (function << 8) + 112 + pin;}
#endif

// pin is disconnected
constexpr int DISCONNECTED = -1;

// get pin index
constexpr int getPin(int pinPortFunction) {return pinPortFunction & 0x00f;}

// get port index
constexpr int getPort(int pinPortFunction) {return (pinPortFunction >> 4) & 0x0f;}

// get pin and port index
constexpr int getPinPort(int pinPortFunction) {return pinPortFunction & 0x0ff;}

// get alternate function
constexpr int getFunction(int pinPortFunction) {return pinPortFunction >> 8;}

// get pointer to GPIO port structure (e.g. GPIOA)
inline GPIO_TypeDef *GPIO(int pinPortFunction) {return (GPIO_TypeDef *)(GPIOA_BASE + ((pinPortFunction & 0xf0) << 6));} // * 0x00000400UL


/ **
 * GPIO configuration
 * /
enum class Config : uint8_t {
    // default configuratoin: input, no pull up/down, drive in both directions, slow speed
    DEFAULT = 0,

    // mode
    INPUT = 0, // default
    OUTPUT = 1,
    ALTERNATE = 2,
    ANALOG = 3,
    MODE_MASK = 3,

    // pull up/down resistor on pin
    PULL_NONE = 0, // default
    PULL_UP = 1 << 2,
    PULL_DOWN = 2 << 2,
    PULL_MASK = 3 << 2,

    // drive
    DRIVE_BOTH = 0, // default
    DRIVE_DOWN = 1 << 4,
    DRIVE_MASK = 1 << 4,

    // speed
    SPEED_LOW = 0, // default, ~2 MHz
    SPEED_MEDIUM = 1 << 6, // ~10-25 MHz
#if defined(STM32F0) || defined(STM32F3)
    SPEED_HIGH = 3 << 6, // f0, f3: ~20-50 MHz
#else
    SPEED_HIGH = 2 << 6, // g4: ~25-100 MHz
#endif
    SPEED_MAX = 3 << 6, // g4: ~50-180 MHz
    SPEED_MASK = 3 << 6
};
COCO_ENUM(Config);


/ **
 * Configure a GPIO pin.
 * Make sure the clock of the port is enabled in SystemInit()
 * @param pinPortFunction pin, port and alternate function
 * @param initialValue initial value of the output bit
 * @param config configuration
 * /
inline void configure(int pinPortFunction, bool initialValue, Config config) {
    auto port = GPIO(pinPortFunction);
    int pin = getPin(pinPortFunction);
    int pos2 = pin << 1;
    int pos4 = (pin & 7) << 2;

    // set initial value of output bit
    port->BSRR = 1 << (pin + (initialValue ? 0 : 16));

    // set pull
    int pull = (int(config) >> 2) & 3;
    port->PUPDR = (port->PUPDR & ~(3 << pos2)) | (pull << pos2);

    // set drive
    int drive = (int(config) >> 4) & 1;
    port->OTYPER = (port->OTYPER & ~(1 << pin)) | (drive << pin);

    // set speed
    int speed = (int(config) >> 6);
    port->OSPEEDR = (port->OSPEEDR & ~(3 << pos2)) | (speed << pos2);

    // set alternate function
    int function = getFunction(pinPortFunction);
    auto &AFR = port->AFR[pin >> 3];
    AFR = (AFR & ~(15 << pos4)) | (function << pos4);

    // set mode
    int mode = int(config) & 3;
    port->MODER = (port->MODER & ~(3 << pos2)) | (mode << pos2);
}

/ **
 * Configure a GPIO pin as input.
 * Make sure the clock of the port is enabled in SystemInit()
 * @param pinPort pin and port
 * @param config configuration (only pull up/down)
* /
inline void configureInput(int pinPort, Config config = Config::DEFAULT) {
    auto port = GPIO(pinPort);
    int pin = getPin(pinPort);
    int pos2 = pin << 1;

    // set pull
    int pull = (int(config) >> 2) & 3;
    port->PUPDR = (port->PUPDR & ~(3 << pos2)) | (pull << pos2);

    // set mode
    int mode = int(Config::INPUT);
    port->MODER = (port->MODER & ~(3 << pos2)) | (mode << pos2);
}

/ **
 * Configure a GPIO pin as output.
 * Make sure the clock of the port is enabled in SystemInit()
 * @param pinPort pin and port
 * @param initialValue initial value of the output bit
 * @param config configuration
 * /
inline void configureOutput(int pinPort, bool initialValue, Config config = Config::DEFAULT) {
    auto port = GPIO(pinPort);
    int pin = getPin(pinPort);
    int pos2 = pin << 1;

    // set initial value of output bit
    port->BSRR = 1 << (pin + (initialValue ? 0 : 16));

    // set pull
    int pull = (int(config) >> 2) & 3;
    port->PUPDR = (port->PUPDR & ~(3 << pos2)) | (pull << pos2);

    // set drive
    int drive = (int(config) >> 4) & 1;
    port->OTYPER = (port->OTYPER & ~(1 << pin)) | (drive << pin);

    // set speed
    int speed = (int(config) >> 6);
    port->OSPEEDR = (port->OSPEEDR & ~(3 << pos2)) | (speed << pos2);

    // set mode
    int mode = int(Config::OUTPUT);
    port->MODER = (port->MODER & ~(3 << pos2)) | (mode << pos2);
}

/ **
 * Configure a GPIO pin for alternate function.
 * Make sure the clock of the port is enabled in SystemInit()
 * @param pinPortFunction pin, port and alternate function
 * @param config configuration
 * /
inline void configureAlternate(int pinPortFunction, Config config) {
    auto port = GPIO(pinPortFunction);
    int pin = getPin(pinPortFunction);
    int pos2 = pin << 1;
    int pos4 = (pin & 7) << 2;

    // set pull
    int pull = (int(config) >> 2) & 3;
    port->PUPDR = (port->PUPDR & ~(3 << pos2)) | (pull << pos2);

    // set drive
    int drive = (int(config) >> 4) & 1;
    port->OTYPER = (port->OTYPER & ~(1 << pin)) | (drive << pin);

    // set speed
    int speed = (int(config) >> 6);
    port->OSPEEDR = (port->OSPEEDR & ~(3 << pos2)) | (speed << pos2);

    // set alternate function
    int function = getFunction(pinPortFunction);
    auto &AFR = port->AFR[pin >> 3];
    AFR = (AFR & ~(15 << pos4)) | (function << pos4);

    // set mode
    int mode = int(Config::ALTERNATE);
    port->MODER = (port->MODER & ~(3 << pos2)) | (mode << pos2);
}

/ **
 * Configure a GPIO pin for alternate function input.
 * Make sure the clock of the port is enabled in SystemInit()
 * @param pinPortFunction pin, port and alternate function
 * @param config configuration (only pull up/down)
 * /
inline void configureAlternateInput(int pinPortFunction, Config config = Config::DEFAULT) {
    auto port = GPIO(pinPortFunction);
    int pin = getPin(pinPortFunction);
    int pos2 = pin << 1;
    int pos4 = (pin & 7) << 2;

    // set pull
    int pull = (int(config) >> 2) & 3;
    port->PUPDR = (port->PUPDR & ~(3 << pos2)) | (pull << pos2);

    // set alternate function
    int function = getFunction(pinPortFunction);
    auto &AFR = port->AFR[pin >> 3];
    AFR = (AFR & ~(15 << pos4)) | (function << pos4);

    // set mode
    int mode = int(Config::ALTERNATE);
    port->MODER = (port->MODER & ~(3 << pos2)) | (mode << pos2);
}

/ **
 * Configure a GPIO pin as analog input.
 * Make sure the clock of the port is enabled in SystemInit()
 * @param pinPort pin and port
 * /
inline void configureAnalog(int pinPort) {
    auto port = GPIO(pinPort);
    int pos2 = getPin(pinPort) << 1;

    // disable pull
    port->PUPDR = port->PUPDR & ~(3 << pos2);

    // set mode to analog
    int mode = int(Config::ANALOG);
    port->MODER = (port->MODER & ~(3 << pos2)) | (mode << pos2);
}

/ **
 * Set the mode of a GPIO
 * @param pinPort pin and port
 * @param config configuration (only mode)
 * /
inline void setMode(int pinPort, Config config) {
    auto port = GPIO(pinPort);
    int pos2 = getPin(pinPort) << 1;

    int mode = int(config) & 3;
    port->MODER = (port->MODER & ~(3 << pos2)) | (mode << pos2);
}

/ **
 * Get input value
 * /
inline bool getInput(int pinPort) {
    auto port = GPIO(pinPort);
    int pin = getPin(pinPort);
    return (port->IDR & (1 << pin)) != 0;
}

/ **
 * Enable ouput of a GPIO, use to switch between input and output after calling configure()
 * @param enable true for output, false for input
 * /
inline void enableOutput(int pinPort, bool enable) {
    auto port = GPIO(pinPort);
    int pos2 = getPin(pinPort) << 1;
    port->MODER = (port->MODER & ~(3 << pos2)) | (int(enable) << pos2);
}

/ **
 * Set output value
 * /
inline void setOutput(int pinPort, bool value) {
    auto port = GPIO(pinPort);
    int pin = getPin(pinPort);
    port->BSRR = 1 << (pin + (value ? 0 : 16));
}

/ **
 * Toggle output value
 * /
inline void toggleOutput(int pinPort) {
    auto port = GPIO(pinPort);
    int pin = getPin(pinPort);
    port->BSRR = (0x00010001 << pin) & ~port->ODR;
}

/ **
 * Set or toggle output value
 * /
inline void setOutput(int pinPort, bool value, bool function) {
    auto port = GPIO(pinPort);
    int pin = getPin(pinPort);
    if (function)
        port->BSRR = 1 << (pin + (value ? 0 : 16));
    else if (value)
        port->BSRR = (0x00010001 << pin) & ~port->ODR;
}
*/

} // namespace gpio
} // namespace coco
