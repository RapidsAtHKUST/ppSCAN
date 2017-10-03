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
    constexpr char UNPROCESSED_CORE = 1;
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
    vector<vector<int>> eps_neighborhood;
    vector<int> eps_neighbor_num;

    // clusters: core and non-core(hubs)
    vector<int> cluster_dict;

    unique_ptr<DisjointSet> disjoint_set_ptr;

private:
    int ComputeCnLowerBound(int du, int dv);

    int IntersectNeighborSets(int u, int v, int min_cn_num);

    int EvalSimilarity(int u, ui edge_idx);

    void Summarize();
public:
    explicit AnySCANGraph(const char *dir_string, const char *eps_s, int min_u);

    void anySCAN();

    void Output(const char *eps_s, const char *miu);
};


#endif //PPSCAN_ANYSCANGRAPH_H
