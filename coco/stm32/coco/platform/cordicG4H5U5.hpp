#pragma once

// do not include directly, use #include <coco/platform/cordic.hpp>

#include "platform.hpp"


/*
    Defines:
    HAVE_CORDIC           Cordic supported
*/


#ifdef CORDIC
#define HAVE_CORDIC


namespace coco {

/// @brief CORDIC helpers.
/// G4 Refernece manual: https://www.st.com/resource/en/reference_manual/rm0440-stm32g4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
///   Section 17
/// H5 https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series/documentation.html Section 17
namespace cordic {

enum class Function {
    COSINE = 0, // angle, length -> cos, sin
    SINE = CORDIC_CSR_FUNC_0, // angle, length -> sin, cos
    PHASE = CORDIC_CSR_FUNC_1, // x, y -> atan2(y, x), length
    MODULUS = CORDIC_CSR_FUNC_1 | CORDIC_CSR_FUNC_0, // x, y -> length, atan2(y,x)
    ARCTANGENT = CORDIC_CSR_FUNC_2, // x -> atan(x)
    HYPERBOLIC_COSINE = CORDIC_CSR_FUNC_2 | CORDIC_CSR_FUNC_0 | CORDIC_CSR_SCALE_0, // x -> cosh(x)
    HYPERBOLIC_SINE = CORDIC_CSR_FUNC_2 | CORDIC_CSR_FUNC_1 | CORDIC_CSR_SCALE_0, // x -> sinh(x)
    HYPERBOLIC_ARCTANGENT = CORDIC_CSR_FUNC_2 | CORDIC_CSR_FUNC_1 | CORDIC_CSR_FUNC_0 | CORDIC_CSR_SCALE_0, // x -> atanh(x)
    NATURAL_LOGARITHM = CORDIC_CSR_FUNC_3, // x -> ln(x)
    SQUARE_ROOT = CORDIC_CSR_FUNC_3 | CORDIC_CSR_FUNC_0, // x -> sqrt(x)
};

enum class Format {
    // two 16 bit arguments, two 16 bit results
    IN_2x16_OUT_2x16 = CORDIC_CSR_ARGSIZE | CORDIC_CSR_RESSIZE,

    // one 32 bit arguments, one 32 bit result
    IN_32_OUT_32 = 0,

    // one 32 bit arguments, two 32 bit results
    IN_32_OUT_2X32 = CORDIC_CSR_NRES,

    // two 32 bit arguments, one 32 bit result
    IN_2X32_OUT_32 = CORDIC_CSR_NARGS,

    // two 32 bit arguments, two 32 bit results
    IN_2X32_OUT_2X32 = CORDIC_CSR_NARGS | CORDIC_CSR_NRES,
};


struct Instance {
    CORDIC_TypeDef *cordic;


    /// @brief Configure the CORDIC.
    /// @param function The function to calculate
    /// @param format Input and output precision and number of arguments
    /// @param precison Precision (iterations / 4)
    /// @param scale Scale factor for ARCTANGENT, NATURAL_LOGARITHM, SQUARE_ROOT
    /// @return *this
    auto &configure(Function function, Format format, int precison, int scale = 0) {
        cordic->CSR = uint32_t(function)
            | uint32_t(format)
            | (precison << CORDIC_CSR_PRECISION_Pos)
            | (scale << CORDIC_CSR_SCALE_Pos);

        return *this;
    }

    /// @brief Write arguments, 2x16 bit
    /// @param arg1 first argument
    /// @param arg2 second argument
    void i16x2(int arg1, int arg2) {
        cordic->WDATA = (arg1 & 0xffff) | (arg2 << 16);
    }

    /// @brief Read result, 2x16 bit signed
    /// Function::MODULUS: length, angle
    /// @return result
    std::pair<int, int> i16x2() {
        int32_t r = cordic->RDATA;
        return {int16_t(r), r >> 16};
    }

    /// @brief Read result, 32 bit
    /// @return result
    uint32_t u32() {
        return cordic->RDATA;
    }
};

} // namespace cordic
} // namespace coco

#endif // CORDIC
