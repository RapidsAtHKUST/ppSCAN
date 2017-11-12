#include "GraphSeqExp.h"

#include <cstring>
#include <cmath>

#include <iostream>
#include <chrono>
#include <algorithm>

#include "../playground/pretty_print.h"

using namespace std::chrono;
using namespace yche;

GraphSeqExp::GraphSeqExp(const char *dir_string, const char *eps_s, int min_u) {
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

    // 3rd: disjoint-set, make-set at the beginning
    disjoint_set_ptr = yche::make_unique<DisjointSet>(n);

    // 4th: cluster dict
    cluster_dict = vector<int>(n);
    std::fill(cluster_dict.begin(), cluster_dict.end(), n);

    auto all_end = high_resolution_clock::now();
    cout << "other construct time:" << duration_cast<milliseconds>(all_end - tmp_start).count()
         << " ms\n";
}

void GraphSeqExp::Output(const char *eps_s, const char *miu) {
    io_helper_ptr->Output(eps_s, miu, noncore_cluster, is_core_lst, cluster_dict, disjoint_set_ptr->parent);
}

int GraphSeqExp::ComputeCnLowerBound(int du, int dv) {
    auto c = (int) (sqrtl((((long double) du) * ((long double) dv) * eps_a2) / eps_b2));
    if (((long long) c) * ((long long) c) * eps_b2 < ((long long) du) * ((long long) dv) * eps_a2) { ++c; }
    return c;
}

int GraphSeqExp::IntersectNeighborSets(int u, int v, int min_cn_num) {
#ifdef NAIVE_SET_INTERSECTION
    int cn = 2; // count for self and v, count for self and u
    int du = out_edge_start[u + 1] - out_edge_start[u] + 2, dv =
            out_edge_start[v + 1] - out_edge_start[v] + 2; // count for self and v, count for self and u

    auto offset_nei_u = out_edge_start[u], offset_nei_v = out_edge_start[v];

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
#ifdef STATISTICS
    intersection_times++;
    max_cmp += du - 2 + dv - 2;
    auto tmp0 = 0;
    auto tmp1 = 0;
#endif

    auto offset_nei_u = out_edge_start[u], offset_nei_v = out_edge_start[v];
    // correctness guaranteed by two pruning previously in computing min_cn
    while (true) {
        while (out_edges[offset_nei_u] < out_edges[offset_nei_v]) {
#ifdef STATISTICS
            ++all_cmp0;
            ++tmp0;
#endif
            --du;
            if (du < min_cn_num) {
#ifdef STATISTICS
                int max_val = max(tmp0, tmp1);
                int min_val = min(tmp0, tmp1);
                int local_portion = min_val == 0 ? max_val : max_val / min_val;
                portion = max(portion, local_portion);
#endif
                return NOT_SIMILAR;
            }
            ++offset_nei_u;
        }
        while (out_edges[offset_nei_u] > out_edges[offset_nei_v]) {
#ifdef STATISTICS
            ++all_cmp1;
            ++tmp1;
#endif
            --dv;
            if (dv < min_cn_num) {
#ifdef STATISTICS
                int max_val = max(tmp0, tmp1);
                int min_val = min(tmp0, tmp1);
                int local_portion = min_val == 0 ? max_val : max_val / min_val;
                portion = max(portion, local_portion);
#endif
                return NOT_SIMILAR;
            }
            ++offset_nei_v;
        }
        if (out_edges[offset_nei_u] == out_edges[offset_nei_v]) {
#ifdef STATISTICS
            ++all_cmp2;
            ++tmp0;
            ++tmp1;
#endif
            ++cn;
            if (cn >= min_cn_num) {
#ifdef STATISTICS
                int max_val = max(tmp0, tmp1);
                int min_val = min(tmp0, tmp1);
                int local_portion = min_val == 0 ? max_val : max_val / min_val;
                portion = max(portion, local_portion);
                single_max_cmp = max(single_max_cmp, tmp0 + tmp1);
#endif
                return SIMILAR;
            }
            ++offset_nei_u;
            ++offset_nei_v;
        }
    }
#endif
}

int GraphSeqExp::EvalSimilarity(int u, ui edge_idx) {
    int v = out_edges[edge_idx];
    return IntersectNeighborSets(u, v, min_cn[edge_idx]);
}

ui GraphSeqExp::BinarySearch(vector<int> &array, ui offset_beg, ui offset_end, int val) {
    auto mid = static_cast<ui>((static_cast<unsigned long>(offset_beg) + offset_end) / 2);
    if (array[mid] == val) { return mid; }
    return val < array[mid] ? BinarySearch(array, offset_beg, mid, val) : BinarySearch(array, mid + 1, offset_end, val);
}

void GraphSeqExp::PruneDetail(int u) {
    for (auto j = out_edge_start[u]; j < out_edge_start[u + 1]; j++) {
        auto v = out_edges[j];
        int deg_a = degree[u], deg_b = degree[v];
        if (deg_a > deg_b) { swap(deg_a, deg_b); };

        if (((long long) deg_a) * eps_b2 < ((long long) deg_b) * eps_a2) {
            // can be pruned
#ifdef STATISTICS
            if (u <= v) {
                    ++prune0;
                }
#endif
            min_cn[j] = NOT_SIMILAR;
        } else {
            // can be pruned, when c <= 2
            int c = ComputeCnLowerBound(deg_a, deg_b);
            if (c <= 2) {
#ifdef STATISTICS
                if (u <= v) {
                        ++prune1;
                    }
#endif
                min_cn[j] = SIMILAR;
            } else {
                min_cn[j] = c;
            }
        }
    }
}

void GraphSeqExp::Prune() {
    for (auto u = 0; u < n; u++) {
        PruneDetail(u);
    }
}

void GraphSeqExp::CheckCoreFirstBSP(int u) {
    auto sd = 0;
    auto ed = degree[u] - 1;
    for (auto edge_idx = out_edge_start[u]; edge_idx < out_edge_start[u + 1]; edge_idx++) {
        auto v = out_edges[edge_idx];
//        if (u <= v) {
        if (min_cn[edge_idx] == SIMILAR) {
            ++sd;
            if (sd >= min_u) {
                is_core_lst[u] = true;
                return;
            }
        } else if (min_cn[edge_idx] == NOT_SIMILAR) {
            --ed;
            if (ed < min_u) {
                is_non_core_lst[u] = true;
                return;
            }
        }
//        }
    }

    for (auto edge_idx = out_edge_start[u]; edge_idx < out_edge_start[u + 1]; edge_idx++) {
        auto v = out_edges[edge_idx];
        if (u <= v && min_cn[edge_idx] > 0) {
#ifdef STATISTICS
            first_bsp_intersection_times++;
#endif
            min_cn[edge_idx] = EvalSimilarity(u, edge_idx);
            min_cn[BinarySearch(out_edges, out_edge_start[v], out_edge_start[v + 1], u)] = min_cn[edge_idx];
            if (min_cn[edge_idx] == SIMILAR) {
                ++sd;
                if (sd >= min_u) {
                    is_core_lst[u] = true;
                    return;
                }
            } else {
                --ed;
                if (ed < min_u) {
                    is_non_core_lst[u] = true;
                    return;
                }
            }
        }
    }
}

void GraphSeqExp::CheckCoreSecondBSP(int u) {
    if (!is_core_lst[u] && !is_non_core_lst[u]) {
        auto sd = 0;
        auto ed = degree[u] - 1;
        for (auto edge_idx = out_edge_start[u]; edge_idx < out_edge_start[u + 1]; edge_idx++) {
            if (min_cn[edge_idx] == SIMILAR) {
                ++sd;
                if (sd >= min_u) {
                    is_core_lst[u] = true;
                    return;
                }
            }
            if (min_cn[edge_idx] == NOT_SIMILAR) {
                --ed;
                if (ed < min_u) {
                    return;
                }
            }
        }

        for (auto edge_idx = out_edge_start[u]; edge_idx < out_edge_start[u + 1]; edge_idx++) {
            auto v = out_edges[edge_idx];
            if (min_cn[edge_idx] > 0) {
#ifdef STATISTICS
                second_bsp_intersection_times++;
#endif
                min_cn[edge_idx] = EvalSimilarity(u, edge_idx);
                min_cn[BinarySearch(out_edges, out_edge_start[v], out_edge_start[v + 1], u)] = min_cn[edge_idx];
                if (min_cn[edge_idx] == SIMILAR) {
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

void GraphSeqExp::ClusterCoreFirstPhase(int u) {
    for (auto j = out_edge_start[u]; j < out_edge_start[u + 1]; j++) {
        auto v = out_edges[j];
        if (u < v && is_core_lst[v] && !disjoint_set_ptr->IsSameSet(u, v)) {
            if (min_cn[j] == SIMILAR) {
                disjoint_set_ptr->Union(u, v);
            }
        }
    }
}

void GraphSeqExp::ClusterCoreSecondPhase(int u) {
    for (auto edge_idx = out_edge_start[u]; edge_idx < out_edge_start[u + 1]; edge_idx++) {
        auto v = out_edges[edge_idx];
        if (u < v && is_core_lst[v] && !disjoint_set_ptr->IsSameSet(u, v)) {
            if (min_cn[edge_idx] > 0) {
#ifdef STATISTICS
                serial_intersection_times++;
#endif
                min_cn[edge_idx] = EvalSimilarity(u, edge_idx);
                if (min_cn[edge_idx] == SIMILAR) {
                    disjoint_set_ptr->Union(u, v);
                }
            }
        }
    }
}

void GraphSeqExp::MarkClusterMinEleAsId() {
    for (auto i = 0; i < n; i++) {
        if (is_core_lst[i]) {
            // after this, root must be left nodes' parent since disjoint_set_ptr->FindRoot(i)
            int x = disjoint_set_ptr->FindRoot(i);
            if (i < cluster_dict[x]) { cluster_dict[x] = i; }
        }
    }
}

void GraphSeqExp::ClusterNonCores() {
    noncore_cluster = std::vector<pair<int, int>>();
    noncore_cluster.reserve(n);

    MarkClusterMinEleAsId();

    auto tmp_start = high_resolution_clock::now();
    for (auto i = 0; i < n; i++) {
        if (is_core_lst[i]) {
            for (auto j = out_edge_start[i]; j < out_edge_start[i + 1]; j++) {
                auto v = out_edges[j];
                if (!is_core_lst[v]) {
                    auto root_of_i = disjoint_set_ptr->FindRoot(i);
                    if (min_cn[j] > 0) {
#ifdef STATISTICS
                        core_non_core_intersection_times++;
#endif
                        min_cn[j] = EvalSimilarity(i, j);
                    }
                    if (min_cn[j] == SIMILAR) {
                        noncore_cluster.emplace_back(cluster_dict[root_of_i], v);
                    }
                }
            }
        }
    }
}

void GraphSeqExp::pSCAN() {
    cout << "new algorithm ppSCAN serial experimental" << endl;

    // 1 prune
    auto prune_start = high_resolution_clock::now();
    Prune();
    auto prune_end = high_resolution_clock::now();
    cout << "1st: prune execution time:" << duration_cast<milliseconds>(prune_end - prune_start).count() << " ms\n";

    // 2.1 check-core 1st bsp
    auto find_core_start = high_resolution_clock::now();
    vector<int> candidates;
    for (auto i = 0; i < n; i++) {
        CheckCoreFirstBSP(i);
    }
    auto first_bsp_end = high_resolution_clock::now();
    cout << "2nd: check core first-phase bsp time:"
         << duration_cast<milliseconds>(first_bsp_end - find_core_start).count() << " ms\n";

    // 2.2 check-core 2nd bsp
    for (auto i = 0; i < n; i++) {
        CheckCoreSecondBSP(i);
        if (is_core_lst[i]) { candidates.emplace_back(i); }
    }
    auto second_bsp_end = high_resolution_clock::now();
    cout << "2nd: check core second-phase bsp time:"
         << duration_cast<milliseconds>(second_bsp_end - first_bsp_end).count() << " ms\n";

    // 3 cluster core
    auto tmp_start = high_resolution_clock::now();
    for (auto candidate:candidates) { ClusterCoreFirstPhase(candidate); }
    auto tmp_end = high_resolution_clock::now();
    cout << "3rd: prepare time: " << duration_cast<milliseconds>(tmp_end - tmp_start).count() << " ms\n";
    for (auto candidate:candidates) { ClusterCoreSecondPhase(candidate); }

    auto end_core_cluster = high_resolution_clock::now();
    cout << "3rd: core clustering time:" << duration_cast<milliseconds>(end_core_cluster - second_bsp_end).count()
         << " ms\n";

    // 4 non-core clustering
    ClusterNonCores();
    auto all_end = high_resolution_clock::now();
    cout << "4th: non-core clustering time:" << duration_cast<milliseconds>(all_end - end_core_cluster).count()
         << " ms\n";

    // output statistics
#ifdef STATISTICS
    cout << "\nprune0 definitely not reachable:" << prune0 << "\nprune1 definitely reachable:" << prune1 << "\n";
    cout << "absolute intersect portion:" << static_cast<double >(intersection_times) / (min_cn.size() / 2) << "\n";
    cout << "filtered intersect portion:"
         << static_cast<double >(intersection_times) / (min_cn.size() / 2 - prune0 - prune1) << "\n\n";

    cout << "1st bsp intersection times:" << first_bsp_intersection_times << ", ratio:"
         << static_cast<double >(first_bsp_intersection_times) / intersection_times << "\n";
    cout << "2nd bsp intersection times:" << second_bsp_intersection_times << ", ratio:"
         << static_cast<double >(second_bsp_intersection_times) / intersection_times << "\n";
    cout << "serial intersection times:" << serial_intersection_times << ", ratio:"
         << static_cast<double >(serial_intersection_times) / intersection_times << "\n";
    cout << "core non-core intersection times:" << core_non_core_intersection_times << ", ratio:"
         << static_cast<double >(core_non_core_intersection_times) / intersection_times << "\n";
    cout << "intersection times:" << intersection_times << "\n\n";

    cout << "single max cmp:" << single_max_cmp << "\n";
    cout << "cmp0:" << all_cmp0 << "\ncmp1:" << all_cmp1
         << "\nequal cmp:" << all_cmp2 << "\n";
    cout << "total:" << (all_cmp0 + all_cmp1 + all_cmp2) << " ,max:" << max_cmp << " ,portion:"
         << (static_cast<double >(all_cmp0 + all_cmp1 + all_cmp2) / max_cmp) << " ,avg eval:"
         << (static_cast<double >(all_cmp0 + all_cmp1 + all_cmp2) / intersection_times) << "\n";
    cout << "max portion:" << portion << "\n" << endl;
#endif
}
