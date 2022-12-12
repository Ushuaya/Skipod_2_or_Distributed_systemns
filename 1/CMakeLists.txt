cmake_minimum_required(VERSION 3.6)
project(FirstTask)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})
SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER mpicxx)

add_executable(Normal task1.cpp)
