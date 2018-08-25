# ppSCAN

ppSCAN: pruning-based structural clustering algorithm on networks.

## Compile

g++ should support c++11, e.g, g++ 4.8+, cmake should be 3.6+, allowing for modern cmake

```zsh
mkdir -p build
cd build
cmake .. -DCMAKE_CXX_COMPILER=/opt/intel/bin/icpc -DKNL=ON
make
```

## Usage

* converter(transform your edge list into `b_degree.bin`, `b_adj.bin`), a sample edge list file is [here](../SCANVariants/documents/example_edge_list.txt), you can copy that
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