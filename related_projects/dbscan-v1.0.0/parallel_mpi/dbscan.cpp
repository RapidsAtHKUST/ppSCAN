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

namespace NWUClustering
{

    	void ClusteringAlgo::set_dbscan_params(double eps, int minPts)
	{
		m_epsSquare =  eps * eps;
		m_minPts =  minPts;
		m_messages_per_round = -1; // always -1
		m_compression = 0; // can set to 1 if want to compress specailly in the first round of communication
	}

	ClusteringAlgo::~ClusteringAlgo()
	{
		m_noise.clear();
        	m_visited.clear();
        	m_parents.clear();
		m_parents_pr.clear();
		m_child_count.clear();

		m_corepoint.clear();
		m_member.clear();
	}

	void ClusteringAlgo::trivial_decompression(vector <int>* data, int nproc, int rank, int round, double& dcomtime)
	{
		double start = MPI_Wtime();

		vector <int> parser;
		parser.reserve((*data).size());
		parser = (*data);
		(*data).clear();

		int pid_count = parser[0], pos, i, j, pid, npid, npid_count;
		
		pos++;

		while(pid_count > 0)
		{
			pid = parser[pos++];
			npid_count = parser[pos++];

			for(j = 0; j < npid_count; j++)
			{
				(*data).push_back(pid);
				(*data).push_back(parser[pos++]);
			}

			pid_count--;
		}

		parser.clear();

        	double stop = MPI_Wtime();

		dcomtime += (stop - start);
	}
	
	void ClusteringAlgo::trivial_compression(vector <int>* data, vector < vector <int> >* parser, int nproc, int rank, int round, double& comtime, double& sum_comp_rate)
	{
		double start = MPI_Wtime();
		double org = 0, comp = 0;
		int pairs, pid, npid, i, j, pid_count, npid_count;

		pairs = (*data).size()/2;

		org = (*data).size();		

		for(i = 0; i < pairs; i++)
		{
			pid = (*data)[2 * i];
			npid = (*data)[2 * i + 1];

			(*parser)[pid].push_back(npid);
		}

		(*data).clear();

		pid_count = 0;
		(*data).push_back(pid_count); // uniques pids, should update later
			
		for(i = 0; i < m_pts->m_i_num_points; i++)
		{
			npid_count = (*parser)[i].size();
			if(npid_count > 0)
			{
					(*data).push_back(i);
					(*data).push_back(npid_count);

					for(j = 0; j < npid_count; j++)
						(*data).push_back((*parser)[i][j]);						 
					
					pid_count++;

					(*parser)[i].clear();
			}
		}
			
		(*data)[0] = pid_count;

		comp = (*data).size();
		
		double stop = MPI_Wtime();
		comtime += (stop - start);

		sum_comp_rate += (comp / org);
	}

	void ClusteringAlgo::convert(vector < vector <int> >* data, int nproc, int rank, int round)
	{
		int j, tid, size, pid, v1, v2, pairs, count;
		vector < vector <int> > parser;
		vector <int> init, verify;
		int min, max;

		for(tid = 0; tid < nproc; tid++)
		{
			pairs = (*data)[tid].size()/2;
			
			verify.resize(2 * pairs, -1);

			if(pairs == 0)
				continue;

			min = m_pts->m_i_num_points;
			max = -1;
			for(pid = 0; pid < pairs; pid++)
			{
				if((*data)[tid][2 * pid] < min)
					min = (*data)[tid][2 * pid];
				
				if((*data)[tid][2 * pid] > max)
					max  = (*data)[tid][2 * pid];

				verify[2 * pid] = (*data)[tid][2 * pid];
				verify[2 * pid + 1] = (*data)[tid][2 * pid + 1];
			}

			init.clear();
			parser.resize(max - min + 1, init);
			
			for(pid = 0; pid < pairs; pid++)
			{
				v2 = (*data)[tid].back();
				(*data)[tid].pop_back();

				v1 = (*data)[tid].back();
                		(*data)[tid].pop_back();

				parser[v1 - min].push_back(v2);
			}

			count = 0;
			(*data)[tid].push_back(-1); // insert local root count later

			for(pid = min; pid <= max; pid++)
			{
				size = parser[pid - min].size();
				if(size > 0)
				{
					count++;
					(*data)[tid].push_back(pid);
					(*data)[tid].push_back(size);

					for(j = 0; j < size; j++)
					{
						(*data)[tid].push_back(parser[pid - min].back());
						parser[pid - min].pop_back();
					}
				}
			}

			(*data)[tid][0] = count;
			parser.clear();

			count = (*data)[tid][0];
			int k = 1, size, u = 0;
			for(pid = 0; pid < count; pid++)
			{		
				v2 = (*data)[tid][k++];
				size = (*data)[tid][k++];

				for(j = k; j < size; j++, k++)
				{
					v1 = (*data)[tid][k++];

					if(v2 != verify[u++])
						cout << "SOMETHING IS WRONG" << endl;

					if(v1 != verify[u++])
                    				cout << "SOMETHING IS WRONG" << endl;
				}	
			}
		}
	}

	void ClusteringAlgo::get_clusters_distributed()
	{
		int rank, nproc, i;
        	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        	MPI_Comm_size(MPI_COMM_WORLD, &nproc);

		// get the root all the local points first
		// store in a message buffer in case need to ask other processors

		vector < vector <int > > merge_received;
        	vector < vector <int > > merge_send1;
        	vector < vector <int > > merge_send2;
		vector <int> init;

		merge_received.resize(nproc, init);
        	merge_send1.resize(nproc, init);
        	merge_send2.resize(nproc, init);

		int pid;

	        for(pid = 0; pid < nproc; pid++)
        	{
	        	merge_received[pid].reserve(m_pts->m_i_num_points);
        	    	merge_send1[pid].reserve(m_pts->m_i_num_points);
            		merge_send2[pid].reserve(m_pts->m_i_num_points);
        	}

        	vector < vector <int > >* pswap;
        	vector < vector <int > >* p_cur_send;
        	vector < vector <int > >* p_cur_insert;

        	p_cur_send = &merge_send1;
        	p_cur_insert = &merge_send2;

		m_child_count.reserve(m_pts->m_i_num_points);
		m_child_count.resize(m_pts->m_i_num_points, 0);		

		int root, local_continue_to_run = 0, global_continue_to_run;
		for(i = 0; i < m_pts->m_i_num_points; i++)
		{
			// find the point containing i
			root = i;
			while(m_parents_pr[root] == rank)
			{
				if(m_parents[root] == root)
					break;
				root = m_parents[root];
			}
			
			if(m_parents[root] == root && m_parents_pr[root] == rank) // root is a local root
			{
				// set the root of i directly to root
				m_parents[i] = root;
				m_child_count[root] = m_child_count[root] + 1; // increase the child count by one
				//m_parents_pr[i] = rank; // NO NEED TO SET THIS AS IT				
			}
			else
			{
				(*p_cur_insert)[m_parents_pr[root]].push_back(0); // flag: 0 means query and 1 means a reply
				(*p_cur_insert)[m_parents_pr[root]].push_back(m_parents[root]);
               			(*p_cur_insert)[m_parents_pr[root]].push_back(i);
            			(*p_cur_insert)[m_parents_pr[root]].push_back(rank);
				local_continue_to_run++;
			}			
		}
		
		// MAY BE REMOVED
		//MPI_Barrier(MPI_COMM_WORLD);

		int pos, round = 0, quadraples, scount, tid, tag = 0, rtag, rsource, rcount, isend[nproc], irecv[nproc], flag;

		MPI_Request s_req_recv[nproc], s_req_send[nproc], d_req_send[nproc], d_req_recv[nproc]; // better to malloc the memory
        	MPI_Status  s_stat, d_stat_send[nproc], d_stat;
		int target_point, source_point, source_pr;

		while(1)
		{
			global_continue_to_run = 0;
            		MPI_Allreduce(&local_continue_to_run, &global_continue_to_run, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

            		if(global_continue_to_run == 0)
                		break;

        		pswap = p_cur_insert;
           		p_cur_insert = p_cur_send;
           		p_cur_send = pswap;

			for(tid = 0; tid < nproc; tid++)
           			(*p_cur_insert)[tid].clear();
	
			scount = 0;
			for(tid = 0; tid < nproc; tid++)
			{
				isend[tid] = (*p_cur_send)[tid].size();
            			if(isend[tid] > 0)
	            		{
    	        			MPI_Isend(&(*p_cur_send)[tid][0], isend[tid], MPI_INT, tid, tag + 1, MPI_COMM_WORLD, &d_req_send[scount]);
        	        		scount++;
	            		}
			}

			MPI_Alltoall(&isend[0], 1, MPI_INT, &irecv[0], 1, MPI_INT, MPI_COMM_WORLD);

			rcount = 0;
			for(tid = 0; tid < nproc; tid++)
			{
				if(irecv[tid] > 0)
				{
					merge_received[tid].clear();
					merge_received[tid].assign(irecv[tid], -1);
					MPI_Irecv(&merge_received[tid][0], irecv[tid], MPI_INT, tid, tag + 1, MPI_COMM_WORLD, &d_req_recv[rcount]);
					rcount++;
				}
			}

			local_continue_to_run = 0;
			
			for(tid = 0; tid < rcount; tid++)
			{
				MPI_Waitany(rcount, &d_req_recv[0], &pos, &d_stat);
			
				rtag = d_stat.MPI_TAG;
				rsource = d_stat.MPI_SOURCE;
	
				if(rtag == tag + 1)
				{
					quadraples = merge_received[rsource].size()/4;

					for(pid = 0; pid < quadraples; pid++)
					{
						// get the quadraple
						source_pr = merge_received[rsource].back();
						merge_received[rsource].pop_back();

                        			source_point = merge_received[rsource].back();
                        			merge_received[rsource].pop_back();

                        			target_point = merge_received[rsource].back();
                        			merge_received[rsource].pop_back();
			
						flag = merge_received[rsource].back();
                        			merge_received[rsource].pop_back();

						if(flag == 0)
						{					
							root = target_point;
							while(m_parents_pr[root] == rank)
							{
								if(m_parents[root] == root)
									break;
								root = m_parents[root];
							}
			
							if(m_parents[root] == root && m_parents_pr[root] == rank) // root is a local root
							{
								m_child_count[root] = m_child_count[root] + 1; // increase the child count by one
								// have to return the child about root
								
                                				(*p_cur_insert)[source_pr].push_back(1);
                                				(*p_cur_insert)[source_pr].push_back(source_point);
                                				(*p_cur_insert)[source_pr].push_back(m_parents[root]);
                                				(*p_cur_insert)[source_pr].push_back(m_parents_pr[root]);
								local_continue_to_run++;
							}
							else
							{
								(*p_cur_insert)[m_parents_pr[root]].push_back(0);
								(*p_cur_insert)[m_parents_pr[root]].push_back(m_parents[root]);
      				         			(*p_cur_insert)[m_parents_pr[root]].push_back(source_point);
			        	    			(*p_cur_insert)[m_parents_pr[root]].push_back(source_pr);
								local_continue_to_run++;
							}
						}
						else
						{
							// got a reply, so just set the parent
							m_parents[target_point] = source_point;
							m_parents_pr[target_point] = source_pr;
						}
					}
				}
			}

			tag++;

			round++;

			if(scount > 0)
				MPI_Waitall(scount, &d_req_send[0], &d_stat_send[0]); // wait for all the sending operation
		}

		// MAY BE REMOVED
		//MPI_Barrier(MPI_COMM_WORLD);

		int final_cluster_root = 0, total_final_cluster_root = 0;

		int points_in_cluster_final = 0, total_points_in_cluster_final = 0;

		for(i = 0; i < m_pts->m_i_num_points; i++)
		{
			if(m_parents[i] == i && m_parents_pr[i] == rank && m_child_count[i] > 1)
			{
				points_in_cluster_final += m_child_count[i];
				final_cluster_root++;
			}
		}
		
		MPI_Allreduce(&points_in_cluster_final, &total_points_in_cluster_final, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
		MPI_Allreduce(&final_cluster_root, &total_final_cluster_root, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

		int total_points = 0;
		MPI_Allreduce(&m_pts->m_i_num_points, &total_points, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

        	if(rank == proc_of_interest) cout << "Points in clusters " << total_points_in_cluster_final << " Noise " << total_points - total_points_in_cluster_final << " Total points " << total_points << endl;

        	if(rank == proc_of_interest) cout << "Total number of clusters " << total_final_cluster_root << endl;

		vector<int> global_roots;
		global_roots.resize(nproc, 0);
		MPI_Allgather(&final_cluster_root, sizeof(int), MPI_BYTE, &global_roots[0], sizeof(int), MPI_BYTE, MPI_COMM_WORLD);	
	

		int cluster_offset = 0;

		for(i = 0; i <= rank; i++)
			cluster_offset += global_roots[i];

		m_pid_to_cid.clear();
		m_pid_to_cid.resize(m_pts->m_i_num_points, -1);

		// assign for the global roots only
		for(i = 0; i < m_pts->m_i_num_points; i++)
		{
			if(m_parents[i] == i && m_parents_pr[i] == rank)
			{
				if(m_child_count[i] > 1)
				{
					m_pid_to_cid[i] = cluster_offset;
					cluster_offset++;
				}
				else
					m_pid_to_cid[i] = 0; // noise point
			}
		}
			
		for(i = 0; i < m_pts->m_i_num_points; i++)
		{
			if(m_parents_pr[i] == rank)
            		{
				if(m_parents[i] != i) //skip the noise points
				{
                			m_pid_to_cid[i] = m_pid_to_cid[m_parents[i]];
               			}
            		}
			else
			{
				// ask the outer to to send back the clusterID
				(*p_cur_insert)[m_parents_pr[i]].push_back(0);
				(*p_cur_insert)[m_parents_pr[i]].push_back(m_parents[i]);
				(*p_cur_insert)[m_parents_pr[i]].push_back(i);
				(*p_cur_insert)[m_parents_pr[i]].push_back(rank);
			}
		}
		
		// MAY BE REMOVED
		//MPI_Barrier(MPI_COMM_WORLD);

		tag++;

		int later_count;
		for(later_count = 0; later_count < 2; later_count++)
		{
        		pswap = p_cur_insert;
           		p_cur_insert = p_cur_send;
           		p_cur_send = pswap;

			for(tid = 0; tid < nproc; tid++)
           			(*p_cur_insert)[tid].clear();
	
			scount = 0;
			for(tid = 0; tid < nproc; tid++)
			{
				isend[tid] = (*p_cur_send)[tid].size();
            			if(isend[tid] > 0)
	            		{
    	        			MPI_Isend(&(*p_cur_send)[tid][0], isend[tid], MPI_INT, tid, tag + 1, MPI_COMM_WORLD, &d_req_send[scount]);
        	        		scount++;
	            		}
			}

			MPI_Alltoall(&isend[0], 1, MPI_INT, &irecv[0], 1, MPI_INT, MPI_COMM_WORLD);

			rcount = 0;
			for(tid = 0; tid < nproc; tid++)
			{
				if(irecv[tid] > 0)
				{
					merge_received[tid].clear();
					merge_received[tid].assign(irecv[tid], -1);
					MPI_Irecv(&merge_received[tid][0], irecv[tid], MPI_INT, tid, tag + 1, MPI_COMM_WORLD, &d_req_recv[rcount]);
					rcount++;
				}
			}

			for(tid = 0; tid < rcount; tid++)
			{
				MPI_Waitany(rcount, &d_req_recv[0], &pos, &d_stat);
			
				rtag = d_stat.MPI_TAG;
				rsource = d_stat.MPI_SOURCE;
	
				if(rtag == tag + 1)
				{
					quadraples = merge_received[rsource].size()/4;

					for(pid = 0; pid < quadraples; pid++)
					{
						// get the quadraple
						source_pr = merge_received[rsource].back();
						merge_received[rsource].pop_back();

                        			source_point = merge_received[rsource].back();
                        			merge_received[rsource].pop_back();

                        			target_point = merge_received[rsource].back();
                        			merge_received[rsource].pop_back();
			
						flag = merge_received[rsource].back();
                        			merge_received[rsource].pop_back();

						if(flag == 0)
						{					
                        				(*p_cur_insert)[source_pr].push_back(1);
                           				(*p_cur_insert)[source_pr].push_back(source_point);
                            				(*p_cur_insert)[source_pr].push_back(m_pid_to_cid[m_parents[target_point]]);
							(*p_cur_insert)[source_pr].push_back(-1); // One extra INT, may be needed in future
						}
						else
						{
							// got a reply, so just set the parent
							m_pid_to_cid[target_point] = source_point; // this assigns the clusterID
						}
					}
				}
			}

            		if(scount > 0)
                		MPI_Waitall(scount, &d_req_send[0], &d_stat_send[0]); // wait for all the sending operation
		
			//MPI_Barrier(MPI_COMM_WORLD); // MAY NEED TO ACTIVATE THIS
			tag++;
		}

		merge_received.clear();
        	merge_send1.clear();
        	merge_send2.clear();
        	init.clear();
		global_roots.clear();
	}

	void ClusteringAlgo::writeCluster_distributed(string outfilename)
	{
		int rank, nproc;
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		MPI_Comm_size(MPI_COMM_WORLD, &nproc);

		int i;

		// get the total number of points
		int total_points = 0;
		MPI_Allreduce(&m_pts->m_i_num_points, &total_points, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	
		vector<int> point_count;
        	point_count.resize(nproc, 0);
        	MPI_Allgather(&m_pts->m_i_num_points, sizeof(int), MPI_BYTE, &point_count[0], sizeof(int), MPI_BYTE, MPI_COMM_WORLD);

		int ret, ncfile;
		
		string outfilename_dis = outfilename;
		outfilename_dis = outfilename_dis; //"_clusters.nc";
	
		// create the file, if exists, open the file.
		ret = ncmpi_create(MPI_COMM_WORLD, outfilename_dis.c_str(), NC_CLOBBER, MPI_INFO_NULL, &ncfile);	
        	if (ret != NC_NOERR)    
		{
        		handle_error(ret, __LINE__);
            		return;
        	}	

		MPI_Offset num_particles = total_points;
		int num_particles_id;

		ret = ncmpi_def_dim(ncfile, "num_particles", num_particles, &num_particles_id);
        	if (ret != NC_NOERR)
        	{
            		handle_error(ret, __LINE__);
            		return;
        	}
		
       		string column_name_initial = "position_col_X";
        	stringstream column_name_id;
        	string column_name;

		int j, ncolumn = 1, col_id = 0, varid[m_pts->m_i_dims + 1]; //number of column is 1, col_id is 0 as we use the first one

        	// write the columns
        	for(j = 0; j < m_pts->m_i_dims; j++)
        	{
        		column_name_id.str("");
        		column_name_id << j;
        
       			column_name = column_name_initial + column_name_id.str(); 
        	
			ret = ncmpi_def_var(ncfile, column_name.c_str(), NC_FLOAT, ncolumn, &col_id, &varid[j]);        	
			if (ret != NC_NOERR) 
        		{
            			handle_error(ret, __LINE__);
    	        		return;
        		}        
     		}

		column_name = "cluster_id";

        	ret = ncmpi_def_var(ncfile, column_name.c_str(), NC_INT, ncolumn, &col_id, &varid[j]);
        	if (ret != NC_NOERR)
        	{
        		handle_error(ret, __LINE__);
            		return;
        	}

  		ret = ncmpi_enddef(ncfile);
  		if (ret != NC_NOERR) 
		{
			handle_error(ret, __LINE__);
			return;
		}

		MPI_Offset start[2], count[2];
			
		start[0] = 0;
		for(i = 0; i < rank; i++)
		{
			start[0] += point_count[i];	
		}

		count[0] = point_count[rank];
		start[1] = 0; // this to satisfy PnetCDF requirement
		count[1] = 1;//dim_sizes[dimids[1]];
		
		// allocate memory	
		float *data = new float[count[0] * count[1]];

		// write the data columns
		for(j = 0; j < m_pts->m_i_dims; j++)
		{
			// get the partial column data
			for(i = 0; i < m_pts->m_i_num_points; i++)
				data[i] = m_pts->m_points[i][j];

			// write the data
			ret = ncmpi_put_vara_float_all(ncfile, varid[j], start, count, data);
			if (ret != NC_NOERR) 
			{
				handle_error(ret, __LINE__);
				return;
			}
		}
		delete [] data;

		int	*data_id = new int[count[0] * count[1]];		

		//write the cluster_ids
		for(i = 0; i < m_pts->m_i_num_points; i++)
			data_id[i] = m_pid_to_cid[i];

		ret = ncmpi_put_vara_int_all(ncfile, varid[m_pts->m_i_dims], start, count, data_id);
		if (ret != NC_NOERR)
        	{
        		handle_error(ret, __LINE__);
            		return;
        	}

		delete [] data_id;	

		// close the file
		ret = ncmpi_close(ncfile);
		if (ret != NC_NOERR) 
		{
			handle_error(ret, __LINE__);
			return;
		}
	
		//cout << "rank " << rank << " AT the end of wrting PnetCDF file" << endl;
	}

	void run_dbscan_algo_uf_mpi_interleaved(ClusteringAlgo& dbs)
	{
		double start = MPI_Wtime();			
		int i, pid, j, k, npid, prID;
		int rank, nproc, mpi_namelen;
		kdtree2_result_vector ne;
		kdtree2_result_vector ne_outer;

		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    		MPI_Comm_size(MPI_COMM_WORLD, &nproc);

        	// initialize some parameters
		dbs.m_clusters.clear();
			
		// assign parent to itestf
		dbs.m_parents.resize(dbs.m_pts->m_i_num_points, -1);
		dbs.m_parents_pr.resize(dbs.m_pts->m_i_num_points, -1);

		int total_points = 0, points_per_pr[nproc], start_pos[nproc];

	        // getting the total number of local points and assigning postions	
		MPI_Allgather(&dbs.m_pts->m_i_num_points, 1, MPI_INT, &points_per_pr[0], 1, MPI_INT, MPI_COMM_WORLD);
		
		for(i = 0; i < nproc; i++)
		{
			start_pos[i] = total_points;
			total_points += points_per_pr[i];
		}

		// assign proc IDs
		vector <int> vec_prID;
		vec_prID.resize(total_points, -1);

		k = 0;
		for(i = 0; i < nproc; i++)
		{
			for(j = 0; j < points_per_pr[i]; j++)
			{
				vec_prID[k++] = i;
			}
		}

		// restting the membership and corepoints values
		dbs.m_member.resize(dbs.m_pts->m_i_num_points, 0);
		dbs.m_corepoint.resize(dbs.m_pts->m_i_num_points, 0);

		vector<int>* ind = dbs.m_kdtree->getIndex();
		vector<int>* ind_outer = dbs.m_kdtree_outer->getIndex();

		// setting paretns to itself and corresponding proc IDs
		for(i = 0; i < dbs.m_pts->m_i_num_points; i++)
		{
			pid = (*ind)[i];
			
			dbs.m_parents[pid] = pid;
			dbs.m_parents_pr[pid] = rank;
		}

		vector < vector <int > > merge_received;
		vector < vector <int > > merge_send1;
		vector < vector <int > > merge_send2;
		vector <int> init;
		int rtag, rsource, tag = 0, pos = 0, scount, rcount, isend[nproc], irecv[nproc];
		
		merge_received.resize(nproc, init);
		merge_send1.resize(nproc, init);
		merge_send2.resize(nproc, init);
		
		// reserving communication buffer memory
		for(pid = 0; pid < nproc; pid++)
		{
			merge_received[pid].reserve(dbs.m_pts->m_i_num_points * nproc);
			merge_send1[pid].reserve(dbs.m_pts->m_i_num_points * nproc);
			merge_send2[pid].reserve(dbs.m_pts->m_i_num_points * nproc);
		}

		int root, root1, root2, tid;

		vector < vector <int > >* pswap;
		vector < vector <int > >* p_cur_send;
		vector < vector <int > >* p_cur_insert;

		p_cur_send = &merge_send1;
		p_cur_insert = &merge_send2;

		if(rank == proc_of_interest) cout << "Init time " << MPI_Wtime() - start << endl;	

		MPI_Barrier(MPI_COMM_WORLD);
	
		// the main part of the DBSCAN algorithm (called local computation)
		start = MPI_Wtime();
		for(i = 0; i < dbs.m_pts->m_i_num_points; i++)
		{
			pid = (*ind)[i];
			
			// getting the local neighborhoods of local point
			ne.clear();
            		dbs.m_kdtree->r_nearest_around_point(pid, 0, dbs.m_epsSquare, ne);
			
 			ne_outer.clear();
			vector<float> qv(dbs.m_pts->m_i_dims);

  			for (int u = 0; u < dbs.m_pts->m_i_dims; u++)
    				qv[u] = dbs.m_kdtree->the_data[pid][u];

			// getting the remote neighborhood of the local point
			if(dbs.m_pts_outer->m_i_num_points > 0)
				dbs.m_kdtree_outer->r_nearest(qv, dbs.m_epsSquare, ne_outer);
		
			qv.clear();
			
			if(ne.size() + ne_outer.size() >= dbs.m_minPts)
			{
				// pid is a core point
				root = pid;

				dbs.m_corepoint[pid] = 1;
				dbs.m_member[pid] = 1;
				
				// traverse the rmote neighbors and add in the communication buffers	
				for(j = 0; j < ne_outer.size(); j++)
				{
					npid = ne_outer[j].idx;

					(*p_cur_insert)[dbs.m_pts_outer->m_prIDs[npid]].push_back(pid);
                    			(*p_cur_insert)[dbs.m_pts_outer->m_prIDs[npid]].push_back(dbs.m_pts_outer->m_ind[npid]);
				}
				
				//traverse the local neighbors and perform union operation
				for (j = 0; j < ne.size(); j++)
                		{
					npid = ne[j].idx;

					// get the root containing npid
					root1 = npid;
					root2 = root;

					if(dbs.m_corepoint[npid] == 1 || dbs.m_member[npid] == 0)
					{
						dbs.m_member[npid] = 1;

						// REMS algorithm to (union) merge the trees
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

                            					// splicing comression technique
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

				                            	// splicing compressio technique
			                            		int z = dbs.m_parents[root2];
                        				    	dbs.m_parents[root2] = dbs.m_parents[root1];                  
    								root2 = z;
							}
						}
					}
				}
				
			}
		}
			
		MPI_Barrier(MPI_COMM_WORLD);

		int v1, v2, par_proc, triples, local_count, global_count;
		double temp_inter_med, inter_med, stop = MPI_Wtime();

		if(rank == proc_of_interest) cout << "Local computation took " << stop - start << endl;

		inter_med = MPI_Wtime();


		start = stop;
		i = 0;
			
		MPI_Request s_req_recv[nproc], s_req_send[nproc], d_req_send[nproc], d_req_recv[nproc]; // better to malloc the memory
		MPI_Status  s_stat, d_stat_send[nproc], d_stat;

		start = MPI_Wtime();

		local_count = 0;
	
		// performing additional compression for the local points that are being sent 
		// this steps identifies the points that actually going to connect the trees in other processors
		// this step will eventually helps further compression before the actual communication happens
		for(tid = 0; tid < nproc; tid++)
		{
			triples = (*p_cur_insert)[tid].size()/2;
			local_count += triples;

			for(pid = 0; pid < triples; pid++)
			{
				v1 = (*p_cur_insert)[tid][2 * pid];

				root1 = v1;
				while(dbs.m_parents[root1] != root1)
                			root1 = dbs.m_parents[root1];

				while(dbs.m_parents[v1] != root1)
		            	{
                			int tmp = dbs.m_parents[v1];
                			dbs.m_parents[v1] = root1;
    		            		v1 = tmp;
            			}

				(*p_cur_insert)[tid][2 * pid] = root1;
			}
		}

		local_count = local_count/nproc;
        	
		global_count = 0;
        	MPI_Allreduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

		//message_per_round
		int uv, uf, um, ul, ucount;
		int local_continue_to_run, global_continue_to_run;
		double dcomtime = 0, comtime = 0, sum_comp_rate = 0;
		vector <vector <int> > parser;
		vector <int> init_ex;
		parser.resize(dbs.m_pts->m_i_num_points, init_ex);
		
		while(1)
		{
			pswap = p_cur_insert;
            		p_cur_insert = p_cur_send;  
            		p_cur_send = pswap;

            		for(tid = 0; tid < nproc; tid++)
                		(*p_cur_insert)[tid].clear();

			// Uncommend the following if you want to compress in the first round, where compression ratio could be high
			//if(dbs.m_compression == 1 && i == 0)
			//{
			//	dbs.trivial_compression(p_cur_send, &parser, nproc, rank, i, comtime, sum_comp_rate);
			//}
			
			// send all the data
			// compress the data before send
			//dbs.convert(p_cur_send, nproc, rank, i);
		
			scount = 0;
			for(tid = 0; tid < nproc; tid++)
			{
            	    		if(dbs.m_compression == 1 && i == 0 && (*p_cur_send)[tid].size() > 0)
                		{
	                    		dbs.trivial_compression(&(*p_cur_send)[tid], &parser, nproc, rank, i, comtime, sum_comp_rate);
    	            		}

				isend[tid] = (*p_cur_send)[tid].size();
            	    		if(isend[tid] > 0)
	                	{
    	                		MPI_Isend(&(*p_cur_send)[tid][0], isend[tid], MPI_INT, tid, tag + 1, MPI_COMM_WORLD, &d_req_send[scount]);
        	            		scount++;
	                	}
			}

			MPI_Alltoall(&isend[0], 1, MPI_INT, &irecv[0], 1, MPI_INT, MPI_COMM_WORLD);

			rcount = 0;
			for(tid = 0; tid < nproc; tid++)
			{
				if(irecv[tid] > 0)
				{
					merge_received[tid].clear();
					merge_received[tid].assign(irecv[tid], -1);
					MPI_Irecv(&merge_received[tid][0], irecv[tid], MPI_INT, tid, tag + 1, MPI_COMM_WORLD, &d_req_recv[rcount]);
						rcount++;
				}
			}

			local_count = 0;
			
			//get the data and process them
			for(tid = 0; tid < rcount; tid++)
			{
				MPI_Waitany(rcount, &d_req_recv[0], &pos, &d_stat);
			
				rtag = d_stat.MPI_TAG;
				rsource = d_stat.MPI_SOURCE;
	
				if(rtag == tag + 1)	
				{
					// process received the data now
					if(dbs.m_messages_per_round == -1 && i == 0)
					{
						if(dbs.m_compression == 1)
						{
							// call the decompression function
							dbs.trivial_decompression(&merge_received[rsource], nproc, rank, i, dcomtime);

							triples = merge_received[rsource].size()/2;
							par_proc = rsource;
						}
						else
						{
							triples = merge_received[rsource].size()/2;
							par_proc = rsource;
						}
					}						
					else
						triples = merge_received[rsource].size()/3;

					for(pid = 0; pid < triples; pid++)
					{
						// get the pair
						v1 = merge_received[rsource].back();
						merge_received[rsource].pop_back();
						
						if((dbs.m_messages_per_round == -1 && i > 0) || (dbs.m_messages_per_round != -1))
						{
							par_proc = merge_received[rsource].back();
							merge_received[rsource].pop_back();
						}
				
						v2 = merge_received[rsource].back();
						merge_received[rsource].pop_back();
				
						int con = 0;
						
						if(i > 0)
							con = 1;
						else if (i == 0 && (dbs.m_corepoint[v1] == 1 || dbs.m_member[v1] == 0))
						{	
							dbs.m_member[v1] = 1;
							con = 1;
						}

						if(con == 1)
						{
				
							root1 = v1;
							// this will find the boundary vertex or the root if the root is in this processor
							while(dbs.m_parents_pr[root1] == rank)
							{
								if(dbs.m_parents[root1] == root1)
									break;

								root1 = dbs.m_parents[root1];
							}
				
							// compress the local path
							while(v1 != root1 && vec_prID[v1] == rank)
							{
								int tmp = dbs.m_parents[v1];
								dbs.m_parents[v1] = root1;
								v1 = tmp;
							}
	
							if(dbs.m_parents[root1] == v2 && dbs.m_parents_pr[root1] == par_proc)
							{
								//same_set++;
								continue;
							}						
								
							if(par_proc == rank)
							{
								if(dbs.m_parents[root1] == dbs.m_parents[v2])
									continue;
							}
								
							if(dbs.m_parents[root1] == root1 && dbs.m_parents_pr[root1] == rank) // root1 is a local root
							{
								if(start_pos[rank] + root1 < start_pos[par_proc] + v2)
								{
									// do union
									dbs.m_parents[root1] = v2;
									dbs.m_parents_pr[root1] = par_proc;
									continue;
								}
								else
								{
									// ask the parent of v2
									(*p_cur_insert)[par_proc].push_back(root1);
									(*p_cur_insert)[par_proc].push_back(dbs.m_parents_pr[root1]);
									(*p_cur_insert)[par_proc].push_back(v2);

									local_count++;
								}
							}
							else
							{
								// root1 is not local
								if(start_pos[dbs.m_parents_pr[root1]] + root1 < start_pos[par_proc] + v2)
								{
									// ask the parent of root1
                       	    						(*p_cur_insert)[dbs.m_parents_pr[root1]].push_back(v2);
									(*p_cur_insert)[dbs.m_parents_pr[root1]].push_back(par_proc);
									(*p_cur_insert)[dbs.m_parents_pr[root1]].push_back(dbs.m_parents[root1]);
										
									local_count++;
								}
								else
								{
									// ask the parent of v2
                	    	            				(*p_cur_insert)[par_proc].push_back(dbs.m_parents[root1]);
                                        				(*p_cur_insert)[par_proc].push_back(dbs.m_parents_pr[root1]);
                                        				(*p_cur_insert)[par_proc].push_back(v2);

									local_count++;
								}
							}
						}
					}
					
					merge_received[rsource].clear();
				}
				else
				{
					cout << "rank " << rank << " SOMETHING IS WRONG" << endl;
				}
			}
				
			if(scount > 0)
				MPI_Waitall(scount, &d_req_send[0], &d_stat_send[0]);	

			tag += 2; // change the tag value although not important
			
			local_continue_to_run = 0;
			local_count = 0;
			for(tid = 0; tid < nproc; tid++)
			{
				local_count += (*p_cur_insert)[tid].size()/3;
				if((*p_cur_insert)[tid].size() > 0)
					local_continue_to_run = 1;
			}
			
			local_count = local_count / nproc;

			global_count = 0;
			global_continue_to_run = 0;
            
			MPI_Allreduce(&local_continue_to_run, &global_continue_to_run, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
			
			if(global_continue_to_run == 0)
                		break;
			i++;
		}

		stop = MPI_Wtime(); 
        	if(rank == proc_of_interest) cout << "Merging took " << stop - start << endl;

		pswap = NULL;
		p_cur_insert = NULL;
		p_cur_send = NULL;

		for(tid = 0; tid < nproc; tid++)
		{
			merge_received[tid].clear();
			merge_send1[tid].clear();
			merge_send2[tid].clear();
		}

		merge_received.clear();
		merge_send1.clear();
		merge_send2.clear();
		ind = NULL;
		ind_outer = NULL;

		vec_prID.clear();
		ne.clear();
		ne_outer.clear();
		parser.clear();
		init_ex.clear();
		init.clear();
	}
};

