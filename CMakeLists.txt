cmake_minimum_required(VERSION 2.8.4)
project(ScatterScript)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    scatter-asm/ssasm.cpp
    scatter-asm/ssasm.h
    scatter-asm/sstype.cpp
    scatter-asm/sstype.h
    LICENSE
    README.md)

include_directories(.)
include_directories(scatter-asm)

add_executable(ScatterScript ${SOURCE_FILES})