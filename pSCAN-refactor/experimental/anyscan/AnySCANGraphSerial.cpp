//
// Created by yche on 10/3/17.
//

#include "AnySCANGraphSerial.h"

#include <cmath>
#include <chrono>
#include <queue>

using namespace std;
using namespace chrono;
using namespace yche;

AnySCANGraph::AnySCANGraph(const char *dir_string, const char *eps_s, int min_u) {
    io_helper_ptr = yche::make_unique<InputOutput>(dir_string);
    io_helper_ptr->ReadGraph();

    auto tmp_start = high_resolution_clock::now();

    // 1st: parameter
    std::tie(eps_a2, eps_b2) = io_helper_ptr->ParseEps(eps_s);
    this->min_u = min_u;

    // 2nd: graph
    // csr representation
    n = static_cast<ui>(io_helper_ptr->n);
    out_edge_start = std::move(io_helper_ptr->offset_out_edges);
    out_edges = std::move(io_helper_ptr->out_edges);

    // vertex properties
    degree = std::move(io_helper_ptr->degree);
    vertex_status = vector<char>(n, anySCAN::UN_TOUCHED);
    eps_neighbor_num = vector<int>(n, 0);
    eps_neighborhood = vector<vector<int>>(n);

    auto all_end = high_resolution_clock::now();
    cout << "other construct time:" << duration_cast<milliseconds>(all_end - tmp_start).count()
         << " ms\n";
}

int AnySCANGraph::ComputeCnLowerBound(int du, int dv) {
    auto c = (int) (sqrtl((((long double) du) * ((long double) dv) * eps_a2) / eps_b2));
    if (((long long) c) * ((long long) c) * eps_b2 < ((long long) du) * ((long long) dv) * eps_a2) { ++c; }
    return c;
}

int AnySCANGraph::IntersectNeighborSets(int u, int v, int min_cn_num) {
    int cn = 2; // count for self and v, count for self and u
    int du = out_edge_start[u + 1] - out_edge_start[u] + 2, dv =
            out_edge_start[v + 1] - out_edge_start[v] + 2; // count for self and v, count for self and u

    // correctness guaranteed by two pruning previously in computing min_cn
    for (auto offset_nei_u = out_edge_start[u], offset_nei_v = out_edge_start[v];
         offset_nei_u < out_edge_start[u + 1] && offset_nei_v < out_edge_start[v + 1] &&
         cn < min_cn_num && du >= min_cn_num && dv >= min_cn_num;) {
        if (out_edges[offset_nei_u] < out_edges[offset_nei_v]) {
            --du;
            ++offset_nei_u;
        } else if (out_edges[offset_nei_u] > out_edges[offset_nei_v]) {
            --dv;
            ++offset_nei_v;
        } else {
            ++cn;
            ++offset_nei_u;
            ++offset_nei_v;
        }
    }
    return cn >= min_cn_num ? SIMILAR : NOT_SIMILAR;
}

int AnySCANGraph::EvalSimilarity(int u, ui edge_idx) {
    int v = out_edges[edge_idx];
    int deg_a = degree[u], deg_b = degree[v];
    int c = ComputeCnLowerBound(deg_a, deg_b);
    return IntersectNeighborSets(u, v, c);
}

void AnySCANGraph::anySCAN() {

}

void AnySCANGraph::Output(const char *eps_s, const char *miu) {

}


