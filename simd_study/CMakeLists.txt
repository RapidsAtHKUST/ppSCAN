cmake_minimum_required(VERSION 3.8)
project(simd_study)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp)

find_package(Boost REQUIRED)
add_executable(simd_study ${SOURCE_FILES})
target_include_directories(simd_study PRIVATE ../boost.simd/include)
target_compile_options(simd_study PRIVATE -O3 -msse4.2)