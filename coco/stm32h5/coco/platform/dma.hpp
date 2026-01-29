#pragma once

#include "dmaH5U3U5.hpp"


namespace coco {
namespace dma {

using FullInfo = Info<Feature::CIRCULAR | Feature::BLOCK_2D>;
using BasicInfo = Info<>;
using FullDualInfo = DualInfo<Feature::CIRCULAR | Feature::BLOCK_2D>;
using BasicDualInfo = DualInfo<>;

// single channel DMA infos
static const BasicInfo DMA1_CH0_INFO{GPDMA1, RCC_AHB1ENR_GPDMA1EN, 0, GPDMA1_Channel0_IRQn};
static const BasicInfo DMA1_CH1_INFO{GPDMA1, RCC_AHB1ENR_GPDMA1EN, 1, GPDMA1_Channel1_IRQn};
static const BasicInfo DMA1_CH2_INFO{GPDMA1, RCC_AHB1ENR_GPDMA1EN, 2, GPDMA1_Channel2_IRQn};
static const BasicInfo DMA1_CH3_INFO{GPDMA1, RCC_AHB1ENR_GPDMA1EN, 3, GPDMA1_Channel3_IRQn};
static const BasicInfo DMA1_CH4_INFO{GPDMA1, RCC_AHB1ENR_GPDMA1EN, 4, GPDMA1_Channel4_IRQn};
static const BasicInfo DMA1_CH5_INFO{GPDMA1, RCC_AHB1ENR_GPDMA1EN, 5, GPDMA1_Channel5_IRQn};
static const FullInfo DMA1_CH6_INFO{GPDMA1, RCC_AHB1ENR_GPDMA1EN, 6, GPDMA1_Channel6_IRQn};
static const FullInfo DMA1_CH7_INFO{GPDMA1, RCC_AHB1ENR_GPDMA1EN, 7, GPDMA1_Channel7_IRQn};

static const BasicInfo DMA2_CH0_INFO{GPDMA2, RCC_AHB1ENR_GPDMA2EN, 0, GPDMA2_Channel0_IRQn};
static const BasicInfo DMA2_CH1_INFO{GPDMA2, RCC_AHB1ENR_GPDMA2EN, 1, GPDMA2_Channel1_IRQn};
static const BasicInfo DMA2_CH2_INFO{GPDMA2, RCC_AHB1ENR_GPDMA2EN, 2, GPDMA2_Channel2_IRQn};
static const BasicInfo DMA2_CH3_INFO{GPDMA2, RCC_AHB1ENR_GPDMA2EN, 3, GPDMA2_Channel3_IRQn};
static const BasicInfo DMA2_CH4_INFO{GPDMA2, RCC_AHB1ENR_GPDMA2EN, 4, GPDMA2_Channel4_IRQn};
static const BasicInfo DMA2_CH5_INFO{GPDMA2, RCC_AHB1ENR_GPDMA2EN, 5, GPDMA2_Channel5_IRQn};
static const FullInfo DMA2_CH6_INFO{GPDMA2, RCC_AHB1ENR_GPDMA2EN, 6, GPDMA2_Channel6_IRQn};
static const FullInfo DMA2_CH7_INFO{GPDMA2, RCC_AHB1ENR_GPDMA2EN, 7, GPDMA2_Channel7_IRQn};

// dual channel DMA infos
static const BasicDualInfo DMA1_CH0_CH1_INFO{GPDMA1, RCC_AHB1ENR_GPDMA1EN, 0, 1, GPDMA1_Channel0_IRQn, GPDMA1_Channel1_IRQn};
static const BasicDualInfo DMA1_CH1_CH2_INFO{GPDMA1, RCC_AHB1ENR_GPDMA1EN, 1, 2, GPDMA1_Channel1_IRQn, GPDMA1_Channel2_IRQn};
static const BasicDualInfo DMA1_CH2_CH3_INFO{GPDMA1, RCC_AHB1ENR_GPDMA1EN, 2, 3, GPDMA1_Channel2_IRQn, GPDMA1_Channel3_IRQn};
static const BasicDualInfo DMA1_CH3_CH4_INFO{GPDMA1, RCC_AHB1ENR_GPDMA1EN, 3, 4, GPDMA1_Channel3_IRQn, GPDMA1_Channel4_IRQn};
static const BasicDualInfo DMA1_CH4_CH5_INFO{GPDMA1, RCC_AHB1ENR_GPDMA1EN, 4, 5, GPDMA1_Channel4_IRQn, GPDMA1_Channel5_IRQn};
static const BasicDualInfo DMA1_CH5_CH6_INFO{GPDMA1, RCC_AHB1ENR_GPDMA1EN, 5, 6, GPDMA1_Channel5_IRQn, GPDMA1_Channel6_IRQn};
static const FullDualInfo DMA1_CH6_CH7_INFO{GPDMA1, RCC_AHB1ENR_GPDMA1EN, 6, 7, GPDMA1_Channel6_IRQn, GPDMA1_Channel7_IRQn};

static const BasicDualInfo DMA2_CH0_CH1_INFO{GPDMA2, RCC_AHB1ENR_GPDMA2EN, 0, 1, GPDMA2_Channel0_IRQn, GPDMA2_Channel1_IRQn};
static const BasicDualInfo DMA2_CH1_CH2_INFO{GPDMA2, RCC_AHB1ENR_GPDMA2EN, 1, 2, GPDMA2_Channel1_IRQn, GPDMA2_Channel2_IRQn};
static const BasicDualInfo DMA2_CH2_CH3_INFO{GPDMA2, RCC_AHB1ENR_GPDMA2EN, 2, 3, GPDMA2_Channel2_IRQn, GPDMA2_Channel3_IRQn};
static const BasicDualInfo DMA2_CH3_CH4_INFO{GPDMA2, RCC_AHB1ENR_GPDMA2EN, 3, 4, GPDMA2_Channel3_IRQn, GPDMA2_Channel4_IRQn};
static const BasicDualInfo DMA2_CH4_CH5_INFO{GPDMA2, RCC_AHB1ENR_GPDMA2EN, 4, 5, GPDMA2_Channel4_IRQn, GPDMA2_Channel5_IRQn};
static const BasicDualInfo DMA2_CH5_CH6_INFO{GPDMA2, RCC_AHB1ENR_GPDMA2EN, 5, 6, GPDMA2_Channel5_IRQn, GPDMA2_Channel6_IRQn};
static const FullDualInfo DMA2_CH6_CH7_INFO{GPDMA2, RCC_AHB1ENR_GPDMA2EN, 6, 7, GPDMA2_Channel6_IRQn, GPDMA2_Channel7_IRQn};

} // namespace dma
} // namespace coco
