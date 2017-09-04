#include "Graph.h"

#include <cstring>
#include <cmath>

#include <iostream>
#include <algorithm>
#include <numeric>
#include <chrono>
#include "../playground/pretty_print.h"

using namespace std::chrono;

Graph::Graph(const char *dir_string, const char *eps_s, int min_u) {
    io_helper_ptr = yche::make_unique<InputOutput>(dir_string);
    io_helper_ptr->ReadGraph();

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
    std::fill(min_cn.begin(), min_cn.end(), NOT_SURE);

    // vertex properties
    degree = std::move(io_helper_ptr->degree);
    is_core_lst = vector<bool>(n, false);
    // 3rd: disjoint-set, make-set at the beginning
    disjoint_set_ptr = yche::make_unique<DisjointSet>(n);
}

void Graph::Output(const char *eps_s, const char *miu) {
    io_helper_ptr->Output(eps_s, miu, noncore_cluster, is_core_lst, cluster_dict, disjoint_set_ptr->parent);
}

ui Graph::BinarySearch(vector<int> &array, ui offset_beg, ui offset_end, int val) {
    auto mid = static_cast<ui>((static_cast<unsigned long>(offset_beg) + offset_end) / 2);
    if (array[mid] == val) { return mid; }
    return val < array[mid] ? BinarySearch(array, offset_beg, mid, val) : BinarySearch(array, mid + 1, offset_end, val);
}

int Graph::ComputeCnLowerBound(int du, int dv) {
    auto c = (int) (sqrtl((((long double) du) * ((long double) dv) * eps_a2) / eps_b2));
    if (((long long) c) * ((long long) c) * eps_b2 < ((long long) du) * ((long long) dv) * eps_a2) { ++c; }
    return c;
}

void Graph::Prune() {
    for (auto u = 0; u < n; u++) {
        for (auto j = out_edge_start[u]; j < out_edge_start[u + 1]; j++) {
            auto v = out_edges[j];
            if (u <= v) {
                int deg_a = degree[u], b = degree[v];
                if (deg_a > b) { swap(deg_a, b); };
                if (((long long) deg_a) * eps_b2 < ((long long) b) * eps_a2) {
                    // can be pruned
#ifdef STATISTICS
                    ++prune0;
#endif
                    min_cn[j] = NOT_DIRECT_REACHABLE;
                } else {
                    // can be pruned, when c <= 2
                    int c = ComputeCnLowerBound(deg_a, b);
                    if (c <= 2) {
#ifdef STATISTICS
                        ++prune1;
#endif
                        min_cn[j] = DIRECT_REACHABLE;
                    } else {
                        min_cn[j] = c;
                    }
                }
            }
        }
    }
}

int Graph::IntersectNeighborSets(int u, int v, int min_cn_num) {
    int cn = 2; // count for self and v, count for self and u
    int du = degree[u] + 1, dv = degree[v] + 1; // count for self and v, count for self and u
#ifdef STATISTICS
    intersection_times++;
    auto tmp0 = 0;
    auto tmp1 = 0;
#endif

    auto offset_nei_u_end = out_edge_start[u + 1];
    auto offset_nei_v_end = out_edge_start[v + 1];
    for (auto offset_nei_u = out_edge_start[u], offset_nei_v = out_edge_start[v];
         offset_nei_u < offset_nei_u_end && offset_nei_v < offset_nei_v_end &&
         cn < min_cn_num && du >= min_cn_num && dv >= min_cn_num;) {
        if (out_edges[offset_nei_u] < out_edges[offset_nei_v]) {
            --du;
            ++offset_nei_u;
#ifdef STATISTICS
            ++all_cmp0;
            ++tmp0;
#endif
        } else if (out_edges[offset_nei_u] > out_edges[offset_nei_v]) {
            --dv;
            ++offset_nei_v;
#ifdef STATISTICS
            ++all_cmp1;
            ++tmp1;
#endif
        } else {
            ++cn;
            ++offset_nei_u;
            ++offset_nei_v;
#ifdef STATISTICS
            ++all_cmp2;
            ++tmp0;
            ++tmp1;
#endif
        }
    }
#ifdef STATISTICS
    int max_val = max(tmp0, tmp1);
    int min_val = min(tmp0, tmp1);
    int local_portion = min_val == 0 ? max_val : max_val / min_val;
    portion = max(portion, local_portion);
#endif
    return cn >= min_cn_num ? DIRECT_REACHABLE : NOT_DIRECT_REACHABLE;
}

int Graph::EvalReachable(int u, ui edge_idx) {
    int v = out_edges[edge_idx];
    return IntersectNeighborSets(u, v, min_cn[edge_idx]);
}

void Graph::CheckCoreFirstBSP(int u) {
    auto sd = 0;
    for (auto edge_idx = out_edge_start[u]; edge_idx < out_edge_start[u + 1]; edge_idx++) {
        if (min_cn[edge_idx] > 0 && u <= out_edges[edge_idx]) {
            min_cn[edge_idx] = EvalReachable(u, edge_idx);
            if (min_cn[edge_idx] == DIRECT_REACHABLE) {
                ++sd;
                if (sd >= min_u) {
                    is_core_lst[u] = true;
                    return;
                }
            }
        }
    }
}

void Graph::CheckCoreSecondBSP(int u) {
    if (!is_core_lst[u]) {
        auto sd = 0;
        auto ed = degree[u];
        for (auto edge_idx = out_edge_start[u]; edge_idx < out_edge_start[u + 1]; edge_idx++) {
            auto v = out_edges[edge_idx];
            if (min_cn[edge_idx] == NOT_SURE) {
                min_cn[edge_idx] = min_cn[BinarySearch(out_edges, out_edge_start[v], out_edge_start[v + 1], u)];
            }

            if (min_cn[edge_idx] == DIRECT_REACHABLE) {
                ++sd;
                if (sd >= min_u) {
                    is_core_lst[u] = true;
                    return;
                }
            }
            if (min_cn[edge_idx] == NOT_DIRECT_REACHABLE) {
                --ed;
                if (ed < min_u) {
                    return;
                }
            }
        }

        for (auto edge_idx = out_edge_start[u]; edge_idx < out_edge_start[u + 1]; edge_idx++) {
            if (min_cn[edge_idx] > 0) {
                min_cn[edge_idx] = EvalReachable(u, edge_idx);
                if (min_cn[edge_idx] == DIRECT_REACHABLE) {
                    ++sd;
                    if (sd >= min_u) {
                        is_core_lst[u] = true;
                        return;
                    }
                } else {
                    --ed;
                    if (ed < min_u) {
                        return;
                    }
                }
            }
        }
    }
}

void Graph::ClusterCore(int u) {
    for (auto j = out_edge_start[u]; j < out_edge_start[u + 1]; j++) {
        auto v = out_edges[j];
        if (u < v && is_core_lst[v] && !disjoint_set_ptr->IsSameSet(u, v)) {
            if (min_cn[j] > 0) {
                min_cn[j] = EvalReachable(u, j);
            }
            if (min_cn[j] == DIRECT_REACHABLE) {
                disjoint_set_ptr->Union(u, out_edges[j]);
            }
        }
    }
}

void Graph::MarkClusterMinEleAsId() {
    cluster_dict = vector<int>(n);
    std::fill(cluster_dict.begin(), cluster_dict.end(), n);

    for (auto i = 0; i < n; i++) {
        if (is_core_lst[i]) {
            // after this, root must be left nodes' parent since disjoint_set_ptr->FindRoot(i)
            int x = disjoint_set_ptr->FindRoot(i);
            if (i < cluster_dict[x]) { cluster_dict[x] = i; }
        }
    }
}

void Graph::ClusterNonCores() {
    noncore_cluster = vector<pair<int, int>>();
    noncore_cluster.reserve(n);

    MarkClusterMinEleAsId();
    for (auto i = 0; i < n; i++) {
        if (is_core_lst[i]) {
            for (auto j = out_edge_start[i]; j < out_edge_start[i + 1]; j++) {
                // observation 3: check non-core neighbors
                auto v = out_edges[j];
                if (!is_core_lst[v]) {
                    if (min_cn[j] == 0) {
                        min_cn[j] = min_cn[BinarySearch(out_edges, out_edge_start[v], out_edge_start[v + 1], i)];
                    }
                    if (min_cn[j] > 0) {
                        min_cn[j] = EvalReachable(i, j);
                    }
                    if (min_cn[j] == DIRECT_REACHABLE) {
                        noncore_cluster.emplace_back(cluster_dict[disjoint_set_ptr->FindRoot(i)], out_edges[j]);
                    }
                }
            }
        }
    }
}

void Graph::pSCAN() {
    // 1st: pre-processing
    cout << "new algo" << endl;
    auto prune_start = high_resolution_clock::now();
    Prune();
    auto prune_end = high_resolution_clock::now();
    cout << "1st: prune execution time:" << duration_cast<milliseconds>(prune_end - prune_start).count() << " ms\n";

    // 2nd: find all cores
    auto find_core_start = high_resolution_clock::now();
    vector<int> candidates;
    for (auto i = 0; i < n; i++) {
        CheckCoreFirstBSP(i);
    }
    auto first_bsp_end = high_resolution_clock::now();
    cout << "2nd: check core first-phase bsp time:"
         << duration_cast<milliseconds>(first_bsp_end - find_core_start).count() << " ms\n";

    for (auto i = 0; i < n; i++) {
        CheckCoreSecondBSP(i);
        if (is_core_lst[i]) { candidates.emplace_back(i); }
    }
    auto second_bsp_end = high_resolution_clock::now();
    cout << "2nd: check core second-phase bsp time:"
         << duration_cast<milliseconds>(second_bsp_end - first_bsp_end).count() << " ms\n";

    for (auto candidate:candidates) { ClusterCore(candidate); }

    auto end_core_cluster = high_resolution_clock::now();
    cout << "3rd: core clustering time:" << duration_cast<milliseconds>(end_core_cluster - second_bsp_end).count()
         << " ms\n";

    // 4th: non-core clustering
    ClusterNonCores();
    auto all_end = high_resolution_clock::now();
    cout << "4th: non-core clustering time:" << duration_cast<milliseconds>(all_end - end_core_cluster).count()
         << " ms\n";

#ifdef STATISTICS
    cout << "\nprune0 definitely not reachable:" << prune0 << "\nprune1 definitely reachable:" << prune1 << "\n";
    cout << "intersection times:" << intersection_times << "\ncmp0:" << all_cmp0 << "\ncmp1:" << all_cmp1
         << "\nequal cmp:" << all_cmp2 << "\n";
    cout << "max portion:" << portion << endl;
#endif
}
