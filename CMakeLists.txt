cmake_minimum_required(VERSION 3.16)
project(map_one)

set(CMAKE_CXX_STANDARD 20)

include_directories(map
                    map/src)

add_executable(map_one
        map/src/exceptions.hpp
        map/src/utility.hpp
        map/src/map.hpp
        map/data/one/code.cpp)