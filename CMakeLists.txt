cmake_minimum_required (VERSION 3.13)

add_subdirectory(lib)

project (auraton)

add_executable(auraton example.cpp)
target_link_libraries(auralib rs232lib)
target_link_libraries(auraton auralib)