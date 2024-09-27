#pragma once

#include "platform.hpp"


namespace coco {

/**
 * Refernece manual:
 *   g4:
 *     https://www.st.com/resource/en/reference_manual/rm0440-stm32g4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 *       VREFBUF: Section 23
 */
namespace vref {

enum class Config {
    // 2.048V
    INTERNAL_2V048 = 0b00 << VREFBUF_CSR_VRS_Pos,

    // 2.5V
    INTERNAL_2V5 = 0b01 << VREFBUF_CSR_VRS_Pos,

    // 2.9V
    INTERNAL_2V9 = 0b10 << VREFBUF_CSR_VRS_Pos,
};

/**
 * Configure the voltage reference.
 */
void configure(Config config) {
    VREFBUF->CSR = VREFBUF_CSR_ENVR | int(config);

    // wait until voltage reference is ready
    while ((VREFBUF->CSR & VREFBUF_CSR_VRR) == 0);
}

/**
 * Disable the voltage reference
 */
void disable() {
    VREFBUF->CSR = VREFBUF_CSR_HIZ;
}

} // namespace vref
} // namespace coco
