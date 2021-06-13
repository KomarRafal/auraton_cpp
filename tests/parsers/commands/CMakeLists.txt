cmake_minimum_required(VERSION 3.18)
project(auraton_test)
include(FetchContent)

set(GTEST_VERSION 1.10.0 CACHE STRING "Google test version")
add_compile_options(-Wall -Wextra -pedantic -Werror)

FetchContent_Declare(googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG release-${GTEST_VERSION})

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

FetchContent_MakeAvailable(googletest)

set(AURATON_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/auralib)
set(STUBS ${CMAKE_CURRENT_SOURCE_DIR}/../../stubs)

add_library(parserslib)

target_sources(parserslib
	PRIVATE
		${AURATON_SRC}/parsers/commands/specific_command.cpp
		${AURATON_SRC}/parsers/commands/test.cpp
		${AURATON_SRC}/parsers/specific_source.cpp
		${AURATON_SRC}/parsers/at.cpp
		${AURATON_SRC}/parsers/command.cpp
        ${AURATON_SRC}/parsers/command_parser.cpp
		${AURATON_SRC}/parsers/parser_executor.cpp
		${AURATON_SRC}/parsers/source.cpp
		${AURATON_SRC}/parsers/status.cpp
		${AURATON_SRC}/parsers/simple_token.cpp
)

target_include_directories(parserslib
	PUBLIC
		$<BUILD_INTERFACE:${AURATON_SRC}/parsers/>
)

add_library(cmdtestlib)

target_sources(cmdtestlib
	PRIVATE
		${AURATON_SRC}/parsers/commands/test.cpp
		${AURATON_SRC}/parsers/commands/specific_command.cpp
)

target_include_directories(cmdtestlib
	PUBLIC
		$<BUILD_INTERFACE:${AURATON_SRC}/parsers/>
)

enable_testing()

add_executable(cmd_test_ut cmd_test_ut.cpp)					  
target_link_libraries(cmd_test_ut gtest gtest_main cmdtestlib parserslib)
add_test(
    NAME cmd_test_ut
    COMMAND cmd_test_ut
)

add_executable(specific_command_parser_ut specific_command_parser_ut.cpp)					  
target_link_libraries(specific_command_parser_ut gtest gtest_main parserslib)
add_test(
    NAME specific_command_parser_ut
    COMMAND specific_command_parser_ut
)

set_target_properties(cmdtestlib
                      parserslib
                      cmd_test_ut
                      specific_command_parser_ut
                      PROPERTIES
                      CXX_STANDARD 17
                      CXX_EXTENSIONS OFF
                      CXX_STANDARD_REQUIRED ON
            )