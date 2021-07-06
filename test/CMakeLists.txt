cmake_minimum_required(VERSION 3.10)


include(GoogleTest)
find_package(GTest REQUIRED)

find_package(Threads REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(GeneratorTest src/generator_test.cpp )
target_link_libraries(GeneratorTest PUBLIC gtest Threads::Threads )

gtest_discover_tests(GeneratorTest)
