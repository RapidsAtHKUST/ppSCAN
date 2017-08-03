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

#include "geometric_partitioning.h"
//#include <boost/foreach.hpp>

namespace NWUClustering
{
	void get_extra_points(ClusteringAlgo& dbs)
	{
		#ifdef _DEBUG_GP
		MPI_Barrier(MPI_COMM_WORLD);
		double end, start = MPI_Wtime();
		#endif

		int k, rank, nproc, i, j;
        	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        	MPI_Comm_size(MPI_COMM_WORLD, &nproc);

        	#ifdef _DEBUG
        	if(rank == proc_of_interest) cout << "extra point time part 0 strating " << endl;
        	#endif
		
		interval* local_box = new interval[dbs.m_pts->m_i_dims];
		compute_local_bounding_box(dbs, local_box);

		// extend the box
		//cout << "proc " << rank << " org: upper "<< local_box[0].upper << " lower " << local_box[0].lower << endl;
		float eps = sqrt(dbs.m_epsSquare);

		for(i = 0; i < dbs.m_pts->m_i_dims; i++)
		{
			local_box[i].upper += eps;
			local_box[i].lower -= eps; 
		}

		//cout << "proc " << rank << " ext: upper "<< local_box[0].upper << " lower " << local_box[0].lower << endl;	
	
		// all together all the extending bounding box
		interval* gather_local_box = new interval[dbs.m_pts->m_i_dims * nproc];

        	// gather the local bounding box first
        	MPI_Allgather(local_box, sizeof(interval) * dbs.m_pts->m_i_dims, MPI_BYTE, gather_local_box,
        						sizeof(interval) * dbs.m_pts->m_i_dims, MPI_BYTE, MPI_COMM_WORLD);

		/*#ifdef _DEBUG_GP
		if(rank == proc_of_interest)
		{
			for(i = 0; i < dbs.m_pts->m_i_dims * nproc; i++)
			{
					if(i % dbs.m_pts->m_i_dims == 0)
						cout << "\nproc " << i / dbs.m_pts->m_i_dims << " bbox: ";
					
					if(i % dbs.m_pts->m_i_dims == dbs.m_pts->m_i_dims - 1)
                		cout << "(" << gather_local_box[i].upper << ", " << gather_local_box[i].lower << ")";
            		else
                		cout << "(" << gather_local_box[i].upper << ", " << gather_local_box[i].lower << "), ";				
			}
			cout << endl;
		}
		#endif
		*/

		bool if_inside, overlap;
		int count = 0, gcount;

		vector <float> empty;
		vector <vector <float> > send_buf;
		vector <vector <float> > recv_buf;
		send_buf.resize(nproc, empty);
		recv_buf.resize(nproc, empty);

	        vector <int> empty_i;
        	vector <vector <int> > send_buf_ind;
        	vector <vector <int> > recv_buf_ind;
        	send_buf_ind.resize(nproc, empty_i);
        	recv_buf_ind.resize(nproc, empty_i);

		#ifdef _DEBUG_GP
		MPI_Barrier(MPI_COMM_WORLD);
		end = MPI_Wtime();
		#ifdef _DEBUG
		if(rank == proc_of_interest) cout << "extra point time part 1: " << end - start << endl;	
		#endif
		start = end;
		#endif

		for(k = 0; k < nproc; k++)
		{
			if(k == rank) // self
				continue;

			// check the two extended bounding box of proc rank and k. If they don't overlap, there must be no points 
			// SHOULD SUBTRACT EPS			

			overlap = true;
			for(j = 0; j < dbs.m_pts->m_i_dims; j++)
			{
				if(gather_local_box[rank * dbs.m_pts->m_i_dims + j].lower < gather_local_box[k * dbs.m_pts->m_i_dims +j].lower)
				{
					//if(gather_local_box[rank * dbs.m_pts->m_i_dims + j].upper < gather_local_box[k * dbs.m_pts->m_i_dims + j].lower)
					if(gather_local_box[rank * dbs.m_pts->m_i_dims + j].upper - gather_local_box[k * dbs.m_pts->m_i_dims + j].lower < eps)	
					{
						overlap = false;
	                    			break;
					}
				}
				else
				{
                    			//if(gather_local_box[k * dbs.m_pts->m_i_dims + j].upper < gather_local_box[rank * dbs.m_pts->m_i_dims + j].lower)
                    			if(gather_local_box[k * dbs.m_pts->m_i_dims + j].upper - gather_local_box[rank * dbs.m_pts->m_i_dims + j].lower < eps)
					{
                        			overlap = false;
                        			break;
                    			}
				}
			}

			// the two bouding boxes are different, so continue to the next processors
			if(overlap == false)
				continue;

			// get the overlapping regions
			for(i = 0; i < dbs.m_pts->m_i_num_points; i++)
			{
				if_inside = true;
				for(j = 0; j < dbs.m_pts->m_i_dims; j++)
				{
					if(dbs.m_pts->m_points[i][j] < gather_local_box[k * dbs.m_pts->m_i_dims + j].lower || 
						dbs.m_pts->m_points[i][j] > gather_local_box[k * dbs.m_pts->m_i_dims + j].upper)
					{
						if_inside = false;
						break;
					}
				}
				
				if(if_inside == true)
				{
					for(j = 0; j < dbs.m_pts->m_i_dims; j++)
						send_buf[k].push_back(dbs.m_pts->m_points[i][j]);
					
					send_buf_ind[k].push_back(i);
					count++;
				}
			}
		}

        	#ifdef _DEBUG_GP
		MPI_Barrier(MPI_COMM_WORLD);
        	end = MPI_Wtime();
        	#ifdef _DEBUG
        	if(rank == proc_of_interest) cout << "extra point time part 2: " << end - start << endl;        
        	#endif
        	start = end;
        	#endif

		// now send buf have all the points. Send the size first to everyone
		vector <int> send_buf_size, recv_buf_size;
		send_buf_size.resize(nproc, 0);
		recv_buf_size.resize(nproc, 0);

		
		/*int tid, irecv[nproc], isend[nproc];
		MPI_Request s_req_recv[nproc], s_req_send[nproc], d_req_send[nproc], d_req_recv[nproc]; // better to malloc the memory
        MPI_Status  s_stat, d_stat_send[nproc], d_stat;

		double start, stop;
	    MPI_Barrier(MPI_COMM_WORLD);
        start = MPI_Wtime();

        for(tid = 0; tid < nproc; tid++)
        	MPI_Irecv(&irecv[tid], 1, MPI_INT, tid, 1000, MPI_COMM_WORLD, &s_req_recv[tid]);

        //int scount = 0, pos;
        int pos;
        for(tid = 0; tid < nproc; tid++)
        {	
			isend[tid] = send_buf[tid].size();
            MPI_Isend(&isend[tid], 1, MPI_INT, tid, 1000, MPI_COMM_WORLD, &s_req_send[tid]);
       	} 

        for(tid = 0; tid < nproc; tid++)
        {
                MPI_Waitany(nproc, &s_req_recv[0], &pos, &s_stat);
		}

		MPI_Barrier(MPI_COMM_WORLD);

		if(rank == proc_of_interest) 
		{
		    stop = MPI_Wtime();
			cout << "Size sending time ISIR " << stop - start << endl;
		}

		MPI_Barrier(MPI_COMM_WORLD);
		start = MPI_Wtime();
		*/

		for(i = 0; i < nproc; i++)
			send_buf_size[i] = send_buf[i].size();
	
		MPI_Alltoall(&send_buf_size[0], 1, MPI_INT, &recv_buf_size[0], 1, MPI_INT, MPI_COMM_WORLD);
		/*MPI_Barrier(MPI_COMM_WORLD);

		#ifdef _DEBUG
		if(rank == proc_of_interest) cout << "Size sending time AtA " << MPI_Wtime() - start << endl;
		#endif
		*/

		//return;
		int tag = 200, send_count, recv_count;
		MPI_Request req_send[2 * nproc], req_recv[2 * nproc];
		MPI_Status stat_send[2 * nproc], stat_recv;

		recv_count = 0;
		for(i = 0; i < nproc; i++)
		{
			if(recv_buf_size[i] > 0)
			{
				recv_buf[i].resize(recv_buf_size[i], 0);
				recv_buf_ind[i].resize(recv_buf_size[i] / dbs.m_pts->m_i_dims, -1);

				MPI_Irecv(&recv_buf[i][0], recv_buf_size[i], MPI_FLOAT, i, tag, MPI_COMM_WORLD, &req_recv[recv_count++]);
				MPI_Irecv(&recv_buf_ind[i][0], recv_buf_size[i] / dbs.m_pts->m_i_dims, MPI_INT, i, tag + 1, MPI_COMM_WORLD, &req_recv[recv_count++]);
			}
		}

		send_count = 0;
        	for(i = 0; i < nproc; i++)
        	{
            		if(send_buf_size[i] > 0)
            		{
                		MPI_Isend(&send_buf[i][0], send_buf_size[i], MPI_FLOAT, i, tag, MPI_COMM_WORLD, &req_send[send_count++]);
				MPI_Isend(&send_buf_ind[i][0], send_buf_size[i] / dbs.m_pts->m_i_dims, MPI_INT, i, tag + 1, MPI_COMM_WORLD, &req_send[send_count++]);
			}
        	}

		int rtag, rsource, rpos;

		#if _GET_PARTION_STAT == 0
		dbs.allocate_outer(dbs.m_pts->m_i_dims);
		#endif

		//cout << "proc " << rank << " recv_count " << recv_count << endl;

		for(i = 0; i < recv_count; i++)
		{
			MPI_Waitany(recv_count, &req_recv[0], &rpos, &stat_recv);
		
			rtag = stat_recv.MPI_TAG;
			rsource = stat_recv.MPI_SOURCE;
		
			if(rtag == tag)
			{
				// process the request
				//cout << "proc " << rank << " add points called " << endl;
				#if _GET_EXTRA_POINT_STAT == 0  // WHY THIS IS HERE??????????????????????????????????????????????????????????????
				dbs.addPoints(rsource, recv_buf_size[rsource], dbs.m_pts->m_i_dims, recv_buf[rsource]);
				#endif
				recv_buf[rsource].clear();
			}
			else if(rtag == tag + 1)
			{
				// postpond this computation and call update points later
				// processing immediately might lead to invalid computation
			}
		}	

		//cout << "proc " << rank << " send_count " << send_count << endl;
		
		// MAY NOT NEED THIS
		if(send_count > 0)
			MPI_Waitall(send_count, &req_send[0], &stat_send[0]);

        	#ifdef _DEBUG_GP
		MPI_Barrier(MPI_COMM_WORLD);
        	end = MPI_Wtime();
	        #ifdef _DEBUG
        	if(rank == proc_of_interest) cout << "extra point time part 3: " << end - start << endl;        
        	#endif
        	start = end;
        	#endif

		// got all the points
		// now update the indices of the outer points

		#if _GET_EXTRA_POINT_STAT == 0 // WHY THIS IS HERE??????????????????????????????????????????????????????????????
		dbs.updatePoints(recv_buf_ind);
		#endif

		/*if(rank == 3)
		{
			cout << "proc " << rank << " recv ";
			for(i = 0; i < nproc; i++)
			{
				cout << recv_buf_size[i] << " ";
			}
			
			cout << endl;
		}
		*/

		//#ifdef _DEBUG
		MPI_Reduce(&count, &gcount, 1, MPI_INT, MPI_SUM, proc_of_interest, MPI_COMM_WORLD);
		
		//dbs.m_extra_point_per_processor = gcount/nproc; // save extra point per processor for future use if needed
		#ifdef _DEBUG
		if(rank == proc_of_interest)
		{
			cout << "Total extra point " << gcount << endl;
			cout << "Extra point per processor " << gcount/nproc << endl;
		}
		#endif

        	#ifdef _DEBUG_GP
		MPI_Barrier(MPI_COMM_WORLD);
        	end = MPI_Wtime();
	        #ifdef _DEBUG
		if(rank == proc_of_interest) cout << "extra point time part 4: " << end - start << endl;        
        	#endif
        	start = end;
        	#endif

		empty.clear();
		send_buf.clear();
		recv_buf.clear();
		send_buf_size.clear();
		recv_buf_size.clear();
		send_buf_ind.clear();
		recv_buf_ind.clear();

        	delete [] gather_local_box;
		delete [] local_box;
	}

	void start_partitioning(ClusteringAlgo& dbs)
	{
		int r_count, s_count, rank, nproc, i, j, k;
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		MPI_Comm_size(MPI_COMM_WORLD, &nproc);

		// compute the local bouding box for each dimention
		interval* box = new interval[dbs.m_pts->m_i_dims];
		//compute_local_bounding_box(dbs, box);
		// we don't need to compute as we have stored this while reading
		
		for(i = 0; i < dbs.m_pts->m_i_dims; i++)
		{	
			box[i].upper = dbs.m_pts->m_box[i].upper;
			box[i].lower = dbs.m_pts->m_box[i].lower;
		}

		// compute the global bouding box for each dimention
		interval* gbox = new interval[dbs.m_pts->m_i_dims];
		compute_global_bounding_box(dbs, box, gbox, nproc);

		//delete [] box;
		//delete [] gbox;
		//return;

		#ifdef _DEBUG
        	if(rank == proc_of_interest) cout << "Partitioning: Pos 1" << endl;
        	#endif

		// find the loop count for nproc processors
		int internal_nodes, partner_rank, loops, b, color, sub_rank, d, max, sub_nprocs;
		//MPI_Comm new_comm;
		MPI_Status status;

		loops = 0;
		i = nproc;
		internal_nodes = 1;
		while((i = i >> 1) > 0)
		{
			loops++;
			internal_nodes = internal_nodes << 1;
		}
		
		internal_nodes = internal_nodes << 1;
		
		//gbox for each node in the tree [ONLY upto to reaching each processor]
		interval** nodes_gbox = new interval*[internal_nodes];
		for(i = 0; i < internal_nodes; i++)
			nodes_gbox[i] = new interval[dbs.m_pts->m_i_dims];
		
		copy_global_box_to_each_node(dbs, nodes_gbox, gbox, internal_nodes);
		// now each node in the tree has gbox

		/*
 		#ifdef _DEBUG
		if(rank == proc_of_interest)
		{	cout << "proc " << rank << " nodes " << internal_nodes << " loops "<< loops << endl;		
			print_points(dbs, rank);
		}
		#endif
		*/

	       	#ifdef _DEBUG
        	if(rank == proc_of_interest) cout << "Partitioning: Pos 2" << endl;
        	#endif
	
		vector <float> send_buf;
		vector <int>   invalid_pos_as;
		vector <float> recv_buf;
				
		int pow2_i;
		float median;

		for(i = 0; i < loops; i++)
		{
			pow2_i = POW2(i);
			b  = nproc - (int) (nproc / pow2_i);
			color = (int)((rank & b) / POW2(loops - i ));
			partner_rank = rank ^ (int)(nproc/POW2(i + 1));

			MPI_Comm new_comm;
            		MPI_Comm_split(MPI_COMM_WORLD, color, rank, &new_comm);
           		MPI_Comm_rank(new_comm, &sub_rank);
	
			//	cout << "i " << i << " proc " << rank << " b " << b << " color " << color << " partner_rank " << partner_rank << " sub_rank " << sub_rank << " pow_2i " << pow2_i << endl;
			
			if(sub_rank == 0)
			{
				d = 0;
				for(j = 1; j < dbs.m_pts->m_i_dims; j++)
				{
					
					if(nodes_gbox[pow2_i + color][j].upper - nodes_gbox[pow2_i + color][j].lower > 
							nodes_gbox[pow2_i + color][d].upper - nodes_gbox[pow2_i + color][d].lower)
						d = j;
				}
			}	
	
			MPI_Bcast(&d, 1, MPI_INT, 0, new_comm);

            		//#ifdef _DEBUG
			//MPI_Barrier(MPI_COMM_WORLD);
            		//if(rank == proc_of_interest) cout << "At before starting median - round " << i << endl;
            		//#endif

			// compute the median in this dimension
			float median  = get_median(dbs, d, new_comm);		

	        	//#ifdef _DEBUG
    	    		//if(rank == proc_of_interest) cout << "median " << median << " round " << i << endl;	
			//#endif

			s_count = get_points_to_send(dbs, send_buf, invalid_pos_as, median, d, rank, partner_rank);

			if (rank < partner_rank)
			{
				MPI_Sendrecv(&s_count, 1, MPI_INT, partner_rank, 4, &r_count, 1, MPI_INT, partner_rank, 5, MPI_COMM_WORLD, &status);
				recv_buf.resize(r_count * dbs.m_pts->m_i_dims, 0.0);
			    	MPI_Sendrecv(&send_buf[0], s_count * dbs.m_pts->m_i_dims, MPI_FLOAT, partner_rank, 2,
							&recv_buf[0], r_count * dbs.m_pts->m_i_dims, MPI_FLOAT, partner_rank, 3, MPI_COMM_WORLD, &status);
				send_buf.clear();
			}
			else
			{
				MPI_Sendrecv(&s_count, 1, MPI_INT, partner_rank, 5, &r_count, 1, MPI_INT, partner_rank, 4, MPI_COMM_WORLD, &status);
				recv_buf.resize(r_count * dbs.m_pts->m_i_dims, 0.0);
				MPI_Sendrecv(&send_buf[0], s_count * dbs.m_pts->m_i_dims, MPI_FLOAT, partner_rank, 3, 
							&recv_buf[0], r_count * dbs.m_pts->m_i_dims, MPI_FLOAT, partner_rank, 2, MPI_COMM_WORLD, &status);
				send_buf.clear();
			}
	
			//#ifdef _DEBUG
			//MPI_Barrier(MPI_COMM_WORLD);
    	    		//if(rank == proc_of_interest) cout << "AT before update points - round " << i << endl;	
			//#endif
		
			update_points(dbs, s_count, invalid_pos_as, recv_buf);
			recv_buf.clear();
						
			//#ifdef _DEBUG
			//MPI_Barrier(MPI_COMM_WORLD);
    	    		//if(rank == proc_of_interest) cout << "AT after update points - round " << i << endl;	
			//#endif
	
			/*
			#ifdef _DEBUG
			if(rank == proc_of_interest)
			{
				cout << "proc " << rank << " sub proc " << sub_rank << " median " << median << " of dimension " << d << " points to recv " << r_count << " points to send " << s_count << " partner " << partner_rank << endl;			
				
				print_points(dbs, rank);
			}
			#endif

			#ifdef _DEBUG
			cout << "proc " << rank << " round " << i << " points " << dbs.m_pts->m_i_num_points << endl; 
			#endif
			*/
			
			//#ifdef _DEBUG
			//MPI_Barrier(MPI_COMM_WORLD);
    	    		//if(rank == proc_of_interest) cout << "AT before copying box - round " << i << endl;	
			//#endif
	
			copy_box(dbs, nodes_gbox[LOWER(pow2_i+color)], nodes_gbox[pow2_i+color]);
			nodes_gbox[LOWER(pow2_i+color)][d].upper =  median;
			copy_box(dbs, nodes_gbox[UPPER(pow2_i+color)], nodes_gbox[pow2_i+color]);
			nodes_gbox[UPPER(pow2_i+color)][d].lower =  median;	

            		/*
  			#ifdef _DEBUG
			MPI_Barrier(MPI_COMM_WORLD);
            		if(rank == proc_of_interest) cout << "AT after copying box - round " << i << " rank " << rank << endl;  
            		#endif

			#ifdef _DEBUG 
			cout << "proc " << rank << " round " << i << " points " << dbs.m_pts->m_i_num_points << endl;
			#endif
			*/
			MPI_Comm_free(&new_comm);
		}
        
		#ifdef _DEBUG
        	if(rank == proc_of_interest) cout << "Partitioning: Pos 3" << endl;
        	#endif
	
		/*#ifdef _DEBUG
       		cout << "proc " << rank << " points " << dbs.m_pts->m_i_num_points << endl;
        	#endif
		*/

		/*compute_local_bounding_box(dbs, box);

		#ifdef _DEBUG
		if(rank == proc_of_interest)
		{
			print_box(dbs, rank, box);
		}
		#endif
		*/

		// free the allocated memory
		for(i = 0; i < nproc; i++)
			delete [] nodes_gbox[i];

		delete [] nodes_gbox;
		delete [] gbox;
		delete [] box;

		// free communicator
		 //MPI_Comm_free(&new_comm);
	}

	void update_points(ClusteringAlgo& dbs, int s_count, vector <int>& invalid_pos_as, vector <float>& recv_buf)
	{
		int i, j, k, l, r_count = recv_buf.size() / dbs.m_pts->m_i_dims;

		//cout << "r_count " << r_count << " s_count " << s_count << endl;
	
		if(r_count >= s_count)
		{
			//invalid_pos_as.reserve(dbs.m_pts->m_i_num_points + r_count - s_count);
			invalid_pos_as.resize(dbs.m_pts->m_i_num_points + r_count - s_count, 1);

			//dbs.m_pts->m_points.reserve(dbs.m_pts->m_i_num_points + r_count - s_count);
			//dbs.m_pts->m_points.resize(extents[dbs.m_pts->m_i_num_points + r_count - s_count][dbs.m_pts->m_i_dims]);
			
			//allocate memory for the points
                	dbs.m_pts->m_points.resize(dbs.m_pts->m_i_num_points + r_count - s_count);
                	for(int ll = 0; ll < dbs.m_pts->m_i_num_points + r_count - s_count; ll++)
                		dbs.m_pts->m_points[ll].resize(dbs.m_pts->m_i_dims);

	
			j = 0;
			for(i = 0; i < invalid_pos_as.size(); i++)
			{
				if(invalid_pos_as[i] == 1)
				{
					for(k = 0; k < dbs.m_pts->m_i_dims; k++)
						dbs.m_pts->m_points[i][k] = recv_buf[j++];
				}
			}			

			dbs.m_pts->m_i_num_points = dbs.m_pts->m_i_num_points + r_count - s_count;
		}
		else
		{
			j = 0;
			i = 0;	
			if(recv_buf.size() > 0)
			{
				for(i = 0; i < dbs.m_pts->m_i_num_points; i++)
				{
					if(invalid_pos_as[i] == 1)
					{
						for(k = 0; k < dbs.m_pts->m_i_dims; k++)
							dbs.m_pts->m_points[i][k] = recv_buf[j++];
					
						if(j == recv_buf.size())
						{
							i++;
							break;
						}
					}
				}
			}
			
			l = dbs.m_pts->m_i_num_points;
			for( ; i < invalid_pos_as.size(); i++)
			{
				if(invalid_pos_as[i] == 1)
				{
					while(l > i)
					{
						l--;
						if(invalid_pos_as[l] == 0)
							break;
					}

					if(invalid_pos_as[l] == 0)	
						for(k = 0; k < dbs.m_pts->m_i_dims; k++)
							dbs.m_pts->m_points[i][k] = dbs.m_pts->m_points[l][k];
				}
			}

			//dbs.m_pts->m_points.resize(extents[dbs.m_pts->m_i_num_points + r_count - s_count][dbs.m_pts->m_i_dims]);

			//allocate memory for the points
                	dbs.m_pts->m_points.resize(dbs.m_pts->m_i_num_points + r_count - s_count);
                	for(int ll = 0; ll < dbs.m_pts->m_i_num_points + r_count - s_count; ll++)
                		dbs.m_pts->m_points[ll].resize(dbs.m_pts->m_i_dims);

			dbs.m_pts->m_i_num_points = dbs.m_pts->m_i_num_points + r_count - s_count;
		}		
	}

	int get_points_to_send(ClusteringAlgo& dbs, vector <float>& send_buf, vector <int>& invalid_pos_as, float median, int d, int rank, int partner_rank)
	{
		int i, count = 0, j;
		send_buf.reserve(dbs.m_pts->m_i_num_points * dbs.m_pts->m_i_dims);
		invalid_pos_as.clear();
		invalid_pos_as.resize(dbs.m_pts->m_i_num_points, 0);

		for(i = 0; i < dbs.m_pts->m_i_num_points; i++)
		{
			if (rank < partner_rank)
			{
				if(dbs.m_pts->m_points[i][d] > median)
				{
					invalid_pos_as[i] = 1;
					count++;
					for(j = 0; j < dbs.m_pts->m_i_dims; j++)
						send_buf.push_back(dbs.m_pts->m_points[i][j]);
				}
			}
			else
			{
                if(dbs.m_pts->m_points[i][d] <= median)
                {
                    invalid_pos_as[i] = 1;
                    count++;
                    for(j = 0; j < dbs.m_pts->m_i_dims; j++)
                        send_buf.push_back(dbs.m_pts->m_points[i][j]);
                }
			}
		}

		return count;
	}	

	float get_median(ClusteringAlgo& dbs, int d, MPI_Comm& new_comm)
	{	
		/*
		double sum, g_sum;
		//float sum, g_sum;

		sum = 0.0;
		g_sum = 0.0;

    	for (int k = 0; k < dbs.m_pts->m_i_num_points; k++) 
      		sum += dbs.m_pts->m_points[k][d];

		//MPI_Allreduce(&sum, &g_sum, 1, MPI_FLOAT, MPI_SUM, new_comm);
		MPI_Allreduce(&sum, &g_sum, 1, MPI_DOUBLE, MPI_SUM, new_comm);

		int g_point_count = 0;
		MPI_Allreduce(&dbs.m_pts->m_i_num_points, &g_point_count, 1, MPI_INT, MPI_SUM, new_comm);

		//return g_sum / static_cast<float>(g_point_count);
		return g_sum / static_cast<double>(g_point_count);
		*/



		// ADDITIONAL CODE
		float median;
		
		vector <float> data;
		data.reserve(dbs.m_pts->m_i_num_points);
		data.resize(dbs.m_pts->m_i_num_points, 0);

		for (int k=0; k < dbs.m_pts->m_i_num_points; k++)
	      	//data.push_back(dbs.m_pts->m_points[k][d]);
	      	data[k] = dbs.m_pts->m_points[k][d];

		/*
       	#ifdef _DEBUG
		int rank;
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        //MPI_Barrier(new_comm);
        MPI_Barrier(MPI_COMM_WORLD);
       	//if(rank == proc_of_interest) 
       	cout << "AT median Pos 0 - rank " << rank << " data size " << data.size() << endl;
        #endif
		*/
	
		median = findKMedian(data, data.size()/2);
		data.clear();
		
		#ifdef _DEBUG
		int rank;
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		if(rank == proc_of_interest) cout << "Median VALUE " << median << endl;
		#endif
	
       	/*#ifdef _DEBUG
		//int rank;
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        //MPI_Barrier(new_comm);
        MPI_Barrier(MPI_COMM_WORLD);
       	//if(rank == proc_of_interest) 
		cout << "AT median Pos 1 - rank " << rank << " data size " << data.size() << endl;
        #endif
		*/


		int proc_count;
		MPI_Comm_size(new_comm, &proc_count);

	    //#ifdef _DEBUG
		//int rank;
		//MPI_Comm_rank(new_comm, &rank);
    	//if(rank == proc_of_interest) cout << "proc_count " << proc_count << endl;	
		//#endif

		//cout << "proc count " << proc_count << endl;

		vector <float> all_medians;
		all_medians.resize(proc_count, 0);

		MPI_Allgather(&median, sizeof(int), MPI_BYTE, &all_medians[0], sizeof(int), MPI_BYTE, new_comm);	

  		median = findKMedian(all_medians, all_medians.size()/2); 
		all_medians.clear();
		
		return median;	
	}

	void compute_local_bounding_box(ClusteringAlgo& dbs, interval* box)
	{
		int i, j;

		//we assume each processor has at least one point
		//if(dbs.m_pts->m_i_num_points > 0)
		{
			
			for(i = 0; i < dbs.m_pts->m_i_dims; i++)
			{
				box[i].upper = dbs.m_pts->m_points[0][i];
				box[i].lower = dbs.m_pts->m_points[0][i];
			}
		
			for(i = 0; i < dbs.m_pts->m_i_dims; i++)
			{
				for(j = 1; j < dbs.m_pts->m_i_num_points; j++)
				{
					if(box[i].lower > dbs.m_pts->m_points[j][i])
						box[i].lower = dbs.m_pts->m_points[j][i];
					else if(box[i].upper < dbs.m_pts->m_points[j][i])
                	    box[i].upper = dbs.m_pts->m_points[j][i];
				}
				//if(rank == 0)
    	    		//  cout << "proc " << rank << " upper " << box[i].upper << " lower " << box[i].lower << endl;
			}
		}
		/*else
		{
			// WHAT TO DO IF THERE IS NO POINTS
			// FOR THE TIME BEING set 0 
			for(i = 0; i < dbs.m_pts->m_i_dims; i++)
			{
				box[i].upper = 0;
				box[i].lower = 0;
			}
		}*/
	}

	void compute_global_bounding_box(ClusteringAlgo& dbs, interval* box, interval* gbox, int nproc)
	{
		int i, j, k;
	
		interval* gather_local_box = new interval[dbs.m_pts->m_i_dims * nproc];
	
		// gather the local bounding box first
		MPI_Allgather(box, sizeof(interval) * dbs.m_pts->m_i_dims, MPI_BYTE, gather_local_box, 
					sizeof(interval) * dbs.m_pts->m_i_dims, MPI_BYTE, MPI_COMM_WORLD);

		// compute the global bounding box
		for(i = 0; i < dbs.m_pts->m_i_dims; i++)
		{
			gbox[i].lower = gather_local_box[i].lower;
			gbox[i].upper = gather_local_box[i].upper;
			
			k = i;
			for(j = 0; j < nproc; j++, k += dbs.m_pts->m_i_dims)
			{
				if(gbox[i].lower > gather_local_box[k].lower)
					gbox[i].lower = gather_local_box[k].lower;
				
				if(gbox[i].upper < gather_local_box[k].upper)
                    gbox[i].upper = gather_local_box[k].upper;
			}
		}
		
		delete [] gather_local_box;
	}

	void copy_global_box_to_each_node(ClusteringAlgo& dbs, interval** nodes_gbox, interval* gbox, int internal_nodes)
	{
        int i, j;
        for(i = 0; i < internal_nodes; i++)
        {
            for(j = 0; j < dbs.m_pts->m_i_dims; j++)
            {
                nodes_gbox[i][j].upper = gbox[j].upper;
                nodes_gbox[i][j].lower = gbox[j].lower;
            }
        }
	}
	
	void copy_box(ClusteringAlgo& dbs, interval* target_box, interval* source_box)
	{
			for(int j = 0; j < dbs.m_pts->m_i_dims; j++)
			{
				target_box[j].upper = source_box[j].upper;
				target_box[j].lower = source_box[j].lower;
			}
	}

	void print_points(ClusteringAlgo& dbs, int rank)
	{
		cout << "proc " << rank << " owned points: " << dbs.m_pts->m_points.size() << " verify " << dbs.m_pts->m_i_num_points << endl;
		int u, v;

		/*
		for(u = 0; u < dbs.m_pts->m_i_num_points; u++)
        {
        	for(v = 0; v < dbs.m_pts->m_i_dims; v++)
            	cout << dbs.m_pts->m_points[u][v] << " ";
            cout << endl;
        }
		*/

		cout << "proc " << rank << " outer points: " << dbs.m_pts_outer->m_points.size() << " verify " << dbs.m_pts_outer->m_i_num_points << endl;

		/*
		for(u = 0; u < dbs.m_pts_outer->m_i_num_points; u++)
        {
            for(v = 0; v < dbs.m_pts_outer->m_i_dims; v++)
                cout << dbs.m_pts_outer->m_points[u][v] << " ";
            cout << endl;
        }
		*/
	}

    void print_box(ClusteringAlgo& dbs, int rank, interval* box)
    {
        cout << "proc " << rank << " bbox: ";

		for(int v = 0; v < dbs.m_pts->m_i_dims; v++)
		{
        	if(v == dbs.m_pts->m_i_dims - 1)
				cout << "(" << box[v].upper << ", " << box[v].lower << ")";
			else
				cout << "(" << box[v].upper << ", " << box[v].lower << "), ";
		}

        cout << endl;
    }
};

