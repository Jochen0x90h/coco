#pragma once

// do not include directly, use #include <coco/platform/cordic.hpp>

#include "platform.hpp"
#include <coco/enum.hpp>


/*
    Defines:
    HAVE_RNG                Random number generator supported (is always supported)
    HAVE_RNG_ENABLE_CLOCK   Need to call rng::enableClock() to enable the clock of the peripheral (not needed on nRF52)
*/

#define HAVE_RNG


namespace coco {

/// @brief RNG helpers
/// Reference manual:
///   https://docs.nordicsemi.com/bundle/ps_nrf52840/page/rng.html
namespace rng {

/// @brief Data type that is produced by the random number generator.
///
using Value = uint8_t;

/// @brief Configuration.
///
enum class Config : uint32_t {
    DEFAULT = 0,
};

/// @brief Interrupt enable flags (CR register).
///
enum class Interrupt : uint32_t {
    NONE = 0,

    // a new value has been generated
    VALUE_READY = N(RNG_INTENSET_VALRDY, Set),

    ALL = VALUE_READY
};
COCO_ENUM(Interrupt);


/// @brief Random number generator (RNG) instance (wrapper for RNG registers)
///
struct Instance {
    NRF_RNG_Type *rng;


    NRF_RNG_Type *operator ->() const {return rng;}
    operator NRF_RNG_Type *() const {return rng;}

    /// @brief Configure the random number generator.
    /// @param config Configuration
    /// @param interrupts Interrupts to enable, all others will be disabled
    /// @return *this
    auto &configure(Config config, Interrupt interrupts = Interrupt::NONE) {
        rng->INTENSET = uint32_t(interrupts);
        rng->INTENCLR = ~uint32_t(interrupts);
        return *this;
    }

    /// @brief Enable interrupts.
    /// @param interrupts Interrupt flags to set
    /// @return *this
    auto &enable(Interrupt interrupts) {
        rng->INTENSET = uint32_t(interrupts);
        return *this;
    }

    /// @brief Disable interrupts.
    /// @param interrupts Interrupt flags to set
    /// @return *this
    auto &disable(Interrupt interrupts) {
        rng->INTENCLR = uint32_t(interrupts);
        return *this;
    }

    /// @brief Get the currently enabled Interrupts.
    /// @return Enabled interrupts
    Interrupt interrupts() const {
        return Interrupt(rng->INTENSET);
    }

    /// @brief Start generating random numbers.
    /// @return *this
    auto &start() {
        rng->TASKS_START = TRIGGER;
        return *this;
    }

    /// @brief Stop generating random numbers.
    /// @return *this
    auto &stop() {
        rng->TASKS_STOP = TRIGGER;
        return *this;
    }

    /// @brief Get 8 bit random value and clear value ready flag.
    /// @return Random value
    Value value() {
        Value value = rng->VALUE;
        rng->EVENTS_VALRDY = 0;
        return value;
    }

    /// @brief Wait for a new 8 bit random value and clear value ready flag.
    /// @return Random value
    Value waitValue() {
        while (rng->EVENTS_VALRDY == 0);
        return value();
    }
};


// interrupt request index
constexpr int irq = RNG_IRQn;


/// @brief Get the instance
/// @return Instance (wrapper for registers)
inline Instance instance() {
    return {NRF_RNG};
}

} // namespace rng
} // namespace coco
