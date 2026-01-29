#pragma once

// do not include directly, use #include <coco/platform/rcc.hpp>

#include "platform.hpp"
#include <stddef.h>


namespace coco {

/// @brief RCC (reset and clock control) helpers.
/// Registers for reset, clock in run mode.
namespace rcc {

/// Info structure for reset and clock control (RCC).
/// @tparam R offset of RCC reset register relative to enable register (e.g offsetof(RCC_TypeDef, APB1RSTR) - offsetof(RCC_TypeDef, APB1ENR))
template <size_t R>
struct Info {
    // clock enable register
    __IO uint32_t *enableReg;

    // flag for a peripheral
    uint32_t flag;


    /// @brief Set reset signal of a core.
    ///
    void setReset() const {
        auto reg = (__IO uint32_t *)(intptr_t(this->enableReg) + R);
        *reg  = *reg | this->flag;
    }

    /// @brief Clear reset signal of a core.
    ///
    void clearReset() const {
        auto reg = (__IO uint32_t *)(intptr_t(this->enableReg) + R);
        *reg  = *reg & ~this->flag;
    }

    /// @brief Reset a core by temporarily setting the reset flag.
    /// Note that this method clears all reset flags in the register
    void reset() const {
        auto reg = (__IO uint32_t *)(intptr_t(this->enableReg) + R);
        *reg = this->flag;
        *reg = 0;
    }

    /// @brief Enable clock of a core.
    ///
    void enableClock() const {
        auto reg = this->enableReg;
        *reg  = *reg | this->flag;

        // Two cycles wait time until peripherals can be accessed, see STM32G4 reference manual section 7.2.17
        __NOP();
        __NOP();
    }

    /// @brief Disable system clock of a core.
    ///
    void disableClock() const {
        auto reg = this->enableReg;
        *reg  = *reg & ~this->flag;
    }
};

/// @brief Info structure for reset and clock control (RCC) where the RCC register offset is known at compile time.
/// @tparam R offset of RCC reset register (e.g offsetof(RCC_TypeDef, AHB2RSTR))
/// @tparam E offset of RCC enable register (e.g offsetof(RCC_TypeDef, AHB2ENR))
template <int R, int E>
struct Info2 {
    // flag for a peripheral
	uint32_t flag;


    /// @brief Set reset signal of a core.
    ///
    void setReset() const {
        auto reg = (__IO uint32_t *)(RCC_BASE + R);
        *reg  = *reg | this->flag;
    }

    /// @brief Clear reset signal of a core.
    ///
    void clearReset() const {
        auto reg = (__IO uint32_t *)(RCC_BASE + R);
        *reg  = *reg & ~this->flag;
    }

    /// @brief Reset a core by temporarily setting the reset flag.
    /// Note that this method clears all reset flags in the register
    void reset() const {
        auto reg = (__IO uint32_t *)(RCC_BASE + R);
        *reg = this->flag;
        *reg = 0;
    }

    /// @brief Enable clock of a core.
    ///
	void enableClock() const {
		auto reg = (__IO uint32_t *)(RCC_BASE + E);
		*reg = *reg | this->flag;

        // Two cycles wait time until peripherals can be accessed, see STM32G4 reference manual section 7.2.17
        __NOP();
        __NOP();
    }

    /// @brief Disable system clock of a core.
    ///
    void disableClock() const {
		auto reg = (__IO uint32_t *)(RCC_BASE + E);
        *reg  = *reg & ~this->flag;
    }
};

} // namespace rcc
} // namespace coco
