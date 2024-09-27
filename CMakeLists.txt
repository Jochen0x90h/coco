cmake_minimum_required(VERSION 3.19)

# set project
project(coco)

# configure languages
enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)

# build type
message("*** Type: ${CMAKE_BUILD_TYPE}${CMAKE_CONFIGURATION_TYPES}")
add_compile_definitions($<$<CONFIG:Debug>:DEBUG>)

# compiler
message("*** Compiler: ${CMAKE_CXX_COMPILER_ID}")
message("*** Crosscompiling: ${CMAKE_CROSSCOMPILING}")

# enable address sanitizer
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

# debug
#set(CMAKE_CXX_FLAGS_RELEASE "-g -Og")
#set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON" FORCE)

# platform
#message("*** OS: ${OS}")
if(NOT PLATFORM)
	set(PLATFORM native)
endif()
message("*** Platform: ${PLATFORM}")


add_subdirectory(coco)


# check if we are on a "normal" operating system such as Windows or Linux
if(NOT ${CMAKE_CROSSCOMPILING})
	find_package(GTest CONFIG)

	# enable testing, adds test or RUN_TESTS target to run all tests
	enable_testing()

endif()

add_subdirectory(test)
