cmake_minimum_required(VERSION 3.10)
project(SimuladorHidrometro)

file(GLOB SOURCES src/*.cpp)

add_executable(SimuladorHidrometro ${SOURCES})
target_include_directories(SimuladorHidrometro PRIVATE include)