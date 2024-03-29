cmake_minimum_required(VERSION 3.16)
project(auraton_test)

set(UT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
include (${UT_DIR}/common.mk)

add_subdirectory(commands)
add_subdirectory(events)

add_library(atparserlib)

target_sources(atparserlib
	PRIVATE
		${AURATON_SRC}/parsers/at.cpp
)

target_include_directories(atparserlib
	PUBLIC
		$<BUILD_INTERFACE:${AURATON_SRC}/parsers/>
)

add_library(attraverselib)

target_sources(attraverselib
	PRIVATE
		${AURATON_SRC}/parsers/at_traverse.cpp
)

target_include_directories(attraverselib
	PUBLIC
		$<BUILD_INTERFACE:${AURATON_SRC}/parsers/>
)

add_library(simplenumericlib)

target_sources(simplenumericlib
	PRIVATE
		${AURATON_SRC}/parsers/simple_numeric.cpp
		${AURATON_SRC}/parsers/simple_token.cpp
)

target_include_directories(simplenumericlib
	PUBLIC
		$<BUILD_INTERFACE:${AURATON_SRC}/parsers/>
)

add_library(simpletokenlib)

target_sources(simpletokenlib
	PRIVATE
		${AURATON_SRC}/parsers/simple_token.cpp
)

target_include_directories(simpletokenlib
	PUBLIC
		$<BUILD_INTERFACE:${AURATON_SRC}/parsers/>
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
		$<BUILD_INTERFACE:${AURATON_SRC}/parsers/>
)

add_library(statusparserlib)

target_sources(statusparserlib
	PRIVATE
		${AURATON_SRC}/parsers/status.cpp
		${AURATON_SRC}/parsers/simple_token.cpp
)

target_include_directories(statusparserlib
	PUBLIC
		$<BUILD_INTERFACE:${AURATON_SRC}/parsers/>
)

add_library(tokentraverselib)

target_sources(tokentraverselib
	PRIVATE
		${AURATON_SRC}/parsers/token_traverse.cpp
)

target_include_directories(tokentraverselib
	PUBLIC
		$<BUILD_INTERFACE:${AURATON_SRC}/parsers/>
)

add_library(clearparserlib)

target_sources(clearparserlib
	PRIVATE
		${AURATON_SRC}/parsers/clear.cpp
		${AURATON_SRC}/parsers/simple_token.cpp
		${AURATON_SRC}/parsers/specific_source.cpp
		${AURATON_SRC}/parsers/source.cpp
		${AURATON_SRC}/parsers/status.cpp
		${AURATON_SRC}/parsers/at.cpp
		${AURATON_SRC}/parsers/parser_executor.cpp
)

target_include_directories(clearparserlib
	PUBLIC
		$<BUILD_INTERFACE:${AURATON_SRC}/parsers/>
)

add_library(nextdeviceidlib)

target_sources(nextdeviceidlib
	PRIVATE
		${AURATON_SRC}/parsers/next_device_id.cpp
		${AURATON_SRC}/parsers/token_traverse.cpp
)

target_include_directories(nextdeviceidlib
	PUBLIC
		$<BUILD_INTERFACE:${AURATON_SRC}/parsers/>
)

add_library(nextparameterlib)

target_sources(nextparameterlib
	PRIVATE
		${AURATON_SRC}/parsers/next_parameter.cpp
		${AURATON_SRC}/parsers/token_traverse.cpp
)

target_include_directories(nextparameterlib
	PUBLIC
		$<BUILD_INTERFACE:${AURATON_SRC}/parsers/>
)

add_library(parserexecutorlib)

target_sources(parserexecutorlib
	PRIVATE
	    ${AURATON_SRC}/parsers/parser_executor.cpp
	    ${AURATON_SRC}/parsers/at.cpp
	    ${AURATON_SRC}/parsers/source.cpp
	    ${AURATON_SRC}/parsers/specific_source.cpp
	    ${AURATON_SRC}/parsers/status.cpp
	    ${AURATON_SRC}/parsers/command.cpp
        ${AURATON_SRC}/parsers/commands/specific_command.cpp
)

target_include_directories(parserexecutorlib
	PUBLIC
		$<BUILD_INTERFACE:${AURATON_SRC}/parsers/>
)

target_link_libraries(parserexecutorlib atparserlib specificsourcelib statusparserlib)

add_executable(at_parser_ut at_parser_ut.cpp)					  
target_link_libraries(at_parser_ut gtest gtest_main atparserlib)
add_test(
    NAME at_parser_ut
    COMMAND at_parser_ut
)

add_executable(at_traverse_ut at_traverse_ut.cpp)					  
target_link_libraries(at_traverse_ut gtest gtest_main attraverselib)
add_test(
    NAME at_traverse_ut
    COMMAND at_traverse_ut
)

add_executable(specific_source_parser_ut specific_source_parser_ut.cpp)					  
target_link_libraries(specific_source_parser_ut gtest gtest_main specificsourcelib)
add_test(
    NAME specific_source_parser_ut
    COMMAND specific_source_parser_ut
)

add_executable(simple_numeric_ut simple_numeric_ut.cpp)					  
target_link_libraries(simple_numeric_ut gtest gtest_main simplenumericlib)
add_test(
    NAME simple_numeric_ut
    COMMAND simple_numeric_ut
)	

add_executable(simple_token_ut simple_token_ut.cpp)					  
target_link_libraries(simple_token_ut gtest gtest_main simpletokenlib)
add_test(
    NAME simple_token_ut
    COMMAND simple_token_ut
)	

add_executable(status_parser_ut status_parser_ut.cpp)					  
target_link_libraries(status_parser_ut gtest gtest_main statusparserlib)
add_test(
    NAME status_parser_ut
    COMMAND status_parser_ut
)			  

add_executable(token_traverse_ut token_traverse_ut.cpp)					  
target_link_libraries(token_traverse_ut gtest gtest_main tokentraverselib)
add_test(
    NAME token_traverse_ut
    COMMAND token_traverse_ut
)	

add_executable(clear_parser_ut clear_parser_ut.cpp)					  
target_link_libraries(clear_parser_ut gtest gtest_main clearparserlib)
add_test(
    NAME clear_parser_ut
    COMMAND clear_parser_ut
)

add_executable(next_device_id_ut next_device_id_ut.cpp)					  
target_link_libraries(next_device_id_ut gtest gtest_main nextdeviceidlib)
add_test(
    NAME next_device_id_ut
    COMMAND next_device_id_ut
)

add_executable(next_parameter_ut next_parameter_ut.cpp)					  
target_link_libraries(next_parameter_ut gtest gtest_main nextparameterlib)
add_test(
    NAME next_parameter_ut
    COMMAND next_parameter_ut
)

add_executable(parser_executor_ut parser_executor_ut.cpp)					  
target_link_libraries(parser_executor_ut gtest gtest_main parserexecutorlib)
add_test(
    NAME parser_executor_ut
    COMMAND parser_executor_ut
)

set_target_properties(atparserlib
                      at_parser_ut
                      attraverselib
                      at_traverse_ut
                      simplenumericlib
                      simple_numeric_ut                      
                      simpletokenlib
                      simple_token_ut
                      specificsourcelib 
                      specific_source_parser_ut 
                      statusparserlib
                      status_parser_ut
                      tokentraverselib
                      token_traverse_ut
                      clearparserlib
                      clear_parser_ut
                      nextdeviceidlib
                      next_device_id_ut
                      nextparameterlib
                      next_parameter_ut
                      parserexecutorlib
                      parser_executor_ut
                      PROPERTIES
            CXX_STANDARD 17
            CXX_EXTENSIONS OFF
            CXX_STANDARD_REQUIRED ON
            )
