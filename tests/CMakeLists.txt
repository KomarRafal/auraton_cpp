cmake_minimum_required(VERSION 3.18)
project(auraton_test)
include(FetchContent)

add_subdirectory(parsers)

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

set(AURATON_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../lib/auralib)
set(STUBS ${CMAKE_CURRENT_SOURCE_DIR}/stubs)

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

add_library(connectionlib)

target_sources(connectionlib
	PRIVATE
		${AURATON_SRC}/connection.cpp
)

target_include_directories(connectionlib
	PUBLIC
		${STUBS}/
		$<BUILD_INTERFACE:${AURATON_SRC}/>
)
target_link_libraries(connectionlib gmock parserlib)

add_library(commandlib)

target_sources(commandlib
	PRIVATE
		${AURATON_SRC}/command.cpp
)

target_include_directories(commandlib
	PUBLIC
		$<BUILD_INTERFACE:${AURATON_SRC}/>
)
target_link_libraries(commandlib gmock)

add_library(aurachiplib)

target_sources(aurachiplib
	PRIVATE
		${AURATON_SRC}/aurachip.cpp
)

target_include_directories(aurachiplib
	PUBLIC
		${STUBS}/
		$<BUILD_INTERFACE:${AURATON_SRC}/>
)
target_link_libraries(aurachiplib gmock connectionlib commandlib devicelib)

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

add_executable(connection_ut connection_ut.cpp)
target_link_libraries(connection_ut gtest gtest_main connectionlib)
add_test(
    NAME connection_ut
    COMMAND connection_ut
)

add_executable(command_ut command_ut.cpp)
target_link_libraries(command_ut gtest gtest_main commandlib)
add_test(
    NAME command_ut
    COMMAND command_ut
)

add_executable(aurachip_ut aurachip_ut.cpp)					  
target_link_libraries(aurachip_ut gtest gtest_main aurachiplib)
add_test(
    NAME aurachip_ut
    COMMAND aurachip_ut
)

set_target_properties(device_ut 
                      connectionlib 
                      connection_ut 
                      aurachip_ut 
                      PROPERTIES
            CXX_STANDARD 17
            CXX_EXTENSIONS OFF
            CXX_STANDARD_REQUIRED ON
            )