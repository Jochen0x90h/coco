#pragma once

#include <cstddef>

// do not include directly

namespace coco {

/**
 * Reset and clock control helpers
 * Used by F0, F3
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


#ifdef RCC_AHBENR_DMA1EN
    static constexpr int resetOffset = (offsetof(RCC_TypeDef, AHBRSTR) - offsetof(RCC_TypeDef, AHBENR));
#endif
#ifdef RCC_AHB1ENR_DMA1EN
    static constexpr int resetOffset = (offsetof(RCC_TypeDef, AHB1RSTR) - offsetof(RCC_TypeDef, AHB1ENR));
#endif
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


#ifdef RCC_AHBENR_DMA1EN
    static constexpr int resetOffset = (offsetof(RCC_TypeDef, AHBRSTR) - offsetof(RCC_TypeDef, AHBENR));
#endif
#ifdef RCC_AHB1ENR_DMA1EN
    static constexpr int resetOffset = (offsetof(RCC_TypeDef, AHB1RSTR) - offsetof(RCC_TypeDef, AHB1ENR));
#endif
};

} // namespace rcc
} // namespace coco
