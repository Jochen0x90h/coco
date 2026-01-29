#pragma once

#include "dmaH5U3U5.hpp"


namespace coco {
namespace dma {

using FullInfo = Info<Feature::CIRCULAR | Feature::BLOCK_2D>;
using BasicInfo = Info<>;
using FullDualInfo = DualInfo<Feature::CIRCULAR | Feature::BLOCK_2D>;
using BasicDualInfo = DualInfo<>;

// single channel DMA infos
static const BasicInfo DMA1_CH0_INFO{GPDMA1, RCC_AHB1ENR1_GPDMA1EN, 0, GPDMA1_Channel0_IRQn};
static const BasicInfo DMA1_CH1_INFO{GPDMA1, RCC_AHB1ENR1_GPDMA1EN, 1, GPDMA1_Channel1_IRQn};
static const BasicInfo DMA1_CH2_INFO{GPDMA1, RCC_AHB1ENR1_GPDMA1EN, 2, GPDMA1_Channel2_IRQn};
static const BasicInfo DMA1_CH3_INFO{GPDMA1, RCC_AHB1ENR1_GPDMA1EN, 3, GPDMA1_Channel3_IRQn};
static const BasicInfo DMA1_CH4_INFO{GPDMA1, RCC_AHB1ENR1_GPDMA1EN, 4, GPDMA1_Channel4_IRQn};
static const BasicInfo DMA1_CH5_INFO{GPDMA1, RCC_AHB1ENR1_GPDMA1EN, 5, GPDMA1_Channel5_IRQn};
static const BasicInfo DMA1_CH6_INFO{GPDMA1, RCC_AHB1ENR1_GPDMA1EN, 6, GPDMA1_Channel6_IRQn};
static const BasicInfo DMA1_CH7_INFO{GPDMA1, RCC_AHB1ENR1_GPDMA1EN, 7, GPDMA1_Channel7_IRQn};
static const BasicInfo DMA1_CH8_INFO{GPDMA1, RCC_AHB1ENR1_GPDMA1EN, 8, GPDMA1_Channel8_IRQn};
static const BasicInfo DMA1_CH9_INFO{GPDMA1, RCC_AHB1ENR1_GPDMA1EN, 9, GPDMA1_Channel9_IRQn};
static const FullInfo DMA1_CH10_INFO{GPDMA1, RCC_AHB1ENR1_GPDMA1EN, 10, GPDMA1_Channel10_IRQn};
static const FullInfo DMA1_CH11_INFO{GPDMA1, RCC_AHB1ENR1_GPDMA1EN, 11, GPDMA1_Channel11_IRQn};

// dual channel DMA infos
static const BasicDualInfo DMA1_CH0_CH1_INFO{GPDMA1, RCC_AHB1ENR1_GPDMA1EN, 0, 1, GPDMA1_Channel0_IRQn, GPDMA1_Channel1_IRQn};
static const BasicDualInfo DMA1_CH1_CH2_INFO{GPDMA1, RCC_AHB1ENR1_GPDMA1EN, 1, 2, GPDMA1_Channel1_IRQn, GPDMA1_Channel2_IRQn};
static const BasicDualInfo DMA1_CH2_CH3_INFO{GPDMA1, RCC_AHB1ENR1_GPDMA1EN, 2, 3, GPDMA1_Channel2_IRQn, GPDMA1_Channel3_IRQn};
static const BasicDualInfo DMA1_CH3_CH4_INFO{GPDMA1, RCC_AHB1ENR1_GPDMA1EN, 3, 4, GPDMA1_Channel3_IRQn, GPDMA1_Channel4_IRQn};
static const BasicDualInfo DMA1_CH4_CH5_INFO{GPDMA1, RCC_AHB1ENR1_GPDMA1EN, 4, 5, GPDMA1_Channel4_IRQn, GPDMA1_Channel5_IRQn};
static const BasicDualInfo DMA1_CH5_CH6_INFO{GPDMA1, RCC_AHB1ENR1_GPDMA1EN, 5, 6, GPDMA1_Channel5_IRQn, GPDMA1_Channel6_IRQn};
static const BasicDualInfo DMA1_CH6_CH7_INFO{GPDMA1, RCC_AHB1ENR1_GPDMA1EN, 6, 7, GPDMA1_Channel6_IRQn, GPDMA1_Channel7_IRQn};
static const BasicDualInfo DMA1_CH8_CH9_INFO{GPDMA1, RCC_AHB1ENR1_GPDMA1EN, 8, 9, GPDMA1_Channel8_IRQn, GPDMA1_Channel9_IRQn};
static const FullDualInfo DMA1_CH10_CH11_INFO{GPDMA1, RCC_AHB1ENR1_GPDMA1EN, 10, 11, GPDMA1_Channel10_IRQn, GPDMA1_Channel11_IRQn};

} // namespace dma
} // namespace coco
