cmake_minimum_required(VERSION 3.1)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(GeneratorTest src/generator_test.cpp )
target_link_libraries(GeneratorTest PUBLIC ${GTEST_BOTH_LIBRARIES} Threads::Threads )
