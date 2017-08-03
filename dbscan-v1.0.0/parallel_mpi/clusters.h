
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
		interval* m_box;	
	};

	struct Points_Outer
    	{
        	array2dfloat m_points;
		vector <int> m_prIDs;
		vector <int> m_ind;

        	int m_i_dims;
       	 	int m_i_num_points;
		interval* m_box;
    	};

	class Clusters
	{
	public:
		Clusters():m_pts(NULL),m_kdtree(NULL),m_pts_outer(NULL),m_kdtree_outer(NULL){ }
		virtual ~Clusters();
		
		bool 	allocate_outer(int dims);
		bool 	addPoints(int source, int buf_size, int dims, vector<float>& raw_data);
		bool 	updatePoints(vector< vector<int> >& raw_ind);

		int     read_file(char* infilename, int isBinaryFile);

		int     build_kdtree();
		int     build_kdtree_outer();	
	
	public:
		Points* 	m_pts;
		kdtree2* 	m_kdtree;

		Points_Outer*	m_pts_outer;
		kdtree2*    	m_kdtree_outer;

		vector <int> 	m_pid_to_cid; // point id to cluster id
		vector <vector <int> > m_clusters;
	};
};

#endif

