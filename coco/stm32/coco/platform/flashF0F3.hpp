#pragma once

// do not include directly, use #include <coco/platform/flash.hpp>

#include "platform.hpp"


namespace coco {

/// @brief FLASH Helpers
/// F0 Section 3
namespace flash {

#if defined(STM32F0)
/// @brief Size of a page that has to be erased at once.
///
constexpr int PAGE_SIZE = 1024;
#elif defined(STM32F3)
/// @brief Size of a page that has to be erased at once.
///
constexpr int PAGE_SIZE = 2048;
#else
#error "PAGE_SIZE unknown"
#endif

/// @brief Block of data that has to be written at once and is the read alignment.
///
using Block = uint16_t;

/// @brief Size of a block that has to be written at once and is the read alignment.
///
constexpr int BLOCK_SIZE = sizeof(Block);


/// @brief Write to flash memory.
/// @param address Flash address to write to, must be aligned to block size
/// @param data Data to write, must be alignas(flash::Block)
/// @param size Size of data to write, gets extended to a multiple of block size
/// @return true if successful
__STATIC_FORCEINLINE void write(uint32_t address, const Block *data, int size) {
    // unlock flash
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;

    // set flash write mode
    FLASH->CR = FLASH_CR_PG;

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
        while ((FLASH->SR & FLASH_SR_BSY) != 0) {}
    }

    // clear PG bit and lock flash
    FLASH->CR = FLASH_CR_LOCK;

    // flush cache
    //cache::flush();
}

/// @brief Erase a page in flash memory.
/// @param address Flash address to erase, must be aligned to page size
/// @param size Size of data to write, gets extended to a multiple of page size
/// @return true if successful
__STATIC_FORCEINLINE void erase(uint32_t address, int size) {
    // unlock flash
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;

    // set flash page erase mode
    FLASH->CR = FLASH_CR_PER;

    while (size > 0) {
        // set address of page to erase (must come after PER)
        FLASH->AR = address;

        // start page erase
        FLASH->CR = FLASH_CR_PER | FLASH_CR_STRT;

        address += PAGE_SIZE;
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
