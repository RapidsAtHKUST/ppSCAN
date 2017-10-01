#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <memory>

#include "../../InputOutput.h"
#include "../../DisjointSet.h"

using namespace std;

class pSCANGraph {
private:
    unique_ptr<InputOutput> io_helper_ptr;
    ui n;

    int eps_a2, eps_b2, miu; // eps_a2/eps_b2 = eps^2

    vector<ui> pstart; //offset of neighbors of nodes
    vector<int> edges; //adjacent ids of edges

    vector<ui> reverse_link; //the position of reverse edge in edges
    vector<int> min_cn; //minimum common neighbor: -2 means not similar; -1 means similar; 0 means not sure; > 0 means the minimum common neighbor


    vector<int> degree;
    vector<int> similar_degree; //number of adjacent edges with similarity no less than epsilon
    vector<int> effective_degree; //number of adjacent edges not pruned by similarity

    vector<ui> edge_buf;
    vector<int> cores;
    vector<int> cid; //cluster id
    unique_ptr<DisjointSet> disjoint_set;
    vector<pair<int, int>> noncore_cluster;

#ifdef STATISTICS
    long prune0 = 0;
    long prune1 = 0;
    long max_cmp = 0;
    int single_max_cmp = 0;
    long all_cmp0 = 0;
    long all_cmp1 = 0;
    long all_cmp2 = 0;
    long intersection_times = 0;
    int portion = 0;
#endif
public:
    explicit pSCANGraph(const char *_dir, const char *eps_s, int min_u);

    void pSCAN();

    void Output(const char *eps_s, const char *miu);

private:
    int ComputeMinCommonNeighbor(int u, int v);

    ui BinarySearch(vector<int> &array, ui b, ui e, int val);

    int EvalSimilarityWrapper(int u, ui idx);

    int EvalSimilarity(int u, int v, int c);

private:
    void PruneCrossLink();

    int CheckCore(int u);

    void ClusterCore(int u, int early_start_idx);

    void ClusterNonCoreVertices();
};

#endif
