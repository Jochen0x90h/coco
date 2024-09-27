#pragma once

#include <stddef.h>

// do not include directly

namespace coco {

/**
 * Reset and clock control helpers
 * Has a register for clock in low power mode
 * Used by F4
 */
namespace rcc {

/**
 * Info structure for reset and clock control (RCC).
 */
struct Info {
    // clock enable register
    __IO uint32_t *enableReg;

    // flag for a peripheral
    uint32_t flag;


    /**
     * Set reset signal of a core.
     */
    void setReset() const {
        auto reg = (__IO uint32_t *)(intptr_t(this->enableReg) + resetOffset);
        *reg  = *reg | this->flag;
    }

    /**
     * Clear reset signal of a core.
     */
    void clearReset() const {
        auto reg = (__IO uint32_t *)(intptr_t(this->enableReg) + resetOffset);
        *reg  = *reg & ~this->flag;
    }

    /**
     * Reset a core by temporarily setting the reset flag.
     * Note that this method clears all reset flags in the register
     */
    void reset() const {
        auto reg = (__IO uint32_t *)(intptr_t(this->enableReg) + resetOffset);
        *reg = this->flag;
        *reg = 0;
    }

    /**
     * Enable clock of a core.
     */
    void enableClock() const {
        auto reg = this->enableReg;
        *reg  = *reg | this->flag;

        // Two cycles wait time until peripherals can be accessed, see STM32G4 reference manual section 7.2.17
        __NOP();
        __NOP();
    }

    /**
     * Disable system clock of a core.
     */
    void disableClock() const {
        auto reg = this->enableReg;
        *reg  = *reg & ~this->flag;
    }

    /**
     * Enable clock of a core in sleep and stop mode.
     */
    void enableClockLowPower() const {
        auto reg = (__IO uint32_t *)(intptr_t(this->enableReg) + clockLowPowerOffset);
        *reg  = *reg | this->flag;
    }

    /**
     * Disable clock of a core in sleep and stop mode.
     */
    void disableClockLowPower() const {
        auto reg = (__IO uint32_t *)(intptr_t(this->enableReg) + clockLowPowerOffset);
        *reg  = *reg & ~this->flag;
    }


    static constexpr int resetOffset = (offsetof(RCC_TypeDef, AHB1RSTR) - offsetof(RCC_TypeDef, AHB1ENR));
    static constexpr int clockLowPowerOffset = (offsetof(RCC_TypeDef, AHB1LPENR) - offsetof(RCC_TypeDef, AHB1ENR));
};

/**
 * Info structure for reset and clock control (RCC) where the RCC register offset is known at compile time.
 * @tparam R offset of RCC register (e.g offsetof(RCC_TypeDef, AHB2ENR))
 */
template <int R>
struct Info2 {
    // flag for a peripheral
	uint32_t flag;


    /**
     * Set reset signal of a core.
     */
    void setReset() const {
        auto reg = (__IO uint32_t *)(RCC_BASE + R + resetOffset);
        *reg  = *reg | this->flag;
    }

    /**
     * Clear reset signal of a core.
     */
    void clearReset() const {
        auto reg = (__IO uint32_t *)(RCC_BASE + R + resetOffset);
        *reg  = *reg & ~this->flag;
    }

    /**
     * Reset a core by temporarily setting the reset flag.
     * Note that this method clears all reset flags in the register
     */
    void reset() const {
        auto reg = (__IO uint32_t *)(RCC_BASE + R + resetOffset);
        *reg = this->flag;
        *reg = 0;
    }

    /**
     * Enable clock of a core.
     */
	void enableClock() const {
		auto reg = (__IO uint32_t *)(RCC_BASE + R);
		*reg = *reg | this->flag;

        // Two cycles wait time until peripherals can be accessed, see STM32G4 reference manual section 7.2.17
        __NOP();
        __NOP();
    }

    /**
     * Disable system clock of a core.
     */
    void disableClock() const {
		auto reg = (__IO uint32_t *)(RCC_BASE + R);
        *reg  = *reg & ~this->flag;
    }

    /**
     * Enable clock of a core in sleep and stop mode.
     */
    void enableClockLowPower() const {
        auto reg = (__IO uint32_t *)(RCC_BASE + R + clockLowPowerOffset);
        *reg  = *reg | this->flag;
    }

    /**
     * Disable clock of a core in sleep and stop mode.
     */
    void disableClockLowPower() const {
        auto reg = (__IO uint32_t *)(RCC_BASE + R + clockLowPowerOffset);
        *reg  = *reg & ~this->flag;
    }


    static constexpr int resetOffset = (offsetof(RCC_TypeDef, AHB1RSTR) - offsetof(RCC_TypeDef, AHB1ENR));
    static constexpr int clockLowPowerOffset = (offsetof(RCC_TypeDef, AHB1LPENR) - offsetof(RCC_TypeDef, AHB1ENR));
};

} // namespace rcc
} // namespace coco
