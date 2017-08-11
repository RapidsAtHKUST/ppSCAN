#include "Graph.h"

#include <cstring>
#include <cmath>
#include <iostream>
#include <algorithm>

#include "play/pretty_print.h"

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
    reverse_edge_idx = vector<ui>(io_helper_ptr->m);
    min_cn = vector<int>(io_helper_ptr->m);
    std::fill(min_cn.begin(), min_cn.end(), NOT_SURE);

    // vertex properties
    degree = std::move(io_helper_ptr->degree);
    similar_degree = vector<int>(n, 0);
    effective_degree.reserve(n);
    std::transform(degree.begin(), degree.end(), back_inserter(effective_degree),
                   [](int degree_val) { return degree_val - 1; });

    // 3rd: disjoint-set, make-set at the beginning
    disjoint_set_ptr = yche::make_unique<DisjointSet>(n);
}

void Graph::Output(const char *eps_s, const char *miu) {
    io_helper_ptr->Output(eps_s, miu, noncore_cluster, similar_degree, cluster_dict, disjoint_set_ptr->parent);
}

ui Graph::BinarySearch(vector<int> &array, ui offset_beg, ui offset_end, int val) {
    --offset_end;
    if (array[offset_end] < val) { return offset_end + 1; }
    while (offset_beg < offset_end) {
        auto mid = offset_beg + (offset_end - offset_beg) / 2;
        if (array[mid] >= val) {
            offset_end = mid;
        } else {
            offset_beg = mid + 1;
        }
    }
    return offset_end;
}

void Graph::InitCrossLink(ui edge_idx, ui rev_edge_idx) {
    reverse_edge_idx[edge_idx] = rev_edge_idx;
    reverse_edge_idx[rev_edge_idx] = edge_idx;
}

void Graph::UpdateViaCrossLink(int edge_idx) {
    min_cn[reverse_edge_idx[edge_idx]] = min_cn[edge_idx];
}

int Graph::ComputeCnLowerBound(int du, int dv) {
    auto c = (int) (sqrtl((((long double) du) * ((long double) dv) * eps_a2) / eps_b2));
    if (((long long) c) * ((long long) c) * eps_b2 < ((long long) du) * ((long long) dv) * eps_a2) { ++c; }
    return c;
}

void Graph::PruneAndCrossLink() {
#ifdef STATISTICS
    for (auto i = 0; i < n; i++) {
        for (auto j = out_edge_start[i]; j < out_edge_start[i + 1]; j++) {
            auto v = out_edges[j];
            if (i <= v) {
                int a = degree[i], b = degree[v];
                if (a > b) { swap(a, b); };
                if (((long long) a) * eps_b2 < ((long long) b) * eps_a2) {
                    // can be pruned
                    ++prune0;
                    min_cn[j] = NOT_DIRECT_REACHABLE;
                } else {
                    // can be pruned, when c <= 2
                    int c = ComputeCnLowerBound(a, b);
                    if (c <= 2) {
                        ++prune1;
                        min_cn[j] = DIRECT_REACHABLE;
                    } else {
                        min_cn[j] = c;
                    }
                }

                // find edge, in order to build cross link
                auto r_id = BinarySearch(out_edges, out_edge_start[v], out_edge_start[v + 1], i);
                InitCrossLink(j, r_id);
                UpdateViaCrossLink(j);
            }
        }
    }
#else
    auto thread_num = std::thread::hardware_concurrency();
    auto batch_num = 16u * thread_num;
    auto batch_size = n / batch_num;
    ThreadPool pool(thread_num);
    for (auto v_i = 0; v_i < n; v_i += batch_size) {
        int my_start = v_i;
        int my_end = min(n, my_start + batch_size);

        // compute min_cn, sd, ed, create cross link in parallel
        pool.enqueue([this](int i_start, int i_end) {
            for (auto i = i_start; i < i_end; i++) {
                for (auto j = out_edge_start[i]; j < out_edge_start[i + 1]; j++) {
                    auto v = out_edges[j];
                    //edge (i,v), correctness of sd/ed updates guaranteed by i<=v
                    if (i <= v) {
                        int a = degree[i], b = degree[v];
                        if (a > b) { swap(a, b); }
                        if (((long long) a) * eps_b2 < ((long long) b) * eps_a2) {
                            // can be pruned
                            min_cn[j] = NOT_DIRECT_REACHABLE;
                        } else {
                            // can be pruned, when c <= 2
                            int c = ComputeCnLowerBound(a, b);
                            min_cn[j] = c <= 2 ? DIRECT_REACHABLE : c;
                        }

                        // find edge, in order to build cross link
                        auto r_id = BinarySearch(out_edges, out_edge_start[v], out_edge_start[v + 1], i);
                        InitCrossLink(j, r_id);
                        UpdateViaCrossLink(j);
                    }
                }
            }
        }, my_start, my_end);
    }
#endif
}

int Graph::IntersectNeighborSets(int u, int v, int min_cn_num) {
    int cn = 2; // count for self and v, count for self and u
    int du = degree[u] + 1, dv = degree[v] + 1; // count for self and v, count for self and u
#ifdef STATISTICS
    intersection_times++;
    auto tmp0 = 0;
    auto tmp1 = 0;
#endif
    for (ui offset_nei_u = out_edge_start[u], offset_nei_v = out_edge_start[v];
         offset_nei_u < out_edge_start[u + 1] && offset_nei_v < out_edge_start[v + 1] &&
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
    ++distribution[local_portion];
    portion = max(portion, local_portion);
#endif
    return cn >= min_cn_num ? DIRECT_REACHABLE : NOT_DIRECT_REACHABLE;
}

int Graph::EvalReachable(int u, ui edge_idx) {
    int v = out_edges[edge_idx];
    return IntersectNeighborSets(u, v, min_cn[edge_idx]);
}

bool Graph::IsDefiniteCoreVertex(int u) {
    return similar_degree[u] >= min_u;
}

void Graph::CheckCore(int u) {
    for (auto j = out_edge_start[u]; j < out_edge_start[u + 1]; j++) {
        // 1st: compute density, build cross link
        if (min_cn[j] > 0) {
            min_cn[j] = EvalReachable(u, j);
            UpdateViaCrossLink(j);
        }

        // 2nd: update sd and ed for u
        if (min_cn[j] == DIRECT_REACHABLE) {
            ++similar_degree[u];
        } else {
            --effective_degree[u];
        }
    }
}

void Graph::ClusterCore(int u) {
    for (auto j = out_edge_start[u]; j < out_edge_start[u + 1]; j++) {
        if (min_cn[j] == DIRECT_REACHABLE && IsDefiniteCoreVertex(out_edges[j])) {
            disjoint_set_ptr->Union(u, out_edges[j]);
        }
    }
}

void Graph::MarkClusterMinEleAsId() {
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

void Graph::ClusterNonCores() {
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

void Graph::pSCAN() {
    // 1st: pre-processing
    auto prune_start = high_resolution_clock::now();
    PruneAndCrossLink();
    auto prune_end = high_resolution_clock::now();
    cout << "1st: prune and cross link execution time:" << duration_cast<milliseconds>(prune_end - prune_start).count()
         << " ms\n";

    // 2nd: find all cores
    auto find_core_start = high_resolution_clock::now();

#ifdef STATISTICS
    vector<int> candidates;
    for (auto i = 0; i < n; i++) {
        if (effective_degree[i] >= min_u) {
            CheckCore(i);
            if (IsDefiniteCoreVertex(i)) { candidates.emplace_back(i); }
        }
    }
#else
    auto thread_num = std::thread::hardware_concurrency();
    auto batch_num = 16u * thread_num;
    auto batch_size = n / batch_num;
    vector<std::future<vector<int>>> future_vec;
    {
        ThreadPool pool(thread_num);

        for (auto v_i = 0; v_i < n; v_i += batch_size) {
            int my_start = v_i;
            int my_end = min(n, my_start + batch_size);
            future_vec.emplace_back(pool.enqueue([this](int i_start, int i_end) -> vector<int> {
                auto candidates = vector<int>();
                for (auto i = i_start; i < i_end; i++) {
                    if (effective_degree[i] >= min_u) {
                        CheckCore(i);
                        if (IsDefiniteCoreVertex(i)) { candidates.emplace_back(i); }
                    }
                }
                return candidates;
            }, my_start, my_end));
        }
    }
#endif
    auto find_core_end = high_resolution_clock::now();
    cout << "2nd: core check time:" << duration_cast<milliseconds>(find_core_end - find_core_start).count() << " ms\n";

// 3rd: cluster cores
#ifdef STATISTICS
    for (auto candidate:candidates) { ClusterCore(candidate); }
#else
    for (auto &future:future_vec) {
        auto candidates = future.get();
        for (auto candidate:candidates) { ClusterCore(candidate); }
    }
#endif

    auto end_core_cluster = high_resolution_clock::now();
    cout << "3rd: core clustering time:" << duration_cast<milliseconds>(end_core_cluster - find_core_end).count()
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
