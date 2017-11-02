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

#ifndef _GEOMETRIC_PARTITIONING_
#define _GEOMETRIC_PARTITIONING_

#include "utils.h"
#include "dbscan.h"
#include "kdtree2.hpp" 

namespace NWUClustering
{
	//void run_dbscan_algo_uf(ClusteringAlgo& dbs); // union find dbscan algorithm
	//void run_dbscan_algo(ClusteringAlgo& dbs); // regular dbscan algorithm
	
	void get_extra_points(ClusteringAlgo& dbs);
	void start_partitioning(ClusteringAlgo& dbs);

	void update_points(ClusteringAlgo& dbs, int s_count, vector <int>& invalid_pos_as, vector <float>& recv_buf);
	int get_points_to_send(ClusteringAlgo& dbs, vector <float>& send_buf, vector <int>& invalid_pos_as, float median, int d, int rank, int partner_rank);
	float get_median(ClusteringAlgo& dbs, int d, MPI_Comm& new_comm);
	void compute_local_bounding_box(ClusteringAlgo& dbs, interval* box);
	void compute_global_bounding_box(ClusteringAlgo& dbs, interval* box, interval* gbox, int nproc);
	void copy_global_box_to_each_node(ClusteringAlgo& dbs, interval** nodes_gbox, interval* gbox, int internal_nodes);
	void copy_box(ClusteringAlgo& dbs, interval* target_box, interval* source_box);
	void print_points(ClusteringAlgo& dbs, int rank);
	void print_box(ClusteringAlgo& dbs, int rank, interval* box);	
};

#endif
