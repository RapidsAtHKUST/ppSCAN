cmake_minimum_required(VERSION 3.8)
project(parallel_mpi)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        clusters.cpp clusters.h dbscan.cpp dbscan.h
        geometric_partitioning.cpp geometric_partitioning.h
        kdtree2.cpp kdtree2.hpp utils.cpp utils.h mpi_main.cpp)

find_package(MPI REQUIRED)
set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})

#message(${MPI_COMPILE_FLAGS})
#message(${MPI_LINK_FLAGS})
message(${MPI_INCLUDE_PATH})

#include_directories(/usr/include/openmpi-x86_64)
add_executable(parallel_mpi ${SOURCE_FILES})
target_include_directories(parallel_mpi PRIVATE ${MPI_INCLUDE_PATH})
target_link_libraries(parallel_mpi ${MPI_LIBRARIES})