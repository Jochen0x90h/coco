add_library(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
	PUBLIC FILE_SET headers TYPE HEADERS FILES
		Array.hpp	
		assert.hpp
		convert.hpp
		Coroutine.hpp
		debug.hpp
		enum.hpp
		IsArray.hpp
		IsSubclass.hpp
		LinkedList.hpp
		StreamOperators.hpp
		String.hpp
		StringBuffer.hpp
		Time.hpp
	PRIVATE
		convert.cpp
)

if(${PLATFORM} STREQUAL "native")
	# native platform (Windows, MacOS, Linux)
	target_sources(${PROJECT_NAME}
		PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/native FILES
			native/Handler.hpp
		PRIVATE
			native/Handler.cpp
	)
elseif(${PLATFORM} MATCHES "^nrf52")
	file(GENERATE OUTPUT nrf52.h CONTENT "#include <coco/platform/system/${PLATFORM}.h>\n#include <coco/platform/system/${PLATFORM}_bitfields.h>\n#include <coco/platform/system/${PLATFORM}_peripherals.h>")
	target_sources(${PROJECT_NAME}
		PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/nrf52 FILES
			nrf52/gpio.hpp
			nrf52/platform.hpp
			nrf52/Handler.hpp
			nrf52/system/cmsis_compiler.h
			nrf52/system/cmsis_gcc.h
			nrf52/system/cmsis_version.h
			nrf52/system/core_cm4.h
			nrf52/system/mpu_armv7.h
			nrf52/system/${PLATFORM}_bitfields.h
			nrf52/system/${PLATFORM}_peripherals.h
			nrf52/system/${PLATFORM}.h
			nrf52/system/system_${PLATFORM}.h
		PUBLIC FILE_SET generated_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR} FILES
			${CMAKE_CURRENT_BINARY_DIR}/nrf52.h
		PRIVATE
			nrf52/Handler.cpp
			nrf52/system/gcc_startup_${PLATFORM}.S
	)
elseif(${PLATFORM} MATCHES "^stm32f0")
	file(GENERATE OUTPUT stm32f0.h CONTENT "#include <coco/platform/system/${PLATFORM}.h>")
	target_sources(${PROJECT_NAME}
		PUBLIC FILE_SET platform_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/stm32f0 FILES
			stm32f0/gpio.hpp
			stm32f0/platform.hpp
			stm32f0/Handler.hpp
			stm32f0/system/cmsis_compiler.h
			stm32f0/system/cmsis_gcc.h
			stm32f0/system/cmsis_version.h
			stm32f0/system/core_cm0.h
			stm32f0/system/${PLATFORM}.h
			stm32f0/system/system_stm32f0xx.h
		PUBLIC FILE_SET generated_headers TYPE HEADERS BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR} FILES
			${CMAKE_CURRENT_BINARY_DIR}/stm32f0.h
		PRIVATE
			stm32f0/Handler.cpp
			stm32f0/system/startup_${PLATFORM}.S
	)
endif()

target_include_directories(${PROJECT_NAME}
	PUBLIC
		..
)

# install the library
install(TARGETS ${PROJECT_NAME}
	FILE_SET headers DESTINATION include/coco
	FILE_SET platform_headers DESTINATION include/coco/platform
	FILE_SET generated_headers DESTINATION include/coco/platform
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
