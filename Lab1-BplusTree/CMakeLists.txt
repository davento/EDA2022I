cmake_minimum_required(VERSION 3.10)
project(BplusTree)

set(CMAKE_CXX_STANDARD)

include_directories(./src)
add_executable(compilado
    src/main.cpp
)