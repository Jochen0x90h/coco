#pragma once

// do not include directly, use #include <coco/platform/cordic.hpp>

#include "platform.hpp"
#include <coco/enum.hpp>


/*
    Defines:
    HAVE_RNG                Random number genrator supported
    HAVE_RNG_ENABLE_CLOCK   Need to call rng::enableClock() to enable the clock of the peripheral
*/

#ifdef RNG
#define HAVE_RNG
#define HAVE_RNG_ENABLE_CLOCK


namespace coco {

/// @brief RNG helpers.
/// G0X0 https://www.st.com/en/microcontrollers-microprocessors/stm32g0-series/documentation.html Not supported
/// G0X1 https://www.st.com/en/microcontrollers-microprocessors/stm32g0-series/documentation.html Section 19
/// G4 https://www.st.com/en/microcontrollers-microprocessors/stm32g4-series/documentation.html Sections 26
/// H503 https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series/documentation.html Section 24
/// H5 https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series/documentation.html Section 32
/// U3 https://www.st.com/en/microcontrollers-microprocessors/stm32u3-series/documentation.html Section 30
namespace rng {

/// @brief Data type that is produced by the random number generator.
///
using Value = uint32_t;

/// @brief Configuration (CR register).
///
enum class Config : uint32_t {
    DEFAULT = 0,

#ifdef RNG_CR_CED
    CLOCK_ERROR_DETECTION = RNG_CR_CED,
#endif
};
COCO_ENUM(Config);

/// @brief Interrupt enable flags (CR register).
///
enum class Interrupt : uint32_t {
    NONE = 0,

    // a new value has been generated
    VALUE_READY = RNG_CR_IE,

    ALL = VALUE_READY
};
COCO_ENUM(Interrupt);

/// @brief Status flags (SR register).
///
enum class Status : uint32_t {
    NONE = 0,

    // a new value has been generated
    VALUE_READY = RNG_SR_DRDY,

    CLOCK_ERROR = RNG_SR_SECS,
    CLOCK_ERROR_INTERRUPT = RNG_SR_CEIS,

    SEED_ERROR = RNG_SR_SECS,
    SEED_ERROR_INTERRUPT = RNG_SR_SEIS,
};
COCO_ENUM(Status);


/// @brief Random number generator (RNG) instance (wrapper for RNG registers)
///
struct Instance {
    RNG_TypeDef *rng;


    RNG_TypeDef *operator ->() const {return rng;}
    operator RNG_TypeDef *() const {return rng;}

    /// @brief Configure the random number generator.
    /// @param config Configuration
    /// @param interrupts Interrupts to enable, all others will be disabled
    /// @return *this
    auto &configure(Config config, Interrupt interrupts = Interrupt::NONE) {
        rng->CR = uint32_t(config)
            | uint32_t(interrupts);
        return *this;
    }

    /// @brief Enable interrupts.
    /// @param interrupts Interrupt flags to set
    /// @return *this
    auto &enable(Interrupt interrupts) {
        rng->CR = rng->CR | uint32_t(interrupts);
        return *this;
    }

    /// @brief Disable interrupts.
    /// @param interrupts Interrupt flags to set
    /// @return *this
    auto &disable(Interrupt interrupts) {
        rng->CR = rng->CR & ~uint32_t(interrupts);
        return *this;
    }

    /// @brief Get the currently enabled Interrupts.
    /// @return Enabled interrupts
    Interrupt interrupts() const {
        return Interrupt(rng->CR) & Interrupt::ALL;
    }

    /// @brief Start the random number generator.
    /// @return *this
    auto &start() {
        rng->CR = rng->CR | RNG_CR_RNGEN;
        return *this;
    }

    /// @brief Stop the random number generator.
    /// @return *this
    auto &stop() {
        rng->CR = rng->CR & ~RNG_CR_RNGEN;
        return *this;
    }

    /// @brief Read status.
    /// @return Status
    Status status() const {
        return Status(rng->SR);
    }

    /// @brief Clear status.
    /// Note that some status bits are read-only.
    /// @return *this
    auto &clear(Status status) {
        rng->SR = uint32_t(status);
        return *this;
    }

    /// @brief Get 32 bit random value and clear value ready flag.
    /// @return Random value
    Value value() {
        Value value = rng->DR;
        rng->SR = RNG_SR_DRDY;
        return value;
    }

    /// @brief Wait for a new 32 bit random value and clear value ready flag.
    /// @return Random value
    Value waitValue() {
        while ((rng->SR & RNG_SR_DRDY) == 0);
        return value();
    }
};

} // namespace rng
} // namespace coco

#endif // RNG
