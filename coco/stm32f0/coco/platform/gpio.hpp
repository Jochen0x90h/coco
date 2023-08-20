#pragma once

#include "platform.hpp"


namespace coco {

/**
	Data sheet:
		f0:
			https://www.st.com/resource/en/datasheet/stm32f042f6.pdf
			https://www.st.com/resource/en/datasheet/dm00039193.pdf
				GPIO AC characteristics: Table 50
		g4:
			https://www.st.com/resource/en/datasheet/stm32g431rb.pdf
				GPIO AC characteristics: Table 55
	Refernece manual:
		f0:
			https://www.st.com/resource/en/reference_manual/dm00031936-stm32f0x1stm32f0x2stm32f0x8-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
				GPIO: Section 8
		g4:
			https://www.st.com/resource/en/reference_manual/rm0440-stm32g4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
				GPIO: Section 9
*/
namespace gpio {

// port pins
constexpr int PA(int index) { return index; }
constexpr int PB(int index) { return 16 + index; }
constexpr int PC(int index) { return 32 + index; }
constexpr int PD(int index) { return 48 + index; }
constexpr int PE(int index) { return 64 + index; }
constexpr int PF(int index) { return 80 + index; }

// get pointer to GPIO port for a given pin (e.g. GPIOA)
inline GPIO_TypeDef *getPort(int pin) { return (GPIO_TypeDef *)(GPIOA_BASE + (pin >> 4) * 0x00000400UL); }


enum class Mode : uint8_t {
	INPUT = 0,
	OUTPUT = 1,
	ALTERNATE = 2,
	ANALOG = 3
};

enum class Pull : uint8_t {
	DISABLED = 0,
	UP = 1, // pull up resistor on pin
	DOWN = 2 // pull down resistor on pin
};

enum class Drive : uint8_t {
	BOTH = 0,
	DOWN = 1,
};

enum class Speed : uint8_t {
	LOW = 0, // ~2 MHz
	MEDIUM = 1, // ~10-25 MHz
#ifdef STM32F0
	HIGH = 3, // f0: ~20-50 MHz
#endif
#ifdef STM32G4
	HIGH = 2, // g4: ~25-100 MHz
#endif
	MAX = 3 // g4: ~50-180 MHz
};

// enable clock
inline void enableClock(int pin) {
#ifdef STM32F0
	RCC->AHBENR = RCC->AHBENR | (1 << (RCC_AHBENR_GPIOAEN_Pos + (pin >> 4)));
	auto x = RCC->AHBENR;
#endif
#ifdef STM32G4
	RCC->AHB2ENR = RCC->AHB2ENR | (1 << (RCC_AHB2ENR_GPIOAEN_Pos + (pin >> 4)));

	// wait two clock cycles (reference manual section 7.2.17)
	auto x = RCC->AHB2ENR;
#endif
}

// set mode of a gpio
inline void setMode(int pin, Mode mode) {
	auto port = getPort(pin);
	int pos2 = (pin & 15) << 1;
	port->MODER = (port->MODER & ~(3 << pos2)) | (int(mode) << pos2);
}


// analog
// ------

inline void configureAnalog(int pin) {
	// enable peripheral clock of the port
	enableClock(pin);

	auto port = getPort(pin);
	int pos2 = (pin & 15) << 1;

	// set mode to analog
	port->MODER = (port->MODER & ~(3 << pos2)) | (int(Mode::ANALOG) << pos2);
}


// input
// -----

// configure as input
inline void configureInput(int pin, Pull pull = Pull::DISABLED) {
	// enable peripheral clock of the port
	enableClock(pin);

	auto port = getPort(pin);
	int pos2 = (pin & 15) << 1;

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

// configure as output
inline void configureOutput(int pin, bool initialValue, Speed speed = Speed::HIGH, Drive drive = Drive::BOTH,
	Pull pull = Pull::DISABLED)
{
	// enable peripheral clock of the port
	enableClock(pin);

	auto port = getPort(pin);
	int pos = pin & 15;
	int pos2 = pos << 1;

	// set initial value of the output
	port->BSRR = 1 << (pos + (initialValue ? 0 : 16));

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

// configure as input and output, default is input
inline void configureInOut(int pin, Pull pull = Pull::DISABLED, Speed speed = Speed::HIGH, Drive drive = Drive::BOTH) {
	// enable peripheral clock of the port
	enableClock(pin);

	auto port = getPort(pin);
	int pos = pin & 15;
	int pos2 = pos << 1;

	// set pull, drive and speed
	port->PUPDR = (port->PUPDR & ~(3 << pos2)) | (int(pull) << pos2);
	port->OSPEEDR = (port->OSPEEDR & ~(3 << pos2)) | (int(speed) << pos2);
	port->OTYPER = (port->OTYPER & ~(1 << pos)) | (int(drive) << pos);

	// set mode to input
	port->MODER = (port->MODER & ~(3 << pos2)) | (int(Mode::INPUT) << pos2);
}

// enable the output driver
inline void enableOutput(int pin, bool enable) {
	auto port = getPort(pin);
	int pos2 = (pin & 15) << 1;
	port->MODER = (port->MODER & ~(3 << pos2)) | (int(enable ? Mode::OUTPUT : Mode::INPUT) << pos2);
}


// alternate functions
// -------------------

struct PinFunction {
	int pin;
	int function;
};

inline void configureAlternateInput(PinFunction pf, Pull pull = Pull::DISABLED) {
	// enable peripheral clock of the port
	enableClock(pf.pin);

	auto port = getPort(pf.pin);
	int pos = pf.pin & 15;
	int pos2 = pos << 1;
	int pos4 = (pos & 7) << 2;

	// set alternate function
	auto &AFR = port->AFR[pos >> 3];
	AFR = (AFR & ~(15 << pos4)) | (pf.function << pos4);

	// set pull
	port->PUPDR = (port->PUPDR & ~(3 << pos2)) | (int(pull) << pos2);

	// set mode to alternate function
	port->MODER = (port->MODER & ~(3 << pos2)) | (int(Mode::ALTERNATE) << pos2);
}

inline void configureAlternateOutput(PinFunction pf, Speed speed = Speed::HIGH, Drive drive = Drive::BOTH,
	Pull pull = Pull::DISABLED)
{
	// enable peripheral clock of the port
	enableClock(pf.pin);

	auto port = getPort(pf.pin);
	int pos = pf.pin & 15;
	int pos2 = pos << 1;
	int pos4 = (pos & 7) << 2;

	// set alternate function
	auto &AFR = port->AFR[pos >> 3];
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
