cmake_minimum_required(VERSION 3.8)
project(pSCANPlay0)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Graph.cpp Graph.h InputOutput.cpp InputOutput.h
        DisjointSet.cpp DisjointSet.h MaxPriorityQueue.cpp MaxPriorityQueue.h ThreadPool.h)

add_executable(pSCANPlay0 ${SOURCE_FILES})
target_compile_definitions(pSCANPlay0 PRIVATE DEBUG=1 FOO=1 WITHGPERFTOOLS=1)
target_compile_options(pSCANPlay0 PRIVATE -O3 -g)
target_link_libraries(pSCANPlay0 profiler)
target_link_libraries(pSCANPlay0 pthread)

