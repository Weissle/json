cmake_minimum_required(VERSION 3.1)

project(wjson)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(main main.cpp )

if(BUILD_TEST)
	add_subdirectory(${CMAKE_SOURCE_DIR}/test)
endif()
