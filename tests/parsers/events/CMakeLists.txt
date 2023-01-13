cmake_minimum_required(VERSION 3.16)
project(auraton_test)

set(UT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)
include (${UT_DIR}/common.mk)

add_executable(specific_event_parser_ut specific_event_parser_ut.cpp)					  
target_link_libraries(specific_event_parser_ut gtest gtest_main parserlib)
add_test(
    NAME specific_event_parser_ut
    COMMAND specific_event_parser_ut
)

add_executable(event_link_ut event_link_ut.cpp)					  
target_link_libraries(event_link_ut gtest gtest_main parserlib)
add_test(
    NAME event_link_ut
    COMMAND event_link_ut
)

set_target_properties(specific_event_parser_ut
                      event_link_ut
                      PROPERTIES
                      CXX_STANDARD 17
                      CXX_EXTENSIONS OFF
                      CXX_STANDARD_REQUIRED ON
            )
