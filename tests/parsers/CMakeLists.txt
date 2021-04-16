cmake_minimum_required(VERSION 3.18)
project(auraton_test)
include(FetchContent)

add_subdirectory(commands)

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

set(AURATON_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/auralib)
set(STUBS ${CMAKE_CURRENT_SOURCE_DIR}/../stubs)

add_library(atparserlib)

target_sources(atparserlib
	PRIVATE
		${AURATON_SRC}/parsers/at.cpp
)

target_include_directories(atparserlib
	PUBLIC
		$<BUILD_INTERFACE:${AURATON_SRC}/>
)

add_library(specificsourcelib)

target_sources(specificsourcelib
	PRIVATE
		${AURATON_SRC}/parsers/specific_source.cpp
		${AURATON_SRC}/parsers/source.cpp
		${AURATON_SRC}/parsers/simple_token.cpp
)

target_include_directories(specificsourcelib
	PUBLIC
		$<BUILD_INTERFACE:${AURATON_SRC}/>
)

add_library(statusparserlib)

target_sources(statusparserlib
	PRIVATE
		${AURATON_SRC}/parsers/status.cpp
		${AURATON_SRC}/parsers/simple_token.cpp
)

target_include_directories(statusparserlib
	PUBLIC
		$<BUILD_INTERFACE:${AURATON_SRC}/>
)

add_library(commandparserlib)

target_sources(commandparserlib
	PRIVATE
        ${AURATON_SRC}/parsers/command.cpp
        ${AURATON_SRC}/parsers/command_parser.cpp
)

target_include_directories(commandparserlib
	PUBLIC
		$<BUILD_INTERFACE:${AURATON_SRC}/>
)

target_link_libraries(commandparserlib atparserlib specificsourcelib statusparserlib)

enable_testing()

add_executable(at_parser_ut at_parser_ut.cpp)					  
target_link_libraries(at_parser_ut gtest gtest_main atparserlib)
add_test(
    NAME at_parser_ut
    COMMAND at_parser_ut
)

add_executable(specific_source_parser_ut specific_source_parser_ut.cpp)					  
target_link_libraries(specific_source_parser_ut gtest gtest_main specificsourcelib)
add_test(
    NAME specific_source_parser_ut
    COMMAND specific_source_parser_ut
)

add_executable(status_parser_ut status_parser_ut.cpp)					  
target_link_libraries(status_parser_ut gtest gtest_main statusparserlib)
add_test(
    NAME status_parser_ut
    COMMAND status_parser_ut
)			  

add_executable(command_parser_ut command_parser_ut.cpp)					  
target_link_libraries(command_parser_ut gtest gtest_main commandparserlib)
add_test(
    NAME command_parser_ut
    COMMAND command_parser_ut
)

set_target_properties(atparserlib
                      at_parser_ut 
                      specificsourcelib 
                      specific_source_parser_ut 
                      statusparserlib
                      status_parser_ut
                      commandparserlib
                      command_parser_ut
                      PROPERTIES
            CXX_STANDARD 17
            CXX_EXTENSIONS OFF
            CXX_STANDARD_REQUIRED ON
            )