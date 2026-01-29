#pragma once

#include "canG0G4H5U3U5.hpp"


namespace coco {
namespace can {

static const Info FDCAN1_INFO{FDCAN1, {RCC_APB1ENR1_FDCANEN}, FDCAN1_IT0_IRQn};
#ifdef FDCAN2
static const Info FDCAN2_INFO{FDCAN2, {RCC_APB1ENR1_FDCANEN}, FDCAN2_IT0_IRQn};
#endif
#ifdef FDCAN3
static const Info FDCAN3_INFO{FDCAN3, {RCC_APB1ENR1_FDCANEN}, FDCAN3_IT0_IRQn};
#endif

} // namespace can
} // namespace coco
