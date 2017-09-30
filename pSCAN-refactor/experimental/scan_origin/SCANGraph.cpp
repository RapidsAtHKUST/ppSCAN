//
// Created by yche on 9/29/17.
//
#include "SCANGraph.h"

#include <cmath>
#include <chrono>
#include <queue>

using namespace std;
using namespace chrono;
using namespace yche;

SCANGraph::SCANGraph(const char *dir_string, const char *eps_s, int min_u) {
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

    // edge properties
    min_cn = vector<int>(io_helper_ptr->m);

    // vertex properties
    degree = std::move(io_helper_ptr->degree);
    is_core_lst = vector<bool>(n, false);
    is_non_core_lst = vector<bool>(n, false);

    auto all_end = high_resolution_clock::now();
    cout << "other construct time:" << duration_cast<milliseconds>(all_end - tmp_start).count()
         << " ms\n";
}

int SCANGraph::ComputeCnLowerBound(int du, int dv) {
    auto c = (int) (sqrtl((((long double) du) * ((long double) dv) * eps_a2) / eps_b2));
    if (((long long) c) * ((long long) c) * eps_b2 < ((long long) du) * ((long long) dv) * eps_a2) { ++c; }
    return c;
}

int SCANGraph::IntersectNeighborSets(int u, int v, int min_cn_num) {
#ifdef NAIVE_SET_INTERSECTION
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
#else
    int cn = 2; // count for self and v, count for self and u
    int du = out_edge_start[u + 1] - out_edge_start[u] + 2, dv =
            out_edge_start[v + 1] - out_edge_start[v] + 2; // count for self and v, count for self and u

    auto offset_nei_u = out_edge_start[u], offset_nei_v = out_edge_start[v];
    // correctness guaranteed by two pruning previously in computing min_cn
    while (cn < min_cn_num) {
        while (out_edges[offset_nei_u] < out_edges[offset_nei_v]) {
            --du;
            if (du < min_cn_num) {
                return NOT_SIMILAR;
            }
            ++offset_nei_u;
        }
        while (out_edges[offset_nei_u] > out_edges[offset_nei_v]) {
            --dv;
            if (dv < min_cn_num) {
                return NOT_SIMILAR;
            }
            ++offset_nei_v;
        }
        if (out_edges[offset_nei_u] == out_edges[offset_nei_v]) {
            ++cn;
            ++offset_nei_u;
            ++offset_nei_v;
        }
    }
    return cn >= min_cn_num ? SIMILAR : NOT_SIMILAR;
#endif
}

int SCANGraph::EvalSimilarity(int u, ui edge_idx) {
    // 1st case : already know from reversed link via binary-search
    if (min_cn[edge_idx] < 0) {
        return min_cn[edge_idx];
    }

    // 2nd case: similarity not known
    int v = out_edges[edge_idx];

    // compute min_cn_val
    int deg_a = degree[u], deg_b = degree[v];
    if (deg_a > deg_b) { swap(deg_a, deg_b); }
    if (((long long) deg_a) * eps_b2 < ((long long) deg_b) * eps_a2) {
        min_cn[edge_idx] = NOT_SIMILAR;
    } else {
        int c = ComputeCnLowerBound(deg_a, deg_b);
        min_cn[edge_idx] = c <= 2 ? SIMILAR : c;
    }

    // 3rd case: do computation when easy computation can not determine similarity
    if (min_cn[edge_idx] >= 0) {
        min_cn[edge_idx] = IntersectNeighborSets(u, v, min_cn[edge_idx]);
        min_cn[BinarySearch(out_edges, out_edge_start[v], out_edge_start[v + 1], u)] = min_cn[edge_idx];
    }
    return min_cn[edge_idx];
}

ui SCANGraph::BinarySearch(vector<int> &array, ui offset_beg, ui offset_end, int val) {
    auto mid = static_cast<ui>((static_cast<unsigned long>(offset_beg) + offset_end) / 2);
    if (array[mid] == val) { return mid; }
    return val < array[mid] ? BinarySearch(array, offset_beg, mid, val) : BinarySearch(array, mid + 1, offset_end, val);
}

bool SCANGraph::CheckCore(int u) {
    auto sd = 0;
    for (auto j = out_edge_start[u]; j < out_edge_start[u + 1]; j++) {
        if (EvalSimilarity(u, j) == SIMILAR) {
            sd++;
        }
    }
    if (sd >= min_u) {
        is_core_lst[u] = true;
        return true;
    } else {
        is_non_core_lst[u] = true;
        return false;
    }
}

void SCANGraph::CheckCoreAndCluster() {
    cluster_dict.resize(n, n);

    auto expansion_queue = queue<int>();

    auto is_non_core_added = vector<bool>(n);
    auto non_core_vertices = vector<int>();
    non_core_vertices.reserve(n);

    auto core_vertices = vector<int>();
    core_vertices.reserve(n);

    for (auto i = 0; i < n; i++) {
        if (!is_core_lst[i] && !is_non_core_lst[i]) { // not visited by core-predicate-checking
            if (CheckCore(i)) {
                // prepare data 1st
                non_core_vertices.clear();
                core_vertices.clear();
                is_non_core_added.assign(is_non_core_added.size(), false);

                // prepare data 2nd
                core_vertices.emplace_back(i);
                expansion_queue.emplace(i);
                int cluster_min_ele = i;

                // 1st: clustering using bfs exploration order
                while (!expansion_queue.empty()) {
                    auto u = expansion_queue.front();
                    expansion_queue.pop();

                    // iterate through similar neighbors of u
                    for (auto j = out_edge_start[u]; j < out_edge_start[u + 1]; j++) {
                        // already evaluated
                        if (min_cn[j] == SIMILAR) {
                            auto v = out_edges[j];
                            if (is_non_core_lst[v]) {
                                // avoid redundant adding of non-cores
                                if (!is_non_core_added[v]) {
                                    non_core_vertices.emplace_back(v);
                                    is_non_core_added[v] = true;
                                }
                            } else if (!is_core_lst[v]) { // avoid redundant adding of cores
                                if (CheckCore(v)) {
                                    core_vertices.emplace_back(v);
                                    expansion_queue.emplace(v);
                                    if (v < cluster_min_ele) { cluster_min_ele = v; }
                                } else {
                                    // avoid redundant adding of non-cores
                                    if (!is_non_core_added[v]) {
                                        non_core_vertices.emplace_back(v);
                                        is_non_core_added[v] = true;
                                    }
                                }
                            }
                        }
                    }
                }

                // 2nd: assign labels for cores and non-cores in the current cluster
                for (auto v_id: core_vertices) {
                    if (cluster_dict[v_id] != n) {
                        cout << "err " << v_id << endl;
                    }
                    cluster_dict[v_id] = cluster_min_ele;
                }
                for (auto v_id: non_core_vertices) {
                    noncore_cluster.emplace_back(cluster_min_ele, v_id);
                }
            }
        }
    }
}

void SCANGraph::SCAN() {
    CheckCoreAndCluster();
}

void SCANGraph::Output(const char *eps_s, const char *miu) {
    io_helper_ptr->Output(eps_s, miu, noncore_cluster, is_core_lst, cluster_dict);
}
