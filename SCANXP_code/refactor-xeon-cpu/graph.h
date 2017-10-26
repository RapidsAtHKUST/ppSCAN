#include<cstdlib>
#include<ctime>

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>

using namespace std;

struct GRAPH {
    int nodemax;
    int edgemax;

    // csr representation
    int *node;
    int *edge;

    // edge property
    int *edgef;
    bool *similarity;
    int *common_node;
    double *ss;

    // vertex property
    bool *core;
    int *core_count;
    int *label;

    // other
    vector<int> degree;
    string dir;

    explicit GRAPH(char *dir_cstr);

    void ReadDegree();

    void CheckInputGraph();

    void ReadAdjacencyList();
};