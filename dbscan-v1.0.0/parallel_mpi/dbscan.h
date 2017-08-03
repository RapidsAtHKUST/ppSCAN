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

#ifndef _DBSCAN_
#define _DBSCAN_

#include "utils.h"
#include "clusters.h"

namespace NWUClustering
{
	class ClusteringAlgo : public Clusters
	{
	public:
		ClusteringAlgo(){ }
		virtual ~ClusteringAlgo();

		void set_dbscan_params(double eps, int minPts);
		void 	get_clusters_distributed();
		#ifdef NETCDF_FORMAT
		void  writeCluster_distributed(string outfilename);
		#endif

		void 	convert(vector< vector <int> >* data, int nproc, int rank, int round);
		void 	trivial_compression(vector <int>* data, vector < vector <int> >* parser, int nproc, int rank, int round, double& comtime, double& sum_comp_rate);
		void 	trivial_decompression(vector <int>* data, int nproc, int rank, int round, double& dcomtime);

	public:
		
		double 	m_epsSquare;
		int 	m_minPts;
 		int 	m_messages_per_round;
		int 	m_compression;

        	vector<bool> m_noise;
        	vector<bool> m_visited;
		vector <int> m_parents;
		vector <int> m_parents_pr;
		vector <int> m_child_count;	

		vector <int> m_member;
		vector <int> m_corepoint;
	};	

	void run_dbscan_algo_uf_mpi_interleaved(ClusteringAlgo& dbs); // union find dbscan algorithm using mpi with interleaved communication
};

#endif
