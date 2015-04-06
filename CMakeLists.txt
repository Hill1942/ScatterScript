cmake_minimum_required(VERSION 2.8.4)
project(ScatterScript)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

aux_source_directory(. SRC_LIST)
aux_source_directory(core CORE_SRC)
aux_source_directory(ssdk SSDK_SRC)

list(APPEND SRC_LIST ${CORE_SRC})
list(APPEND SRC_LIST ${SSDK_SRC})


add_executable(ss ${SRC_LIST})