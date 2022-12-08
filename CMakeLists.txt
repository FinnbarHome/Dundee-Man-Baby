cmake_minimum_required(VERSION 3.24)
project(Dundee_Man_Baby)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Dundee_Man_Baby
        simulator.cpp
        Simulator.h)
