cmake_minimum_required(VERSION 3.5)
project(algo2_trecu_basura)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES tests.cpp anillo.h)
add_executable(algo2_trecu_basura ${SOURCE_FILES})