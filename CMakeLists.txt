cmake_minimum_required(VERSION 3.16)
project(vector_one)

set(CMAKE_CXX_STANDARD 20)

add_executable(vector_one
        vector/src/exceptions.hpp
        vector/src/utility.hpp
        vector/src/vector.hpp
        vector/data/one/code.cpp)