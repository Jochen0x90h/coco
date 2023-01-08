#pragma once

#include "platform.hpp"


namespace coco {
namespace gpio {

// ports
constexpr int P0(int index) { return index; }

constexpr int P1(int index) { return 32 + index; }

constexpr NRF_GPIO_Type *getPort(int pin) { return (NRF_GPIO_Type *) (pin < 32 ? NRF_P0_BASE : NRF_P1_BASE); }

// pin is disconnected
constexpr int DISCONNECTED = 0xffffffff;


enum class Mode {
	INPUT = 0,
	OUTPUT = 1
};

enum class Pull {
	DISABLED = 0,
	DOWN = 1, // Pull down on pin
	UP = 3 // Pull up on pin
};

enum class Drive {
	S0S1 = 0, // Standard '0', standard '1'
	H0S1 = 1, // High drive '0', standard '1'
	S0H1 = 2, // Standard '0', high drive '1'
	H0H1 = 3, // High drive '0', high drive '1'
	D0S1 = 4, // Disconnect '0' standard '1' (normally used for wired-or connections)
	D0H1 = 5, // Disconnect '0', high drive '1' (normally used for wired-or connections)
	S0D1 = 6, // Standard '0', disconnect '1' (normally used for wired-and connections)
	H0D1 = 7  // High drive '0', disconnect '1' (normally used for wired-and connections)
};


// set mode of a gpio
inline void setMode(int pin, Mode mode) {
	auto port = getPort(pin);
	uint32_t c = port->PIN_CNF[pin & 31];
	port->PIN_CNF[pin & 31] = (c & ~GPIO_PIN_CNF_DIR_Msk) | V(GPIO_PIN_CNF_DIR, int(mode));
}


// analog
// ------

inline void configureAnalog(int pin) {
	auto port = getPort(pin);
	uint32_t c = N(GPIO_PIN_CNF_DIR, Input)
		| N(GPIO_PIN_CNF_INPUT, Disconnect)
		| N(GPIO_PIN_CNF_PULL, Disabled);
	port->PIN_CNF[pin & 31] = c;
}


// input
// -----

// configure input
inline void configureInput(int pin, Pull pull = Pull::DISABLED) {
	auto port = getPort(pin);
	port->PIN_CNF[pin & 31] =
		N(GPIO_PIN_CNF_INPUT, Connect)
			| V(GPIO_PIN_CNF_PULL, int(pull));
}

// get input value
inline bool getInput(int pin) {
	auto port = getPort(pin);
	return (port->IN & (1 << (pin & 31))) != 0;
}


// output
// ------

// configure output
inline void configureOutput(int pin, Pull pull = Pull::DISABLED, Drive drive = Drive::S0S1) {
	auto port = getPort(pin);
	port->PIN_CNF[pin & 31] = N(GPIO_PIN_CNF_DIR, Output)
		| N(GPIO_PIN_CNF_INPUT, Disconnect)
		| V(GPIO_PIN_CNF_PULL, int(pull))
		| V(GPIO_PIN_CNF_DRIVE, int(drive));
}

// set output value
inline void setOutput(int pin, bool value) {
	auto port = getPort(pin);
	if (value)
		port->OUTSET = 1 << (pin & 31);
	else
		port->OUTCLR = 1 << (pin & 31);
}

// toggle output value
inline void toggleOutput(int pin) {
	auto port = getPort(pin);
	uint32_t bit = 1 << (pin & 31);
	bool value = (port->OUT & bit) == 0;
	if (value)
		port->OUTSET = bit;
	else
		port->OUTCLR = bit;
}


// in/out
// ------

// configure as input and output
inline void configureInOut(int pin, Mode mode, Pull pull = Pull::DISABLED, Drive drive = Drive::S0S1) {
	auto port = getPort(pin);
	port->PIN_CNF[pin & 31] = V(GPIO_PIN_CNF_DIR, int(mode))
		| N(GPIO_PIN_CNF_INPUT, Connect)
		| V(GPIO_PIN_CNF_PULL, int(pull))
		| V(GPIO_PIN_CNF_DRIVE, int(drive));
}


} // namespace gpio
} // namespace coco
