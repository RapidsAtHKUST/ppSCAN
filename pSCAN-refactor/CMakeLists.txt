cmake_minimum_required(VERSION 3.6)
project(ppSCAN)

set(CMAKE_CXX_STANDARD 11)

find_package(Threads REQUIRED)

## utility 1: check binary file, and some playground codes
add_subdirectory(playground)

## utility2: graoph converter edge list to binary files
add_subdirectory(converter)

## ppSCAN release 0: serial
set(SERIAL_SOURCE_FILES experimental/main_exp_serial.cpp experimental/GraphSeqExp.cpp InputOutput.cpp DisjointSet.cpp)
add_executable(pSCANSerial ${SERIAL_SOURCE_FILES})
#target_compile_definitions(pSCANSerial PRIVATE WITHGPERFTOOLS=1 SERIAL=1)
target_compile_definitions(pSCANSerial PRIVATE SERIAL=1)
target_compile_options(pSCANSerial PRIVATE -O3 -g)
#target_link_libraries(pSCANSerial profiler)

## ppSCAN release 1: parallel
set(SOURCE_FILES main.cpp Graph.cpp Graph.h InputOutput.cpp InputOutput.h
        DisjointSet.cpp DisjointSet.h ThreadPool.h Util.h ThreadSafeDisjointSet.h)
add_executable(pSCANParallel ${SOURCE_FILES})
target_compile_options(pSCANParallel PRIVATE -O3 -g)
target_link_libraries(pSCANParallel ${CMAKE_THREAD_LIBS_INIT})

## ppSCAN release 2: parallel with sse4.2
add_executable(pSCANParallelSSE ${SOURCE_FILES})
target_compile_definitions(pSCANParallelSSE PRIVATE ENABLE_SSE=1)
target_compile_options(pSCANParallelSSE PRIVATE -O3 -g -march=native)
target_link_libraries(pSCANParallelSSE ${CMAKE_THREAD_LIBS_INIT})

## ppSCAN release 3: parallel with avx2
add_executable(pSCANParallelAVX2 ${SOURCE_FILES})
target_compile_definitions(pSCANParallelAVX2 PRIVATE ENABLE_AVX2=1)
target_compile_options(pSCANParallelAVX2 PRIVATE -O3 -g -march=core-avx2)
target_link_libraries(pSCANParallelAVX2 ${CMAKE_THREAD_LIBS_INIT})

## ppSCAN serial experimental 0: statistics 0
add_executable(pSCANStatistics0 ${SERIAL_SOURCE_FILES})
target_compile_definitions(pSCANStatistics0 PRIVATE SERIAL=1 STATISTICS=1)
target_compile_options(pSCANStatistics0 PRIVATE -O3 -g)
#target_compile_definitions(pSCANStatistics0 PRIVATE WITHGPERFTOOLS=1 SERIAL=1 STATISTICS=1)
#target_link_libraries(pSCANStatistics0 profiler)

## ppSCAN serial experimental 1: naive-set-intersection
add_executable(pSCANSerialNaive ${SERIAL_SOURCE_FILES})
target_compile_definitions(pSCANSerialNaive PRIVATE SERIAL=1 NAIVE_SET_INTERSECTION=1)
target_compile_options(pSCANSerialNaive PRIVATE -O3 -g)

## ppSCAN parallel experimental 0
set(EXP_FILES experimental/main_exp_parallel.cpp experimental/GraphParallelExp.cpp experimental/GraphParallelExp.h
        InputOutput.cpp InputOutput.h DisjointSet.cpp DisjointSet.h ThreadPool.h)
add_executable(pSCANParallelExp0 ${EXP_FILES})
target_compile_options(pSCANParallelExp0 PRIVATE -O3 -g)
target_link_libraries(pSCANParallelExp0 ${CMAKE_THREAD_LIBS_INIT})

## ppSCAN parallel experimental 1: naive-set-intersection
add_executable(pSCANParallelExp1 ${EXP_FILES})
target_compile_options(pSCANParallelExp1 PRIVATE -O3 -g)
target_compile_definitions(pSCANParallelExp1 PRIVATE NAIVE_SET_INTERSECTION=1)
target_link_libraries(pSCANParallelExp1 ${CMAKE_THREAD_LIBS_INIT})

## pscan: refactor implementaion, under GPLv3 license
add_subdirectory(experimental/pscan_experimental)

## original scan: reimplementation
add_subdirectory(experimental/scan_origin)

## anySCAN
add_subdirectory(experimental/anyscan)