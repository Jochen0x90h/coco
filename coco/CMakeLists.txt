add_library(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
	PUBLIC FILE_SET headers TYPE HEADERS FILES
		align.hpp
		Array.hpp
		ArrayBuffer.hpp
		ArrayConcept.hpp
		assert.hpp
		Callback.hpp
		convert.hpp
		Coroutine.hpp
		CStringConcept.hpp
		debug.hpp
		enum.hpp
		Frequency.hpp
		IsSubclass.hpp
		LinkedList.hpp
		Queue.hpp
		PointerConcept.hpp
		PseudoRandom.hpp
		StreamOperators.hpp
		String.hpp
		StringBuffer.hpp
		StringConcept.hpp
		Task.hpp
		Time.hpp
		TimedTask.hpp
	PRIVATE
		convert.cpp
)

if(${PLATFORM} STREQUAL "native" OR ${PLATFORM} STREQUAL "emu")
	# native platform (Windows, MacOS, Linux)
	target_sources(${PROJECT_NAME}
		PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/native FILES
			native/coco/platform/File.hpp
		#PRIVATE
	)
elseif(${PLATFORM} MATCHES "^nrf52")
	file(GENERATE OUTPUT nrf52.h CONTENT "#include <coco/platform/system/${PLATFORM}.h>\n#include <coco/platform/system/${PLATFORM}_bitfields.h>\n#include <coco/platform/system/${PLATFORM}_peripherals.h>")
	target_sources(${PROJECT_NAME}
		PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/cortex ${CMAKE_CURRENT_SOURCE_DIR}/nrf52 FILES
			cortex/coco/platform/nvic.hpp
			cortex/coco/platform/system/cmsis_compiler.h
			cortex/coco/platform/system/cmsis_gcc.h
			cortex/coco/platform/system/cmsis_version.h
			cortex/coco/platform/system/core_cm4.h
			cortex/coco/platform/system/mpu_armv7.h
			nrf52/coco/platform/gpio.hpp
			nrf52/coco/platform/platform.hpp
			nrf52/coco/platform/system/${PLATFORM}_bitfields.h
			nrf52/coco/platform/system/${PLATFORM}_peripherals.h
			nrf52/coco/platform/system/${PLATFORM}.h
			nrf52/coco/platform/system/system_${PLATFORM}.h
		PUBLIC FILE_SET generated_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR} FILES
			${CMAKE_CURRENT_BINARY_DIR}/nrf52.h
		PRIVATE
			nrf52/coco/platform/system/gcc_startup_${PLATFORM}.S
	)
elseif(${PLATFORM} MATCHES "^stm32f0")
	file(GENERATE OUTPUT stm32f0.h CONTENT "#include <coco/platform/system/${PLATFORM}.h>")
	target_sources(${PROJECT_NAME}
		PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/cortex ${CMAKE_CURRENT_SOURCE_DIR}/stm32f0 FILES
			cortex/coco/platform/nvic.hpp
			cortex/coco/platform/system/cmsis_compiler.h
			cortex/coco/platform/system/cmsis_gcc.h
			cortex/coco/platform/system/cmsis_version.h
			cortex/coco/platform/system/core_cm0.h
			stm32f0/coco/platform/gpio.hpp
			stm32f0/coco/platform/platform.hpp
			stm32f0/coco/platform/system/${PLATFORM}.h
			stm32f0/coco/platform/system/system_stm32f0xx.h
		PUBLIC FILE_SET generated_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR} FILES
			${CMAKE_CURRENT_BINARY_DIR}/stm32f0.h
		PRIVATE
			stm32f0/coco/platform/system/startup_${PLATFORM}.S
	)
elseif(${PLATFORM} MATCHES "^stm32g4")
	file(GENERATE OUTPUT stm32g4.h CONTENT "#include <coco/platform/system/${PLATFORM}.h>")
	target_sources(${PROJECT_NAME}
		PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/cortex ${CMAKE_CURRENT_SOURCE_DIR}/stm32f0 ${CMAKE_CURRENT_SOURCE_DIR}/stm32g4 FILES
			cortex/coco/platform/nvic.hpp
			cortex/coco/platform/system/cmsis_compiler.h
			cortex/coco/platform/system/cmsis_gcc.h
			cortex/coco/platform/system/cmsis_version.h
			cortex/coco/platform/system/core_cm4.h
			cortex/coco/platform/system/mpu_armv7.h
			stm32f0/coco/platform/gpio.hpp
			stm32g4/coco/platform/platform.hpp
			stm32g4/coco/platform/system/${PLATFORM}.h
			stm32g4/coco/platform/system/system_stm32g4xx.h
		PUBLIC FILE_SET generated_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR} FILES
			${CMAKE_CURRENT_BINARY_DIR}/stm32g4.h
		PRIVATE
			stm32g4/coco/platform/system/startup_${PLATFORM}.S
	)
endif()

target_include_directories(${PROJECT_NAME}
	PUBLIC
		..
		cortex/coco/platform/system
)

# install the library
install(TARGETS ${PROJECT_NAME}
	FILE_SET headers DESTINATION include/coco
	FILE_SET platform_headers DESTINATION include # goes into include/coco/platform
	FILE_SET generated_headers DESTINATION include/coco/platform
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
