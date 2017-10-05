# ppSCAN

ppSCAN: pruning-based structural clustering algorithm on networks.

for supplementary documents, see [documents](documents).

## Compile

g++ should support c++11, e.g, g++ 4.8+, cmake should be 3.6+, allowing for modern cmake

```zsh
mkdir -p build
cd build
cmake .. -DCMAKE_CXX_COMPILER=g++
make
```

## Usage

* converter(transform your edge list into `b_degree.bin`, `b_adj.bin`), a sample edge list file is [here](documents/example_edge_list.txt), you can copy that 
to `../dataset/toy_graph/` directory and execute the following command

```zsh
build/converter/converter ../dataset/toy_graph/example_edge_list.txt ../dataset/toy_graph/b_degree.bin ../dataset/toy_graph/b_adj.bin
```

* ppSCAN algorithm release(serial/parallel), the input binary files and output file are in `../dataset/toy_graph/` directory, 
eps is `0.3`, min_pts is `5`. `output` indicates outputs to the file, you can remove it 
if you do not want to see the result file for time measurement for large datasets.

```zsh
build/pSCANSerial ../dataset/toy_graph/ 0.3 5 output
build/pSCANParallel ../dataset/toy_graph/ 0.3 5 output
```

## Package Organization

ppSCAN follows modern cpp rules, to make things easier without loss of performance.

* main files

file | utility
--- | ---
[ThreadPool.h](../pSCAN-refactor/ThreadPool.h) | third-party thread pool, a simple impl
[ThreadSafeDisjointSet.h](../pSCAN-refactor/ThreadSafeDisjointSet.h) | third-party thread-safe lock-free disjoint-set
[DisjointSet.h](../pSCAN-refactor/DisjointSet.h), [DisjointSet.cpp](../pSCAN-refactor/DisjointSet.cpp) | see CLRS for detail, for connected componet
[Graph.h](../pSCAN-refactor/Graph.h), [Graph.cpp](../pSCAN-refactor/Graph.cpp) | graph representation and algorithm related

* experimental, see [../pSCAN-refactor/experimental](../pSCAN-refactor/experimental)

implementations: original SCAN, pSCAN(GPLv3 license), ppSCAN parallel(varying thread num), ppSCAN serial.

* io related

file | utility
--- | ---
[InputOutput.h](../pSCAN-refactor/InputOutput.h), [InputOutput.cpp](../pSCAN-refactor/InputOutput.cpp) | read binary degree/adjacent edges utility
[EdgeListToBinsConverter.cpp](converter/EdgeListToBinsConverter.cpp) | utility converter for processing edge list into binary files

* utils and plays

file | utility
--- | ---
[pretty_print.h](../pSCAN-refactor/playground/pretty_print.h) | third-party pretty print utilities
[graph_io.cpp](../pSCAN-refactor/playground/graph_io.cpp) | play ground about i/o
[play.cpp](../pSCAN-refactor/playground/play.cpp) | play ground others


