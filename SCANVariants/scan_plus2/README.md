# Implementation of SCAN++

Last update Sep. 8th, 2015

## About

This is an alpha version implementation of SCAN++, which efficiently finds clusters, hubs and outliers from large-scale graphs.
The details of SCAN++ are discussed in the following paper:

- [H. Shiokawa, Y. Fujiwara, M. Onizuka: SCAN++: Efficient Algorithm for Finding Clusters, Hubs and Outliers on Large-scale Graphs. PVLDB 8(11) (2015)](http://www.vldb.org/pvldb/vol8.html)

## License

**All of this software are to be used for research purposes only and for no other purpose.**
This software is released under the License of Nippon Telegraph and Telephone Corporation, see LICENSE.txt.

## How to use

### Requirements for SCAN++

This software is built by ```gcc-4.8.1``` with some ```C++11``` functions, and we confirmed that this software runs on CentOS 6.6 (x86_64) and Intel(R) Xeon(R) CPU E5-4650.

### Input file format

This software reads input graph file formatted as follows:

```
src1 \s dst1
src2 \s dst2
src3 \s dst3
...
```

Each line denotes an edge between ```src``` node and ```dst``` node.

### Run SCAN++

SCAN++ runs by executing the following commands:

```
./scanpp [options] input_file
```

```[options]``` are detailed as follows:

| option | arguments (Default) | description |
|:------:|:--------------------|:------------|
|```-e ``` | Real number between 0 and 1 (Default: 0) | Set the epsilon parameter for the clustering. |
|```-m ``` | Natural number (Default: 1) | Set the mu parameter for the clustering. |
|```-v ``` | No arguments are required. | Display statistics of the clustering. |
|```-r ``` | No arguments are required. | Display the clustering results. |

For example, if you want to run SCAN++ with ```epsilon=0.5``` and ```mu=3``` for a graph file ```input_graph.txt``` and display statistics of the clustering, execute the following commands:
```
% ./scanpp -e 0.5 -m 3 -v input_graph.txt
```

