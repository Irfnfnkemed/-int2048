cmake_minimum_required(VERSION 3.22)
project(int2048_2022)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(int2048_2022
        int2048.cpp
        int2048.h
        main.cpp)
