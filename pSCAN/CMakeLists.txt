cmake_minimum_required(VERSION 3.8)
project(pSCAN)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        Graph.cpp
        Graph.h
        main.cpp
        Utility.cpp
        Utility.h)

add_executable(pSCAN ${SOURCE_FILES})