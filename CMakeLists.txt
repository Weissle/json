cmake_minimum_required(VERSION 3.10)

project(wjson)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE WJSON_CPP_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_library( wjson STATIC ${WJSON_CPP_FILES} )

add_executable(main main.cpp)
target_link_libraries(main wjson)

if(BUILD_TEST)
	ENABLE_TESTING()
	add_subdirectory(${CMAKE_SOURCE_DIR}/test)
endif()
