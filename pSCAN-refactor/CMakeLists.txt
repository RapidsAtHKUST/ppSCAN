cmake_minimum_required(VERSION 3.8)
project(pSCAN)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        main.cpp Graph.cpp Graph.h
        Utility.cpp Utility.h InputOutput.cpp InputOutput.h)

add_executable(io_play play/graph_io.cpp Utility.cpp)
add_executable(other_play play/play.cpp)

add_executable(pSCAN ${SOURCE_FILES})
target_compile_definitions(pSCAN PRIVATE _DEBUG_=1 FOO=1)
target_compile_options(pSCAN PRIVATE -O3)