cmake_minimum_required(VERSION 3.16)
project(priority_queue_one)

set(CMAKE_CXX_STANDARD 20)

add_executable(pq
        priority_queue/src/exceptions.hpp
        priority_queue/src/utility.hpp
        priority_queue/src/priority_queue.hpp
        priority_queue/data/one/code.cpp)