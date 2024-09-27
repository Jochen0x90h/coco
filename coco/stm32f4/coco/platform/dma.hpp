#pragma once

#include "platform.hpp"
#include "dmaInfoF4.hpp"


namespace coco {

/**
 * DMA helpers
 * Refernece manual: https://www.st.com/resource/en/reference_manual/rm0364-stm32f334xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 * DMA: Section 11
 */
namespace dma {

// single channel DMA infos
static const Info DMA1_S0_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 0, DMA1_Stream0_IRQn};
static const Info DMA1_S1_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 1, DMA1_Stream1_IRQn};
static const Info DMA1_S2_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 2, DMA1_Stream2_IRQn};
static const Info DMA1_S3_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 3, DMA1_Stream3_IRQn};
static const Info DMA1_S4_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 4, DMA1_Stream4_IRQn};
static const Info DMA1_S5_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 5, DMA1_Stream5_IRQn};
static const Info DMA1_S6_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 6, DMA1_Stream6_IRQn};
static const Info DMA1_S7_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 7, DMA1_Stream7_IRQn};

static const Info DMA2_S0_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 0, DMA2_Stream0_IRQn};
static const Info DMA2_S1_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 1, DMA2_Stream1_IRQn};
static const Info DMA2_S2_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 2, DMA2_Stream2_IRQn};
static const Info DMA2_S3_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 3, DMA2_Stream3_IRQn};
static const Info DMA2_S4_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 4, DMA2_Stream4_IRQn};
static const Info DMA2_S5_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 5, DMA2_Stream5_IRQn};
static const Info DMA2_S6_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 6, DMA2_Stream6_IRQn};
static const Info DMA2_S7_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 7, DMA2_Stream7_IRQn};

// dual channel DMA infos
static const Info2 DMA1_S0_S5_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 0, 5, DMA1_Stream0_IRQn, DMA1_Stream5_IRQn};
static const Info2 DMA1_S0_S7_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 0, 7, DMA1_Stream0_IRQn, DMA1_Stream7_IRQn};
static const Info2 DMA1_S1_S3_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 1, 3, DMA1_Stream1_IRQn, DMA1_Stream3_IRQn};
static const Info2 DMA1_S2_S4_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 2, 4, DMA1_Stream2_IRQn, DMA1_Stream4_IRQn};
static const Info2 DMA1_S2_S5_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 2, 5, DMA1_Stream2_IRQn, DMA1_Stream5_IRQn};
static const Info2 DMA1_S2_S7_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 2, 7, DMA1_Stream2_IRQn, DMA1_Stream7_IRQn};
static const Info2 DMA1_S3_S1_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 3, 1, DMA1_Stream3_IRQn, DMA1_Stream1_IRQn};
static const Info2 DMA1_S3_S4_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 3, 4, DMA1_Stream3_IRQn, DMA1_Stream4_IRQn};
static const Info2 DMA1_S3_S7_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 3, 7, DMA1_Stream3_IRQn, DMA1_Stream7_IRQn};
static const Info2 DMA1_S5_S6_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 5, 6, DMA1_Stream5_IRQn, DMA1_Stream6_IRQn};
static const Info2 DMA1_S6_S0_INFO{DMA1, RCC_AHB1ENR_DMA1EN, 6, 0, DMA1_Stream6_IRQn, DMA1_Stream0_IRQn};

static const Info2 DMA2_S0_S2_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 0, 2, DMA2_Stream0_IRQn, DMA2_Stream2_IRQn};
static const Info2 DMA2_S0_S1_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 0, 1, DMA2_Stream0_IRQn, DMA2_Stream1_IRQn};
static const Info2 DMA2_S0_S3_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 0, 3, DMA2_Stream0_IRQn, DMA2_Stream3_IRQn};
static const Info2 DMA2_S0_S5_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 0, 5, DMA2_Stream0_IRQn, DMA2_Stream5_IRQn};
static const Info2 DMA2_S1_S6_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 1, 6, DMA2_Stream1_IRQn, DMA2_Stream6_IRQn};
static const Info2 DMA2_S1_S7_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 1, 7, DMA2_Stream1_IRQn, DMA2_Stream7_IRQn};
static const Info2 DMA2_S2_S3_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 2, 3, DMA2_Stream2_IRQn, DMA2_Stream3_IRQn};
static const Info2 DMA2_S2_S5_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 2, 5, DMA2_Stream2_IRQn, DMA2_Stream5_IRQn};
static const Info2 DMA2_S2_S6_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 2, 6, DMA2_Stream2_IRQn, DMA2_Stream6_IRQn};
static const Info2 DMA2_S2_S7_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 2, 7, DMA2_Stream2_IRQn, DMA2_Stream7_IRQn};
static const Info2 DMA2_S3_S4_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 3, 4, DMA2_Stream3_IRQn, DMA2_Stream4_IRQn};
static const Info2 DMA2_S5_S6_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 5, 6, DMA2_Stream5_IRQn, DMA2_Stream6_IRQn};
static const Info2 DMA2_S5_S7_INFO{DMA2, RCC_AHB1ENR_DMA2EN, 5, 7, DMA2_Stream5_IRQn, DMA2_Stream7_IRQn};


} // namespace dma
} // namespace coco
