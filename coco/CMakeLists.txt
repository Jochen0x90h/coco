set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
    PUBLIC FILE_SET headers TYPE HEADERS FILES
        align.hpp
        Array.hpp
        ArrayBuffer.hpp
        ArrayConcept.hpp
        assert.hpp
        bits.hpp
        Callback.hpp
        ContainerConcept.hpp
        convert.hpp
        Coroutine.hpp
        CStringConcept.hpp
        debug.hpp
        enum.hpp
        Frequency.hpp
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
        Vector2.hpp
        Vector3.hpp
        Vector4.hpp
    PRIVATE
        convert.cpp
        debug.cpp
)

# if(${PLATFORM} STREQUAL "native")
#     # native platform (Windows, MacOS, Linux)
#     target_sources(${PROJECT_NAME}
#         PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/native FILES
#             native/coco/platform/File.hpp
#         PRIVATE
#             native/coco/platform/debug.cpp
#     )
# elseif(${PLATFORM} STREQUAL "emu")
#     # emulator (basic functionality implemented in coco-loop)
#     target_sources(${PROJECT_NAME}
#         PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/native ${CMAKE_CURRENT_SOURCE_DIR}/emu FILES
#             native/coco/platform/File.hpp
#         PRIVATE
#             emu/coco/platform/debug.cpp
#     )
if(${PLATFORM} MATCHES "^nrf52")
    file(GENERATE OUTPUT coco/platform/nrf52.h CONTENT "#include <coco/platform/system/${PLATFORM}.h>\n#include <coco/platform/system/${PLATFORM}_bitfields.h>\n#include <coco/platform/system/${PLATFORM}_peripherals.h>")
    target_sources(${PROJECT_NAME}
        PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/cortex ${CMAKE_CURRENT_SOURCE_DIR}/nrf52 FILES
            cortex/coco/platform/nvic.hpp
            cortex/coco/platform/scb.hpp
            cortex/coco/platform/system/cmsis_compiler.h
            cortex/coco/platform/system/cmsis_gcc.h
            cortex/coco/platform/system/cmsis_version.h
            cortex/coco/platform/system/core_cm4.h
            cortex/coco/platform/system/mpu_armv7.h
            nrf52/coco/platform/gpio.hpp
            nrf52/coco/platform/ppi.hpp
            nrf52/coco/platform/rtc.hpp
            nrf52/coco/platform/spi.hpp
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
elseif(${PLATFORM} MATCHES "^stm32f0")
    file(GENERATE OUTPUT coco/platform/stm32f0.h CONTENT "#include <coco/platform/system/${PLATFORM}.h>")
    target_sources(${PROJECT_NAME}
        PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/cortex ${CMAKE_CURRENT_SOURCE_DIR}/stm32 ${CMAKE_CURRENT_SOURCE_DIR}/stm32f0 FILES
            cortex/coco/platform/nvic.hpp
            cortex/coco/platform/scb.hpp
            cortex/coco/platform/system/cmsis_compiler.h
            cortex/coco/platform/system/cmsis_gcc.h
            cortex/coco/platform/system/cmsis_version.h
            cortex/coco/platform/system/core_cm0.h
            stm32/coco/platform/dmaInfoF0F3.hpp
            stm32/coco/platform/gpio.hpp
            stm32/coco/platform/rccInfoF0F3.hpp
            stm32/coco/platform/spiConfig.hpp
            stm32/coco/platform/timerInfo.hpp
            stm32/coco/platform/usartConfig.hpp
            stm32f0/coco/platform/adc.hpp
            stm32f0/coco/platform/dac.hpp
            stm32f0/coco/platform/exti.hpp
            stm32f0/coco/platform/dma.hpp
            stm32f0/coco/platform/i2c.hpp
            stm32f0/coco/platform/spi.hpp
            stm32f0/coco/platform/timer.hpp
            stm32f0/coco/platform/usart.hpp
            stm32f0/coco/platform/platform.hpp
            stm32f0/coco/platform/system/${PLATFORM}.h # obtain from https://github.com/STMicroelectronics/STM32CubeF0/tree/master/Drivers/CMSIS/Device/ST/STM32F0xx
            stm32f0/coco/platform/system/system_stm32f0xx.h
        PUBLIC FILE_SET generated_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR} FILES
            ${CMAKE_CURRENT_BINARY_DIR}/coco/platform/stm32f0.h
        PRIVATE
            stm32f0/coco/platform/platform.cpp
            stm32f0/coco/platform/system/startup_${PLATFORM}.s
    )
elseif(${PLATFORM} MATCHES "^stm32f3")
    file(GENERATE OUTPUT coco/platform/stm32f3.h CONTENT "#include <coco/platform/system/${PLATFORM}.h>")
    target_sources(${PROJECT_NAME}
        PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/cortex ${CMAKE_CURRENT_SOURCE_DIR}/stm32 ${CMAKE_CURRENT_SOURCE_DIR}/stm32f3 FILES
            cortex/coco/platform/nvic.hpp
            cortex/coco/platform/scb.hpp
            cortex/coco/platform/system/cmsis_compiler.h
            cortex/coco/platform/system/cmsis_gcc.h
            cortex/coco/platform/system/cmsis_version.h
            cortex/coco/platform/system/core_cm4.h
            stm32/coco/platform/dmaInfoF0F3.hpp
            stm32/coco/platform/gpio.hpp
            stm32/coco/platform/rccInfoF0F3.hpp
            stm32/coco/platform/spiConfig.hpp
            stm32/coco/platform/timerInfo.hpp
            stm32/coco/platform/usartConfig.hpp
            stm32f3/coco/platform/adc.hpp
            stm32f3/coco/platform/dac.hpp
            stm32f3/coco/platform/dma.hpp
            stm32f3/coco/platform/exti.hpp
            stm32f3/coco/platform/i2c.hpp
            stm32f3/coco/platform/spi.hpp
            stm32f3/coco/platform/timer.hpp
            stm32f3/coco/platform/usart.hpp
            stm32f3/coco/platform/platform.hpp
            stm32f3/coco/platform/system/${PLATFORM}.h # obtain from https://github.com/STMicroelectronics/STM32CubeF3/tree/master/Drivers/CMSIS/Device/ST/STM32F3xx
            stm32f3/coco/platform/system/system_stm32f3xx.h
        PUBLIC FILE_SET generated_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR} FILES
            ${CMAKE_CURRENT_BINARY_DIR}/coco/platform/stm32f3.h
        PRIVATE
            stm32f3/coco/platform/system/startup_${PLATFORM}.s
    )
elseif(${PLATFORM} MATCHES "^stm32f4")
    file(GENERATE OUTPUT coco/platform/stm32f4.h CONTENT "#include <coco/platform/system/${PLATFORM}.h>")
    target_sources(${PROJECT_NAME}
        PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/cortex ${CMAKE_CURRENT_SOURCE_DIR}/stm32 ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4 FILES
            cortex/coco/platform/nvic.hpp
            cortex/coco/platform/scb.hpp
            cortex/coco/platform/system/cmsis_compiler.h
            cortex/coco/platform/system/cmsis_gcc.h
            cortex/coco/platform/system/cmsis_version.h
            cortex/coco/platform/system/core_cm4.h
            cortex/coco/platform/system/mpu_armv7.h
            stm32/coco/platform/dmaInfoF4.hpp
            stm32/coco/platform/gpio.hpp
            stm32/coco/platform/rccInfoF4.hpp
            stm32/coco/platform/timerInfo.hpp
            stm32/coco/platform/usartConfig.hpp
            stm32f4/coco/platform/adc.hpp
            stm32f4/coco/platform/dac.hpp
            stm32f4/coco/platform/dma.hpp
            stm32f4/coco/platform/exti.hpp
            stm32f4/coco/platform/i2c.hpp
            stm32f4/coco/platform/spi.hpp
            stm32f4/coco/platform/timer.hpp
            stm32f4/coco/platform/usart.hpp
            stm32f4/coco/platform/platform.hpp
            stm32f4/coco/platform/system/${PLATFORM}.h # obtain from https://github.com/STMicroelectronics/cmsis_device_f4/tree/master/Include
            stm32f4/coco/platform/system/system_stm32f4xx.h
        PUBLIC FILE_SET generated_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR} FILES
            ${CMAKE_CURRENT_BINARY_DIR}/coco/platform/stm32f4.h
        PRIVATE
            stm32f4/coco/platform/system/startup_${PLATFORM}.s
    )
elseif(${PLATFORM} MATCHES "^stm32c0")
    file(GENERATE OUTPUT coco/platform/stm32c0.h CONTENT "#include <coco/platform/system/${PLATFORM}.h>")
    target_sources(${PROJECT_NAME}
        PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/cortex ${CMAKE_CURRENT_SOURCE_DIR}/stm32 ${CMAKE_CURRENT_SOURCE_DIR}/stm32c0 FILES
            cortex/coco/platform/nvic.hpp
            cortex/coco/platform/scb.hpp
            cortex/coco/platform/system/cmsis_compiler.h
            cortex/coco/platform/system/cmsis_gcc.h
            cortex/coco/platform/system/cmsis_version.h
            cortex/coco/platform/system/core_cm0plus.h
            cortex/coco/platform/system/mpu_armv7.h
            stm32/coco/platform/dmaInfoC0G4.hpp
            stm32/coco/platform/gpio.hpp
            stm32/coco/platform/rccInfoC0G4.hpp
            stm32/coco/platform/spiConfig.hpp
            stm32/coco/platform/timerInfo.hpp
            stm32/coco/platform/usartConfig.hpp
            stm32c0/coco/platform/adc.hpp
            stm32c0/coco/platform/dac.hpp
            stm32c0/coco/platform/dma.hpp
            stm32c0/coco/platform/exti.hpp
            stm32c0/coco/platform/i2c.hpp
            stm32c0/coco/platform/spi.hpp
            stm32c0/coco/platform/timer.hpp
            stm32c0/coco/platform/usart.hpp
            stm32c0/coco/platform/platform.hpp
            stm32c0/coco/platform/system/${PLATFORM}.h # obtain from https://github.com/STMicroelectronics/STM32CubeG4/tree/master/Drivers/CMSIS/Device/ST/STM32G4xx
            stm32c0/coco/platform/system/system_stm32c0xx.h
        PUBLIC FILE_SET generated_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR} FILES
            ${CMAKE_CURRENT_BINARY_DIR}/coco/platform/stm32c0.h
        PRIVATE
            stm32c0/coco/platform/platform.cpp
            stm32c0/coco/platform/system/startup_${PLATFORM}.s
    )
elseif(${PLATFORM} MATCHES "^stm32g4")
    file(GENERATE OUTPUT coco/platform/stm32g4.h CONTENT "#include <coco/platform/system/${PLATFORM}.h>")
    target_sources(${PROJECT_NAME}
        PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/cortex ${CMAKE_CURRENT_SOURCE_DIR}/stm32 ${CMAKE_CURRENT_SOURCE_DIR}/stm32g4 FILES
            cortex/coco/platform/nvic.hpp
            cortex/coco/platform/scb.hpp
            cortex/coco/platform/system/cmsis_compiler.h
            cortex/coco/platform/system/cmsis_gcc.h
            cortex/coco/platform/system/cmsis_version.h
            cortex/coco/platform/system/core_cm4.h
            cortex/coco/platform/system/mpu_armv7.h
            stm32/coco/platform/dmaInfoC0G4.hpp
            stm32/coco/platform/gpio.hpp
            stm32/coco/platform/rccInfoC0G4.hpp
            stm32/coco/platform/spiConfig.hpp
            stm32/coco/platform/timerInfo.hpp
            stm32/coco/platform/usartConfig.hpp
            stm32g4/coco/platform/adc.hpp
            stm32g4/coco/platform/dac.hpp
            stm32g4/coco/platform/dma.hpp
            stm32g4/coco/platform/exti.hpp
            stm32g4/coco/platform/i2c.hpp
            stm32g4/coco/platform/opamp.hpp
            stm32g4/coco/platform/spi.hpp
            stm32g4/coco/platform/timer.hpp
            stm32g4/coco/platform/usart.hpp
            stm32g4/coco/platform/vref.hpp
            stm32g4/coco/platform/platform.hpp
            stm32g4/coco/platform/system/${PLATFORM}.h # obtain from https://github.com/STMicroelectronics/STM32CubeG4/tree/master/Drivers/CMSIS/Device/ST/STM32G4xx
            stm32g4/coco/platform/system/system_stm32g4xx.h
        PUBLIC FILE_SET generated_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR} FILES
            ${CMAKE_CURRENT_BINARY_DIR}/coco/platform/stm32g4.h
        PRIVATE
            stm32g4/coco/platform/system/startup_${PLATFORM}.s
    )
elseif(${PLATFORM} MATCHES "^nrf5" OR ${PLATFORM} MATCHES "^stm32")
    # other NRF5x or STM32 not supported yet
    message("${PLATFORM} not supported yet")
else()
    # native platform (Windows, MacOS, Linux)
    target_sources(${PROJECT_NAME}
        PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/native FILES
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
        ${CMAKE_CURRENT_BINARY_DIR} # can include generated headers
)

# install the library
install(TARGETS ${PROJECT_NAME}
    FILE_SET headers DESTINATION include/coco
    FILE_SET platform_headers DESTINATION include # platform headers go into include/coco/platform
    FILE_SET generated_headers DESTINATION include # generated headers go into include/coco/platform
)
