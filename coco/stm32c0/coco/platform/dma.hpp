#pragma once

#include "platform.hpp"
#include "dmaInfoC0G4.hpp"


namespace coco {

/**
 * DMA helpers
 * Refernece manual: https://www.st.com/resource/en/reference_manual/rm0490-stm32c0x1-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 * DMA: Section 9
 * DMAMUX: Section 10
 */
namespace dma {

// single channel DMA infos
static const Info DMA1_CH1_INFO{DMA1, RCC_AHBENR_DMA1EN, 0, DMA1_Channel1_IRQn};
static const Info DMA1_CH2_INFO{DMA1, RCC_AHBENR_DMA1EN, 1, DMA1_Channel2_3_IRQn};
static const Info DMA1_CH3_INFO{DMA1, RCC_AHBENR_DMA1EN, 2, DMA1_Channel2_3_IRQn};

// dual channel DMA infos
static const Info2 DMA1_CH1_CH2_INFO{DMA1, RCC_AHBENR_DMA1EN, 0, 1, DMA1_Channel1_IRQn, DMA1_Channel2_3_IRQn};
static const Info2 DMA1_CH2_CH3_INFO{DMA1, RCC_AHBENR_DMA1EN, 1, 2, DMA1_Channel2_3_IRQn, DMA1_Channel2_3_IRQn};

} // namespace dma
} // namespace coco
