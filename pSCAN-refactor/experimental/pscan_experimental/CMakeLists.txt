# pSCAN for time breakdown
set(PSCAN_SOURCE_FILES ./pSCANGraph.cpp ./pSCANGraph.h
        ./MaxPriorityQueue.cpp ./MaxPriorityQueue.h
        ./main_exp_pscan.cpp ../../DisjointSet.cpp ../../InputOutput.cpp)
add_executable(pSCANForTimeBreakDown ${PSCAN_SOURCE_FILES})
target_compile_options(pSCANForTimeBreakDown PRIVATE -O3 -g)
#target_compile_definitions(pSCANForTimeBreakDown PRIVATE WITHGPERFTOOLS=1)
#target_link_libraries(pSCANForTimeBreakDown profiler)

# pSCAN with improved set-intersection
add_executable(pSCANBetterSetIntersect ${PSCAN_SOURCE_FILES})
target_compile_options(pSCANBetterSetIntersect PRIVATE -O3 -g)
target_compile_definitions(pSCANBetterSetIntersect PRIVATE IMPROVED_SET_INTERSECT=1)

# pSCAN with improved set-intersection statistics
add_executable(pSCANBetterSetStat ${PSCAN_SOURCE_FILES})
target_compile_options(pSCANBetterSetStat PRIVATE -O3 -g)
target_compile_definitions(pSCANBetterSetStat PRIVATE IMPROVED_SET_INTERSECT=1 STATISTICS=1)