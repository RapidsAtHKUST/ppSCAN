cmake_minimum_required(VERSION 3.8)
project(anySCANReImpl)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main_exp_any_scan_serial.cpp AnySCANGraphSerial.h AnySCANGraphSerial.cpp DisjointSet.cpp DisjointSet.h InputOutput.cpp InputOutput.h)
add_executable(anySCANReImpl ${SOURCE_FILES})
target_compile_options(anySCANReImpl PRIVATE -O3 -g)
target_compile_definitions(anySCANReImpl PRIVATE STATISTICS=1)