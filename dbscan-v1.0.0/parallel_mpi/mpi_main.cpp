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

#include "dbscan.h"
#include "utils.h"
#include "kdtree2.hpp"
#include "geometric_partitioning.h"

static void usage(char *argv0) 
{
    const char *params =
	"Usage: %s [switches] -i filename -b -m minpts -e epsilon -o output\n"
    	"	-i filename	: file containing input data to be clustered\n"
    	"	-b		: input file is in binary format (default, binary and currently the only supported format)\n"
	"	-m minpts	: input parameter of DBSCAN, min points to form a cluster, e.g. 2\n"
	"	-e epsilon	: input parameter of DBSCAN, radius or threshold on neighbourhoods retrieved, e.g. 0.8\n"
	"	-o output	: clustering results, format, (points coordinates, cluster id)\n\n";
	
    fprintf(stderr, params, argv0);
    exit(-1);
}


int main(int argc, char** argv)
{
	double	seconds;
	int 	opt;

	int 	minPts, procs;
	double 	eps;
	char* 	outfilename;
	int     isBinaryFile;
	char*   infilename;

	int rank, nproc;
	MPI_Status status;

    	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    	MPI_Comm_size(MPI_COMM_WORLD, &nproc);

	// some default values
	minPts 		= -1;
	eps		= -1;
	isBinaryFile 	= 1; // default binary file
	outfilename 	= NULL;
	infilename	= NULL;

    	while ((opt=getopt(argc,argv,"i:m:e:o:?b"))!= EOF)
    	{
		switch (opt)
        	{
        		case 'i':
            			infilename = optarg;
               			break;
	            	case 'b':
		            	isBinaryFile = 1;
            			break;
            		case 'm':
		            	minPts = atoi(optarg);
            			break;
		        case 'e':
            			eps  = atof(optarg);
            			break;
			case 'o':
				outfilename = optarg;
				break;
            		case '?':
           			usage(argv[0]);
	           		break;
           		default:
           			usage(argv[0]);
           			break;
    		}
	}

	if(infilename == NULL || minPts < 0 || eps < 0)
	{
		if(rank == proc_of_interest)
			usage(argv[0]);
		MPI_Finalize();
		exit(-1);
	}

	if(rank == proc_of_interest) cout << "Number of process cores " << nproc << endl;

	// check if nproc is NOT multiple of TWO
	unsigned int proc_count = nproc;
	
 	while (((proc_count % 2) == 0) && proc_count > 1) // While x is even and > 1
   		proc_count /= 2;
 	
	if(proc_count != 1)
	{
		if(rank == proc_of_interest) cout << "\n\nPlease use the number of process cores as a multiple of TWO" << endl;
		MPI_Finalize();
		return 0;
	}

	NWUClustering::ClusteringAlgo dbs;
	
	dbs.set_dbscan_params(eps, minPts);

    	if(rank == proc_of_interest) cout << "Epsilon: " << eps << " MinPts: " << minPts << endl;
	
	MPI_Barrier(MPI_COMM_WORLD);
	double start = MPI_Wtime();

	if(rank == proc_of_interest) cout << "Reading points from file: " << infilename << endl;

	if(dbs.read_file(infilename, isBinaryFile) == -1)
	{
		MPI_Finalize();
		exit(-1);
	}

	if(rank == proc_of_interest) cout << "Reading the input data file took " << MPI_Wtime() - start << " seconds [pre_processing]"<< endl;

	MPI_Barrier(MPI_COMM_WORLD);
	start = MPI_Wtime();
	
	// parttition the data file geometrically: preprocessing step
	start_partitioning(dbs);
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank == proc_of_interest) cout << "Partitioning the data geometrically took " << MPI_Wtime() - start << " seconds [pre_processing]" << endl;
	
	// gather extra points that falls within the eps radius from the boundary: preprocessing step
	start = MPI_Wtime();
	get_extra_points(dbs);
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank == proc_of_interest) cout << "Gathering extra point took " << MPI_Wtime() - start << " seconds [pre_processing]" << endl;
		
	// build the kdtrees: preprocessing step 
	start = MPI_Wtime();
	dbs.build_kdtree();
	dbs.build_kdtree_outer();
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank == proc_of_interest) cout << "Build kdtree took " << MPI_Wtime() - start << " seconds [pre_processing]" << endl;
	
	if(rank == proc_of_interest) cout << endl;	
	//run the DBSCAN algorithm
	start = MPI_Wtime();
    	run_dbscan_algo_uf_mpi_interleaved(dbs);
	MPI_Barrier(MPI_COMM_WORLD);
   	if(rank == proc_of_interest) cout << "Parallel DBSCAN (init, local computation, and merging) took " << MPI_Wtime() - start << " seconds "<< endl;
	if(rank == proc_of_interest) cout << endl;
	
	// assign cluster IDs to points
	start = MPI_Wtime();
	dbs.get_clusters_distributed();	
	if(rank == proc_of_interest) cout << "Assigning cluster IDs to points " << MPI_Wtime() - start << " seconds [post_processing]" << endl;
	
	if(outfilename != NULL)
	{
		start = MPI_Wtime();	

		// activate the followingline to write the cluster to file
		dbs.writeCluster_distributed(outfilename);
		if(rank == proc_of_interest) cout << "Writing clusterIDs to disk took " << MPI_Wtime() - start << " seconds [pre_processing]"<< endl;
	}
		
	MPI_Finalize();
	return 0;
}
