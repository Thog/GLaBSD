# ONLY USED FOR CLION PROJECT
cmake_minimum_required(VERSION 3.3)
project(GLaBSD)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
enable_language(CXX)
file(GLOB SRC kernel/arch/*/*.c kernel/kernel/*.c libc/*/*.c)
include_directories(kernel/include)
include_directories(libc/include)
add_executable(GLaBSD ${SRC})