cmake_minimum_required(VERSION 3.28)

project(funu)

set(CMAKE_CXX_STANDARD 23)

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIRS})

add_executable(funu main.cpp
        common/triangle_mesh.h
        common/triangle_mesh.cpp
        common/common_types.h)
