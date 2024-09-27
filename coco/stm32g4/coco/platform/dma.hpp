#pragma once

#include "platform.hpp"
#include "dmaInfoC0G4.hpp"


namespace coco {

/**
 * DMA helpers
 * Refernece manual: https://www.st.com/resource/en/reference_manual/rm0440-stm32g4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 * DMA: Section 12
 * DMAMUX: Section 13
 */
namespace dma {

// single channel DMA infos
static const Info DMA1_CH1_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 0, DMA1_Channel1_IRQn};
static const Info DMA1_CH2_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 1, DMA1_Channel2_IRQn};
static const Info DMA1_CH3_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 2, DMA1_Channel3_IRQn};
static const Info DMA1_CH4_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 3, DMA1_Channel4_IRQn};
static const Info DMA1_CH5_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 4, DMA1_Channel5_IRQn};
static const Info DMA1_CH6_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 5, DMA1_Channel6_IRQn};
#ifdef DMA1_Channel7
static const Info DMA1_CH7_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 6, DMA1_Channel7_IRQn};
#endif
#ifdef DMA1_Channel8
static const Info DMA1_CH8_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 7, DMA1_Channel8_IRQn};
#endif

static const Info DMA2_CH1_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 0, DMA2_Channel1_IRQn};
static const Info DMA2_CH2_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 1, DMA2_Channel2_IRQn};
static const Info DMA2_CH3_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 2, DMA2_Channel3_IRQn};
static const Info DMA2_CH4_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 3, DMA2_Channel4_IRQn};
static const Info DMA2_CH5_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 4, DMA2_Channel5_IRQn};
static const Info DMA2_CH6_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 5, DMA2_Channel6_IRQn};
#ifdef DMA2_Channel7
static const Info DMA2_CH7_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 6, DMA2_Channel7_IRQn};
#endif
#ifdef DMA2_Channel8
static const Info DMA2_CH8_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 7, DMA2_Channel8_IRQn};
#endif

// dual channel DMA infos
static const Info2 DMA1_CH1_CH2_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 0, 1, DMA1_Channel1_IRQn, DMA1_Channel2_IRQn};
static const Info2 DMA1_CH2_CH3_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 1, 2, DMA1_Channel2_IRQn, DMA1_Channel3_IRQn};
static const Info2 DMA1_CH3_CH4_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 2, 3, DMA1_Channel3_IRQn, DMA1_Channel4_IRQn};
static const Info2 DMA1_CH4_CH5_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 3, 4, DMA1_Channel4_IRQn, DMA1_Channel5_IRQn};
static const Info2 DMA1_CH5_CH6_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 4, 5, DMA1_Channel5_IRQn, DMA1_Channel6_IRQn};
#ifdef DMA1_Channel7
static const Info2 DMA1_CH6_CH7_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 5, 6, DMA1_Channel6_IRQn, DMA1_Channel7_IRQn};
#endif
#ifdef DMA1_Channel8
static const Info2 DMA1_CH7_CH8_INFO{DMA1, RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN, 6, 7, DMA1_Channel7_IRQn, DMA1_Channel8_IRQn};
#endif

static const Info2 DMA2_CH1_CH2_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 0, 1, DMA2_Channel1_IRQn, DMA2_Channel2_IRQn};
static const Info2 DMA2_CH2_CH3_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 1, 2, DMA2_Channel2_IRQn, DMA2_Channel3_IRQn};
static const Info2 DMA2_CH3_CH4_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 2, 3, DMA2_Channel3_IRQn, DMA2_Channel4_IRQn};
static const Info2 DMA2_CH4_CH5_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 3, 4, DMA2_Channel4_IRQn, DMA2_Channel5_IRQn};
static const Info2 DMA2_CH5_CH6_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 4, 5, DMA2_Channel5_IRQn, DMA2_Channel6_IRQn};
#ifdef DMA1_Channel7
static const Info2 DMA2_CH6_CH7_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 5, 6, DMA2_Channel6_IRQn, DMA2_Channel7_IRQn};
#endif
#ifdef DMA1_Channel8
static const Info2 DMA2_CH7_CH8_INFO{DMA2, RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMAMUX1EN, 6, 7, DMA2_Channel7_IRQn, DMA2_Channel8_IRQn};
#endif

} // namespace dma
} // namespace coco