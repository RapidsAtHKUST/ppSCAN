Read_me.txt

This codes is SCAN-XP (ver AVX) codes.

# This codes consist of the following files:
- scanxp.cpp
- scanxp.h
- graph.cpp
- graph.h
- unionfind.h

We explained this code in our paper 'SCAN-XP: Parallel Structural Graph Clustering Algorithm on Intel Xeon Phi Coprocessors'.

# How to use
Please, you enter the following command in your commandline:
./scanxp Graph_file Number_of_thread

・Graph_file
Graph_file needs that it is CRS format.

・Number_of_thread
This codes decide the number of threads when parallelizing by referring to "Number_of_thread”.

If you want to change SCAN’s parameter ‘\epsilon’, you change EPSILON in scanxp.h
This codes assume that SCAN's parameter \mu is 2, because \mu is recommended to set 2 in SCAN's paper.
