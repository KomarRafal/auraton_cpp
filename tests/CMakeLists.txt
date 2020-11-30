cmake_minimum_required(VERSION 3.11)
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

enable_testing()

add_executable(auraton_ut parser_ut.cpp)
					  
target_link_libraries(auraton_ut gtest gtest_main parserlib)

add_test(
    NAME auraton_ut
    COMMAND auraton_ut
)

