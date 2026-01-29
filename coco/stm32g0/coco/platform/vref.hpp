#pragma once

#include "platform.hpp"


namespace coco {

/// @brief VREFBUF helpers
///
namespace vref {

enum class Config {
    // 2.048V
    INTERNAL_2V048 = 0 << VREFBUF_CSR_VRS_Pos,

    // 2.5V
    INTERNAL_2V5 = 1 << VREFBUF_CSR_VRS_Pos,
};

/// @brief Enable the voltage reference.
///
inline void enable(Config config) {
    VREFBUF->CSR = VREFBUF_CSR_ENVR | int(config);

    // wait until voltage reference is ready
    while ((VREFBUF->CSR & VREFBUF_CSR_VRR) == 0);
}

/// @brief Disable the voltage reference.
///
inline void disable() {
    VREFBUF->CSR = VREFBUF_CSR_HIZ;
}

} // namespace vref
} // namespace coco
