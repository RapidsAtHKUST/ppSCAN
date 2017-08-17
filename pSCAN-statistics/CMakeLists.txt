cmake_minimum_required(VERSION 3.6)
project(pSCAN)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES Graph.cpp Graph.h main.cpp Utility.cpp Utility.h)

add_executable(play play/graph_io.cpp Utility.cpp)
target_compile_definitions(play PRIVATE _DEBUG_=1 FOO=1)
target_compile_options(play PRIVATE -O3)

add_executable(pSCANOptSerialStat ${SOURCE_FILES})
target_compile_definitions(pSCANOptSerialStat PRIVATE STATISTICS=1)
target_compile_options(pSCANOptSerialStat PRIVATE -O3 -g)
