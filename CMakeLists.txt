cmake_minimum_required(VERSION 3.27)
project(CPU)

set(CMAKE_CXX_STANDARD 14)

add_executable(CPU
        main.cpp
        pcb.h
        pcb.cpp
        cpu.cpp
        utils.cpp)
