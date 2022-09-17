cmake_minimum_required(VERSION 3.16)
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

add_subdirectory(${AURATON_SRC}/parsers parserlib)

enable_testing()

add_executable(cmd_device_list_ut cmd_device_list_ut.cpp)					  
target_link_libraries(cmd_device_list_ut gtest gtest_main parserlib)
add_test(
    NAME cmd_device_list_ut
    COMMAND cmd_device_list_ut
)

add_executable(cmd_device_parameter_ut cmd_device_parameter_ut.cpp)					  
target_link_libraries(cmd_device_parameter_ut gtest gtest_main parserlib)
add_test(
    NAME cmd_device_parameter_ut
    COMMAND cmd_device_parameter_ut
)

add_executable(cmd_test_ut cmd_test_ut.cpp)					  
target_link_libraries(cmd_test_ut gtest gtest_main parserlib)
add_test(
    NAME cmd_test_ut
    COMMAND cmd_test_ut
)

add_executable(cmd_version_ut cmd_version_ut.cpp)					  
target_link_libraries(cmd_version_ut gtest gtest_main parserlib)
add_test(
    NAME cmd_version_ut
    COMMAND cmd_version_ut
)

add_executable(cmd_address_ut cmd_address_ut.cpp)					  
target_link_libraries(cmd_address_ut gtest gtest_main parserlib)
add_test(
    NAME cmd_address_ut
    COMMAND cmd_address_ut
)

add_executable(cmd_xtal_correction_ut cmd_xtal_correction_ut.cpp)					  
target_link_libraries(cmd_xtal_correction_ut gtest gtest_main parserlib)
add_test(
    NAME cmd_xtal_correction_ut
    COMMAND cmd_xtal_correction_ut
)

add_executable(specific_command_parser_ut specific_command_parser_ut.cpp)					  
target_link_libraries(specific_command_parser_ut gtest gtest_main parserlib)
add_test(
    NAME specific_command_parser_ut
    COMMAND specific_command_parser_ut
)

set_target_properties(cmd_device_list_ut
                      cmd_device_parameter_ut
                      cmd_test_ut
                      cmd_version_ut
                      cmd_address_ut
                      cmd_xtal_correction_ut
                      specific_command_parser_ut
                      PROPERTIES
                      CXX_STANDARD 17
                      CXX_EXTENSIONS OFF
                      CXX_STANDARD_REQUIRED ON
            )
