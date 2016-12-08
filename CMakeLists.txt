cmake_minimum_required(VERSION 3.0)

project(graphix-walkthrough)

set(executable_name graphix)

include_directories(utilities/include)

add_compile_options(-std=c++11)

add_executable(${executable_name} main.cpp utilities/src/logger.cpp)
