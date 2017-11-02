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



#include "clusters.h"

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
	}

	int Clusters::read_file(char* infilename, int isBinaryFile)
	{
		ssize_t numBytesRead;
		int     i, j;
		int num_points, dims;

		if(isBinaryFile == 1)
        	{
			ifstream file (infilename, ios::in|ios::binary);
			if(file.is_open())
  			{
				file.read((char*)&num_points, sizeof(int));
				file.read((char*)&dims, sizeof(int));
    				
				cout << "Points " << num_points << " dims " << dims << endl;

				// allocate memory for points
				m_pts = new Points;				

				m_pts->m_i_dims = dims;
                		m_pts->m_i_num_points = num_points;
				
				//allocate memory for the points
                                m_pts->m_points.resize(num_points);
                                for(int ll = 0; ll < num_points; ll++)
                                        m_pts->m_points[ll].resize(dims);

				
				point_coord_type* pt;					
				
                        	pt = (point_coord_type*) malloc(dims * sizeof(point_coord_type));
                        
                        	for (i = 0; i < num_points; i++)
                        	{
                                	file.read((char*)pt, dims*sizeof(point_coord_type));
                                
                                	for (j = 0; j < dims; j++)
                                        	m_pts->m_points[i][j] = pt[j];
                        	}
			
				delete [] pt;	
				file.close();
  			}
			else
			{
				cout << "Error: no such file: " << infilename << endl;
				return -1;
			}
		}
		else
		{
			string line, line2, buf;
			ifstream file(infilename);
			stringstream ss;

			if (file.is_open())
  			{
				// get the first line and get the dimensions
				getline(file, line);
				line2 = line;
				ss.clear();				
				ss << line2;
			
				dims = 0;
				while(ss >> buf) // get the corordinate of the points
					dims++;

				// get point count
				num_points = 0;
				while (!file.eof())
				{
					if(line.length() == 0)
                                                continue;
					//cout << line << endl;
					num_points++;
					getline(file, line);
				}
				
				cout << "Points " << num_points << " dimensions " << dims << endl;
                               
				// allocate memory for points
				m_pts = new Points;
				m_pts->m_points.resize(num_points);
                                for(int ll = 0; ll < num_points; ll++)
                                        m_pts->m_points[ll].resize(dims);
				

				file.clear();
				file.seekg (0, ios::beg);
				
				getline(file, line);

				i = 0;
    				while (!file.eof())
    				{
					if(line.length() == 0)
						continue;

					//cout << line << endl;
					ss.clear();
					ss << line;

					j = 0;
					while(ss >> buf && j < dims) // get the corordinate of the points
					{
						m_pts->m_points[i][j] = atof(buf.c_str());
						j++;
					}
					
					i++;
					getline(file, line);
    				}

    				file.close();
  			
                                m_pts->m_i_dims = dims;
                                m_pts->m_i_num_points = num_points;
			}                
			else
			{
                                cout << "Error: no such file: " << infilename << endl;
                                return -1;
			}			
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

		m_kdtree = new kdtree2(m_pts->m_points, false);
		
		if(m_kdtree == NULL)
		{
			cout << "Falied to allocate new kd tree" << endl;
			return -1;
		}
		
		return 0;		
	} 
}
