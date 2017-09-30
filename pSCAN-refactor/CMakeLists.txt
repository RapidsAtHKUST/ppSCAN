cmake_minimum_required(VERSION 3.6)
project(pSCAN)

set(CMAKE_CXX_STANDARD 11)

find_package(Threads REQUIRED)

# playground and utility 1: check binary file
add_subdirectory(playground)

# utility 0: converter, transform edge list txt file into b_degree.bin, b_adj.bin
add_executable(converter converter/EdgeListToBinsConverter.cpp)
target_compile_options(converter PRIVATE -O3 -g)
target_compile_definitions(converter PRIVATE WITHGPERFTOOLS=1)
target_link_libraries(converter profiler)

# ppSCAN release 0: serial
set(SERIAL_SOURCE_FILES experimental/main_exp_serial.cpp experimental/GraphSeqExp.cpp InputOutput.cpp DisjointSet.cpp)
add_executable(pSCANSerial ${SERIAL_SOURCE_FILES})
#target_compile_definitions(pSCANSerial PRIVATE WITHGPERFTOOLS=1 SERIAL=1)
target_compile_definitions(pSCANSerial PRIVATE SERIAL=1)
target_compile_options(pSCANSerial PRIVATE -O3 -g)
#target_link_libraries(pSCANSerial profiler)

# ppSCAN release 1: parallel
set(SOURCE_FILES main.cpp Graph.cpp Graph.h InputOutput.cpp InputOutput.h
        DisjointSet.cpp DisjointSet.h ThreadPool.h Util.h)
add_executable(pSCANParallel ${SOURCE_FILES})
#target_compile_definitions(pSCANParallel PRIVATE WITHGPERFTOOLS=1)
target_compile_definitions(pSCANParallel PRIVATE)
target_compile_options(pSCANParallel PRIVATE -O3 -g)
target_link_libraries(pSCANParallel ${CMAKE_THREAD_LIBS_INIT})
#target_link_libraries(pSCANParallel profiler)

# ppSCAN serial experimental 0: statistics 0
add_executable(pSCANStatistics0 ${SERIAL_SOURCE_FILES})
target_compile_definitions(pSCANStatistics0 PRIVATE WITHGPERFTOOLS=1 SERIAL=1 STATISTICS=1)
target_compile_options(pSCANStatistics0 PRIVATE -O3 -g)
target_link_libraries(pSCANStatistics0 profiler)

# ppSCAN serial experimental 1: naive-set-intersection
add_executable(pSCANSerialNaive ${SERIAL_SOURCE_FILES})
target_compile_definitions(pSCANSerialNaive PRIVATE SERIAL=1 NAIVE_SET_INTERSECTION=1)
target_compile_options(pSCANSerialNaive PRIVATE -O3 -g)
#target_link_libraries(pSCANSerialNaive profiler)

# ppSCAN parallel experimental 0
set(EXP_FILES experimental/main_exp_parallel.cpp experimental/GraphParallelExp.cpp experimental/GraphParallelExp.h
        InputOutput.cpp InputOutput.h DisjointSet.cpp DisjointSet.h ThreadPool.h)
add_executable(pSCANParallelExp0 ${EXP_FILES})
target_compile_options(pSCANParallelExp0 PRIVATE -O3 -g)
target_link_libraries(pSCANParallelExp0 ${CMAKE_THREAD_LIBS_INIT})

# ppSCAN parallel experimental 1: naive-set-intersection
add_executable(pSCANParallelExp1 ${EXP_FILES})
target_compile_options(pSCANParallelExp1 PRIVATE -O3 -g)
target_compile_definitions(pSCANParallelExp1 PRIVATE NAIVE_SET_INTERSECTION=1)
target_link_libraries(pSCANParallelExp1 ${CMAKE_THREAD_LIBS_INIT})

# pSCAN for time breakdown
set(PSCAN_SOURCE_FILES experimental/pscan_experimental/pSCANGraph.cpp experimental/pscan_experimental/pSCANGraph.h
        experimental/pscan_experimental/main_exp_pscan.cpp DisjointSet.cpp InputOutput.cpp
        experimental/pscan_experimental/MaxPriorityQueue.cpp experimental/pscan_experimental/MaxPriorityQueue.h)
add_executable(pSCANForTimeBreakDown ${PSCAN_SOURCE_FILES})
target_compile_options(pSCANForTimeBreakDown PRIVATE -O3 -g)
#target_compile_definitions(pSCANForTimeBreakDown PRIVATE WITHGPERFTOOLS=1)
#target_link_libraries(pSCANForTimeBreakDown profiler)

# pSCAN with IMPROVED_SET_INTERSECT
add_executable(pSCANBetterSetIntersect ${PSCAN_SOURCE_FILES})
target_compile_options(pSCANBetterSetIntersect PRIVATE -O3 -g)
target_compile_definitions(pSCANBetterSetIntersect PRIVATE IMPROVED_SET_INTERSECT=1)

# original SCAN
set(SCAN_FILES experimental/scan_origin/main_exp_scan.cpp experimental/scan_origin/SCANGraph.cpp
        experimental/scan_origin/SCANGraph.h InputOutput.cpp)
add_executable(SCANOrigin ${SCAN_FILES})
target_compile_options(SCANOrigin PRIVATE -O3 -g)
target_compile_definitions(SCANOrigin PRIVATE NAIVE_SET_INTERSECTION=1)

add_executable(SCANOriginBetterSetIntersec ${SCAN_FILES})
target_compile_options(SCANOriginBetterSetIntersec PRIVATE -O3 -g)
