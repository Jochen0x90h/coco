#pragma once

#include "platform.hpp"
#include "dmaInfoF0F3.hpp"


namespace coco {

/**
 * DMA helpers
 * Refernece manual: https://www.st.com/resource/en/reference_manual/dm00031936-stm32f0x1stm32f0x2stm32f0x8-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 * DMA: Section 10
 */
namespace dma {

// single channel DMA infos
static const Info DMA1_CH1_INFO{DMA1, RCC_AHBENR_DMA1EN, 0, DMA1_Channel1_IRQn};
static const Info DMA1_CH2_INFO{DMA1, RCC_AHBENR_DMA1EN, 1, DMA1_Channel2_3_IRQn};
static const Info DMA1_CH3_INFO{DMA1, RCC_AHBENR_DMA1EN, 2, DMA1_Channel2_3_IRQn};
static const Info DMA1_CH4_INFO{DMA1, RCC_AHBENR_DMA1EN, 3, DMA1_Channel4_5_IRQn};
static const Info DMA1_CH5_INFO{DMA1, RCC_AHBENR_DMA1EN, 4, DMA1_Channel4_5_IRQn};
#ifdef DMA1_Channel6
static const Info DMA1_CH6_INFO{DMA1, RCC_AHBENR_DMA1EN, 5, DMA1_Channel4_5_6_7_IRQn};
#endif
#ifdef DMA1_Channel7
static const Info DMA1_CH7_INFO{DMA1, RCC_AHBENR_DMA1EN, 6, DMA1_Channel4_5_6_7_IRQn};
#endif

/*
todo: map methods should support DMA2
#ifdef DMA2
static const Info DMA2_CH1_INFO{DMA2, RCC_AHBENR_DMA2EN, 0, DMA1_Ch2_3_DMA2_Ch1_2_IRQn};
static const Info DMA2_CH2_INFO{DMA2, RCC_AHBENR_DMA2EN, 1, DMA1_Ch2_3_DMA2_Ch1_2_IRQn};
static const Info DMA2_CH3_INFO{DMA2, RCC_AHBENR_DMA2EN, 2, DMA1_Ch4_7_DMA2_Ch3_5_IRQn};
static const Info DMA2_CH4_INFO{DMA2, RCC_AHBENR_DMA2EN, 3, DMA1_Ch4_7_DMA2_Ch3_5_IRQn};
static const Info DMA2_CH5_INFO{DMA2, RCC_AHBENR_DMA2EN, 4, DMA1_Ch4_7_DMA2_Ch3_5_IRQn};
#endif
*/

// dual channel DMA infos
static const Info2 DMA1_CH2_CH3_INFO{DMA1, RCC_AHBENR_DMA1EN, 1, 2, DMA1_Channel2_3_IRQn, DMA1_Channel2_3_IRQn};
static const Info2 DMA1_CH3_CH2_INFO{DMA1, RCC_AHBENR_DMA1EN, 2, 1, DMA1_Channel2_3_IRQn, DMA1_Channel2_3_IRQn};
static const Info2 DMA1_CH4_CH5_INFO{DMA1, RCC_AHBENR_DMA1EN, 3, 4, DMA1_Channel4_5_IRQn, DMA1_Channel4_5_IRQn};
static const Info2 DMA1_CH5_CH4_INFO{DMA1, RCC_AHBENR_DMA1EN, 4, 3, DMA1_Channel4_5_IRQn, DMA1_Channel4_5_IRQn};

} // namespace dma
} // namespace coco
