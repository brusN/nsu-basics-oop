cmake_minimum_required(VERSION 3.17)
project(lab1_ooop)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(lib/googletests)
include_directories(lib/googletests/googletest/include)

add_executable(main src/main.cpp src/tritset.cpp src/trit.cpp src/tritvalue.cpp)
add_executable(Google_Tests_run test/tests.cpp src/tritset.cpp src/tritvalue.cpp src/trit.cpp)

target_link_libraries(Google_Tests_run gtest gtest_main)