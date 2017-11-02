cmake_minimum_required(VERSION 3.8)
project(parallel_multicore)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES clusters.cpp clusters.h dbscan.cpp dbscan.h
        kdtree2.cpp kdtree2.hpp omp_main.cpp utils.cpp utils.h)

find_package(OpenMP)
if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif ()
add_executable(omp_dbscan ${SOURCE_FILES})
target_compile_options(omp_dbscan PRIVATE -O3 -g)
