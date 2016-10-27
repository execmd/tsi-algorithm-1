cmake_minimum_required(VERSION 3.3)
project(tsi_algorithm_1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    LICENSE
    README.md main.cpp)

add_executable(tsi_algorithm_1 ${SOURCE_FILES})