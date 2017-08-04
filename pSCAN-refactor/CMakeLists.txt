cmake_minimum_required(VERSION 3.8)
project(pSCAN)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        Graph.cpp Graph.h
        main.cpp Utility.cpp Utility.h InputOutput.cpp InputOutput.h)

add_executable(pSCAN ${SOURCE_FILES})

add_executable(play play/graph_io.cpp Utility.cpp )
add_executable(play2 play/play.cpp)
target_compile_definitions(play PRIVATE _DEBUG_=1 FOO=1)
target_compile_options(play PRIVATE -O3)