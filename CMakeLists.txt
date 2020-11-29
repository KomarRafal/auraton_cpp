cmake_minimum_required (VERSION 3.13)

add_subdirectory(lib)

project (auraton)

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

add_executable(auraton example.cpp)
target_link_libraries(auralib serialib)
target_link_libraries(auraton auralib)