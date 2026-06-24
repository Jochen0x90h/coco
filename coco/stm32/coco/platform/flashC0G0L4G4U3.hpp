#pragma once

// do not include directly, use #include <coco/platform/flash.hpp>

#include "platform.hpp"


namespace coco {

/// @brief FLASH Helpers
/// G4 https://www.st.com/en/microcontrollers-microprocessors/stm32g4-series/documentation.html Sections 3-5
/// U3 https://www.st.com/en/microcontrollers-microprocessors/stm32u3-series/documentation.html Section 7
namespace flash {

// C0, G0
#ifndef FLASH_SR_BSY
#ifdef FLASH_SR_BSY2
#define FLASH_SR_BSY (FLASH_SR_BSY1 | FLASH_SR_BSY2)
#else
#define FLASH_SR_BSY FLASH_SR_BSY1
#endif
#endif

#if defined(STM32C0) || defined(STM32G0) || defined(STM32L4) || defined(STM32G4)
/// @brief Size of a page that has to be erased at once.
///
constexpr int PAGE_SIZE = 2048;
#elif defined(STM32U3)
/// @brief Size of a page that has to be erased at once.
///
constexpr int PAGE_SIZE = 4096;
#else
#error "PAGE_SIZE unknown"
#endif

/// @brief Word of data that has to be written at once and is the read alignment.
///
struct Word {
    uint32_t data[2];
};

/// @brief Size of a word that has to be written at once and is the read alignment.
///
constexpr int WORD_SIZE = sizeof(Word);


/// @brief Write to flash memory.
/// @param address Flash address to write to, must be aligned to word size
/// @param data Data to write
/// @param size Size of data to write, gets extended to a multiple of word size
/// @return true if successful
__STATIC_FORCEINLINE void write(uint32_t address, const Word *data, int size) {
    // unlock flash
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;

    // set flash write mode
    FLASH->CR = FLASH_CR_PG;

    auto src = data;
    auto dst = (Word *)address;
    while (size > 0) {
        // write word
        *dst = *src;

        // data memory barrier
        __DMB();

        ++src;
        ++dst;
        size -= sizeof(Word);

        // wait until flash is ready
        while ((FLASH->SR & FLASH_SR_BSY) != 0) {}
    }

    // clear PG bit and lock flash
    FLASH->CR = FLASH_CR_LOCK;

    // flush cache
    //cache::flush();
}

/// @brief Erase a page in flash memory.
/// Note that flushing the cache (cache::flush()) is necessary when accessing the flash after erase() before writing it again.
/// @param address Flash address to erase, must be aligned to page size
/// @param size Size of data to write, gets extended to a multiple of page size
/// @return true if successful
__STATIC_FORCEINLINE void erase(uint32_t address, int size) {
    // unlock flash
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;

    // calc page index from address
    int page = (address - 0x8000000u) / PAGE_SIZE;

    while (size > 0) {
        uint32_t pnb = page << FLASH_CR_PNB_Pos;

#if defined(STM32G0) && defined(FLASH_CR_BKER)
        // 2x128 pages for 512k, 2x64 pages for 256k
        const uint32_t PNB_Msk = (FLASH_SIZE > 262144 ? 0x7f : 0x3f) << FLASH_CR_PNB_Pos;
#else
        const uint32_t PNB_Msk = FLASH_CR_PNB_Msk;
#endif

        // erase using page index
        uint32_t cr = FLASH_CR_PER | (pnb & PNB_Msk);

        // dual bank mode
#ifdef FLASH_CR_BKER
        if (pnb > PNB_Msk)
            cr |= FLASH_CR_BKER;
#endif
        // set flash page erase mode and start page erase
        FLASH->CR = cr | FLASH_CR_STRT;

        ++page;
        size -= PAGE_SIZE;

        // wait until flash is ready
        while ((FLASH->SR & FLASH_SR_BSY) != 0) {}
    }

    // clear PER bit and lock flash
    FLASH->CR = FLASH_CR_LOCK;

    // flush cache
    //cache::flush();
}

} // namespace flash
} // namespace coco
