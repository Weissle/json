cmake_minimum_required(VERSION 3.10)


include(GoogleTest)
find_package(GTest REQUIRED)

find_package(Threads REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)
message(STATUS ${CMAKE_SOURCE_DIR})

aux_source_directory(${CMAKE_SOURCE_DIR}/test/src ALL_TEST_CPP)

message(STATUS ${ALL_TEST_CPP})
message(STATUS ${ALL_JSON_CPP})
add_executable(ALL_Test ${ALL_TEST_CPP} ${CPP_FILES})
target_link_libraries(ALL_Test PUBLIC gtest gtest_main Threads::Threads wjson)

gtest_discover_tests(ALL_Test)
