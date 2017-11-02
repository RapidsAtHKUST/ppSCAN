/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*   Files: omp_main.cpp clusters.cpp  clusters.h utils.h utils.cpp          */
/*   			dbscan.cpp dbscan.h kdtree2.cpp kdtree2.hpp          */
/*		    						             */
/*   Description: an openmp implementation of dbscan clustering algorithm    */
/*				using the disjoint set data structure        */
/*                                                                           */
/*   Author:  Md. Mostofa Ali Patwary                                        */
/*            EECS Department, Northwestern University                       */
/*            email: mpatwary@eecs.northwestern.edu                          */
/*                                                                           */
/*   Copyright, 2012, Northwestern University                                */
/*   See COPYRIGHT notice in top-level directory.                            */
/*                                                                           */
/*   Please cite the following publication if you use this package 	     */
/* 									     */
/*   Md. Mostofa Ali Patwary, Diana Palsetia, Ankit Agrawal, Wei-keng Liao,  */
/*   Fredrik Manne, and Alok Choudhary, "A New Scalable Parallel DBSCAN      */
/*   Algorithm Using the Disjoint Set Data Structure", Proceedings of the    */
/*   International Conference on High Performance Computing, Networking,     */
/*   Storage and Analysis (Supercomputing, SC'12), pp.62:1-62:11, 2012.	     */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


A Disjoint-Set Data Structure based Parallel DBSCAN clustering implementation (OpenMP version)

How to run the tool:
1. Compile the source files using the following command

	make

2. Run using following command 

	./omp_dbscan -i filename -b -m minpts -e epsilon -o output -t threads

	Example:

	./omp_dbscan -i clus50k.bin -b -m 5 -e 25 -t 8 -o test

	run the following to get detail description on the program arguments

	./omp_dbscan ?

3. Input file format:
	
	binary file:
	number of points, N and number of dimensions, D (each 4 bytes) followed by the points coordinates (N x D floating point numbers).

	text file:
	each line is a point (no need to mention the number of points or the number of dimensions).
