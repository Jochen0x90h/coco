#pragma once

// do not include directly, use #include <coco/platform/flash.hpp>

#include "platform.hpp"


// patch FLASH_CR_SNB_Msk for H503
#ifdef STM32H503xx_H
#undef FLASH_CR_SNB_Msk
#define FLASH_CR_SNB_Msk (0x07UL << FLASH_CR_SNB_Pos)
#endif

namespace coco {

/// @brief FLASH Helpers
/// H503 https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series/documentation.html Section 7
/// H5 https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series/documentation.html Section 7
namespace flash {

// size of a page that has to be erased at once
constexpr int PAGE_SIZE = 8192;

// block of data that has to be written at once
struct Block {
    uint32_t data[4];
};

// size of a block that has to be written at once and is the read alignment
constexpr int BLOCK_SIZE = sizeof(Block);


/// @brief Write to flash memory.
/// @param address Flash address to write to, must be aligned to block size
/// @param data Data to write, must be alignas(flash::Block)
/// @param size Size of data to write, gets extended to a multiple of block size
/// @return true if successful
__STATIC_FORCEINLINE void write(uint32_t address, const Block *data, int size) {
    // unlock flash
    FLASH->NSKEYR = 0x45670123;
    FLASH->NSKEYR = 0xCDEF89AB;

    // set flash write mode
    FLASH->NSCR = FLASH_CR_PG;

    auto src = data;
    auto dst = (Block *)address;
    while (size > 0) {
        // write block
        *dst = *src;

        // data memory barrier
        __DMB();

        ++src;
        ++dst;
        size -= sizeof(Block);

        // wait until flash is ready
        while ((FLASH->NSSR & FLASH_SR_BSY) != 0) {}
    }

    // lock flash and clear PG bit
    FLASH->NSCR = FLASH_CR_LOCK;

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
    FLASH->NSKEYR = 0x45670123;
    FLASH->NSKEYR = 0xCDEF89AB;

    // calc page index from address
    int page = (address - 0x8000000u) / PAGE_SIZE;

    bool result = true;
    while (result && size > 0) {
#ifdef STM32H5
        uint32_t snb = page << FLASH_CR_SNB_Pos;

        // erase using page (sector) index
        uint32_t cr = FLASH_CR_SER | (snb & FLASH_CR_SNB_Msk);

        // select bank
        if (snb > FLASH_CR_SNB_Msk)
            cr |= FLASH_CR_BKSEL;
#endif
#ifdef STM32U5
        uint32_t snb = page << FLASH_CR_SNB_Pos;

        // erase using page (sector) index
        uint32_t cr = FLASH_CR_PER | (snb & FLASH_CR_SNB_Msk);

        // select bank
        if (snb > FLASH_CR_SNB_Msk)
            cr |= FLASH_CR_BKER;
#endif
        // set flash page erase mode and start page erase
        FLASH->NSCR = cr | FLASH_CR_START;

        ++page;
        size -= PAGE_SIZE;

        // wait until flash is ready
        while ((FLASH->NSSR & FLASH_SR_BSY) != 0) {}
    }

    // clear SER or PER bit and lock flash
    FLASH->NSCR = FLASH_CR_LOCK;

    // flush cache
    //cache::flush();
}

} // namespace flash
} // namespace coco
