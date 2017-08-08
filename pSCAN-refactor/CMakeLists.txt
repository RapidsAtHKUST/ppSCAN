cmake_minimum_required(VERSION 3.8)
project(pSCAN)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp Graph.cpp Graph.h InputOutput.cpp InputOutput.h
        DisjointSet.cpp DisjointSet.h MaxPriorityQueue.cpp MaxPriorityQueue.h)

add_executable(io_play play/graph_io.cpp)
add_executable(other_play play/play.cpp)

add_executable(pSCAN ${SOURCE_FILES})
target_compile_definitions(pSCAN PRIVATE DEBUG=1 FOO=1 WITHGPERFTOOLS=1)
target_compile_options(pSCAN PRIVATE -O3 -g)
target_link_libraries(pSCAN profiler)

add_executable(converter converter/EdgeListToBinsConverter.cpp)
target_compile_options(converter PRIVATE -O3 -g)
