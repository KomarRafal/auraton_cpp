cmake_minimum_required (VERSION 3.13)

project (auraton)
add_compile_options(-Wall -Wextra -pedantic -Werror)

add_library(auralib)

target_sources(auralib
	PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/aurachip.cpp
		${CMAKE_CURRENT_LIST_DIR}/connection.cpp
		${CMAKE_CURRENT_LIST_DIR}/device.cpp
		${CMAKE_CURRENT_LIST_DIR}/parser.cpp
)

target_include_directories(auralib
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/>
)