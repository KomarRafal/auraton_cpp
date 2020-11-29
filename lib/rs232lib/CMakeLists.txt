cmake_minimum_required(VERSION 3.13)

project(auraton)

add_library(rs232lib)

target_sources(rs232lib
	PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/serialib/lib/serialib.cpp
)

target_include_directories(rs232lib
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/serialib/lib>
)