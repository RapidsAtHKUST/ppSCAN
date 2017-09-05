#include "Graph.h"

#include <cstring>
#include <cmath>

#include <iostream>
#include <algorithm>
#include <numeric>
#include <chrono>
#include "playground/pretty_print.h"
#include "ThreadPool.h"

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
    is_non_core_lst = vector<bool>(n, false);
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
    auto thread_num = std::thread::hardware_concurrency();
    auto batch_size = 8192u;
    ThreadPool pool(thread_num);
    for (auto v_i = 0; v_i < n; v_i += batch_size) {
        int my_start = v_i;
        int my_end = min(n, my_start + batch_size);

        pool.enqueue([this](int i_start, int i_end) {
            for (auto u = i_start; u < i_end; u++) {
                for (auto edge_idx = out_edge_start[u]; edge_idx < out_edge_start[u + 1]; edge_idx++) {
                    auto v = out_edges[edge_idx];
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
        }, my_start, my_end);
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
    auto ed = degree[u] - 1;
    for (auto edge_idx = out_edge_start[u]; edge_idx < out_edge_start[u + 1]; edge_idx++) {
        auto v = out_edges[edge_idx];
        if (min_cn[edge_idx] > 0 && u <= v) {
            min_cn[edge_idx] = EvalReachable(u, edge_idx);
            min_cn[BinarySearch(out_edges, out_edge_start[v], out_edge_start[v + 1], u)] = min_cn[edge_idx];
            if (min_cn[edge_idx] == DIRECT_REACHABLE) {
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

void Graph::CheckCoreSecondBSP(int u) {
    if (!is_core_lst[u] && !is_non_core_lst[u]) {
        auto sd = 0;
        auto ed = degree[u] - 1;
        for (auto edge_idx = out_edge_start[u]; edge_idx < out_edge_start[u + 1]; edge_idx++) {
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
            auto v = out_edges[edge_idx];
            if (min_cn[edge_idx] == NOT_SURE) {
                min_cn[edge_idx] = min_cn[BinarySearch(out_edges, out_edge_start[v], out_edge_start[v + 1], u)];
            }
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

void Graph::ClusterCoreFirstPhase(int u) {
    for (auto j = out_edge_start[u]; j < out_edge_start[u + 1]; j++) {
        auto v = out_edges[j];
        if (u < v && is_core_lst[v] && !disjoint_set_ptr->IsSameSet(u, v)) {
            if (min_cn[j] == DIRECT_REACHABLE) {
                disjoint_set_ptr->Union(u, out_edges[j]);
            }
        }
    }
}

void Graph::ClusterCore(int u) {
    for (auto edge_idx = out_edge_start[u]; edge_idx < out_edge_start[u + 1]; edge_idx++) {
        auto v = out_edges[edge_idx];
        if (u < v && is_core_lst[v] && !disjoint_set_ptr->IsSameSet(u, v)) {
            candidate_count++;
            if (min_cn[edge_idx] == NOT_SURE) {
                min_cn[edge_idx] = min_cn[BinarySearch(out_edges, out_edge_start[v], out_edge_start[v + 1], u)];
            }
            if (min_cn[edge_idx] > 0) {
                min_cn[edge_idx] = EvalReachable(u, edge_idx);
                if (min_cn[edge_idx] == DIRECT_REACHABLE) {
                    union_candidates.emplace_back(u, out_edges[edge_idx]);
                }
                if (candidate_count % 1024 * 128 == 0) {
                    for (auto candidate:union_candidates) {
                        disjoint_set_ptr->Union(candidate.first, candidate.second);
                    }
                    union_candidates.clear();
                }
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
    noncore_cluster = std::vector<pair<int, int>>();
    noncore_cluster.reserve(n);

    MarkClusterMinEleAsId();

    auto tmp_start = high_resolution_clock::now();
    auto thread_num=std::thread::hardware_concurrency();
    vector<std::future<vector<int>>> future_vec;
    {
        ThreadPool pool(thread_num);

        auto batch_size = 1024u;
        for (auto v_i = 0; v_i < n; v_i += batch_size) {
            int my_start = v_i;
            int my_end = min(n, my_start + batch_size);
            future_vec.emplace_back(pool.enqueue([this](int i_start, int i_end) -> vector<int> {
                auto candidates = vector<int>();
                for (auto i = i_start; i < i_end; i++) {
                    if (is_core_lst[i]) {
                        for (auto j = out_edge_start[i]; j < out_edge_start[i + 1]; j++) {
                            auto v = out_edges[j];
                            if (!is_core_lst[v]) {
                                if (min_cn[j] == NOT_SURE) {
                                    min_cn[j] = min_cn[BinarySearch(out_edges, out_edge_start[v], out_edge_start[v + 1], i)];
                                }
                                if (min_cn[j] > 0) {
                                    min_cn[j] = EvalReachable(i, j);
                                }
                            }
                        }
                    }
                }
                return candidates;
            }, my_start, my_end));
        }
    }
    auto tmp_end = high_resolution_clock::now();
    cout << "4th: eval cost in cluster-non-core:" << duration_cast<milliseconds>(tmp_end - tmp_start).count()
         << " ms\n";

    for (auto i = 0; i < n; i++) {
        if (is_core_lst[i]) {
            for (auto j = out_edge_start[i]; j < out_edge_start[i + 1]; j++) {
                auto v = out_edges[j];
                if (!is_core_lst[v]) {
                    auto root_of_i = disjoint_set_ptr->FindRoot(i);
                    if (min_cn[j] == DIRECT_REACHABLE) {
                        noncore_cluster.emplace_back(cluster_dict[root_of_i], v);
                    }
                }
            }
        }
    }
}

void Graph::pSCAN() {
    // 1 prune
    cout << "new algo" << endl;
    auto prune_start = high_resolution_clock::now();
    Prune();
    auto prune_end = high_resolution_clock::now();
    cout << "1st: prune execution time:" << duration_cast<milliseconds>(prune_end - prune_start).count() << " ms\n";

    // 2.1 check-core 1st bsp
    auto find_core_start = high_resolution_clock::now();
    auto thread_num = std::thread::hardware_concurrency();
    {
        ThreadPool pool(thread_num);

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
            auto task_count = 0u;

            // 50,000,000
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
         << duration_cast<milliseconds>(first_bsp_end - find_core_start).count() << " ms\n";

    // 2.2 check-core 2nd bsp
    vector<std::future<vector<int>>> future_vec;
    {
        ThreadPool pool(thread_num);

        auto batch_size = 1024u;
        for (auto v_i = 0; v_i < n; v_i += batch_size) {
            int my_start = v_i;
            int my_end = min(n, my_start + batch_size);
            future_vec.emplace_back(pool.enqueue([this](int i_start, int i_end) -> vector<int> {
                auto candidates = vector<int>();
                for (auto i = i_start; i < i_end; i++) {
                    CheckCoreSecondBSP(i);
                    if (is_core_lst[i]) { candidates.emplace_back(i); }
                }
                return candidates;
            }, my_start, my_end));
        }
    }

    auto second_bsp_end = high_resolution_clock::now();
    cout << "2nd: check core second-phase bsp time:"
         << duration_cast<milliseconds>(second_bsp_end - first_bsp_end).count() << " ms\n";

    // 3 cluster core
    auto tmp_start = high_resolution_clock::now();
    auto candidates_lst = vector<vector<int>>();
    for (auto &future_value: future_vec) { candidates_lst.emplace_back(std::move(future_value.get())); }

    for (auto &candidates: candidates_lst) {
        for (auto candidate:candidates) { ClusterCoreFirstPhase(candidate); }
    }
    auto tmp_end = high_resolution_clock::now();
    cout << "3rd: prepare time: " << duration_cast<milliseconds>(tmp_end - tmp_start).count() << " ms\n";
    // cluster-core 2nd phase
    candidates_lst.reserve(future_vec.size());
    for (auto &candidates: candidates_lst) {
        for (auto candidate:candidates) { ClusterCore(candidate); }
    }
    for (auto candidate:union_candidates) {
        disjoint_set_ptr->Union(candidate.first, candidate.second);
    }

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
    cout << "intersection times:" << intersection_times << "\ncmp0:" << all_cmp0 << "\ncmp1:" << all_cmp1
         << "\nequal cmp:" << all_cmp2 << "\n";
    cout << "max portion:" << portion << endl;
#endif
}
