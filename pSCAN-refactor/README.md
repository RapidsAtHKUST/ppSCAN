# ppSCAN

ppSCAN: pruning-based structural clustering algorithm on networks.

for supplementary documents, see [documents](documents).

# Compile

g++ should support c++11, e.g, g++ 4.8+, cmake should be 3.6+, allowing for modern cmake

```zsh
mkdir -p build
cd build
cmake .. -DCMAKE_CXX_COMPILER=g++
make
```

# Parameter

* eps，influence the computation workloads a lot

* min_pts，not influence the workloads

# Usage

```zsh
build/pSCANSerial ../dataset 0.3 5 output
build/pSCANParallel ../dataset 0.3 5 output
```

# Package Organization

ppSCAN package further usage must follow [GPLv3 license](../pSCAN-refactor/LICENSE).

* pSCAN-fork-optimization implementations follow modern cpp rules, to make things easier without loss of performance.

file | utility
--- | ---
[ThreadPool.h](../pSCAN-refactor/ThreadPool.h) | third-party thread pool, a simple impl
[DisjointSet.h](../pSCAN-refactor/DisjointSet.h), [DisjointSet.cpp](../pSCAN-refactor/DisjointSet.cpp) | see CLRS for detail, for connected componet
[Graph.h](../pSCAN-refactor/Graph.h), [Graph.cpp](../pSCAN-refactor/Graph.cpp) | graph representation and algorithm  related
[InputOutput.h](../pSCAN-refactor/InputOutput.h), [InputOutput.cpp](../pSCAN-refactor/InputOutput.cpp) | read binary degree/adjacent edges utility
[EdgeListToBinsConverter.cpp](converter/EdgeListToBinsConverter.cpp) | utility converter for processing edge list into binary files

* utils and plays

file | utility
--- | ---
[pretty_print.h](../pSCAN-refactor/playground/pretty_print.h) | third-party pretty print utilities
[graph_io.cpp](../pSCAN-refactor/playground/graph_io.cpp) | play ground about i/o
[play.cpp](../pSCAN-refactor/playground/play.cpp) | play ground others

* experimental(parallel version with adjustable thread num)

see [../pSCAN-refactor/experimental](../pSCAN-refactor/experimental).
* deprecated

file | utility
--- | ---
[MaxPriorityQueue.h](./playground/MaxPriorityQueue.h)，[MaxPriorityQueue.cpp](./playground/MaxPriorityQueue.cpp) | not useful for pruning, deprecated now
[ThreadSafeDisjointSet.h](ThreadSafeDisjointSet.h) | not useful after change some codes to make it simpler, deprecated now

* profiling scrips

see [run_pscan_gperftools.sh](shell_playground/run_pscan_gperftools.sh), [run_pscan_perf.sh](shell_playground/run_pscan_perf.sh), [run_pscan_valgrind.sh](shell_playground/run_pscan_valgrind.sh) and this post [profiler tutorial](http://gernotklingler.com/blog/gprof-valgrind-gperftools-evaluation-tools-application-level-cpu-profiling-linux/).

for more advanced profiling, please use intel vtune 

# pSCAN Citation

> If you are using this code, please kindly cite the following paper.

> Lijun Chang, Wei Li, Xuemin Lin, Lu Qin, Wenjie Zhang,
"pSCAN: Fast and Exact Structural Graph Clustering"
Proceedings of the 31st International Conference on Data Engineering (ICDE’16), 2016
