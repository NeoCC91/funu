cmake_minimum_required(VERSION 3.24)
project(funu)

set(CMAKE_CXX_STANDARD 20)

add_executable(funu main.cpp src/base/vec.h src/base/halfedge.h src/base/mat.h)
