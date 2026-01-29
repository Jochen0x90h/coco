if(NOT ${CMAKE_CROSSCOMPILING})
    # unit tests for "normal" operating systems
    add_executable(gTest
        CoroutineTest.cpp
        gTest.cpp
        IntrusiveMpscQueueTest.cpp
        TaskTest.cpp
    )
    #target_include_directories(gTest
    #	PRIVATE
    #		..
    #)
    target_link_libraries(gTest
        ${PROJECT_NAME}
        GTest::gtest
    )

    add_test(NAME gTest
        COMMAND gTest --gtest_output=xml:report.xml
        #WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../testdata
    )
endif()

# check if platform dependent stuff compiles
if(${PLATFORM} MATCHES "^nrf52")
    add_executable(compileTest
        nrf52/compileTest.cpp
    )

    # we need these includes as all platform files are copied to /coco/platform in the installed lib
    target_include_directories(compileTest
        PRIVATE
            ../coco/cortex/coco/platform
            ../coco/nrf52/coco/platform
    )
    target_link_libraries(compileTest
        ${PROJECT_NAME}
    )
elseif(${PLATFORM} MATCHES "^stm32")
    add_executable(compileTest
        stm32/compileTest.cpp
    )

    # we need these includes as all platform files are copied to /coco/platform in the installed lib
    target_include_directories(compileTest
        PRIVATE
            ../coco/cortex/coco/platform
            ../coco/stm32/coco/platform
    )
    if(${PLATFORM} MATCHES "^stm32f0")
        target_include_directories(compileTest
            PRIVATE
                ../coco/stm32f0/coco/platform
        )
    elseif(${PLATFORM} MATCHES "^stm32c0")
        target_include_directories(compileTest
            PRIVATE
                ../coco/stm32c0/coco/platform
        )
    elseif(${PLATFORM} MATCHES "^stm32g0")
        target_include_directories(compileTest
            PRIVATE
                ../coco/stm32g0/coco/platform
        )
    elseif(${PLATFORM} MATCHES "^stm32f3")
        target_include_directories(compileTest
            PRIVATE
                ../coco/stm32f3/coco/platform
        )
    elseif(${PLATFORM} MATCHES "^stm32f4")
        target_include_directories(compileTest
            PRIVATE
                ../coco/stm32f4/coco/platform
        )
    elseif(${PLATFORM} MATCHES "^stm32l4")
        target_include_directories(compileTest
            PRIVATE
                ../coco/stm32l4/coco/platform
        )
    elseif(${PLATFORM} MATCHES "^stm32g4")
        target_include_directories(compileTest
            PRIVATE
                ../coco/stm32g4/coco/platform
        )
    elseif(${PLATFORM} MATCHES "^stm32h5")
        target_include_directories(compileTest
            PRIVATE
                ../coco/stm32h5/coco/platform
        )
    elseif(${PLATFORM} MATCHES "^stm32u3")
        target_include_directories(compileTest
            PRIVATE
                ../coco/stm32u3/coco/platform
        )
    endif()
    target_link_libraries(compileTest
        ${PROJECT_NAME}
    )
endif()
