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

		// functions for dbscan algorithm
		void set_dbscan_params(double eps, int minPts);
		
		void 	writeClusters(ostream& o); // regular dbscan algorithm
		void    writeClusters_uf(ostream& o); // union find dbscan algorithm

	public:
		
		// parameters to run dbscan algorithm
		double 	m_epsSquare;
		int 	m_minPts;
		//int     m_parcent_of_data;
  
		// noise vector
        	vector<bool> m_noise;
	       	// noise vector
        	vector<bool> m_visited;

		vector <int> m_parents;
		vector <int> m_corepoint;
		vector <int> m_member;
	};	

	void run_dbscan_algo_uf(ClusteringAlgo& dbs); // union find dbscan algorithm
	void run_dbscan_algo(ClusteringAlgo& dbs); // regular dbscan algorithm
};

#endif
