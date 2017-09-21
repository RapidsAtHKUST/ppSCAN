cmake_minimum_required(VERSION 3.6)
project(pSCAN)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES Graph.cpp Graph.h main.cpp Utility.cpp Utility.h)

add_executable(play play/graph_io.cpp Utility.cpp)
target_compile_definitions(play PRIVATE _DEBUG_=1 FOO=1)
target_compile_options(play PRIVATE -O3)

# original
add_executable(pSCANOptSerialOrgin ${SOURCE_FILES})
target_compile_options(pSCANOptSerialOrgin PRIVATE -O3 -g)

# statistics
add_executable(pSCANOptSerialStat ${SOURCE_FILES})
target_compile_definitions(pSCANOptSerialStat PRIVATE STATISTICS=1)
target_compile_options(pSCANOptSerialStat PRIVATE -O3 -g)

# rewrite logic to make it clear
add_executable(pSCANExp0MultiPass exp/GraphExp0.cpp exp/main_exp_multipass.cpp Utility.cpp)
target_compile_definitions(pSCANExp0MultiPass PRIVATE STATISTICS=1)
target_compile_options(pSCANExp0MultiPass PRIVATE -O3 -g)

# improved set-intersection
add_executable(pSCANExpImprovedSetInter ${SOURCE_FILES})
target_compile_definitions(pSCANExpImprovedSetInter PRIVATE IMPROVED_SET_INTERSECT=1)
target_compile_options(pSCANExpImprovedSetInter PRIVATE -O3 -g)


