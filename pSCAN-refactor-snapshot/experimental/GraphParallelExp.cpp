#include "GraphParallelExp.h"

#include <cstring>
#include <cmath>

#include <iostream>
#include <algorithm>

#include "../playground/pretty_print.h"

#include "../ThreadPool.h"

using namespace std::chrono;

GraphExp::GraphExp(const char *dir_string, const char *eps_s, int min_u, ui thread_num) {
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
    similar_degree = vector<int>(n, 0);

    // 3rd: disjoint-set, make-set at the beginning
    disjoint_set_ptr = yche::make_unique<DisjointSet>(n);

    thread_num_ = thread_num;
}

void GraphExp::Output(const char *eps_s, const char *miu) {
    io_helper_ptr->Output(eps_s, miu, noncore_cluster, similar_degree, cluster_dict, disjoint_set_ptr->parent);
}

ui GraphExp::BinarySearch(vector<int> &array, ui offset_beg, ui offset_end, int val) {
    auto mid = static_cast<ui>((static_cast<unsigned long>(offset_beg) + offset_end) / 2);
    if (array[mid] == val) { return mid; }
    return val < array[mid] ? BinarySearch(array, offset_beg, mid, val) : BinarySearch(array, mid + 1, offset_end, val);
}

int GraphExp::ComputeCnLowerBound(int du, int dv) {
    auto c = (int) (sqrtl((((long double) du) * ((long double) dv) * eps_a2) / eps_b2));
    if (((long long) c) * ((long long) c) * eps_b2 < ((long long) du) * ((long long) dv) * eps_a2) { ++c; }
    return c;
}

void GraphExp::Prune() {
    auto batch_size = 8192u;
    ThreadPool pool(thread_num_);
    for (auto v_i = 0; v_i < n; v_i += batch_size) {
        int my_start = v_i;
        int my_end = min(n, my_start + batch_size);

        pool.enqueue([this](int i_start, int i_end) {
            for (auto u = i_start; u < i_end; u++) {
                for (auto edge_idx = out_edge_start[u]; edge_idx < out_edge_start[u + 1]; edge_idx++) {
                    auto v = out_edges[edge_idx];
                    if (u <= v) {
                        int deg_a = degree[u], deg_b = degree[v];
                        if (deg_a > deg_b) { swap(deg_a, deg_b); }
                        if (((long long) deg_a) * eps_b2 < ((long long) deg_b) * eps_a2) {
                            // can be pruned
                            min_cn[edge_idx] = NOT_DIRECT_REACHABLE;
                        } else {
                            // can be pruned, when c <= 2
                            int c = ComputeCnLowerBound(deg_a, deg_b);
                            min_cn[edge_idx] = c <= 2 ? DIRECT_REACHABLE : c;
                        }
                    }
                }
            }
        }, my_start, my_end);
    }
}

int GraphExp::IntersectNeighborSets(int u, int v, int min_cn_num) {
    int cn = 2; // count for self and v, count for self and u
    int du = degree[u] + 1, dv = degree[v] + 1; // count for self and v, count for self and u

    auto offset_nei_u_end = out_edge_start[u + 1];
    auto offset_nei_v_end = out_edge_start[v + 1];
    for (auto offset_nei_u = out_edge_start[u], offset_nei_v = out_edge_start[v];
         offset_nei_u < offset_nei_u_end && offset_nei_v < offset_nei_v_end &&
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

    return cn >= min_cn_num ? DIRECT_REACHABLE : NOT_DIRECT_REACHABLE;
}

int GraphExp::EvalReachable(int u, ui edge_idx) {
    int v = out_edges[edge_idx];
    return IntersectNeighborSets(u, v, min_cn[edge_idx]);
}

bool GraphExp::IsDefiniteCoreVertex(int u) {
    return similar_degree[u] >= min_u;
}

void GraphExp::CheckCoreFirstBSP(int u) {
    for (auto edge_idx = out_edge_start[u]; edge_idx < out_edge_start[u + 1]; edge_idx++) {
        if (min_cn[edge_idx] > 0 && u <= out_edges[edge_idx]) {
            min_cn[edge_idx] = EvalReachable(u, edge_idx);
        }
    }
}

void GraphExp::CheckCoreSecondBSP(int u) {
    for (auto edge_idx = out_edge_start[u]; edge_idx < out_edge_start[u + 1]; edge_idx++) {
        auto v = out_edges[edge_idx];
        if (u > v) {
            min_cn[edge_idx] = min_cn[BinarySearch(out_edges, out_edge_start[v], out_edge_start[v + 1], u)];
        }
        if (min_cn[edge_idx] == DIRECT_REACHABLE) {
            ++similar_degree[u];
        }
    }
}

void GraphExp::ClusterCore(int u) {
    for (auto j = out_edge_start[u]; j < out_edge_start[u + 1]; j++) {
        if (min_cn[j] == DIRECT_REACHABLE && IsDefiniteCoreVertex(out_edges[j])) {
            disjoint_set_ptr->Union(u, out_edges[j]);
        }
    }
}

void GraphExp::MarkClusterMinEleAsId() {
    cluster_dict = vector<int>(n);
    std::fill(cluster_dict.begin(), cluster_dict.end(), n);

    for (auto i = 0; i < n; i++) {
        if (IsDefiniteCoreVertex(i)) {
            // after this, root must be left nodes' parent since disjoint_set_ptr->FindRoot(i)
            int x = disjoint_set_ptr->FindRoot(i);
            if (i < cluster_dict[x]) { cluster_dict[x] = i; }
        }
    }
}

void GraphExp::ClusterNonCores() {
    noncore_cluster = vector<pair<int, int>>();
    noncore_cluster.reserve(n);

    MarkClusterMinEleAsId();
    for (auto i = 0; i < n; i++) {
        if (IsDefiniteCoreVertex(i)) {
            for (auto j = out_edge_start[i]; j < out_edge_start[i + 1]; j++) {
                // observation 3: check non-core neighbors
                if (!IsDefiniteCoreVertex(out_edges[j]) && min_cn[j] == DIRECT_REACHABLE) {
                    noncore_cluster.emplace_back(cluster_dict[disjoint_set_ptr->FindRoot(i)], out_edges[j]);
                }
            }
        }
    }
}

void GraphExp::pSCAN() {
    // 1st: pre-processing
    auto prune_start = high_resolution_clock::now();
    Prune();
    auto prune_end = high_resolution_clock::now();
    cout << "1st: prune execution time:" << duration_cast<milliseconds>(prune_end - prune_start).count() << " ms\n";

    // 2nd: find all cores
    {
        ThreadPool pool(thread_num_);

        // lfr benchmark graphs
        if (n == 10000001) {
            auto batch_size = 256u;
            for (auto v_i = 0; v_i < n; v_i += batch_size) {
                int my_start = v_i;
                int my_end = min(n, my_start + batch_size);
                pool.enqueue([this](int i_start, int i_end) {
                    for (auto i = i_start; i < i_end; i++) { CheckCoreFirstBSP(i); }
                }, my_start, my_end);
            }
        } else {
            // 50,000,000
            auto task_count = 0u;
            if (min_cn.size() > 50000000) {
                // medium, large datasets
                // > 1000,000,000 large datsets, `min_cn.size() / n < 20`: non-social-network
                auto threshold_sum =
                        min_cn.size() < 1000000000 ? 1024 * 16 : (min_cn.size() / n < 20 ? 64 * 1024 : 1024 * 32);

                auto prev_start = 0;
                int cur_end;
                for (auto v_i = 0; v_i < n; ++v_i) {
                    cur_end = v_i + 1;

                    auto tmp_sum = 0u;
                    auto cmp0 = degree[v_i];
                    for (auto edge_idx = out_edge_start[v_i]; edge_idx < out_edge_start[v_i + 1]; edge_idx++) {
                        auto dst_idx = out_edges[edge_idx];
                        if (v_i < dst_idx && min_cn[edge_idx] > 0) {
                            tmp_sum += cmp0 + degree[dst_idx];
                        }
                    }

                    if (tmp_sum > threshold_sum) {
                        pool.enqueue([this](int i_start, int i_end) {
                            for (auto i = i_start; i < i_end; i++) { CheckCoreFirstBSP(i); }
                        }, prev_start, cur_end);

                        prev_start = cur_end;
                        ++task_count;
                    }
                }
                pool.enqueue([this](int i_start, int i_end) {
                    for (auto i = i_start; i < i_end; i++) { CheckCoreFirstBSP(i); }
                }, prev_start, cur_end);
                cout << "total task count:" << task_count << endl;
            } else {
                // small datasets
                auto batch_size = 256u;
                for (auto v_i = 0; v_i < n; v_i += batch_size) {
                    int my_start = v_i;
                    int my_end = min(n, my_start + batch_size);
                    pool.enqueue([this](int i_start, int i_end) {
                        for (auto i = i_start; i < i_end; i++) { CheckCoreFirstBSP(i); }
                    }, my_start, my_end);
                }
            }
        }
    }
    auto first_bsp_end = high_resolution_clock::now();
    cout << "2nd: check core first-phase bsp time:"
         << duration_cast<milliseconds>(first_bsp_end - prune_end).count() << " ms\n";

    vector<std::future<vector<int>>> future_vec;
    {
        ThreadPool pool(thread_num_);

        auto batch_size = 1024u;
        for (auto v_i = 0; v_i < n; v_i += batch_size) {
            int my_start = v_i;
            int my_end = min(n, my_start + batch_size);
            future_vec.emplace_back(pool.enqueue([this](int i_start, int i_end) -> vector<int> {
                auto candidates = vector<int>();
                for (auto i = i_start; i < i_end; i++) {
                    CheckCoreSecondBSP(i);
                    if (IsDefiniteCoreVertex(i)) { candidates.emplace_back(i); }
                }
                return candidates;
            }, my_start, my_end));
        }
    }
    auto second_bsp_end = high_resolution_clock::now();
    cout << "2nd: check core second-phase bsp time:"
         << duration_cast<milliseconds>(second_bsp_end - first_bsp_end).count() << " ms\n";

    // 3rd: cluster cores
    for (auto &future:future_vec) {
        auto candidates = future.get();
        for (auto candidate:candidates) { ClusterCore(candidate); }
    }
    auto end_core_cluster = high_resolution_clock::now();
    cout << "3rd: core clustering time:" << duration_cast<milliseconds>(end_core_cluster - second_bsp_end).count()
         << " ms\n";

    // 4th: non-core clustering
    ClusterNonCores();
    auto all_end = high_resolution_clock::now();
    cout << "4th: non-core clustering time:" << duration_cast<milliseconds>(all_end - end_core_cluster).count()
         << " ms\n";
}