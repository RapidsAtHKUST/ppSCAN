#include<cstdlib>
#include<ctime>

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>

using namespace std;

struct Graph {
    int nodemax;
    int edgemax;

    // csr representation
    int *node_off;
    int *edge_dst;

    // edge_dst property
    int *edge_src;
    bool *similarity;
    int *common_node_num;
    double *sim_values;

    // vertex property
    bool *core;
    int *core_count;
    int *label;

    // other
    vector<int> degree;
    string dir;

    explicit Graph(char *dir_cstr);

    void ReadDegree();

    void CheckInputGraph();

    void ReadAdjacencyList();
};