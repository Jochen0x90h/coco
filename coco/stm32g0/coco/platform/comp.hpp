#pragma once

#include "platform.hpp"
#include <coco/enum.hpp>


namespace coco {

/// @brief COMP helpers
///
namespace comp {

enum class Config : uint32_t {
    // output
    OUT_INVERT = COMP_CSR_POLARITY,

    // plus input
    COMP1_INP_PC5 = 0,
    COMP1_INP_PB2 = COMP_CSR_INPSEL_0,
    COMP1_INP_PA1 = COMP_CSR_INPSEL_1,

    COMP2_INP_PB4 = 0,
    COMP2_INP_PB6 = COMP_CSR_INPSEL_0,
    COMP2_INP_PA3 = COMP_CSR_INPSEL_1,

    COMP3_INP_PB0 = 0,
    COMP3_INP_PC1 = COMP_CSR_INPSEL_0,
    COMP3_INP_PE7 = COMP_CSR_INPSEL_1,

    // minus input
    INM_1_4_VREFINT = 0,
    INM_1_2_VREFINT = COMP_CSR_INMSEL_0,
    INM_3_4_VREFINT = COMP_CSR_INMSEL_1,
    INM_VREFINT = COMP_CSR_INMSEL_1 | COMP_CSR_INMSEL_0,
    INM_DAC1_CH1 = COMP_CSR_INMSEL_2,
    INM_DAC1_CH2 = COMP_CSR_INMSEL_2 | COMP_CSR_INMSEL_0,

    COMP1_INM_PB1 = COMP_CSR_INMSEL_2 | COMP_CSR_INMSEL_1,
    COMP1_INM_PC4 = COMP_CSR_INMSEL_2 | COMP_CSR_INMSEL_1 | COMP_CSR_INMSEL_0,
    COMP1_INM_PA0 = COMP_CSR_INMSEL_3,

    COMP2_INM_PB3 = COMP_CSR_INMSEL_2 | COMP_CSR_INMSEL_1,
    COMP2_INM_PB7 = COMP_CSR_INMSEL_2 | COMP_CSR_INMSEL_1 | COMP_CSR_INMSEL_0,
    COMP2_INM_PA2 = COMP_CSR_INMSEL_3,

    COMP3_INM_PB2 = COMP_CSR_INMSEL_2 | COMP_CSR_INMSEL_1,
    COMP3_INM_PC0 = COMP_CSR_INMSEL_2 | COMP_CSR_INMSEL_1 | COMP_CSR_INMSEL_0,
    COMP3_INM_PE8 = COMP_CSR_INMSEL_3,

    // hysteresis
    HYST_NONE = 0, // default
    HYST_LOW = COMP_CSR_HYST_0,
    HYST_MEDIUM = COMP_CSR_HYST_1,
    HYST_HIGH = COMP_CSR_HYST_1 | COMP_CSR_HYST_0,

    // lock configuration register (can only be set, reset by system reset)
    LOCK = COMP_CSR_LOCK
};
COCO_ENUM(Config)

/// @param Configure a comparator.
/// Make sure SYSCFG clock enable bit is set (RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_SYSCFGEN)
inline void enable(COMP_TypeDef *comp, Config config) {
    comp->CSR = COMP_CSR_EN | int(config);
}

/// @param Disable an op-amp
///
inline void disable(COMP_TypeDef *opamp) {
    opamp->CSR = 0;
}

} // namespace comp
} // namespace coco
