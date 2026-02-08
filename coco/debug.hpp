#pragma once

#include "String.hpp"
#include "StringConcept.hpp"
#include "Time.hpp"
#include <cstdint>


/**
    Simple debug output on up to three LEDs and blocking sleep function.
    Only header file, the implementation has to come from a platform or development board
*/
namespace coco {
namespace debug {

/// @brief Initialize debug outputs. Call this from the system init function
///
void init();

/// @brief Set or toggle the debug outputs
/// @param bits pins bit mask for the outputs
/// @param function function for each output: 0: keep/toogle, 1: reset/set
void set(uint32_t bits, uint32_t function = 0xffffffff);

inline void setRed(bool value = true) {set(uint32_t(value), 1);}
inline void clearRed() {set(0, 1);}
inline void toggleRed() {set(1, 0);}

inline void setGreen(bool value = true) {set(uint32_t(value) << 1, 2);}
inline void clearGreen() {set(0, 2);}
inline void toggleGreen() {set(2, 0);}

inline void setBlue(bool value = true) {set(uint32_t(value) << 2, 4);}
inline void clearBlue() {set(0, 4);}
inline void toggleBlue() {set(4, 0);}

enum Color {
    BLACK = 0,

    RED = 1,
    GREEN = 2,
    BLUE = 4,

    YELLOW = 3,
    MAGENTA = 5,
    CYAN = 6,

    WHITE = 7,
};

inline void set(Color color) {
    set(uint32_t(color));
}

class Counter {
public:

    Counter &operator ++() {
        ++this->c;
        set(this->c);
        return *this;
    }

    Counter &operator --() {
        --this->c;
        set(this->c);
        return *this;
    }

    uint32_t c = 0;
};

/// @brief Sleep for the given amount of time in a blocking loop
///
void sleep(Microseconds<> time);

/// @brief Write a message to the debug console (e.g. UART or USB virtual com port of development board)
///
void write(const char *message, int length);
inline void write(const String &message) {write(message.data(), message.size());}

struct Out {
    /// @brief Stream a single character into the debug output.
    ///
    Out &operator <<(char ch) {
        write(&ch, 1);
        return *this;
    }

    /// @brief Stream a string into the debug output.
    ///
    Out &operator <<(const String &str) {
        write(str.data(), str.size());
        return *this;
    }

    /// @brief Stream a string concept into the debug output (C-string, coco::StringBuffer, std::string).
    ///
    template <typename T> requires (StringConcept<T>)
    Out &operator <<(const T &str) {
        write(str);
        return *this;
    }
};

extern Out out;

} // namespace debug
} // namespace coco
