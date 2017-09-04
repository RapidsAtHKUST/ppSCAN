perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations build/pSCANParallelExp0 ../dataset/snap_orkut 0.3 5 8 output
