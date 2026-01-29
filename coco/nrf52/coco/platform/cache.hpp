#pragma once

// do not include directly, use #include <coco/platform/cache.hpp>

#include "platform.hpp"
#include <cstdint>


namespace coco {

/// @brief Cache Helpers.
///
namespace cache {

/// @brief Invalidate the instruction cache (nRF52 has no data cache).
/// https://docs.nordicsemi.com/bundle/ps_nrf52840/page/nvmc.html#register.ICACHECNF
__STATIC_FORCEINLINE void flush() {
	// disable cache (this invalidates all entries)
    uint32_t icachecnf = NRF_NVMC->ICACHECNF;
	NRF_NVMC->ICACHECNF = N(NVMC_ICACHECNF_CACHEEN, Disabled);

	// restore ICACHECNF
	NRF_NVMC->ICACHECNF = icachecnf;
}

} // namespace flash
} // namespace coco
