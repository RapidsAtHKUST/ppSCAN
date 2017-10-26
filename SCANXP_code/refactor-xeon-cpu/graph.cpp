#include"graph.h"

#define UNCLASSIFIED 0

GRAPH::GRAPH(char *infilename) {

    ifstream infile(infilename, ios_base::in | ios_base::binary);

    infile.read((char *) &nodemax, 4);
    infile.read((char *) &edgemax, 4);

    label = new int[nodemax];
    node = new int[nodemax + 1];
    edge = new int[edgemax];
    edgef = new int[edgemax];
    core = new bool[nodemax];
    common_node = new int[edgemax];
    ss = new double[edgemax];
    similarity = new bool[edgemax];
    core_count = new int[nodemax];

    //Reading nodes
    infile.read((char *) &node[0], sizeof(int) * (nodemax + 1));

    //Reading edges
    infile.read((char *) &edge[0], sizeof(int) * edgemax);
}
