cmake_minimum_required(VERSION 3.6)
project(pSCAN)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Graph.cpp Graph.h InputOutput.cpp InputOutput.h
        DisjointSet.cpp DisjointSet.h ThreadPool.h)

find_package(Threads REQUIRED)

# playgound 0, playgound 1
add_executable(other_play playground/play.cpp)
add_executable(io_play playground/graph_io.cpp)

# utility 0: converter, transform edge list txt file into b_degree.bin, b_adj.bin
add_executable(converter converter/EdgeListToBinsConverter.cpp)
target_compile_options(converter PRIVATE -O3 -g)
target_compile_definitions(converter PRIVATE WITHGPERFTOOLS=1)
target_link_libraries(converter profiler)

# utility 1: binary_file_checker, check b_degree.bin, b_adj.bin
add_executable(binary_file_checker playground/check_binary_files.cpp InputOutput.cpp)
target_compile_options(binary_file_checker PRIVATE -O3 -g)
target_compile_definitions(binary_file_checker PRIVATE WITHGPERFTOOLS=1)
target_link_libraries(binary_file_checker profiler)

# pSCAN release 0: serial
add_executable(pSCANSerial ${SOURCE_FILES})
target_compile_definitions(pSCANSerial PRIVATE WITHGPERFTOOLS=1 SERIAL=1)
target_compile_options(pSCANSerial PRIVATE -O3 -g)
target_link_libraries(pSCANSerial profiler)

# pSCAN release 1: parallel
add_executable(pSCANParallel ${SOURCE_FILES})
target_compile_definitions(pSCANParallel PRIVATE WITHGPERFTOOLS=1)
target_compile_options(pSCANParallel PRIVATE -O3 -g)
target_link_libraries(pSCANParallel ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(pSCANParallel profiler)

# pSCAN experimental 0: serial statistics 0
add_executable(pSCANStatistics0 ${SOURCE_FILES})
target_compile_definitions(pSCANStatistics0 PRIVATE WITHGPERFTOOLS=1 SERIAL=1 STATISTICS=1)
target_compile_options(pSCANStatistics0 PRIVATE -O3 -g)
target_link_libraries(pSCANStatistics0 profiler)

# pSCAN parallel experimental 0
set(EXP_FILES experimental/main_exp_parallel.cpp experimental/GraphParallelExp.cpp experimental/GraphParallelExp.h
        InputOutput.cpp InputOutput.h DisjointSet.cpp DisjointSet.h ThreadPool.h)
add_executable(pSCANParallelExp0 ${EXP_FILES})
target_compile_options(pSCANParallelExp0 PRIVATE -O3 -g)
target_link_libraries(pSCANParallelExp0 ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(pSCANParallelExp0 profiler)