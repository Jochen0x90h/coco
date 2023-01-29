#pragma once

#include "platform.hpp"


/*
	Data sheet: https://www.st.com/resource/en/datasheet/stm32f042f6.pdf
	Refernece manual section 8: https://www.st.com/resource/en/reference_manual/dm00031936-stm32f0x1stm32f0x2stm32f0x8-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
*/
namespace coco {
namespace gpio {

// port pins
constexpr int PA(int index) { return index; }
constexpr int PB(int index) { return 16 + index; }
constexpr int PC(int index) { return 32 + index; }
constexpr int PF(int index) { return 80 + index; }

// get pointer to GPIO port for a given pin (e.g. GPIOA)
inline GPIO_TypeDef *getPort(int pin) { return (GPIO_TypeDef *)(GPIOA_BASE + (pin >> 4) * 0x00000400UL); }


enum class Mode {
	INPUT = 0,
	OUTPUT = 1,
	ALTERNATE = 2,
	ANALOG = 3
};

enum class Pull {
	DISABLED = 0,
	UP = 1, // Pull up on pin
	DOWN = 2 // Pull down on pin
};

enum class Drive {
	PUSH_PULL = 0,
	OPEN_DRAIN = 1
};

enum class Speed {
	LOW = 0,
	MEDIUM = 1,
	HIGH = 3
};


// set mode of a gpio
inline void setMode(int pin, Mode mode) {
	auto port = getPort(pin);
	int pos2 = (pin & 15) << 1;
	port->MODER = (port->MODER & ~(3 << pos2)) | (int(mode) << pos2);
}


// input
// -----

// configure input
inline void configureInput(int pin, Pull pull = Pull::DISABLED) {
	auto port = getPort(pin);
	int pos2 = (pin & 15) << 1;

	// enable peripheral clock for the port
	RCC->AHBENR = RCC->AHBENR | (1 << (RCC_AHBENR_GPIOAEN_Pos + (pin >> 4)));

	// set pull
	port->PUPDR = (port->PUPDR & ~(3 << pos2)) | (int(pull) << pos2);

	// set mode to input
	port->MODER = (port->MODER & ~(3 << pos2)) | (int(Mode::INPUT) << pos2);
}

// get input value
inline bool getInput(int pin) {
	auto port = getPort(pin);
	return (port->IDR & (1 << (pin & 15))) != 0;
}


// output
// ------

inline void configureOutput(int pin, Pull pull = Pull::DISABLED, Speed speed = Speed::HIGH,
	Drive drive = Drive::PUSH_PULL)
{
	auto port = getPort(pin);
	int pos = pin & 15;
	int pos2 = (pin & 15) << 1;

	// enable peripheral clock for the port
	RCC->AHBENR = RCC->AHBENR | (1 << (RCC_AHBENR_GPIOAEN_Pos + (pin >> 4)));

	// set pull, drive and speed
	port->PUPDR = (port->PUPDR & ~(3 << pos2)) | (int(pull) << pos2);
	port->OSPEEDR = (port->OSPEEDR & ~(3 << pos2)) | (int(speed) << pos2);
	port->OTYPER = (port->OTYPER & ~(1 << pos)) | (int(drive) << pos);

	// set mode to output
	port->MODER = (port->MODER & ~(3 << pos2)) | (int(Mode::OUTPUT) << pos2);
}

// set output value
inline void setOutput(int pin, bool value) {
	auto port = getPort(pin);
	int pos = pin & 15;
	port->BSRR = 1 << (pos + (value ? 0 : 16));
}

// toggle output value
inline void toggleOutput(int pin) {
	auto port = getPort(pin);
	int pos = pin & 15;
	port->BSRR = (0x00010001 << pos) & ~port->ODR;
}


// in/out
// ------

// configure as input and output
inline void configureInOut(int pin, Mode mode, Pull pull = Pull::DISABLED, Speed speed = Speed::HIGH,
	Drive drive = Drive::PUSH_PULL)
{
	auto port = getPort(pin);
	int pos = pin & 15;
	int pos2 = (pin & 15) << 1;

	// enable peripheral clock for the port
	RCC->AHBENR = RCC->AHBENR | (1 << (RCC_AHBENR_GPIOAEN_Pos + (pin >> 4)));

	// set pull, drive and speed
	port->PUPDR = (port->PUPDR & ~(3 << pos2)) | (int(pull) << pos2);
	port->OSPEEDR = (port->OSPEEDR & ~(3 << pos2)) | (int(speed) << pos2);
	port->OTYPER = (port->OTYPER & ~(1 << pos)) | (int(drive) << pos);

	// set mode to input or output
	port->MODER = (port->MODER & ~(3 << pos2)) | (int(mode) << pos2);
}


// peripheral (alternate function)
// -------------------------------

struct PinFunction {
	int pin;
	int function;
};

inline void configureAlternateInput(PinFunction pf, Pull pull = Pull::DISABLED) {
	auto port = getPort(pf.pin);
	int pos2 = (pf.pin & 15) << 1;
	int pos4 = (pf.pin & 7) << 2;

	// enable peripheral clock for the port
	RCC->AHBENR = RCC->AHBENR | (1 << (RCC_AHBENR_GPIOAEN_Pos + (pf.pin >> 4)));

	// set alternate function
	auto &AFR = port->AFR[(pf.pin >> 3) & 1];
	AFR = (AFR & ~(15 << pos4)) | (pf.function << pos4);

	// set pull
	port->PUPDR = (port->PUPDR & ~(3 << pos2)) | (int(pull) << pos2);

	// set mode to alternate function
	port->MODER = (port->MODER & ~(3 << pos2)) | (int(Mode::ALTERNATE) << pos2);
}

inline void configureAlternateOutput(PinFunction pf, Pull pull = Pull::DISABLED, Speed speed = Speed::HIGH,
	Drive drive = Drive::PUSH_PULL)
{
	auto port = getPort(pf.pin);
	int pos = pf.pin & 15;
	int pos2 = pos << 1;
	int pos4 = (pf.pin & 7) << 2;

	// enable peripheral clock for the port
	RCC->AHBENR = RCC->AHBENR | (1 << (RCC_AHBENR_GPIOAEN_Pos + (pf.pin >> 4)));

	// set alternate function
	auto &AFR = port->AFR[(pf.pin >> 3) & 1];
	AFR = (AFR & ~(15 << pos4)) | (pf.function << pos4);

	// set drive, speed and pull
	port->PUPDR = (port->PUPDR & ~(3 << pos2)) | (int(pull) << pos2);
	port->OSPEEDR = (port->OSPEEDR & ~(3 << pos2)) | (int(speed) << pos2);
	port->OTYPER = (port->OTYPER & ~(1 << pos)) | (int(drive) << pos);

	// set mode to alternate function
	port->MODER = (port->MODER & ~(3 << pos2)) | (int(Mode::ALTERNATE) << pos2);
}

} // namespace gpio
} // namespace coco
