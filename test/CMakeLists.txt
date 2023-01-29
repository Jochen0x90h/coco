# unit tests for "normal" operating systems
add_executable(gTest
	CoroutineTest.cpp
	gTest.cpp
)
target_include_directories(gTest
	PRIVATE
	..
)
target_link_libraries(gTest
	${PROJECT_NAME}
	GTest::gtest
)

add_test(NAME gTest
	COMMAND gTest --gtest_output=xml:report.xml
	#WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../testdata
)
