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
	target_include_directories(compileTest
		PRIVATE
			../coco/nrf52/coco/platform
	)
	target_link_libraries(compileTest
		${PROJECT_NAME}
	)
elseif(${PLATFORM} MATCHES "^stm32")
	add_executable(compileTest
		stm32/compileTest.cpp
	)
	target_include_directories(compileTest
		PRIVATE
			../coco/stm32/coco/platform
	)
	target_link_libraries(compileTest
		${PROJECT_NAME}
	)
endif()
