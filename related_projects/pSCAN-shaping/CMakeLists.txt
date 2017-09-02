cmake_minimum_required(VERSION 3.6)
project(pSCANShaping)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Graph.cpp Graph.h InputOutput.cpp InputOutput.h
        DisjointSet.cpp DisjointSet.h MaxPriorityQueue.cpp MaxPriorityQueue.h ThreadPool.h)

add_executable(pSCANShaping ${SOURCE_FILES})
target_compile_definitions(pSCANShaping PRIVATE DEBUG=1 FOO=1 WITHGPERFTOOLS=1 SERIAL=1)
#target_compile_definitions(pSCANShaping PRIVATE DEBUG=1 FOO=1 WITHGPERFTOOLS=1)
target_compile_options(pSCANShaping PRIVATE -O3 -g)
target_link_libraries(pSCANShaping profiler)
target_link_libraries(pSCANShaping pthread)

