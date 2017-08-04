#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <utility>
#include <chrono>
#include <bits/unique_ptr.h>

#include "Utility.h"
#include "InputOutput.h"

using namespace std;

// fast consumption object
class Graph {
private:
    unique_ptr<InputOutput> io_helper_ptr;

    int eps_a2, eps_b2, miu; // eps_a2/eps_b2 = eps^2

    int n;
    ui *pstart; //offset of neighbors of nodes
    int *edges;
    ui *reverse; //the position of reverse edge in edges
    int *min_cn; //minimum common neighbor: -2 means not similar; -1 means similar; 0 means not sure; > 0 means the minimum common neighbor

    int *pa;
    int *rank; //pa and rank are used for the disjoint-set data structure

    int *cid; //cluster id

    int *degree;
    int *similar_degree; //number of adjacent edges with similarity no less than epsilon
    int *effective_degree; //number of adjacent edges not pruned by similarity

    vector<pair<int, int> > noncore_cluster;

public:
    explicit Graph(const char *dir_string);

    ~Graph();

    void pSCAN(const char *eps_s, int miu);

    void cluster_noncore_vertices(int eps_a2, int eps_b2, int mu);

    void output(const char *eps_s, const char *miu);

private:
    ui binary_search(const int *array, ui b, ui e, int val);

    int similar_check_OP(int u, ui idx, int eps_a, int eps_b);

    int check_common_neighbor(int u, int v, int c);

    int compute_common_neighbor_lowerbound(int u, int v, int eps_a2, int eps_b2);

    void prune_and_cross_link(int eps_a2, int eps_b2, int miu, int *cores, int &cores_e);

    int find_root(int u);

    void my_union(int u, int v);
};

#endif
