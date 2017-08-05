#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <memory>

#include "InputOutput.h"

using namespace std;

constexpr int NOT_SIMILAR = -2;
constexpr int SIMILAR = -1;
constexpr int NOT_SURE = 0;

// Graph instance: fast consumption object
class Graph {
private:
    unique_ptr<InputOutput> io_helper_ptr;
    // e.g eps: 0.13, eps_a:13, eps_b:100;
    // min_u: 5, 5 nearest neighbor as threshold
    int eps_a2, eps_b2, min_u;

    // compressed spare row graph
    ui n;
    ui *out_edge_start;
    int *out_edges;

    // edge properties
    ui *reverse_edge_idx; //the position of reverse_edge_idx edge in out_edges
    vector<int> min_cn; //minimum common neighbor: -2 means not similar; -1 means similar; 0 means not sure; > 0 means the minimum common neighbor

    // vertex properties
    int *degree;
    vector<int> similar_degree;
    vector<int> effective_degree;

    // disjoint-set: used for core-vertex induced connected components
    vector<int> parent;
    vector<int> rank;

    // clusters: core and non-core(hubs)
    vector<int> cid;    // observation 2: core vertex clusters are disjoint
    vector<pair<int, int>> noncore_cluster; // observation 1: clusters may overlap, observation 3: non-core uniquely determined by core

public:
    explicit Graph(const char *dir_string, const char *eps_s, int miu);

    ~Graph();

    void pSCAN();

    void output(const char *eps_s, const char *miu);

private:
    // disjoint-set related, see CLRS chapter 22 for detail
    void make_disjoint_set_batch();

    int find_root(int u);

    void union_if_not_same_set(int u, int v);

    // 1st phase core implementation: prune_and_cross_link related, for cores clustering:
    ui binary_search(const int *array, ui offset_beg, ui offset_end, int val);

    int compute_cn_lower_bound(int u, int v);

    void prune_and_cross_link(vector<int> &cores, int &cores_e);

    // 2nd phase core implementation:  non-core vertices clustering related:
    int check_common_neighbor(int u, int v, int c);

    int similar_check_OP(int u, ui idx);

    void cluster_noncore_vertices();

};

#endif
