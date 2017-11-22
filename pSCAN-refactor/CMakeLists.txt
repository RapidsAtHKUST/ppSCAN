cmake_minimum_required(VERSION 3.6)
project(ppSCAN)

set(CMAKE_CXX_STANDARD 11)

# setup pthread environmental variables
find_package(Threads REQUIRED)

# usgae(knl): cmake .. -DKNL=ON
# usgae(avx2): cmake .. -DKNL=OFF -DAVX2=ON
# usage(sse): cmake .. -DKNL=OFF -DAVX2=OFF
if (KNL)
    message(KNL)
elseif (AVX2)
    message(AVX2)
else ()
    message(NOT_KNL,NOT_AVX2)
endif ()

## ppSCAN release 0: serial
set(SERIAL_SOURCE_FILES experimental/main_exp_serial.cpp experimental/GraphSeqExp.cpp InputOutput.cpp DisjointSet.cpp)
add_executable(pSCANSerial ${SERIAL_SOURCE_FILES})
target_compile_definitions(pSCANSerial PRIVATE SERIAL=1)
target_compile_options(pSCANSerial PRIVATE -O3 -g)

## ppSCAN experimental: galloping search
set(GALLOPING_FILES experimental/main_exp_parallel_galloping.cpp experimental/GraphGallopingSearchExp.cpp experimental/GraphGallopingSearchExp.h
        InputOutput.cpp)
add_executable(pSCANGalloping ${GALLOPING_FILES})
target_compile_options(pSCANGalloping PRIVATE -O3 -g)
target_link_libraries(pSCANGalloping ${CMAKE_THREAD_LIBS_INIT})

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
if (KNL OR AVX2)
    message(REDAY_SIMD)
    add_executable(pSCANParallelAVX2 ${SOURCE_FILES})
    target_compile_definitions(pSCANParallelAVX2 PRIVATE ENABLE_AVX2=1)
    target_compile_options(pSCANParallelAVX2 PRIVATE -O3 -g -march=core-avx2)
    target_link_libraries(pSCANParallelAVX2 ${CMAKE_THREAD_LIBS_INIT})

    add_executable(pSCANParallelAVX2Merge ${SOURCE_FILES})
    target_compile_definitions(pSCANParallelAVX2Merge PRIVATE ENABLE_AVX2_MERGE=1)
    target_compile_options(pSCANParallelAVX2Merge PRIVATE -O3 -g -march=core-avx2)
    target_link_libraries(pSCANParallelAVX2Merge ${CMAKE_THREAD_LIBS_INIT})
endif ()

## ppSCAN release 4: parallel with avx512
if (KNL)
    add_executable(pSCANParallelAVX512 ${SOURCE_FILES})
    target_compile_definitions(pSCANParallelAVX512 PRIVATE ENABLE_AVX512=1)
    target_compile_options(pSCANParallelAVX512 PRIVATE -O3 -g -march=native)
    target_link_libraries(pSCANParallelAVX512 ${CMAKE_THREAD_LIBS_INIT})
endif ()

## ppSCAN release 4: parallel with avx512 merge
if (KNL)
    add_executable(pSCANParallelAVX512Merge ${SOURCE_FILES})
    target_compile_definitions(pSCANParallelAVX512Merge PRIVATE ENABLE_AVX512_MERGE=1)
    target_compile_options(pSCANParallelAVX512Merge PRIVATE -O3 -g -march=native)
    target_link_libraries(pSCANParallelAVX512Merge ${CMAKE_THREAD_LIBS_INIT})
endif ()

## ppSCAN serial experimental 0: statistics 0
add_executable(pSCANStatistics0 ${SERIAL_SOURCE_FILES})
target_compile_definitions(pSCANStatistics0 PRIVATE SERIAL=1 STATISTICS=1)
target_compile_options(pSCANStatistics0 PRIVATE -O3 -g)

## ppSCAN serial experimental 1: naive-set-intersection
add_executable(pSCANSerialNaive ${SERIAL_SOURCE_FILES})
target_compile_definitions(pSCANSerialNaive PRIVATE SERIAL=1 NAIVE_SET_INTERSECTION=1)
target_compile_options(pSCANSerialNaive PRIVATE -O3 -g)

## ppSCAN parallel experimental 0: avx2 enabled pivot-set-intersection
set(EXP_FILES experimental/main_exp_parallel.cpp experimental/GraphParallelExp.cpp experimental/GraphParallelExp.h
        InputOutput.cpp InputOutput.h DisjointSet.cpp DisjointSet.h ThreadPool.h)

if (KNL OR AVX2)
    add_executable(pSCANParallelExp0 ${EXP_FILES})
    target_compile_options(pSCANParallelExp0 PRIVATE -O3 -g -march=core-avx2)
    target_compile_definitions(pSCANParallelExp0 PRIVATE AVX2_PIVOT_SET_INTERSECTION=1)
    target_link_libraries(pSCANParallelExp0 ${CMAKE_THREAD_LIBS_INIT})
endif ()

if (KNL)
    add_executable(pSCANParallelExp0AVX512 ${EXP_FILES})
    target_compile_options(pSCANParallelExp0AVX512 PRIVATE -O3 -g -march=native)
    target_compile_definitions(pSCANParallelExp0AVX512 PRIVATE AVX512_PIVOT_SET_INTERSECTION=1)
    target_link_libraries(pSCANParallelExp0AVX512 ${CMAKE_THREAD_LIBS_INIT})
endif ()

## ppSCAN parallel experimental 1: naive-set-intersection
add_executable(pSCANParallelExp1 ${EXP_FILES})
target_compile_options(pSCANParallelExp1 PRIVATE -O3 -g)
target_compile_definitions(pSCANParallelExp1 PRIVATE NAIVE_SET_INTERSECTION=1)
target_link_libraries(pSCANParallelExp1 ${CMAKE_THREAD_LIBS_INIT})

## ppSCAN parallel experimental 1: pivot-set-intersection
add_executable(pSCANParallelExp2 ${EXP_FILES})
target_compile_options(pSCANParallelExp2 PRIVATE -O3 -g)
target_compile_definitions(pSCANParallelExp2 PRIVATE PIVOT_SET_INTERSECTION=1)
target_link_libraries(pSCANParallelExp2 ${CMAKE_THREAD_LIBS_INIT})

## 2007 original scan: reimplementation
add_subdirectory(experimental/scan_origin)

## 2016 pscan: refactor implementaion, under GPLv3 license
add_subdirectory(experimental/pscan_experimental)

## 2017 anySCAN: reimplementation
add_subdirectory(experimental/anyscan)

## utility 1: check binary file, and some playground codes
add_subdirectory(playground)

## utility2: graoph converter edge list to binary files
add_subdirectory(converter)