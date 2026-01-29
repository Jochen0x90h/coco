#pragma once

#include "platform.hpp"
#include <coco/enum.hpp>


namespace coco {

/// @brief OPAMP helpers
/// Refernece manual: https://www.st.com/resource/en/reference_manual/rm0440-stm32g4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
///   Section 25
namespace opamp {

enum class Config : uint32_t {
    // output
    OUT_PIN = 0,
    OUT_ADC = OPAMP_CSR_OPAMPINTEN,

    // plus input
    INP_PIN0 = 0,
    INP_PIN1 = 1 << OPAMP_CSR_VPSEL_Pos,
    INP_PIN2 = 2 << OPAMP_CSR_VPSEL_Pos,
    OPAMP1_INP_DAC3_CH1 = 3 << OPAMP_CSR_VPSEL_Pos,
    OPAMP2_INP_PIN3 = 3 << OPAMP_CSR_VPSEL_Pos,
    OPAMP3_INP_DAC3_CH2 = 3 << OPAMP_CSR_VPSEL_Pos,
    OPAMP4_INP_DAC4_CH1 = 3 << OPAMP_CSR_VPSEL_Pos,
    OPAMP5_INP_DAC4_CH2 = 3 << OPAMP_CSR_VPSEL_Pos,
    OPAMP6_INP_DAC3_CH1 = 3 << OPAMP_CSR_VPSEL_Pos,

    // minus input
    INM_PIN0 = 0,
    INM_PIN1 = 1 << OPAMP_CSR_VMSEL_Pos,
    INM_PGA = 2 << OPAMP_CSR_VMSEL_Pos, // programmabel gain amplifier
    INM_FOLLOWER = 3 << OPAMP_CSR_VMSEL_Pos, // follower mode, connected to output

    // lock configuration register (can only be set, reset by system reset)
    LOCK = OPAMP_CSR_LOCK
};
COCO_ENUM(Config)

/// @param Configure an operational amplifier.
/// Make sure SYSCFG clock enable bit is set (RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_SYSCFGEN)
inline void enable(OPAMP_TypeDef *opamp, Config config) {
    opamp->CSR = OPAMP_CSR_OPAMPxEN | int(config);
}

/// @param Disable an op-amp
///
inline void disable(OPAMP_TypeDef *opamp) {
    opamp->CSR = 0;
}

} // namespace comp
} // namespace coco
