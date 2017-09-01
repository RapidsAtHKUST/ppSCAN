1) Parameters

	"Usage: %s [switches] -i filename -m minpts -e epsilon -o output -t threads\n"
    		"	-c algorithm: algorithm used \n"
    		"	-i filename	: file containing input data to be clustered\n"
    		"	-g gname 	: file containing ground truth \n"
    		"	-m minpts	: input parameter of DBSCAN, min points to form a cluster, e.g. 2\n"
    		"	-e epsilon	: input parameter of DBSCAN, radius or threshold on neighbourhoods retrieved, e.g. 0.8\n"
    		"	-o output	: clustering results, format, (each line, point id, clusterid)\n"
    		"	-a alpha    : block size alpha \n"
    		"	-b beta     : block size beta \n"
    		"	-t threads	: number of threads to be employed\n\n";

Example:

./anyscan -c 5 -i test04.adj -g label.gold -o out.txt -m 4 -e 0.5 -a 32768 -b 32768 -t 8

Ground truth and output can be ignored.

The algorithm 

c = 1 : The orginal SCAN algorithm
c = 2 : The algorithm pSCAN
c = 3 : The anytime SCAN algorithm (anyscan)
c = 4 : AnySCAN in parallel
c = 5 : Ideal parallel SCAN

AnySCAN uses ltcmalloc for aiding the memory allocation. 


2) Input file format:

Number of node
[Node0 size] [Adjacency list of node 0]
[Node1 size] [Adjacency list of node 1]

Example:

24
5 0 1 5 6 23 
4 0 1 2 6 
4 1 2 3 6 
5 2 3 4 8 12 
6 3 4 5 6 21 22
7 0 4 5 6 
14 15 22 
6 0 1 2 4 5 6 
5 7 8 9 12 13 
5 3 7 8 9 13 
5 7 8 9 10 13 
3 9 10 13 
3 11 13 16 
4 3 7 12 13 
7 7 8 9 10 11 12 13
3 5 14 15
5 5 14 15 20 22
4 11 16 17 19
4 16 17 18 19
3 17 18 19
4 16 17 18 19  
4 15 20 21 22
3 4 20 21
5 4 5 15 20 22
2 0 23

Note that the node ID must start from 0. Each node must contain itself in its adjacency list.

3) Output file format
Number of node
Class label for each node (start from 0)

where: -1 (Noise) -2 (Hub)

Example:
24 
-1 -1 -1 -1 -1 -1 -1 1 1 1 1 -2 -1 1 -1 -1 0 0 0 0 -1 -1 -1 -1 

4) Linux System is OpenSUSE 

