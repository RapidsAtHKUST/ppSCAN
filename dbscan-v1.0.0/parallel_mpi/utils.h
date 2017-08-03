/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*   Files: mpi_main.cpp clusters.cpp  clusters.h utils.h utils.cpp          */
/*   			dbscan.cpp dbscan.h kdtree2.cpp kdtree2.hpp          */
/*			geometric_partitioning.h geometric_partitioning.cpp  */
/*		    						             */
/*   Description: an mpi implementation of dbscan clustering algorithm       */
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


#ifndef _UTILS_
#define _UTILS_

//#define _DEBUG
#define proc_of_interest 0
#define _GET_EXTRA_POINT_STAT 0
#define _GET_LOCAL_POINT_COUNT 0
#define NETCDF_FORMAT

#include <mpi.h>
#include <algorithm>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#ifdef NETCDF_FORMAT
#include <pnetcdf.h>
#endif


#define POW2(x) (1 << (x))
#define ROOT 1
#define LOWER(i) (i<<1)
#define UPPER(i) ((i<<1)+1)
#define PARENT(i) (i>>1)
#define SIBLING(i) ((i&1)?i-1:i+1)

using namespace std;

typedef float point_coord_type;
typedef vector <vector <point_coord_type> >         array2dfloat;

float findKMedian(vector<float>& A,int K);

#ifdef NETCDF_FORMAT
static void handle_error(int status, int lineno)
{
	fprintf(stderr, "Error at line %d: %s\n", lineno, ncmpi_strerror(status));
//	MPI_Abort(MPI_COMM_WORLD, 1);
}
#endif

#endif
