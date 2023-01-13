cmake_minimum_required(VERSION 3.16)
project(auraton_test)

include(common.mk)

add_subdirectory(parsers)

add_library(parameterlib)

target_sources(parameterlib
	PRIVATE
		${AURATON_SRC}/parameter.cpp
)

target_include_directories(parameterlib
	PUBLIC
		$<BUILD_INTERFACE:${AURATON_SRC}/>
)

target_link_libraries(parameterlib attraverselib nextparameterlib simplenumericlib)

add_library(devicelib)

target_sources(devicelib
	PRIVATE
		${AURATON_SRC}/device.cpp
)

target_include_directories(devicelib
	PUBLIC
		$<BUILD_INTERFACE:${AURATON_SRC}/>
)

target_link_libraries(devicelib parserlib)

add_library(connectionlib)

target_sources(connectionlib
	PRIVATE
		${AURATON_SRC}/connection.cpp
)

target_include_directories(connectionlib
	PUBLIC
		${STUBS}/
		$<BUILD_INTERFACE:${AURATON_SRC}/>
)
target_link_libraries(connectionlib gmock)

add_library(commandlib)

target_sources(commandlib
	PRIVATE
		${AURATON_SRC}/command.cpp
)

target_include_directories(commandlib
	PUBLIC
		$<BUILD_INTERFACE:${AURATON_SRC}/>
)
target_link_libraries(commandlib gmock)

add_library(aurachiplib)

target_sources(aurachiplib
	PRIVATE
		${AURATON_SRC}/aurachip.cpp
)

target_include_directories(aurachiplib
	PUBLIC
		${STUBS}/
		$<BUILD_INTERFACE:${AURATON_SRC}/>
)
target_link_libraries(aurachiplib gmock connectionlib commandlib devicelib parserlib attraverselib clearparserlib)

add_executable(parameter_ut parameter_ut.cpp)					  
target_link_libraries(parameter_ut gtest gtest_main parameterlib parserlib)
add_test(
    NAME parameter_ut
    COMMAND parameter_ut
)

add_executable(device_ut device_ut.cpp)
target_link_libraries(device_ut gtest gtest_main devicelib)
           
add_test(
    NAME device_ut
    COMMAND device_ut
)

add_executable(connection_ut connection_ut.cpp)
target_link_libraries(connection_ut gtest gtest_main connectionlib)
add_test(
    NAME connection_ut
    COMMAND connection_ut
)

add_executable(command_ut command_ut.cpp)
target_link_libraries(command_ut gtest gtest_main commandlib)
add_test(
    NAME command_ut
    COMMAND command_ut
)

add_executable(aurachip_ut aurachip_ut.cpp)					  
target_link_libraries(aurachip_ut gtest gtest_main aurachiplib parameterlib)
add_test(
    NAME aurachip_ut
    COMMAND aurachip_ut
)

set_target_properties(device_ut 
                      connectionlib 
                      connection_ut 
                      aurachip_ut
                      parameter_ut
                      aurachiplib
                      devicelib
                      parameterlib
                      PROPERTIES
            CXX_STANDARD 17
            CXX_EXTENSIONS OFF
            CXX_STANDARD_REQUIRED ON
            )
