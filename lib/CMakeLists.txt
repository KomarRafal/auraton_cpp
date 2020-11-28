cmake_minimum_required(VERSION 3.13)

project(auraton)

add_library(serialib)

target_sources(serialib
	PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/serialib/lib/serialib.cpp
)

target_include_directories(serialib
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/serialib/lib>
)