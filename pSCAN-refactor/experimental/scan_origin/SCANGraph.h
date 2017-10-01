//
// Created by yche on 9/29/17.
//

#ifndef PPSCAN_SCANGRAPH_H
#define PPSCAN_SCANGRAPH_H

#include <memory>

#include "../../InputOutput.h"

class SCANGraph {
private:
    unique_ptr<InputOutput> io_helper_ptr;
    // parameter1: e.g eps: 0.13, eps_a:13, eps_b:100;
    // parameter2: min_u: 5, 5 nearest neighbor as threshold
    int eps_a2, eps_b2, min_u;

    // compressed spare row graph
    ui n;
    vector<ui> out_edge_start;
    vector<int> out_edges;

    // edge properties
    vector<int> min_cn; //minimum common neighbor: -2 means not similar; -1 means similar; 0 means not sure; > 0 means the minimum common neighbor

    // vertex properties
    vector<int> degree;
    vector<bool> is_core_lst;
    vector<bool> is_non_core_lst;

    // clusters: core and non-core(hubs)
    vector<int> cluster_dict;    // observation 2: core vertex clusters are disjoint
    // first: cluster id(min core-vertex id in cluster), second: non-core vertex id
    vector<pair<int, int>> noncore_cluster; // observation 1: clusters may overlap, observation 3: non-core uniquely determined by core

    // statistics
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
private:
    // optimization: common-neighbor check pruning, as a pre-processing phase
    int ComputeCnLowerBound(int du, int dv);

    // density-eval related
    int IntersectNeighborSets(int u, int v, int min_cn_num);

    int EvalSimilarity(int u, ui edge_idx);

    // optimization: find reverse edge index, e.g, (i,j) index know, compute (j,i) index
    ui BinarySearch(vector<int> &array, ui offset_beg, ui offset_end, int val);

    bool CheckCore(int u);

    void CheckCoreAndCluster();

public:
    explicit SCANGraph(const char *dir_string, const char *eps_s, int min_u);

    void SCAN();

    void Output(const char *eps_s, const char *miu);
};

#endif //PPSCAN_SCANGRAPH_H
