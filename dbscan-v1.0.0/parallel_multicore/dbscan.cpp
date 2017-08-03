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


#include "dbscan.h"

namespace NWUClustering
{

    	void ClusteringAlgo::set_dbscan_params(double eps, int minPts)
	{
		m_epsSquare =  eps * eps;
		m_minPts =  minPts;
	}

	ClusteringAlgo::~ClusteringAlgo()
	{
		m_noise.clear();
		m_visited.clear();
		m_parents.clear();
		
		m_corepoint.clear();
		m_member.clear();
	}
	
	void ClusteringAlgo::writeClusters(ostream& o)
	{
		// writing point id and cluster id pairs per line, noise has cluster id 0	
		int iMaxID = m_clusters.size(), id, i, j;
		for(i = 0; i < m_pts->m_i_num_points; i++)
		{
			//for (j = 0; j < m_pts->m_i_dims; j++)
			//	o << " " << m_pts->m_points[i][j];
			
			id = m_pid_to_cid[i];

			o << i << " " << id << endl;
		}

		int sum_points = 0;
		int noise = 0;
		for(i = 0; i < m_clusters.size(); i++)
		{
			sum_points += m_clusters[i].size();
			//cout << i << "(" << m_clusters[i].size() << ") ";
		}
	
		for (i = 0; i < m_pts->m_i_num_points; i++)
		{
			if(m_noise[i])
				noise++;
		}	
		
		cout << "Total points " << noise + sum_points << " pt_in_cls " << sum_points << " noise " << noise << endl;
		cout << "Number of clusters: " << m_clusters.size() << endl;
	}

	void ClusteringAlgo::writeClusters_uf(ostream& o)
	{
		// writing point id and cluster id pairs per line, noise has cluster id 0	
		vector <int> clusters;
		clusters.resize(m_pts->m_i_num_points, 0);

		int i, j, sum_points = 0, noise = 0, root, rootcount = 0, tmp;

		for(i = 0; i < m_pts->m_i_num_points; i++)
		{
			root = m_parents[i];

			// get the number of trees
			if(i == m_parents[i])
				rootcount++;

			// get the root of the tree containing i	
			while(root != m_parents[root])
				root = m_parents[root];

			// compress the tree to reduce the height of the branch of the tree to 1
			j = i;
			while(m_parents[j] != root)
			{
				tmp  = m_parents[j];
				m_parents[j] = root;
				j = tmp;
			}

			//m_parents[i] = root;
			// count the number of vertex in this tree
			clusters[root]++;
		}

		//cout << "clusters" << endl;
		int count = 0;
		for(i = 0; i < m_pts->m_i_num_points; i++)
		{
			if(clusters[i] == 1)
			{
				// vertex i is a noise
				clusters[i] = 0;
				noise++;
			}
			else if(clusters[i] > 1)
			{
				// get the size of cluster count: this will happen only if i is a root
				count++;
				sum_points += clusters[i];
				clusters[i] = count;
			}
			// skip if i is not a root
		}

		// write point id and cluster ids to file
		for(i = 0; i < m_pts->m_i_num_points; i++)
		{
			//for (j = 0; j < m_pts->m_i_dims; j++)
            		//	o << " " << m_pts->m_points[i][j];	
		
			o << i << " " << clusters[m_parents[i]] << endl;
		}

		cout << "Total points " << noise + sum_points << " pt_in_cls " << sum_points << " noise " << noise << endl;
		cout << "Number of clusters: " << count << endl;

		clusters.clear();
	}

	void run_dbscan_algo_uf(ClusteringAlgo& dbs)
	{			
		int tid, i, pid, j, k, npid, root, root1, root2;
 
        	// initialize some parameters
		dbs.m_clusters.clear();

		// get the neighbor of the first point and print them

		//cout << "DBSCAN ALGORITHMS============================" << endl;

		kdtree2_result_vector ne;
			
		// assign parent to itestf
		dbs.m_parents.resize(dbs.m_pts->m_i_num_points, -1);
		dbs.m_member.resize(dbs.m_pts->m_i_num_points, 0);
		dbs.m_corepoint.resize(dbs.m_pts->m_i_num_points, 0);

		int sch, maxthreads = omp_get_max_threads();
		
		if(dbs.m_pts->m_i_num_points % maxthreads == 0)
			sch = dbs.m_pts->m_i_num_points/maxthreads;
		else
			sch = dbs.m_pts->m_i_num_points/maxthreads + 1;
		
		vector < vector <int > > merge;
		vector <int> init;
		merge.resize(maxthreads, init);
		for(tid = 0; tid < maxthreads; tid++)
			merge[tid].reserve(dbs.m_pts->m_i_num_points);
		
		vector < int > prID;
		prID.resize(dbs.m_pts->m_i_num_points, -1);

		vector<int>* ind = dbs.m_kdtree->getIndex();		

		double start = omp_get_wtime();	
		#pragma omp parallel private(root, root1, root2, tid, ne, npid, i, j, pid) shared(sch, ind) //, prID)
		{
			int lower, upper;
			tid = omp_get_thread_num();

        		lower = sch * tid;
	        	upper = sch * (tid + 1);

        		if(upper > dbs.m_pts->m_i_num_points)
		            	upper = dbs.m_pts->m_i_num_points;

	        	for(i = lower; i < upper; i++)
    	    		{
				pid = (*ind)[i]; // CAN TRY RANDOMLY SELECTING
				dbs.m_parents[pid] = pid;
				prID[pid] = tid;
			}

			#pragma omp barrier

			for(i = lower; i < upper; i++)
			{
				pid = (*ind)[i];

				ne.clear();
            			dbs.m_kdtree->r_nearest_around_point(pid, 0, dbs.m_epsSquare, ne);

				if(ne.size() >= dbs.m_minPts)
				{
					dbs.m_corepoint[pid] = 1;
					dbs.m_member[pid] = 1;
					
					// get the root containing pid
					root = pid;

					for (j = 0; j < ne.size(); j++)
    	            			{
						npid= ne[j].idx;
						if(prID[npid] != tid)
						{
							merge[tid].push_back(pid);
							merge[tid].push_back(npid);
							continue;
						}

						// get the root containing npid
						root1 = npid;
						root2 = root;

						if(dbs.m_corepoint[npid] == 1 || dbs.m_member[npid] == 0)
						{
							dbs.m_member[npid] = 1;
	
							// REMS algorithm to merge the trees
							while(dbs.m_parents[root1] != dbs.m_parents[root2])
							{
								if(dbs.m_parents[root1] < dbs.m_parents[root2])
								{
									if(dbs.m_parents[root1] == root1)
									{
										dbs.m_parents[root1] = dbs.m_parents[root2];
										root = dbs.m_parents[root2];
										break;
									}

				        	                    	// splicing
                	        				    	int z = dbs.m_parents[root1];
				            	                	dbs.m_parents[root1] = dbs.m_parents[root2];
                    	       						root1 = z;
								}
								else
								{
									if(dbs.m_parents[root2] == root2)
									{
										dbs.m_parents[root2] = dbs.m_parents[root1];
										root = dbs.m_parents[root1];
										break;
									}

					   	                        // splicing
				        	                    	int z = dbs.m_parents[root2];
            	                					dbs.m_parents[root2] = dbs.m_parents[root1];					
									root2 = z;
								}
							}
						}
					}
				}
			}
		}

		int v1, v2, size;
		// merge the trees that have not been merged yet
		double stop = omp_get_wtime() ;
		cout << "Local computation took " << stop - start << " seconds." << endl;

		//allocate and initiate locks
    		omp_lock_t *nlocks;
		nlocks = (omp_lock_t *) malloc(dbs.m_pts->m_i_num_points*sizeof(omp_lock_t));

		//start = stop;
		start = omp_get_wtime();

		#pragma omp parallel for private(i) shared(nlocks)
    		for(i = 0; i < dbs.m_pts->m_i_num_points; i++) 
      			omp_init_lock(&nlocks[i]); // initialize locks

		#pragma omp parallel for shared(maxthreads, merge, nlocks) private(i, v1, v2, root1, root2, size, tid)
		for(tid = 0; tid < maxthreads; tid++)
		{
			size = merge[tid].size()/2;

			for(i = 0; i < size; i++)
			{
                		v1 = merge[tid][2 * i];
				v2 = merge[tid][2 * i + 1];
		
				int con = 0;
				if(dbs.m_corepoint[v2] == 1)
					con = 1;
				else if(dbs.m_member[v2] == 0)
				{
                			omp_set_lock(&nlocks[v2]);
                    			if(dbs.m_member[v2] == 0) // if v2 is not a member yet
                    			{
                        			con = 1;
						dbs.m_member[v2] = 1;
                    			}
                    			omp_unset_lock(&nlocks[v2]);
				}

				if(con == 1)
				{
				
					// lock based approach for merging
					root1 = v1;
					root2 = v2;

					// REMS algorithm with splicing compression techniques
					while (dbs.m_parents[root1] != dbs.m_parents[root2]) 
					{
						if (dbs.m_parents[root1] < dbs.m_parents[root2])
						{
							if(dbs.m_parents[root1] == root1) // root1 is a root
							{
								omp_set_lock(&nlocks[root1]);
								int p_set = false;
								if(dbs.m_parents[root1] == root1) // if root1 is still a root
								{
									dbs.m_parents[root1] = dbs.m_parents[root2];
									p_set = true;
								}

								omp_unset_lock(&nlocks[root1]);
								if (p_set) // merge successful
    	      								break;
							}
	
							// splicing
							int z = dbs.m_parents[root1];
							dbs.m_parents[root1] = dbs.m_parents[root2];
							root1 = z;
							// root1 = dbs.m_parents[root1];
						}
						else
						{
							if(dbs.m_parents[root2] == root2) // root2 is a root
            	               				{
                	               				omp_set_lock(&nlocks[root2]);
                    	           				int p_set = false;
                        	       				if(dbs.m_parents[root2] == root2) // check if root2 is a root
                            	   				{
       				                         	   	dbs.m_parents[root2] = dbs.m_parents[root1];
                                			   		p_set = true;
	                               				}
    	                           				omp_unset_lock(&nlocks[root2]);
        	                       				if (p_set) // merge successful
            	                       					break;
                	           			}
							
								//splicing
				        		int z = dbs.m_parents[root2];
                           				dbs.m_parents[root2] = dbs.m_parents[root1];
 	                          			root2 = z;

							//root2 = dbs.m_parents[root2];
						}	
					}
				}
			}
		}

		stop = omp_get_wtime();
		free(nlocks);
		cout << "Merging took " << stop - start << " seconds."<< endl;

		for(tid = 0; tid < maxthreads; tid++)
			merge[tid].clear();
		
		merge.clear();
		ne.clear();
	}
   
	void run_dbscan_algo(ClusteringAlgo& dbs)
	{
		// classical DBSCAN algorithm (only sequential)
		int i, pid, j, k, npid;
		int cid = 1; // cluster id
		vector <int> c;
		c.reserve(dbs.m_pts->m_i_num_points);

           	// initialize some parameters
		dbs.m_noise.resize(dbs.m_pts->m_i_num_points, false);
            	dbs.m_visited.resize(dbs.m_pts->m_i_num_points, false);		
		dbs.m_pid_to_cid.resize(dbs.m_pts->m_i_num_points, 0);
		dbs.m_clusters.clear();

		// get the neighbor of the first point and print them

		//cout << "DBSCAN ALGORITHMS============================" << endl;

		kdtree2_result_vector ne;
		kdtree2_result_vector ne2;
		//kdtree2_result_vector ne3;
		ne.reserve(dbs.m_pts->m_i_num_points);
		ne2.reserve(dbs.m_pts->m_i_num_points);

		vector<int>* ind = dbs.m_kdtree->getIndex();

		double start = omp_get_wtime() ;		

		for(i = 0; i < dbs.m_pts->m_i_num_points; i++)
		{
			pid = (*ind)[i];

			if (!dbs.m_visited[pid])
			{
				dbs.m_visited[pid] = true;
				ne.clear();
				dbs.m_kdtree->r_nearest_around_point(pid, 0, dbs.m_epsSquare, ne);
				
				if(ne.size() < dbs.m_minPts)
					dbs.m_noise[pid] = true;
				else
				{
					// start a new cluster
					c.clear();
					c.push_back(pid);
					dbs.m_pid_to_cid[pid] = cid;

					// traverse the neighbors
					for (j = 0; j < ne.size(); j++)
					{
						npid= ne[j].idx;

						// not already visited
						if(!dbs.m_visited[npid])
						{
							dbs.m_visited[npid] = true;
	
							// go to neighbors
							ne2.clear();
							dbs.m_kdtree->r_nearest_around_point(npid, 0, dbs.m_epsSquare, ne2);

							// enough support
							if (ne2.size() >= dbs.m_minPts)
							{
								// join
								for(k = 0; k < ne2.size(); k++)
									ne.push_back(ne2[k]);
							}
						}

						// not already assigned to a cluster
						if (!dbs.m_pid_to_cid[npid])
						{
							c.push_back(npid);
							dbs.m_pid_to_cid[npid]=cid;
							dbs.m_noise[npid] = false;
						}
					}

					dbs.m_clusters.push_back(c);
					cid++;
				}
					
			}
		}
		
	        double stop = omp_get_wtime();
        	cout << "Local computation took " << stop - start << " seconds." << endl;
		cout << "No merging stage in classical DBSCAN"<< endl;
		ind = NULL;
		ne.clear();
		ne2.clear();
	}
};

