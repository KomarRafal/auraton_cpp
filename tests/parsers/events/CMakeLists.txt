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

# TODO: make parserslib common for commands and events
add_library(parserslib2)

target_sources(parserslib2
	PRIVATE
		${AURATON_SRC}/parsers/commands/specific_command.cpp
		${AURATON_SRC}/parsers/commands/address.cpp
		${AURATON_SRC}/parsers/commands/test.cpp
		${AURATON_SRC}/parsers/commands/version.cpp
		${AURATON_SRC}/parsers/commands/xtal_correction.cpp
		${AURATON_SRC}/parsers/specific_source.cpp
		${AURATON_SRC}/parsers/at.cpp
		${AURATON_SRC}/parsers/command.cpp
        ${AURATON_SRC}/parsers/command_parser.cpp
		${AURATON_SRC}/parsers/parser_executor.cpp
		${AURATON_SRC}/parsers/source.cpp
		${AURATON_SRC}/parsers/status.cpp
		${AURATON_SRC}/parsers/simple_token.cpp
)

target_include_directories(parserslib2
	PUBLIC
		$<BUILD_INTERFACE:${AURATON_SRC}/parsers/>
)

add_library(eventslib)

target_sources(eventslib
	PRIVATE
		${AURATON_SRC}/parsers/events/event.cpp
		${AURATON_SRC}/parsers/events/direction.cpp
		${AURATON_SRC}/parsers/events/link.cpp
		${AURATON_SRC}/parsers/events/specific_event.cpp
)

target_include_directories(eventslib
	PUBLIC
		$<BUILD_INTERFACE:${AURATON_SRC}/parsers/>
)

enable_testing()

add_executable(specific_event_parser_ut specific_event_parser_ut.cpp)					  
target_link_libraries(specific_event_parser_ut gtest gtest_main eventslib parserslib2)
add_test(
    NAME specific_event_parser_ut
    COMMAND specific_event_parser_ut
)

add_executable(event_link_ut event_link_ut.cpp)					  
target_link_libraries(event_link_ut gtest gtest_main eventslib parserslib2)
add_test(
    NAME event_link_ut
    COMMAND event_link_ut
)

set_target_properties(parserslib2
                      eventslib
                      specific_event_parser_ut
                      event_link_ut
                      PROPERTIES
                      CXX_STANDARD 17
                      CXX_EXTENSIONS OFF
                      CXX_STANDARD_REQUIRED ON
            )