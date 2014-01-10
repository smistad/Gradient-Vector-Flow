cmake_minimum_required(VERSION 2.8)
project(GVF)

add_subdirectory(SIPL)
include_directories(${SIPL_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++0x -O3")

add_executable(GVF main.cpp gradientVectorFlow.cpp)
target_link_libraries(GVF SIPL)
