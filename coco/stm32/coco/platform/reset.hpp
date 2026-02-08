#pragma once

#include "backup.hpp"


namespace coco {

/// @brief Reset the microcontroller with a specific intent.
/// The intent can be used to instruct a bootloader to stay in the bootloader and wait for commands.
/// The intent value is stored in the backup register BKP0R.
/// @param intent Intent value
/// @return
__NO_RETURN __STATIC_FORCEINLINE void reset(int intent) {
    __disable_irq();

#ifdef HAVE_BACKUP
    backup::unlock();
    backup::set(0, intent);
#endif

    NVIC_SystemReset();
}

/// @brief Get reset intent.
/// Returns 0 when HAVE_BACKUP is not defined.
/// @return Reset intent
__STATIC_FORCEINLINE int getResetIntent() {
#ifdef HAVE_BACKUP
    return backup::get(0);
#else
    return 0;
#endif
}

} // namespace coco
