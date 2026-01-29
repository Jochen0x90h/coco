#pragma once

#include "canG0G4H5U3U5.hpp"


namespace coco {
namespace can {

static const Info FDCAN1_INFO{FDCAN1, {RCC_APB1HENR_FDCANEN}, FDCAN1_IT0_IRQn};
#ifdef FDCAN2
static const Info FDCAN2_INFO{FDCAN2, {RCC_APB1HENR_FDCANEN}, FDCAN2_IT0_IRQn};
#endif

} // namespace can
} // namespace coco
