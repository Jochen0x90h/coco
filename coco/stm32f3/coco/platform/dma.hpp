#pragma once

#include "platform.hpp"
#include "dmaInfoF0F3.hpp"


namespace coco {

/**
 * DMA helpers
 * Refernece manual: https://www.st.com/resource/en/reference_manual/rm0364-stm32f334xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 * DMA: Section 11
 */
namespace dma {

// single channel DMA infos
static const Info DMA1_CH1_INFO{DMA1, RCC_AHBENR_DMA1EN, 0, DMA1_Channel1_IRQn};
static const Info DMA1_CH2_INFO{DMA1, RCC_AHBENR_DMA1EN, 1, DMA1_Channel2_IRQn};
static const Info DMA1_CH3_INFO{DMA1, RCC_AHBENR_DMA1EN, 2, DMA1_Channel3_IRQn};
static const Info DMA1_CH4_INFO{DMA1, RCC_AHBENR_DMA1EN, 3, DMA1_Channel4_IRQn};
static const Info DMA1_CH5_INFO{DMA1, RCC_AHBENR_DMA1EN, 4, DMA1_Channel5_IRQn};
static const Info DMA1_CH6_INFO{DMA1, RCC_AHBENR_DMA1EN, 5, DMA1_Channel6_IRQn};
static const Info DMA1_CH7_INFO{DMA1, RCC_AHBENR_DMA1EN, 6, DMA1_Channel7_IRQn};

/*
todo: map methods should support DMA2
#ifdef DMA2
static const Info DMA2_CH1_INFO{DMA2, RCC_AHBENR_DMA2EN, 0, DMA2_Channel1_IRQn};
static const Info DMA2_CH2_INFO{DMA2, RCC_AHBENR_DMA2EN, 1, DMA2_Channel2_IRQn};
static const Info DMA2_CH3_INFO{DMA2, RCC_AHBENR_DMA2EN, 2, DMA2_Channel3_IRQn};
static const Info DMA2_CH4_INFO{DMA2, RCC_AHBENR_DMA2EN, 3, DMA2_Channel4_IRQn};
static const Info DMA2_CH5_INFO{DMA2, RCC_AHBENR_DMA2EN, 4, DMA2_Channel5_IRQn};
#endif
*/

// dual channel DMA infos
static const Info2 DMA1_CH2_CH3_INFO{DMA1, RCC_AHBENR_DMA1EN, 1, 2, DMA1_Channel2_IRQn, DMA1_Channel3_IRQn};
static const Info2 DMA1_CH3_CH2_INFO{DMA1, RCC_AHBENR_DMA1EN, 2, 1, DMA1_Channel3_IRQn, DMA1_Channel2_IRQn};
static const Info2 DMA1_CH4_CH5_INFO{DMA1, RCC_AHBENR_DMA1EN, 3, 4, DMA1_Channel4_IRQn, DMA1_Channel5_IRQn};
static const Info2 DMA1_CH5_CH4_INFO{DMA1, RCC_AHBENR_DMA1EN, 4, 3, DMA1_Channel5_IRQn, DMA1_Channel4_IRQn};
static const Info2 DMA1_CH6_CH7_INFO{DMA1, RCC_AHBENR_DMA1EN, 5, 6, DMA1_Channel6_IRQn, DMA1_Channel7_IRQn};

} // namespace dma
} // namespace coco
