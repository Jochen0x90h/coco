set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
    PUBLIC FILE_SET headers TYPE HEADERS FILES
        align.hpp
        Array.hpp
        ArrayBuffer.hpp
        ArrayConcept.hpp
        bits.hpp
        Callback.hpp
        ContainerConcept.hpp
        convert.hpp
        Coroutine.hpp
        CStringConcept.hpp
        debug.hpp
        enum.hpp
        Event.hpp
        Frequency.hpp
        InterruptQueue.hpp
        IntrusiveList.hpp
        IntrusiveQueue.hpp
        IntrusiveMpscQueue.hpp
        IsSubclass.hpp
        Queue.hpp
        PointerConcept.hpp
        PseudoRandom.hpp
        Semaphore.hpp
        StreamOperators.hpp
        String.hpp
        StringBuffer.hpp
        StringConcept.hpp
        Task.hpp
        Time.hpp
        TimedTask.hpp
        Unit.hpp
        #utf8.hpp
        Vector2.hpp
        Vector3.hpp
        Vector4.hpp
    PRIVATE
        convert.cpp
        debug.cpp
)

# Platform dependent files, native platforms (Windows, MacOS, Linux) are handled in the else clause at the end
if(${PLATFORM} MATCHES "^nrf52") # Cortex-M4F
    file(GENERATE OUTPUT coco/platform/nrf52.h CONTENT "#include <coco/platform/system/${PLATFORM}.h>\n#include <coco/platform/system/${PLATFORM}_bitfields.h>\n#include <coco/platform/system/${PLATFORM}_peripherals.h>")
    target_sources(${PROJECT_NAME}
        PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/cortex ${CMAKE_CURRENT_SOURCE_DIR}/nrf52 FILES
            cortex/coco/platform/compiler.hpp
            cortex/coco/platform/nvic.hpp
            cortex/coco/platform/scb.hpp
            cortex/coco/platform/system/cmsis_compiler.h
            cortex/coco/platform/system/cmsis_gcc.h
            cortex/coco/platform/system/cmsis_version.h
            cortex/coco/platform/system/core_cm4.h
            cortex/coco/platform/system/m-profile/cmsis_gcc_m.h
            cortex/coco/platform/system/m-profile/armv7m_mpu.h
            nrf52/coco/platform/cache.hpp
            nrf52/coco/platform/flash.hpp
            nrf52/coco/platform/gpio.hpp
            nrf52/coco/platform/ppi.hpp
            nrf52/coco/platform/rng.hpp
            nrf52/coco/platform/rtc.hpp
            nrf52/coco/platform/spim.hpp
            nrf52/coco/platform/timer.hpp
            nrf52/coco/platform/uart.hpp
            nrf52/coco/platform/platform.hpp
            nrf52/coco/platform/system/${PLATFORM}_bitfields.h
            nrf52/coco/platform/system/${PLATFORM}_peripherals.h
            nrf52/coco/platform/system/${PLATFORM}.h
            nrf52/coco/platform/system/system_${PLATFORM}.h
        PUBLIC FILE_SET generated_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR} FILES
            ${CMAKE_CURRENT_BINARY_DIR}/coco/platform/nrf52.h
        PRIVATE
            nrf52/coco/platform/system/gcc_startup_${PLATFORM}.S
    )
    target_include_directories(${PROJECT_NAME}
        PRIVATE
            ../coco/nrf52/coco/platform
    )
elseif(${PLATFORM} MATCHES "^stm32f0") # Cortex-M0
    file(GENERATE OUTPUT coco/platform/stm32f0.h CONTENT "#include <coco/platform/system/${PLATFORM}.h>")
    target_sources(${PROJECT_NAME}
        PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/cortex ${CMAKE_CURRENT_SOURCE_DIR}/stm32 ${CMAKE_CURRENT_SOURCE_DIR}/stm32f0 FILES
            cortex/coco/platform/compiler.hpp
            cortex/coco/platform/nvic.hpp
            cortex/coco/platform/scb.hpp
            cortex/coco/platform/system/cmsis_compiler.h
            cortex/coco/platform/system/cmsis_gcc.h
            cortex/coco/platform/system/cmsis_version.h
            cortex/coco/platform/system/core_cm0.h
            cortex/coco/platform/system/m-profile/cmsis_gcc_m.h
            stm32/coco/platform/adcF0C0G0F3L4G4H5U3U5.hpp
            stm32/coco/platform/backupF0F3F4L4.hpp
            stm32/coco/platform/cacheF0F3.hpp
            stm32/coco/platform/crcAll.hpp
            stm32/coco/platform/dacF0F3F4.hpp
            stm32/coco/platform/dmaF0L0C0G0F1L1F3L4G4.hpp
            stm32/coco/platform/extiF0F3F4.hpp
            stm32/coco/platform/flashF0F3.hpp
            stm32/coco/platform/gpio.hpp
            stm32/coco/platform/i2cF0C0G0F3L4G4H5U3U5.hpp
            stm32/coco/platform/jump.hpp
            stm32/coco/platform/rccF0F1F3.hpp
            stm32/coco/platform/reset.hpp
            stm32/coco/platform/spiF0C0G0F3L4G4.hpp
            stm32/coco/platform/timerAll.hpp
            stm32/coco/platform/uartF0L0C0G0F3L4G4H5U3U5.hpp
            stm32/coco/platform/usbdF0C0F3G4L4.hpp
            stm32f0/coco/platform/adc.hpp
            stm32f0/coco/platform/backup.hpp
            stm32f0/coco/platform/cache.hpp
            stm32f0/coco/platform/crc.hpp
            stm32f0/coco/platform/dac.hpp
            stm32f0/coco/platform/dma.hpp
            stm32f0/coco/platform/exti.hpp
            stm32f0/coco/platform/flash.hpp
            stm32f0/coco/platform/i2c.hpp
            stm32f0/coco/platform/rcc.hpp
            stm32f0/coco/platform/spi.hpp
            stm32f0/coco/platform/timer.hpp
            stm32f0/coco/platform/uart.hpp
            stm32f0/coco/platform/usbd.hpp
            stm32f0/coco/platform/platform.hpp
            stm32f0/coco/platform/system/${PLATFORM}.h # obtain from https://github.com/STMicroelectronics/STM32CubeF0/tree/master/Drivers/CMSIS/Device/ST/STM32F0xx
            stm32f0/coco/platform/system/system_stm32f0xx.h
        PUBLIC FILE_SET generated_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR} FILES
            ${CMAKE_CURRENT_BINARY_DIR}/coco/platform/stm32f0.h
        PRIVATE
            stm32f0/coco/platform/platform.cpp
            stm32f0/coco/platform/system/startup_${PLATFORM}.s
    )
    target_include_directories(${PROJECT_NAME}
        PRIVATE
            ../coco/stm32/coco/platform
            ../coco/stm32f0/coco/platform
    )
elseif(${PLATFORM} MATCHES "^stm32c0") # Cortex-M0+
    file(GENERATE OUTPUT coco/platform/stm32c0.h CONTENT "#include <coco/platform/system/${PLATFORM}.h>")
    target_sources(${PROJECT_NAME}
        PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/cortex ${CMAKE_CURRENT_SOURCE_DIR}/stm32 ${CMAKE_CURRENT_SOURCE_DIR}/stm32c0 FILES
            cortex/coco/platform/compiler.hpp
            cortex/coco/platform/nvic.hpp
            cortex/coco/platform/scb.hpp
            cortex/coco/platform/system/cmsis_compiler.h
            cortex/coco/platform/system/cmsis_gcc.h
            cortex/coco/platform/system/cmsis_version.h
            cortex/coco/platform/system/core_cm0plus.h
            cortex/coco/platform/system/m-profile/cmsis_gcc_m.h
            cortex/coco/platform/system/m-profile/armv7m_mpu.h
            stm32/coco/platform/adcF0C0G0F3L4G4H5U3U5.hpp
            stm32/coco/platform/backupC0.hpp
            stm32/coco/platform/cacheC0G0F4L4G4.hpp
            stm32/coco/platform/crcAll.hpp
            stm32/coco/platform/dmaF0L0C0G0F1L1F3L4G4.hpp
            stm32/coco/platform/extiC0G0H5U3U5.hpp
            stm32/coco/platform/flashC0G0L4G4U3.hpp
            stm32/coco/platform/gpio.hpp
            stm32/coco/platform/i2cF0C0G0F3L4G4H5U3U5.hpp
            stm32/coco/platform/jump.hpp
            stm32/coco/platform/rccC0G0.hpp
            stm32/coco/platform/reset.hpp
            stm32/coco/platform/spiF0C0G0F3L4G4.hpp
            stm32/coco/platform/timerAll.hpp
            stm32/coco/platform/uartF0L0C0G0F3L4G4H5U3U5.hpp
            stm32c0/coco/platform/adc.hpp
            stm32c0/coco/platform/backup.hpp
            stm32c0/coco/platform/cache.hpp
            stm32c0/coco/platform/crc.hpp
            stm32c0/coco/platform/dac.hpp
            stm32c0/coco/platform/dma.hpp
            stm32c0/coco/platform/exti.hpp
            stm32c0/coco/platform/flash.hpp
            stm32c0/coco/platform/i2c.hpp
            stm32c0/coco/platform/rcc.hpp
            stm32c0/coco/platform/spi.hpp
            stm32c0/coco/platform/timer.hpp
            stm32c0/coco/platform/uart.hpp
            stm32c0/coco/platform/usbd.hpp
            stm32c0/coco/platform/platform.hpp
            stm32c0/coco/platform/system/${PLATFORM}.h # obtain from https://github.com/STMicroelectronics/STM32CubeG4/tree/master/Drivers/CMSIS/Device/ST/STM32G4xx
            stm32c0/coco/platform/system/system_stm32c0xx.h
        PUBLIC FILE_SET generated_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR} FILES
            ${CMAKE_CURRENT_BINARY_DIR}/coco/platform/stm32c0.h
        PRIVATE
            stm32c0/coco/platform/platform.cpp
            stm32c0/coco/platform/system/startup_${PLATFORM}.s
    )
    target_include_directories(${PROJECT_NAME}
        PRIVATE
            ../coco/stm32/coco/platform
            ../coco/stm32c0/coco/platform
    )
elseif(${PLATFORM} MATCHES "^stm32g0") # Cortex-M0+
    file(GENERATE OUTPUT coco/platform/stm32g0.h CONTENT "#include <coco/platform/system/${PLATFORM}.h>")
    target_sources(${PROJECT_NAME}
        PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/cortex ${CMAKE_CURRENT_SOURCE_DIR}/stm32 ${CMAKE_CURRENT_SOURCE_DIR}/stm32g0 FILES
            cortex/coco/platform/compiler.hpp
            cortex/coco/platform/nvic.hpp
            cortex/coco/platform/scb.hpp
            cortex/coco/platform/system/cmsis_compiler.h
            cortex/coco/platform/system/cmsis_gcc.h
            cortex/coco/platform/system/cmsis_version.h
            cortex/coco/platform/system/core_cm0plus.h
            cortex/coco/platform/system/m-profile/cmsis_gcc_m.h
            cortex/coco/platform/system/m-profile/armv7m_mpu.h
            stm32/coco/platform/adcF0C0G0F3L4G4H5U3U5.hpp
            stm32/coco/platform/backupG0G4H5U3U5.hpp
            stm32/coco/platform/cacheC0G0F4L4G4.hpp
            stm32/coco/platform/canG0G4H5U3U5.hpp
            stm32/coco/platform/crcAll.hpp
            stm32/coco/platform/dacG0L4G4H5U3U5.hpp
            stm32/coco/platform/dmaF0L0C0G0F1L1F3L4G4.hpp
            stm32/coco/platform/extiC0G0H5U3U5.hpp
            stm32/coco/platform/flashC0G0L4G4U3.hpp
            stm32/coco/platform/gpio.hpp
            stm32/coco/platform/i2cF0C0G0F3L4G4H5U3U5.hpp
            stm32/coco/platform/jump.hpp
            stm32/coco/platform/rccC0G0.hpp
            stm32/coco/platform/reset.hpp
            stm32/coco/platform/rngG0F4L4G4H5U3U5.hpp
            stm32/coco/platform/spiF0C0G0F3L4G4.hpp
            stm32/coco/platform/timerAll.hpp
            stm32/coco/platform/uartF0L0C0G0F3L4G4H5U3U5.hpp
            stm32/coco/platform/usbdG0H5U3U5.hpp
            stm32g0/coco/platform/adc.hpp
            stm32g0/coco/platform/backup.hpp
            stm32g0/coco/platform/cache.hpp
            stm32g0/coco/platform/can.hpp
            stm32g0/coco/platform/crc.hpp
            stm32g0/coco/platform/comp.hpp
            stm32g0/coco/platform/dac.hpp
            stm32g0/coco/platform/dma.hpp
            stm32g0/coco/platform/exti.hpp
            stm32g0/coco/platform/flash.hpp
            stm32g0/coco/platform/i2c.hpp
            stm32g0/coco/platform/rcc.hpp
            stm32g0/coco/platform/rng.hpp
            stm32g0/coco/platform/spi.hpp
            stm32g0/coco/platform/timer.hpp
            stm32g0/coco/platform/uart.hpp
            stm32g0/coco/platform/usbd.hpp
            stm32g0/coco/platform/vref.hpp
            stm32g0/coco/platform/platform.hpp
            stm32g0/coco/platform/system/${PLATFORM}.h # obtain from https://github.com/STMicroelectronics/STM32CubeG4/tree/master/Drivers/CMSIS/Device/ST/STM32G4xx
            stm32g0/coco/platform/system/system_stm32g0xx.h
        PUBLIC FILE_SET generated_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR} FILES
            ${CMAKE_CURRENT_BINARY_DIR}/coco/platform/stm32g0.h
        PRIVATE
            stm32g0/coco/platform/platform.cpp
            stm32g0/coco/platform/system/startup_${PLATFORM}.s
    )
    target_include_directories(${PROJECT_NAME}
        PRIVATE
            ../coco/stm32/coco/platform
            ../coco/stm32g0/coco/platform
    )
elseif(${PLATFORM} MATCHES "^stm32f3") # Cortex-M4F
    file(GENERATE OUTPUT coco/platform/stm32f3.h CONTENT "#include <coco/platform/system/${PLATFORM}.h>")
    target_sources(${PROJECT_NAME}
        PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/cortex ${CMAKE_CURRENT_SOURCE_DIR}/stm32 ${CMAKE_CURRENT_SOURCE_DIR}/stm32f3 FILES
            cortex/coco/platform/compiler.hpp
            cortex/coco/platform/nvic.hpp
            cortex/coco/platform/scb.hpp
            cortex/coco/platform/system/cmsis_compiler.h
            cortex/coco/platform/system/cmsis_gcc.h
            cortex/coco/platform/system/cmsis_version.h
            cortex/coco/platform/system/core_cm4.h
            cortex/coco/platform/system/m-profile/cmsis_gcc_m.h
            cortex/coco/platform/system/m-profile/armv7m_mpu.h
            stm32/coco/platform/adcF0C0G0F3L4G4H5U3U5.hpp
            stm32/coco/platform/adcF3F4.hpp
            stm32/coco/platform/backupF0F3F4L4.hpp
            stm32/coco/platform/cacheF0F3.hpp
            stm32/coco/platform/crcAll.hpp
            stm32/coco/platform/dacF0F3F4.hpp
            stm32/coco/platform/dmaF0L0C0G0F1L1F3L4G4.hpp
            stm32/coco/platform/extiF0F3F4.hpp
            stm32/coco/platform/flashF0F3.hpp
            stm32/coco/platform/gpio.hpp
            stm32/coco/platform/i2cF0C0G0F3L4G4H5U3U5.hpp
            stm32/coco/platform/jump.hpp
            stm32/coco/platform/rccF0F1F3.hpp
            stm32/coco/platform/reset.hpp
            stm32/coco/platform/spiF0C0G0F3L4G4.hpp
            stm32/coco/platform/timerAll.hpp
            stm32/coco/platform/uartF0L0C0G0F3L4G4H5U3U5.hpp
            stm32/coco/platform/usbdF0C0F3G4L4.hpp
            stm32f3/coco/platform/adc.hpp
            stm32f3/coco/platform/backup.hpp
            stm32f3/coco/platform/cache.hpp
            stm32f3/coco/platform/dac.hpp
            stm32f3/coco/platform/dma.hpp
            stm32f3/coco/platform/exti.hpp
            stm32f3/coco/platform/flash.hpp
            stm32f3/coco/platform/i2c.hpp
            stm32f3/coco/platform/rcc.hpp
            stm32f3/coco/platform/spi.hpp
            stm32f3/coco/platform/timer.hpp
            stm32f3/coco/platform/uart.hpp
            stm32f3/coco/platform/usbd.hpp
            stm32f3/coco/platform/platform.hpp
            stm32f3/coco/platform/system/${PLATFORM}.h # obtain from https://github.com/STMicroelectronics/STM32CubeF3/tree/master/Drivers/CMSIS/Device/ST/STM32F3xx
            stm32f3/coco/platform/system/system_stm32f3xx.h
        PUBLIC FILE_SET generated_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR} FILES
            ${CMAKE_CURRENT_BINARY_DIR}/coco/platform/stm32f3.h
        PRIVATE
            stm32f3/coco/platform/system/startup_${PLATFORM}.s
    )
    target_include_directories(${PROJECT_NAME}
        PRIVATE
            ../coco/stm32/coco/platform
            ../coco/stm32f3/coco/platform
    )
elseif(${PLATFORM} MATCHES "^stm32f4") # Cortex-M4F
    file(GENERATE OUTPUT coco/platform/stm32f4.h CONTENT "#include <coco/platform/system/${PLATFORM}.h>")
    target_sources(${PROJECT_NAME}
        PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/cortex ${CMAKE_CURRENT_SOURCE_DIR}/stm32 ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4 FILES
            cortex/coco/platform/compiler.hpp
            cortex/coco/platform/nvic.hpp
            cortex/coco/platform/scb.hpp
            cortex/coco/platform/system/cmsis_compiler.h
            cortex/coco/platform/system/cmsis_gcc.h
            cortex/coco/platform/system/cmsis_version.h
            cortex/coco/platform/system/core_cm4.h
            cortex/coco/platform/system/m-profile/cmsis_gcc_m.h
            cortex/coco/platform/system/m-profile/armv7m_mpu.h
            stm32/coco/platform/adcF3F4.hpp
            stm32/coco/platform/backupF0F3F4L4.hpp
            stm32/coco/platform/cacheC0G0F4L4G4.hpp
            stm32/coco/platform/crcAll.hpp
            stm32/coco/platform/dacF0F3F4.hpp
            stm32/coco/platform/extiF0F3F4.hpp
            stm32/coco/platform/gpio.hpp
            stm32/coco/platform/i2cF1L1F4.hpp
            stm32/coco/platform/jump.hpp
            stm32/coco/platform/qspiF4L4G4.hpp
            stm32/coco/platform/reset.hpp
            stm32/coco/platform/rngG0F4L4G4H5U3U5.hpp
            stm32/coco/platform/spiF1L1F4.hpp
            stm32/coco/platform/timerAll.hpp
            stm32/coco/platform/uartF1L1F4.hpp
            stm32f4/coco/platform/adc.hpp
            stm32f4/coco/platform/backup.hpp
            stm32f4/coco/platform/cache.hpp
            stm32f4/coco/platform/dac.hpp
            stm32f4/coco/platform/dma.hpp
            stm32f4/coco/platform/exti.hpp
            stm32f4/coco/platform/flash.hpp
            stm32f4/coco/platform/i2c.hpp
            stm32f4/coco/platform/qspi.hpp
            stm32f4/coco/platform/rcc.hpp
            stm32f4/coco/platform/rng.hpp
            stm32f4/coco/platform/spi.hpp
            stm32f4/coco/platform/timer.hpp
            stm32f4/coco/platform/uart.hpp
            stm32f4/coco/platform/usbd.hpp
            stm32f4/coco/platform/platform.hpp
            stm32f4/coco/platform/system/${PLATFORM}.h # obtain from https://github.com/STMicroelectronics/cmsis_device_f4/tree/master/Include
            stm32f4/coco/platform/system/system_stm32f4xx.h
        PUBLIC FILE_SET generated_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR} FILES
            ${CMAKE_CURRENT_BINARY_DIR}/coco/platform/stm32f4.h
        PRIVATE
            stm32f4/coco/platform/system/startup_${PLATFORM}.s
    )
    target_include_directories(${PROJECT_NAME}
        PRIVATE
            ../coco/stm32/coco/platform
    )
elseif(${PLATFORM} MATCHES "^stm32l4") # Cortex-M4F
    file(GENERATE OUTPUT coco/platform/stm32l4.h CONTENT "#include <coco/platform/system/${PLATFORM}.h>")
    target_sources(${PROJECT_NAME}
        PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/cortex ${CMAKE_CURRENT_SOURCE_DIR}/stm32 ${CMAKE_CURRENT_SOURCE_DIR}/stm32l4 FILES
            cortex/coco/platform/compiler.hpp
            cortex/coco/platform/nvic.hpp
            cortex/coco/platform/scb.hpp
            cortex/coco/platform/system/cmsis_compiler.h
            cortex/coco/platform/system/cmsis_gcc.h
            cortex/coco/platform/system/cmsis_version.h
            cortex/coco/platform/system/core_cm4.h
            cortex/coco/platform/system/m-profile/cmsis_gcc_m.h
            cortex/coco/platform/system/m-profile/armv7m_mpu.h
            stm32/coco/platform/adcF0C0G0F3L4G4H5U3U5.hpp
            stm32/coco/platform/backupF0F3F4L4.hpp
            stm32/coco/platform/cacheC0G0F4L4G4.hpp
            stm32/coco/platform/crcAll.hpp
            stm32/coco/platform/dacG0L4G4H5U3U5.hpp
            stm32/coco/platform/dmaF0L0C0G0F1L1F3L4G4.hpp
            stm32/coco/platform/extiL4G4.hpp
            stm32/coco/platform/flashC0G0L4G4U3.hpp
            stm32/coco/platform/gpio.hpp
            stm32/coco/platform/i2cF0C0G0F3L4G4H5U3U5.hpp
            stm32/coco/platform/jump.hpp
            stm32/coco/platform/qspiF4L4G4.hpp
            stm32/coco/platform/rccL4G4.hpp
            stm32/coco/platform/reset.hpp
            stm32/coco/platform/rngG0F4L4G4H5U3U5.hpp
            stm32/coco/platform/spiF0C0G0F3L4G4.hpp
            stm32/coco/platform/timerAll.hpp
            stm32/coco/platform/uartF0L0C0G0F3L4G4H5U3U5.hpp
            stm32/coco/platform/usbdF0C0F3G4L4.hpp
            stm32l4/coco/platform/adc.hpp
            stm32l4/coco/platform/backup.hpp
            stm32l4/coco/platform/cache.hpp
            stm32l4/coco/platform/crc.hpp
            stm32l4/coco/platform/dac.hpp
            stm32l4/coco/platform/dma.hpp
            stm32l4/coco/platform/exti.hpp
            stm32l4/coco/platform/flash.hpp
            stm32l4/coco/platform/i2c.hpp
            stm32l4/coco/platform/qspi.hpp
            stm32l4/coco/platform/rcc.hpp
            stm32l4/coco/platform/rng.hpp
            stm32l4/coco/platform/spi.hpp
            stm32l4/coco/platform/timer.hpp
            stm32l4/coco/platform/uart.hpp
            stm32l4/coco/platform/usbd.hpp
            stm32l4/coco/platform/platform.hpp
            stm32l4/coco/platform/system/${PLATFORM}.h # obtain from https://github.com/STMicroelectronics/cmsis_device_f4/tree/master/Include
            stm32l4/coco/platform/system/system_stm32l4xx.h
        PUBLIC FILE_SET generated_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR} FILES
            ${CMAKE_CURRENT_BINARY_DIR}/coco/platform/stm32l4.h
        PRIVATE
            stm32l4/coco/platform/system/startup_${PLATFORM}.s
    )
    target_include_directories(${PROJECT_NAME}
        PRIVATE
            ../coco/stm32/coco/platform
            ../coco/stm32l4/coco/platform
    )
elseif(${PLATFORM} MATCHES "^stm32g4") # Cortex-M4F
    file(GENERATE OUTPUT coco/platform/stm32g4.h CONTENT "#include <coco/platform/system/${PLATFORM}.h>")
    target_sources(${PROJECT_NAME}
        PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/cortex ${CMAKE_CURRENT_SOURCE_DIR}/stm32 ${CMAKE_CURRENT_SOURCE_DIR}/stm32g4 FILES
            cortex/coco/platform/compiler.hpp
            cortex/coco/platform/nvic.hpp
            cortex/coco/platform/scb.hpp
            cortex/coco/platform/system/cmsis_compiler.h
            cortex/coco/platform/system/cmsis_gcc.h
            cortex/coco/platform/system/cmsis_version.h
            cortex/coco/platform/system/core_cm4.h
            cortex/coco/platform/system/m-profile/cmsis_gcc_m.h
            cortex/coco/platform/system/m-profile/armv7m_mpu.h
            stm32/coco/platform/adcF0C0G0F3L4G4H5U3U5.hpp
            stm32/coco/platform/adcG4H5.hpp
            stm32/coco/platform/backupG0G4H5U3U5.hpp
            stm32/coco/platform/cacheC0G0F4L4G4.hpp
            stm32/coco/platform/canG0G4H5U3U5.hpp
            stm32/coco/platform/cordicG4H5U5.hpp
            stm32/coco/platform/crcAll.hpp
            stm32/coco/platform/dacG0L4G4H5U3U5.hpp
            stm32/coco/platform/dmaF0L0C0G0F1L1F3L4G4.hpp
            stm32/coco/platform/extiL4G4.hpp
            stm32/coco/platform/flashC0G0L4G4U3.hpp
            stm32/coco/platform/gpio.hpp
            stm32/coco/platform/i2cF0C0G0F3L4G4H5U3U5.hpp
            stm32/coco/platform/jump.hpp
            stm32/coco/platform/qspiF4L4G4.hpp
            stm32/coco/platform/rccL4G4.hpp
            stm32/coco/platform/reset.hpp
            stm32/coco/platform/rngG0F4L4G4H5U3U5.hpp
            stm32/coco/platform/spiF0C0G0F3L4G4.hpp
            stm32/coco/platform/timerAll.hpp
            stm32/coco/platform/uartF0L0C0G0F3L4G4H5U3U5.hpp
            stm32/coco/platform/usbdF0C0F3G4L4.hpp
            stm32g4/coco/platform/adc.hpp
            stm32g4/coco/platform/backup.hpp
            stm32g4/coco/platform/cache.hpp
            stm32g4/coco/platform/can.hpp
            stm32g4/coco/platform/comp.hpp
            stm32g4/coco/platform/cordic.hpp
            stm32g4/coco/platform/crc.hpp
            stm32g4/coco/platform/dac.hpp
            stm32g4/coco/platform/dma.hpp
            stm32g4/coco/platform/flash.hpp
            stm32g4/coco/platform/exti.hpp
            stm32g4/coco/platform/i2c.hpp
            stm32g4/coco/platform/opamp.hpp
            stm32g4/coco/platform/qspi.hpp
            stm32g4/coco/platform/rcc.hpp
            stm32g4/coco/platform/rng.hpp
            stm32g4/coco/platform/spi.hpp
            stm32g4/coco/platform/timer.hpp
            stm32g4/coco/platform/uart.hpp
            stm32g4/coco/platform/usbd.hpp
            stm32g4/coco/platform/vref.hpp
            stm32g4/coco/platform/platform.hpp
            stm32g4/coco/platform/system/${PLATFORM}.h # obtain from https://github.com/STMicroelectronics/STM32CubeG4/tree/master/Drivers/CMSIS/Device/ST/STM32G4xx
            stm32g4/coco/platform/system/system_stm32g4xx.h
        PUBLIC FILE_SET generated_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR} FILES
            ${CMAKE_CURRENT_BINARY_DIR}/coco/platform/stm32g4.h
        PRIVATE
            stm32g4/coco/platform/system/startup_${PLATFORM}.s
    )
    target_include_directories(${PROJECT_NAME}
        PRIVATE
            ../coco/stm32/coco/platform
            ../coco/stm32g4/coco/platform
    )
elseif(${PLATFORM} MATCHES "^stm32h5") # Cortex-M33F
    file(GENERATE OUTPUT coco/platform/stm32h5.h CONTENT "#include <coco/platform/system/${PLATFORM}.h>")
    target_sources(${PROJECT_NAME}
        PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/cortex ${CMAKE_CURRENT_SOURCE_DIR}/stm32 ${CMAKE_CURRENT_SOURCE_DIR}/stm32h5 FILES
            cortex/coco/platform/compiler.hpp
            cortex/coco/platform/nvic.hpp
            cortex/coco/platform/scb.hpp
            cortex/coco/platform/system/cmsis_compiler.h
            cortex/coco/platform/system/cmsis_gcc.h
            cortex/coco/platform/system/cmsis_version.h
            cortex/coco/platform/system/core_cm33.h
            cortex/coco/platform/system/m-profile/cmsis_gcc_m.h
            cortex/coco/platform/system/m-profile/armv8m_mpu.h
            stm32/coco/platform/adcF0C0G0F3L4G4H5U3U5.hpp
            stm32/coco/platform/adcG4H5.hpp
            stm32/coco/platform/backupG0G4H5U3U5.hpp
            stm32/coco/platform/cacheH5U3U5.hpp
            stm32/coco/platform/canG0G4H5U3U5.hpp
            stm32/coco/platform/cordicG4H5U5.hpp
            stm32/coco/platform/crcAll.hpp
            stm32/coco/platform/dacG0L4G4H5U3U5.hpp
            stm32/coco/platform/dmaH5U3U5.hpp
            stm32/coco/platform/extiC0G0H5U3U5.hpp
            stm32/coco/platform/flashH5U5.hpp
            stm32/coco/platform/gpio.hpp
            stm32/coco/platform/i2cF0C0G0F3L4G4H5U3U5.hpp
            stm32/coco/platform/jump.hpp
            stm32/coco/platform/rccH5.hpp
            stm32/coco/platform/reset.hpp
            stm32/coco/platform/rngG0F4L4G4H5U3U5.hpp
            stm32/coco/platform/spiH5U3U5.hpp
            stm32/coco/platform/timerAll.hpp
            stm32/coco/platform/uartF0L0C0G0F3L4G4H5U3U5.hpp
            stm32/coco/platform/usbdG0H5U3U5.hpp
            stm32h5/coco/platform/adc.hpp
            stm32h5/coco/platform/backup.hpp
            stm32h5/coco/platform/cache.hpp
            stm32h5/coco/platform/comp.hpp
            stm32h5/coco/platform/cordic.hpp
            stm32h5/coco/platform/crc.hpp
            stm32h5/coco/platform/dac.hpp
            stm32h5/coco/platform/dma.hpp
            stm32h5/coco/platform/exti.hpp
            stm32h5/coco/platform/flash.hpp
            stm32h5/coco/platform/i2c.hpp
            stm32h5/coco/platform/opamp.hpp
            stm32h5/coco/platform/rcc.hpp
            stm32h5/coco/platform/rng.hpp
            stm32h5/coco/platform/spi.hpp
            stm32h5/coco/platform/timer.hpp
            stm32h5/coco/platform/uart.hpp
            stm32h5/coco/platform/usbd.hpp
            stm32h5/coco/platform/platform.hpp
            stm32h5/coco/platform/system/${PLATFORM}.h
            stm32h5/coco/platform/system/system_stm32h5xx.h
        PUBLIC FILE_SET generated_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR} FILES
            ${CMAKE_CURRENT_BINARY_DIR}/coco/platform/stm32h5.h
        PRIVATE
            stm32h5/coco/platform/system/startup_${PLATFORM}.s
    )
    target_include_directories(${PROJECT_NAME}
        PRIVATE
            ../coco/stm32/coco/platform
            ../coco/stm32h5/coco/platform
    )
elseif(${PLATFORM} MATCHES "^stm32u3") # Cortex-M33F
    file(GENERATE OUTPUT coco/platform/stm32u3.h CONTENT "#include <coco/platform/system/${PLATFORM}.h>")
    target_sources(${PROJECT_NAME}
        PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/cortex ${CMAKE_CURRENT_SOURCE_DIR}/stm32 ${CMAKE_CURRENT_SOURCE_DIR}/stm32u3 FILES
            cortex/coco/platform/compiler.hpp
            cortex/coco/platform/nvic.hpp
            cortex/coco/platform/scb.hpp
            cortex/coco/platform/system/cmsis_compiler.h
            cortex/coco/platform/system/cmsis_gcc.h
            cortex/coco/platform/system/cmsis_version.h
            cortex/coco/platform/system/core_cm33.h
            cortex/coco/platform/system/m-profile/cmsis_gcc_m.h
            cortex/coco/platform/system/m-profile/armv8m_mpu.h
            stm32/coco/platform/adcF0C0G0F3L4G4H5U3U5.hpp
            stm32/coco/platform/backupG0G4H5U3U5.hpp
            stm32/coco/platform/cacheH5U3U5.hpp
            stm32/coco/platform/canG0G4H5U3U5.hpp
            stm32/coco/platform/crcAll.hpp
            stm32/coco/platform/dacG0L4G4H5U3U5.hpp
            stm32/coco/platform/dmaH5U3U5.hpp
            stm32/coco/platform/extiC0G0H5U3U5.hpp
            stm32/coco/platform/flashC0G0L4G4U3.hpp
            stm32/coco/platform/gpio.hpp
            stm32/coco/platform/i2cF0C0G0F3L4G4H5U3U5.hpp
            stm32/coco/platform/jump.hpp
            stm32/coco/platform/rccU3U5.hpp
            stm32/coco/platform/reset.hpp
            stm32/coco/platform/rngG0F4L4G4H5U3U5.hpp
            stm32/coco/platform/spiH5U3U5.hpp
            stm32/coco/platform/timerAll.hpp
            stm32/coco/platform/uartF0L0C0G0F3L4G4H5U3U5.hpp
            stm32/coco/platform/usbdG0H5U3U5.hpp
            stm32u3/coco/platform/adc.hpp
            stm32u3/coco/platform/backup.hpp
            stm32u3/coco/platform/cache.hpp
            stm32u3/coco/platform/comp.hpp
            stm32u3/coco/platform/crc.hpp
            stm32u3/coco/platform/dac.hpp
            stm32u3/coco/platform/dma.hpp
            stm32u3/coco/platform/exti.hpp
            stm32u3/coco/platform/flash.hpp
            stm32u3/coco/platform/i2c.hpp
            stm32u3/coco/platform/opamp.hpp
            stm32u3/coco/platform/rcc.hpp
            stm32u3/coco/platform/rng.hpp
            stm32u3/coco/platform/spi.hpp
            stm32u3/coco/platform/timer.hpp
            stm32u3/coco/platform/uart.hpp
            stm32u3/coco/platform/usbd.hpp
            stm32u3/coco/platform/platform.hpp
            stm32u3/coco/platform/system/${PLATFORM}.h
            stm32u3/coco/platform/system/system_stm32u3xx.h
        PUBLIC FILE_SET generated_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR} FILES
            ${CMAKE_CURRENT_BINARY_DIR}/coco/platform/stm32u3.h
        PRIVATE
            stm32u3/coco/platform/system/startup_${PLATFORM}.s
    )
    target_include_directories(${PROJECT_NAME}
        PRIVATE
            ../coco/stm32/coco/platform
            ../coco/stm32u3/coco/platform
    )
elseif(${PLATFORM} MATCHES "^nrf5" OR ${PLATFORM} MATCHES "^stm32")
    # other NRF5x or STM32 not supported yet
    message("${PLATFORM} not supported yet")
else()
    # native platform (Windows, MacOS, Linux)
    target_sources(${PROJECT_NAME}
        PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/native FILES
            native/coco/platform/compiler.hpp
            native/coco/platform/File.hpp
    )

    # if platform is "native", implement debug interface using std::cout. All other platforms have to bring their own debug.cpp
    if(${PLATFORM} STREQUAL "native")
        target_sources(${PROJECT_NAME}
            PRIVATE
                native/coco/platform/debug.cpp
        )
    endif()
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ..
        cortex/coco/platform/system
        ${CMAKE_CURRENT_BINARY_DIR} # include generated headers
)

# install the library
install(TARGETS ${PROJECT_NAME}
    FILE_SET headers DESTINATION include/coco
    FILE_SET platform_headers DESTINATION include # all platform headers (also from /cortex/ or /stm32g0/ etc.) go into include/coco/platform
    FILE_SET generated_headers DESTINATION include # generated headers go into include/coco/platform
)
