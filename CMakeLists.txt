cmake_minimum_required (VERSION 3.13)

add_subdirectory(lib)

project (auraton)

add_executable(auraton auraton.cpp aurachip.cpp example.cpp)
target_link_libraries(auraton serialib)