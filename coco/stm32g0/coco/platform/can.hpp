#pragma once

#include "canG0G4H5U3U5.hpp"


namespace coco {
namespace can {

#ifdef FDCAN1
static const Info FDCAN1_INFO{FDCAN1, {RCC_APBENR1_FDCANEN}, TIM16_FDCAN_IT0_IRQn};
#endif
#ifdef FDCAN2
static const Info FDCAN2_INFO{FDCAN2, {RCC_APBENR1_FDCANEN}, TIM16_FDCAN_IT0_IRQn};
#endif

} // namespace can
} // namespace coco
