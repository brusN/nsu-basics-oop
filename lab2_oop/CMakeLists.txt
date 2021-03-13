cmake_minimum_required(VERSION 3.17)
project(lab2_ooop)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(lib/googletests)
include_directories(lib/googletests/googletest/include)

add_executable(main src/main.cpp src/parser.cpp src/worker.cpp src/fabrica.cpp)
add_executable(Google_Tests_run test/tests.cpp src/parser.cpp)

target_link_libraries(Google_Tests_run gtest gtest_main)
