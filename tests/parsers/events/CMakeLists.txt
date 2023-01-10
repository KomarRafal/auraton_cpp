cmake_minimum_required(VERSION 3.16)
project(auraton_test)
include(FetchContent)

set(GTEST_VERSION 1.12.1 CACHE STRING "Google test version")
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

enable_testing()

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
