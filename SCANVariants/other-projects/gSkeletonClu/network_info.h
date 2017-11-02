#include "graph.h"

class network_info
{
public:
     string method;
     long begin_time;
     long end_time;
     long comm_number;
     int min_comm_size;
     long max_comm_size;
     double ave_comm_size;
     int hubs_number;
     int out_number;
     float eps;
     float Qs;
     float swc;
     network_info();
     ~network_info();
     void info_out(Graph *g);

};
inline network_info::network_info()
{

}
inline network_info::~network_info()
{

}
inline void network_info::info_out(Graph *g)
{
    ofstream outfile3("3.txt",ofstream::trunc);
    outfile3<<"[1]Network Infomation"<<endl
            <<"--------------------------------------------------------------"<<endl
            <<"Network File Name:"<<g->filename<<endl
            <<"Total Number of Nodes: "<<g->verNum()<<endl
            <<"Total Number of Edges: "<<g->edgeNum()<<endl<<endl;
    outfile3<<"[2]Clustering Options"<<endl
            <<"--------------------------------------------------------------"<<endl
            <<"Similarity Caculate Method:"<<method<<endl
            <<"Merge Style: "<<"Local Community"<<endl
            <<"Proceesing method of Noises: "<<"Leave Alone"<<endl<<endl;
    outfile3<<"[3]Running Time"<<endl
            <<"--------------------------------------------------------------"<<endl
            <<"Begin Time :"<<begin_time<<endl
            <<"End Time: "<<end_time<<endl
            <<"Total Running Time: "<<(end_time-begin_time)/1000.0<<"s"<<endl<<endl;
    outfile3<<"[4]Information of Communities"<<endl
            <<"--------------------------------------------------------------"<<endl
            <<"Total Number of Discovered Communities: "<<comm_number<<endl
            <<"Minimum Community Size: "<<min_comm_size<<endl
            <<"Maximum Community Size: "<<max_comm_size<<endl
            <<"Average Community Size: "<<ave_comm_size<<endl<<endl;
    outfile3<<"[5]Information of Hubs and Outliers"<<endl
            <<"--------------------------------------------------------------"<<endl
            <<"Total Number of Hubs and Outliers:"<<hubs_number+out_number<<endl
            <<"Total Number of Hubs:"<<hubs_number<<endl
            <<"Total Number of Outliers:"<<out_number<<endl
            <<"Total Number of Overlapping Hubs:"        <<endl<<endl;
    outfile3<<"[6]Qs Score"<<endl
            <<"--------------------------------------------------------------"<<endl
            <<"eps: "<<eps<<endl
            <<"Final Qs score: "<<Qs<<endl;
     }
