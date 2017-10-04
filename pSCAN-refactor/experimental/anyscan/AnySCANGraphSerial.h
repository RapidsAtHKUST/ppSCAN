//
// Created by yche on 10/3/17.
//

#ifndef PPSCAN_ANYSCANGRAPH_H
#define PPSCAN_ANYSCANGRAPH_H

#include <memory>

#include "../../InputOutput.h"
#include "../../DisjointSet.h"

namespace anySCAN {
    constexpr char UN_TOUCHED = 0;

    // eps-neighborhood unknown
    constexpr char UNPROCESSED_CORE = 1;
    // eps-neighborhood known
    constexpr char PROCESSED_CORE = 2;

    constexpr char UNPROCESSED_BORDER = 3;
    constexpr char PROCESSED_BORDER = 4;
    constexpr char UNPROCESSED_NOISE = 5;
    constexpr char PROCESSED_NOISE = 6;
}

class AnySCANGraph {
private:
    unique_ptr<InputOutput> io_helper_ptr;
    // parameter1: e.g eps: 0.13, eps_a:13, eps_b:100;
    // parameter2: min_u: 5, 5 nearest neighbor as threshold
    int eps_a2, eps_b2, min_u;
    int alpha_block_size, beta_block_size;

    // compressed spare row graph
    ui n;
    vector<ui> out_edge_start;
    vector<int> out_edges;

    // vertex property
    vector<int> degree;
    vector<char> vertex_status;
    vector<int> eps_neighbor_num;

    // clusters: core and non-core(hubs)
    vector<int> cluster_dict;

    // disjoint-sets represent core-induced clusters, eps_neighborhood has one-hop information
    unique_ptr<DisjointSet> disjoint_set_ptr;
    vector<vector<int>> eps_neighborhood;

    // used for computation data initialization
    vector<int> checking_lst;
private:
    int ComputeCnLowerBound(int du, int dv);

    int IntersectNeighborSets(int u, int v, int min_cn_num);

    int EvalSimilarity(int u, ui edge_idx);

    bool IsDefiniteCore(int u);

    void Summarize();

    void MergeStronglyRelatedCluster();

    void MergeWeaklyRelatedCluster();

    void PostProcessing();

public:
    explicit AnySCANGraph(const char *dir_string, const char *eps_s, int min_u);

    void anySCAN();

    void Output(const char *eps_s, const char *miu);
};


#endif //PPSCAN_ANYSCANGRAPH_H
