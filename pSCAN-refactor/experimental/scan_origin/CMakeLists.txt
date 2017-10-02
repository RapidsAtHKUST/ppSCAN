# original SCAN
set(SCAN_FILES ./main_exp_scan.cpp ./SCANGraph.cpp ./SCANGraph.h ../../InputOutput.cpp)
add_executable(SCANOrigin ${SCAN_FILES})
target_compile_options(SCANOrigin PRIVATE -O3 -g)
target_compile_definitions(SCANOrigin PRIVATE NAIVE_SET_INTERSECTION=1)
target_compile_definitions(SCANOrigin PRIVATE NAIVE_SET_INTERSECTION=1 MIN_MAX_PRUNE=1)

add_executable(SCANOriginBetterSetIntersec ${SCAN_FILES})
target_compile_options(SCANOriginBetterSetIntersec PRIVATE -O3 -g)
target_compile_definitions(SCANOriginBetterSetIntersec PRIVATE MIN_MAX_PRUNE=1)

add_executable(SCANOriginBetterSetStat ${SCAN_FILES})
target_compile_options(SCANOriginBetterSetStat PRIVATE -O3 -g)
target_compile_definitions(SCANOriginBetterSetStat PRIVATE STATISTICS=1 MIN_MAX_PRUNE=1)

# super naive original SCAN with no optimization
set(SCAN_SUPER_NAIVE_FILES ./main_super_naive_scan.cpp ./SCANSuperNaiveGraph.cpp ./SCANSuperNaiveGraph.h ../../InputOutput.cpp)
add_executable(SCANSuperNaive ${SCAN_SUPER_NAIVE_FILES})
target_compile_options(SCANSuperNaive PRIVATE -O3 -g)