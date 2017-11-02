
#include "clusters.h"
#include <boost/foreach.hpp>

namespace NWUClustering
{
	Clusters::~Clusters()
	{
		if(m_pts)
		{
			m_pts->m_points.clear();
			delete m_pts;
			m_pts = NULL;
		}

		if(m_kdtree)
		{
			delete m_kdtree;
			m_kdtree = NULL;
		}

        	if(m_pts_outer)
        	{
			m_pts_outer->m_prIDs.clear();
			m_pts_outer->m_ind.clear();
            		m_pts_outer->m_points.clear();
            		delete m_pts_outer;
            		m_pts_outer = NULL;
        	}

	        if(m_kdtree_outer)
        	{
	            delete m_kdtree_outer;
        	    m_kdtree_outer = NULL;
        	}
	}

	bool Clusters::allocate_outer(int dims)
	{
		if(m_pts_outer == NULL)
        	{
	            	m_pts_outer = new Points_Outer;
        		m_pts_outer->m_prIDs.clear();
            		m_pts_outer->m_ind.clear();
            		m_pts_outer->m_i_dims = dims;
            		m_pts_outer->m_i_num_points = 0;
        	}
	}

	bool Clusters::addPoints(int source, int buf_size, int dims, vector<float>& raw_data)
	{
		int i, j, k, pos, num_points = buf_size / dims;

		//cout << "add points called" << endl; 

		// incorrect dimension
		if(m_pts_outer->m_i_dims != dims)
			return false;

		pos = m_pts_outer->m_i_num_points;
        	m_pts_outer->m_i_num_points += num_points;
        	//m_pts_outer->m_points.resize(extents[m_pts_outer->m_i_num_points][dims]);

		//allocate memory for the points
                m_pts_outer->m_points.resize(m_pts_outer->m_i_num_points);
                for(int ll = 0; ll < m_pts_outer->m_i_num_points; ll++)
                	m_pts_outer->m_points[ll].resize(dims);

		m_pts_outer->m_prIDs.resize(m_pts_outer->m_i_num_points, -1);
		
		k = 0;
		for(i = 0; i < num_points; i++)
		{
			for(j = 0; j < dims; j++)
				m_pts_outer->m_points[pos][j] = raw_data[k++];
			
			m_pts_outer->m_prIDs[pos] = source;
			
			pos++;
		}

		//cout << "outer " << m_pts_outer->m_i_num_points << endl;

		return true;
	}
	
	bool Clusters::updatePoints(vector< vector<int> >& raw_ind)
	{
		int i, source = -1, j = -1, prev_source = -1;
	
		m_pts_outer->m_ind.resize(m_pts_outer->m_i_num_points, -1);

		for(i = 0; i < m_pts_outer->m_i_num_points; i++)
		{
			source = m_pts_outer->m_prIDs[i];
			
			if(source != prev_source)
				j = 0;

			m_pts_outer->m_ind[i] = raw_ind[source][j++];

			prev_source = source;
		}
		
		return true;
	}

	int Clusters::read_file(char* infilename, int isBinaryFile)
	{
		ssize_t numBytesRead;
		int     i, j, rank, nproc;
		int num_points, dims;
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		MPI_Comm_size(MPI_COMM_WORLD, &nproc);

		// only supports binary data file
		if(isBinaryFile == 1)
        	{
			// NOTE: the input data points are equally partioned and each core read their corresponding part
			// later the points will be partioned geometrically

			ifstream file (infilename, ios::in|ios::binary);
			if(file.is_open())
  			{
				file.read((char*)&num_points, sizeof(int));
				file.read((char*)&dims, sizeof(int));
    				
				#ifdef _DEBUG	
				if(rank == proc_of_interest) cout << "Points " << num_points << " dims " << dims << endl;
				#endif

				// compute the respective segments
			    	long long sch, lower, upper;

    	    			if(num_points % nproc == 0)
		       	    		sch = num_points/nproc;
        			else
           				sch = num_points/nproc + 1;

	        		#ifdef _DEBUG
    	            		if(rank == proc_of_interest)
        	       		{
		            	    	cout << "Segment size " << sch << endl;
                       			cout << "Points per process on average " << num_points/nproc << endl;
              			}
        			#endif

		    	    	lower = sch * rank;
    		    		upper = sch * (rank + 1);
	    		    	if(upper > num_points)
    	    		    		upper = num_points;

				// allocate memory for points
				m_pts = new Points;
				m_pts->m_i_dims = dims;
                		m_pts->m_i_num_points = upper - lower;//num_points;

				m_pts->m_box = new interval[m_pts->m_i_dims];	

				//allocate memory for the points
				//m_pts->m_points.resize(extents[num_points][dims]);
				//m_pts->m_points.resize(extents[m_pts->m_i_num_points][dims]);
				//allocate memory for the points
                		m_pts->m_points.resize(m_pts->m_i_num_points);
                		for(int ll = 0; ll < m_pts->m_i_num_points; ll++)
                			m_pts->m_points[ll].resize(dims);

				point_coord_type* pt;					
				
        	        	pt = (point_coord_type*) malloc(dims * sizeof(point_coord_type));
                
				// fseek to the respective position
				file.seekg(lower * dims * sizeof(point_coord_type), ios::cur);

        	       		for (i = 0; i < upper - lower; i++)
				{
               				file.read((char*)pt, dims*sizeof(point_coord_type));
                                
                   			for (j = 0; j < dims; j++)
					{
                    				m_pts->m_points[i][j] = pt[j];
							
						if(i == 0)
						{	
							m_pts->m_box[j].upper = m_pts->m_points[i][j];
							m_pts->m_box[j].lower = m_pts->m_points[i][j];
						}
						else
						{
		                   		 	if(m_pts->m_box[j].lower > m_pts->m_points[i][j])
			                      			m_pts->m_box[j].lower = m_pts->m_points[i][j];
                    					else if(m_pts->m_box[j].upper < m_pts->m_points[i][j])
                       						m_pts->m_box[j].upper = m_pts->m_points[i][j];
						}
					}
                		}
			
				free(pt);
				pt = NULL;

				file.close();
			}	
			else
			{
				cout << "rank " << rank << " Error: no such file: " << infilename << endl;
				return -1;
			}
		}
		else
		{
			cout << "Only supports binary data: Failed to read data" << endl;
			return -1;
		}
		return 0;
		
	}

	int Clusters::build_kdtree()
	{
		if(m_pts == NULL)
		{
			cout << "Point set is empty" << endl;
			return -1;
		}

		//m_kdtree = new kdtree2(m_pts->m_points, true);
		m_kdtree = new kdtree2(m_pts->m_points, false);
		
		if(m_kdtree == NULL)
		{
			cout << "Falied to allocate new kd tree for orginial points" << endl;
			return -1;
		}
		
		return 0;		
	} 

	int Clusters::build_kdtree_outer()
	{
		if(m_pts_outer == NULL)
		{
			cout << "Outer point set is empty" << endl;
			return -1;
		}
		
		if(m_pts_outer->m_i_num_points > 0)
		{

			//m_kdtree_outer = new kdtree2(m_pts->m_points_outer, true);
			m_kdtree_outer = new kdtree2(m_pts_outer->m_points, false);
		
			if(m_kdtree_outer == NULL)
			{
				cout << "Falied to allocate new kd tree for outer points" << endl;
				return -1;
			}
		}

		return 0;		
	} 
}
