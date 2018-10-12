cmake_minimum_required(VERSION 3.1)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)

project(cache)

file(GLOB SOURCES "src/*.cpp")

add_executable(cache ${SOURCES})
