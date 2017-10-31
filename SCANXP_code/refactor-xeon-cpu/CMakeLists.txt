cmake_minimum_required(VERSION 3.6)
project(refactor_xeon_cpu)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
find_package(OpenMP)

if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif ()

set(SOURCE_FILES scanxp.cpp scanxp.h union_find.h graph.cpp graph.h csr_set_intersection.h csr_set_intersection.cpp)

add_executable(scan-xp-cpu ${SOURCE_FILES})
target_compile_options(scan-xp-cpu PRIVATE -march=core-avx2 -O3 -g)
target_compile_definitions(scan-xp-cpu PRIVATE AVX2=1)

add_executable(scan-xp-naive ${SOURCE_FILES})
target_compile_options(scan-xp-naive PRIVATE -march=core-avx2 -O3 -g)
target_compile_definitions(scan-xp-naive PRIVATE NAIVE=1)

add_executable(scan-xp-galloping-single ${SOURCE_FILES})
target_compile_options(scan-xp-galloping-single PRIVATE -march=core-avx2 -O3 -g)
target_compile_definitions(scan-xp-galloping-single PRIVATE GALLOPING_SINGLE=1)

add_executable(scan-xp-galloping-double ${SOURCE_FILES})
target_compile_options(scan-xp-galloping-double PRIVATE -march=core-avx2 -O3 -g)
target_compile_definitions(scan-xp-galloping-double PRIVATE GALLOPING_DOUBLE=1)


