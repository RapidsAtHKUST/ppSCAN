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
    int *node;
    int *edge;
    int *edgef;
    bool *core;
    bool *similarity;
    int *common_node;
    double *ss;
    int *core_count;
    int *label;

    explicit GRAPH(char *infilename);
};


