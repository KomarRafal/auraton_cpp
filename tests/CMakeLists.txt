cmake_minimum_required(VERSION 3.18)
project(auraton_test)
include(FetchContent)

set(GTEST_VERSION 1.10.0 CACHE STRING "Google test version")

FetchContent_Declare(googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG release-${GTEST_VERSION})

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

FetchContent_MakeAvailable(googletest)

set(AURATON_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../lib/auralib)

add_library(parserlib)

target_sources(parserlib
	PRIVATE
		${AURATON_SRC}/parser.cpp
)

target_include_directories(parserlib
	PUBLIC
		$<BUILD_INTERFACE:${AURATON_SRC}/>
)

add_library(devicelib)

target_sources(devicelib
	PRIVATE
		${AURATON_SRC}/device.cpp
		${AURATON_SRC}/parser.cpp
)

target_include_directories(devicelib
	PUBLIC
		$<BUILD_INTERFACE:${AURATON_SRC}/>
)

set_target_properties(devicelib PROPERTIES
            CXX_STANDARD 17
            CXX_EXTENSIONS OFF
            )

enable_testing()

add_executable(parser_ut parser_ut.cpp)					  
target_link_libraries(parser_ut gtest gtest_main parserlib)
add_test(
    NAME parser_ut
    COMMAND parser_ut
)

add_executable(device_ut device_ut.cpp)
target_link_libraries(device_ut gtest gtest_main parserlib devicelib)
add_test(
    NAME device_ut
    COMMAND device_ut
)
