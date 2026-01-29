#pragma once

#include "platform.hpp"
//#include "cache.hpp"
#include <cstdint>


namespace coco {

/// @brief FLASH Helpers
/// Single bank mode for 1M devices
namespace flash {

/// @brief Use maximum sector size as page size
///
constexpr int PAGE_SIZE_16K = 16384;
constexpr int PAGE_SIZE_64K = 65536;
constexpr int PAGE_SIZE_128K = 131072;

/// @brief Block of data that has to be written at once and is the read alignment.
/// 2.7 - 3.6V, parallelism x32, PSIZE = 10
using Block = uint32_t;

// size of a block that has to be written at once and is the read alignment
constexpr int BLOCK_SIZE = sizeof(Block);


// sector sizes in kBytes
static const uint8_t sectorSizes[] = {16, 16, 16, 16, 64, 128, 128, 128, 128, 128, 128, 128, 16, 16, 16, 16, 64, 128, 128, 128, 128, 128, 128, 128};


/// @brief Write to flash memory.
/// @param address Flash address to write to, must be aligned to block size
/// @param data Data to write
/// @param size Size of data to write, gets extended to a multiple of block size
/// @return true if successful
inline void write(uint32_t address, void *data, int size) {
    // unlock flash
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;

    // set flash write mode (PSIZE = 10)
    FLASH->CR = FLASH_CR_PG | FLASH_CR_PSIZE_1;

    auto src = (const Block *)data;
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

    // lock flash and clear PG bit
    FLASH->CR = FLASH_CR_LOCK;

    // flush cache
    //cache::flush();
}

/// @brief Erase a page in flash memory.
/// @param address Flash address to erase, must be aligned to page size
/// @param size Size of data to write, gets extended to a multiple of page size
/// @return true if successful
inline void erase(uint32_t address, int size) {
    // unlock flash
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;

    // determine sector index
    uint32_t sectorAddress = 0x8000000u;
    int sector = 0;
    while (address > sectorAddress) {
        sectorAddress += sectorSizes[sector] << 10;
        ++sector;
    }

    while (size > 0) {
        // erase using page index
        uint32_t cr = FLASH_CR_SER | FLASH_CR_PSIZE_1 | (sector << FLASH_CR_SNB_Pos);

        // set flash page erase mode
        FLASH->CR = cr;

        // start page erase
        FLASH->CR = cr | FLASH_CR_STRT;

        size -= sectorSizes[sector] << 10;
        ++sector;

        // wait until flash is ready
        while ((FLASH->SR & FLASH_SR_BSY) != 0) {}
    }

    // lock flash and clear PG or PER bit
    FLASH->CR = FLASH_CR_LOCK;

    // flush cache
    //cache::flush();
}

} // namespace flash
} // namespace coco
