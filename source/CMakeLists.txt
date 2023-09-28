cmake_minimum_required(VERSION 3.22)
project(CompMath)


set(CMAKE_CXX_STANDARD 23)

add_library(CompMath INTERFACE)
target_include_directories(CompMath INTERFACE differentiation/differentiation.hpp)
