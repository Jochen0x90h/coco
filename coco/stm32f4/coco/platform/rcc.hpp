#pragma once

#include <stddef.h>


namespace coco {

/// @brief Reset and clock control helpers.
/// Registers for reset, clock in run mode, clock in low power mode.
/// Used by F4
namespace rcc {

/// @brief Info structure for reset and clock control (RCC).
///
struct Info {
    // clock enable register
    __IO uint32_t *enableReg;

    // flag for a peripheral
    uint32_t flag;


    /**
     * Set reset signal of a core.
     */
    void setReset() const {
        auto reg = (__IO uint32_t *)(intptr_t(enableReg) + resetOffset);
        *reg  = *reg | flag;
    }

    /**
     * Clear reset signal of a core.
     */
    void clearReset() const {
        auto reg = (__IO uint32_t *)(intptr_t(enableReg) + resetOffset);
        *reg  = *reg & ~flag;
    }

    /**
     * Reset a core by temporarily setting the reset flag.
     * Note that this method clears all reset flags in the register
     */
    void reset() const {
        auto reg = (__IO uint32_t *)(intptr_t(enableReg) + resetOffset);
        *reg = flag;
        *reg = 0;
    }

    /**
     * Enable clock of a core.
     */
    void enableClock() const {
        auto reg = enableReg;
        *reg  = *reg | flag;

        // Two cycles wait time until peripherals can be accessed, see STM32G4 reference manual section 7.2.17
        __NOP();
        __NOP();
    }

    /**
     * Disable system clock of a core.
     */
    void disableClock() const {
        auto reg = enableReg;
        *reg  = *reg & ~flag;
    }

    /**
     * Enable clock of a core in sleep and stop mode.
     */
    void enableClockLowPower() const {
        auto reg = (__IO uint32_t *)(intptr_t(enableReg) + clockLowPowerOffset);
        *reg  = *reg | flag;
    }

    /**
     * Disable clock of a core in sleep and stop mode.
     */
    void disableClockLowPower() const {
        auto reg = (__IO uint32_t *)(intptr_t(enableReg) + clockLowPowerOffset);
        *reg  = *reg & ~flag;
    }


    static constexpr int resetOffset = (offsetof(RCC_TypeDef, AHB1RSTR) - offsetof(RCC_TypeDef, AHB1ENR));
    static constexpr int clockLowPowerOffset = (offsetof(RCC_TypeDef, AHB1LPENR) - offsetof(RCC_TypeDef, AHB1ENR));
};

/// @brief Info structure for reset and clock control (RCC) where the RCC register offset is known at compile time.
/// @tparam R offset of RCC register (e.g offsetof(RCC_TypeDef, AHB2ENR))
template <int R>
struct Info2 {
    // flag for a peripheral
	uint32_t flag;


    /**
     * Set reset signal of a core.
     */
    void setReset() const {
        auto reg = (__IO uint32_t *)(RCC_BASE + R + resetOffset);
        *reg  = *reg | flag;
    }

    /**
     * Clear reset signal of a core.
     */
    void clearReset() const {
        auto reg = (__IO uint32_t *)(RCC_BASE + R + resetOffset);
        *reg  = *reg & ~flag;
    }

    /**
     * Reset a core by temporarily setting the reset flag.
     * Note that this method clears all reset flags in the register
     */
    void reset() const {
        auto reg = (__IO uint32_t *)(RCC_BASE + R + resetOffset);
        *reg = flag;
        *reg = 0;
    }

    /**
     * Enable clock of a core.
     */
	void enableClock() const {
		auto reg = (__IO uint32_t *)(RCC_BASE + R);
		*reg = *reg | flag;

        // Two cycles wait time until peripherals can be accessed, see STM32G4 reference manual section 7.2.17
        __NOP();
        __NOP();
    }

    /**
     * Disable system clock of a core.
     */
    void disableClock() const {
		auto reg = (__IO uint32_t *)(RCC_BASE + R);
        *reg  = *reg & ~flag;
    }

    /**
     * Enable clock of a core in sleep and stop mode.
     */
    void enableClockLowPower() const {
        auto reg = (__IO uint32_t *)(RCC_BASE + R + clockLowPowerOffset);
        *reg  = *reg | flag;
    }

    /**
     * Disable clock of a core in sleep and stop mode.
     */
    void disableClockLowPower() const {
        auto reg = (__IO uint32_t *)(RCC_BASE + R + clockLowPowerOffset);
        *reg  = *reg & ~flag;
    }


    static constexpr int resetOffset = (offsetof(RCC_TypeDef, AHB1RSTR) - offsetof(RCC_TypeDef, AHB1ENR));
    static constexpr int clockLowPowerOffset = (offsetof(RCC_TypeDef, AHB1LPENR) - offsetof(RCC_TypeDef, AHB1ENR));
};

/// @brief Disable clocks and reset all peripherals except for PWR.
/// Useful for bootloaders before jumping to the application.
__STATIC_FORCEINLINE void resetAll() {
    // disable clocks
    RCC->AHB1ENR = 0;
    RCC->AHB2ENR = 0;
    RCC->AHB3ENR = 0;
    RCC->APB1ENR = RCC_APB1ENR_PWREN;
    RCC->APB2ENR = 0;

    // reset
    RCC->AHB1RSTR = 0xFFFFFFFF;
    RCC->AHB2RSTR = 0xFFFFFFFF;
    RCC->AHB3RSTR = 0xFFFFFFFF;
    RCC->APB1RSTR = ~RCC_APB1RSTR_PWRRST;
    RCC->APB2RSTR = 0xFFFFFFFF;
    RCC->AHB1RSTR = 0;
    RCC->AHB2RSTR = 0;
    RCC->AHB3RSTR = 0;
    RCC->APB1RSTR = 0;
    RCC->APB2RSTR = 0;
}

using AdcInfo = Info2<offsetof(RCC_TypeDef, APB2ENR)>;
using DacInfo = Info2<offsetof(RCC_TypeDef, APB1ENR)>;
using I2cInfo = Info2<offsetof(RCC_TypeDef, APB1ENR)>;
using QuadSpiInfo = Info2<offsetof(RCC_TypeDef, AHB3ENR)>;
using SpiInfo = Info;
using UsartInfo = Info;

} // namespace rcc
} // namespace coco
