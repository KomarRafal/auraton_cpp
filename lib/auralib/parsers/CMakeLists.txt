cmake_minimum_required (VERSION 3.13)

project (auraton)
add_compile_options(-Wall -Wextra -pedantic -Werror)

add_library(parserlib)

target_sources(parserlib
	PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/at.cpp
		${CMAKE_CURRENT_LIST_DIR}/command.cpp
		${CMAKE_CURRENT_LIST_DIR}/command_parser.cpp
		${CMAKE_CURRENT_LIST_DIR}/parser_executor.cpp
        ${CMAKE_CURRENT_LIST_DIR}/simple_token.cpp
		${CMAKE_CURRENT_LIST_DIR}/source.cpp
        ${CMAKE_CURRENT_LIST_DIR}/specific_source.cpp
		${CMAKE_CURRENT_LIST_DIR}/status.cpp
)

target_include_directories(parserlib
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/>
)

set_target_properties(parserlib
                      PROPERTIES
                      CXX_STANDARD 17
                      CXX_EXTENSIONS OFF
                      CXX_STANDARD_REQUIRED ON
                      )