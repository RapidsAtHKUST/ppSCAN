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


#ifndef _CLUSTER_
#define _CLUSTER_

#include "utils.h"
#include "kdtree2.hpp"

namespace NWUClustering
{
	struct Points
	{
		array2dfloat m_points;
		int m_i_dims;
		int m_i_num_points;
	};

	class Clusters
	{
	public:
		Clusters():m_pts(NULL),m_kdtree(NULL){ }
		virtual ~Clusters();

		int     read_file(char* infilename, int isBinaryFile);
		int     build_kdtree();
		
	public:
		Points* 	m_pts;
		kdtree2* 	m_kdtree;
		vector <int> 	m_pid_to_cid; // point id to cluster id
		vector <vector <int> > m_clusters;
		int     m_parcent_of_data;
	};
};

#endif

