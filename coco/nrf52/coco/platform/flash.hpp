#pragma once

#include "platform.hpp"
#include <cstdint>


namespace coco {

/// @brief FLASH Helpers
///
namespace flash {

/// @brief Size of a page that has to be erased at once.
///
constexpr int PAGE_SIZE = 4096;

/// @brief Word of data that has to be written at once and is the read alignment.
///
using Word = uint32_t;

/// @brief Size of a word that has to be written at once and is the read alignment.
///
constexpr int WORD_SIZE = sizeof(Word);


/// @brief Write to flash memory.
/// @param address Flash address to write to, must be aligned to word size
/// @param data Data to write, must be alignas(flash::Word)
/// @param size Size of data to write, gets extended to a multiple of word size
/// @return true if successful
__STATIC_FORCEINLINE void write(uint32_t address, const Word *data, int size) {
    // set flash write mode
    NRF_NVMC->CONFIG = N(NVMC_CONFIG_WEN, Wen);

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
        while (!NRF_NVMC->READY) {}
    }

    // set flash read mode
    NRF_NVMC->CONFIG = N(NVMC_CONFIG_WEN, Ren);
}

/// @brief Erase a page in flash memory.
/// @param address Flash address to erase, must be aligned to page size
/// @param size Size of data to write, gets extended to a multiple of page size
/// @return true if successful
__STATIC_FORCEINLINE void erase(uint32_t address, int size) {
    // set flash erase mode
    NRF_NVMC->CONFIG = N(NVMC_CONFIG_WEN, Een);

    while (size > 0) {
        // erase page
        NRF_NVMC->ERASEPAGE = address;

        address += PAGE_SIZE;
        size -= PAGE_SIZE;

        // wait until flash is ready
        while (!NRF_NVMC->READY) {}
    }

    // set flash read mode
    NRF_NVMC->CONFIG = N(NVMC_CONFIG_WEN, Ren);
}

} // namespace flash
} // namespace coco
