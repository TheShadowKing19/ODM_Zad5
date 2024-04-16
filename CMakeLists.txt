cmake_minimum_required(VERSION 3.28)
project(ODM_Zad5)

set(CMAKE_CXX_STANDARD 17)
set(GCC_COVERAGE_COMPILE_FLAGS "-O3") # Optymalizacja
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

add_executable(ODM_Zad5 main.cpp)
