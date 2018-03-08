cmake_minimum_required(VERSION 3.9)
project(Particle-Sim-Openmp)

File(GLOB SRC_FILES src/*.cpp)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-std=gnu++11 -lm -fopenmp" )

message(src: ${SRC_FILES})
add_executable(Project ${SRC_FILES})