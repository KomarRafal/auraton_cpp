cmake_minimum_required(VERSION 3.16)
project(auraton_test)

set(UT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)
include (${UT_DIR}/common.mk)

add_subdirectory(${AURATON_SRC}/parsers parserlib)

add_executable(cmd_test_ut cmd_test_ut.cpp)					  
target_link_libraries(cmd_test_ut gtest gtest_main parserlib)
add_test(
    NAME cmd_test_ut
    COMMAND cmd_test_ut
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

set_target_properties(cmd_test_ut
                      cmd_xtal_correction_ut
                      specific_command_parser_ut
                      PROPERTIES
                      CXX_STANDARD 17
                      CXX_EXTENSIONS OFF
                      CXX_STANDARD_REQUIRED ON
            )
