#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <memory>

#include "InputOutput.h"

using namespace std;

// Graph instance: fast consumption object
class Graph {
private:
    unique_ptr<InputOutput> io_helper_ptr;

    int eps_a2, eps_b2, miu; // eps_a2/eps_b2 = eps^2

    ui n;
    ui *pstart; //offset of neighbors of nodes
    int *edges;
    ui *reverse; //the position of reverse edge in edges
    int *min_cn; //minimum common neighbor: -2 means not similar; -1 means similar; 0 means not sure; > 0 means the minimum common neighbor

    int *degree;
    int *similar_degree;
    int *effective_degree;

    int *pa;
    int *rank;

    vector<int> cid;
    vector<pair<int, int>> noncore_cluster;

public:
    explicit Graph(const char *dir_string, const char *eps_s, int miu);

    ~Graph();

    void pSCAN();

    void cluster_noncore_vertices();

    void output(const char *eps_s, const char *miu);

private:
    int find_root(int u);

    void my_union(int u, int v);

    ui binary_search(const int *array, ui b, ui e, int val);

    int compute_common_neighbor_lowerbound(int u, int v);

    void prune_and_cross_link(int *cores, int &cores_e);

    int check_common_neighbor(int u, int v, int c);

    int similar_check_OP(int u, ui idx);
};

#endif
