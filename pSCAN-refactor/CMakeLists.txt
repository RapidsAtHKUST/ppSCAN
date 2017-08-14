cmake_minimum_required(VERSION 3.6)
project(pSCAN)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Graph.cpp Graph.h InputOutput.cpp InputOutput.h
        DisjointSet.cpp DisjointSet.h ThreadPool.h)

find_package(Threads REQUIRED)

add_executable(other_play play/play.cpp)

add_executable(io_play play/graph_io.cpp)

add_executable(converter converter/EdgeListToBinsConverter.cpp)
target_compile_options(converter PRIVATE -O3 -g)
#target_compile_definitions(converter PRIVATE WITHGPERFTOOLS=1)
#target_link_libraries(converter profiler)

add_executable(binary_file_checker play/check_binary_files.cpp InputOutput.cpp)
target_compile_options(binary_file_checker PRIVATE -O3 -g)
target_compile_definitions(binary_file_checker PRIVATE WITHGPERFTOOLS=1)
target_link_libraries(binary_file_checker profiler)

add_executable(pSCAN ${SOURCE_FILES})
#target_compile_definitions(pSCAN PRIVATE DEBUG=1 FOO=1 WITHGPERFTOOLS=1 STATISTICS=1)
target_compile_definitions(pSCAN PRIVATE DEBUG=1 FOO=1 WITHGPERFTOOLS=1)
#target_compile_definitions(pSCAN PRIVATE DEBUG=1 FOO=1)
target_compile_options(pSCAN PRIVATE -O3 -g)
target_link_libraries(pSCAN ${CMAKE_THREAD_LIBS_INIT})
#target_link_libraries(pSCAN profiler)




