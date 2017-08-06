#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <memory>

#include "InputOutput.h"

using namespace std;

constexpr int NOT_SIMILAR = -2;
constexpr int SIMILAR = -1;
constexpr int NOT_SURE = 0;

constexpr int INVALID_VERTEX_IDX = -1;
constexpr int ALREADY_EXPLORED = -1;

// Graph instance: fast consumption object
class Graph {
private:
    unique_ptr<InputOutput> io_helper_ptr;
    // algorithm parameter1: e.g eps: 0.13, eps_a:13, eps_b:100;
    // algorithm parameter2: min_u: 5, 5 nearest neighbor as threshold
    int eps_a2, eps_b2, min_u;

    // compressed spare row graph
    ui n;
    vector<ui> out_edge_start;
    vector<int> out_edges;

    // edge properties
    vector<ui> reverse_edge_idx; //the position of reverse_edge_idx edge in out_edges
    vector<int> min_cn; //minimum common neighbor: -2 means not similar; -1 means similar; 0 means not sure; > 0 means the minimum common neighbor

    // vertex properties
    vector<int> degree;
    vector<int> similar_degree;
    vector<int> effective_degree;

    // disjoint-set: used for core-vertex induced connected components
    vector<int> parent;
    vector<int> rank;

    // clusters: core and non-core(hubs)
    vector<int> cid;    // observation 2: core vertex clusters are disjoint
    vector<pair<int, int>> noncore_cluster; // observation 1: clusters may overlap, observation 3: non-core uniquely determined by core
    vector<int> cores;
    vector<ui> dst_vertices;

public:
    explicit Graph(const char *dir_string, const char *eps_s, int miu);

    void pSCAN();

    void Output(const char *eps_s, const char *miu);

private:
    pair<int, int> ParseEps(const char *eps_s);

    // disjoint-set related, see CLRS chapter 22 for detail
    void MakeDisjointSetBatch();

    int FindRoot(int u);

    void Union(int u, int v);

    // 1st phase
    ui BinarySearch(vector<int> &array, ui offset_beg, ui offset_end, int val);

    int ComputeCnLowerBound(int u, int v);

    void PruneAndCrossLink();

    int CheckCore(int u);

    void ClusterCore(int u, int index_i);

    // 2nd phase
    int CheckCn(int u, int v, int c);

    int SimilarCheckOpt(int u, ui idx);

    void ClusterNonCores();

};

#endif
