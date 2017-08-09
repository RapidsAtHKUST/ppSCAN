#include "Graph.h"

#include <cstring>
#include <cmath>
#include <chrono>
#include <iostream>
#include <algorithm>

#include "ThreadPool.h"

#include "MaxPriorityQueue.h"

using namespace std::chrono;

Graph::Graph(const char *dir_string, const char *eps_s, int min_u) {
    io_helper_ptr = make_unique<InputOutput>(dir_string);
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
    similar_degree = vector<int>(n);
    std::fill(similar_degree.begin(), similar_degree.end(), 0);
    effective_degree.reserve(n);
    std::transform(degree.begin(), degree.end(), back_inserter(effective_degree),
                   [](auto degree_val) { return degree_val - 1; });

    // 3rd: disjoint-set, make-set at the beginning
    disjoint_set_ptr = make_unique<DisjointSet>(n);
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
    auto thread_num = 8u;
    auto batch_num = 16u * thread_num;
    auto batch_size = n / batch_num;
    //must iterate from 0 to n-1
    ThreadPool pool(thread_num);
    for (auto v_i = 0; v_i < n; v_i += batch_size) {
         int my_start = v_i;
        int my_end = min(n , my_start + batch_size);
        pool.enqueue([this](int i_start, int i_end) {
            for (auto i = i_start; i < i_end; i++)
                for (auto j = out_edge_start[i]; j < out_edge_start[i + 1]; j++) {
                    auto v = out_edges[j];
                    //edge (i,v)
                    if (i <= v) {
                        // use pruning rule
                        int a = degree[i], b = degree[v];
                        if (a > b) { swap(a, b); }
                        if (((long long) a) * eps_b2 < ((long long) b) * eps_a2) {
                            // can be pruned
                            min_cn[j] = NOT_SIMILAR;
                            --effective_degree[i];
                            --effective_degree[v];
                        } else {
                            int c = ComputeCnLowerBound(a, b);
                            // can be pruned
                            if (c <= 2) {
                                min_cn[j] = SIMILAR;
                                ++similar_degree[i];
                                if (IsDefiniteCoreVertex(i)) { cores.emplace_back(i); }
                                ++similar_degree[v];
                                if (IsDefiniteCoreVertex(v)) { cores.emplace_back(v); }
                            } else {
                                // need to refine later
                                min_cn[j] = c;
                            }
                        }

                        // find edge, in order to build cross link
                        auto r_id = BinarySearch(out_edges, out_edge_start[v], out_edge_start[v + 1], i);
                        InitCrossLink(j, r_id);
                        UpdateViaCrossLink(j);
                    }
                }
        }, my_start, my_end);
    }


}

int Graph::IntersectNeighborSets(int u, int v, int min_cn_num) {
    int cn = 2; // count for self and v, count for self and u
    if (degree[u] > degree[v]) { swap(u, v); }

    // merge-operation for two sorted edge-list
    int du = degree[u] + 1, dv = degree[v] + 1; // count for self and v, count for self and u
    ui offset_nei_u = out_edge_start[u], offset_nei_v = out_edge_start[v];
    while (offset_nei_u < out_edge_start[u + 1] && offset_nei_v < out_edge_start[v + 1] &&
           cn < min_cn_num && du >= min_cn_num && dv >= min_cn_num) {
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

int Graph::EvalDensity(int u, ui edge_idx) {
    // check density for edge (u,v)
    int v = out_edges[edge_idx];
    return IntersectNeighborSets(u, v, min_cn[edge_idx]);
}

bool Graph::IsSimilarityUnKnow(ui edge_idx) {
    return min_cn[edge_idx] > 0;
}

// 1st phase: core clustering
int Graph::CheckCore(int u) {
    dst_vertices.clear();
    for (auto j = out_edge_start[u]; j < out_edge_start[u + 1]; j++) {
        if (min_cn[j] != NOT_SIMILAR) {
            if (similar_degree[u] < min_u || !disjoint_set_ptr->IsSameSet(u, out_edges[j])) {
                dst_vertices.emplace_back(j);
            }
        }
    }

    int i = 0;
    for (; similar_degree[u] < min_u && effective_degree[u] >= min_u && i < dst_vertices.size(); ++i) {
        auto idx = dst_vertices[i];
        if (min_cn[idx] != SIMILAR) {
            int v = out_edges[idx];
            // 1st: compute density, build cross link
            min_cn[idx] = EvalDensity(u, idx);
            UpdateViaCrossLink(idx);

            // 2nd: update sd and ed for u
            if (min_cn[idx] == SIMILAR) {
                ++similar_degree[u];
            } else {
                --effective_degree[u];
            }

            // 3rd: update sd and ed for v
            if (effective_degree[v] != ALREADY_EXPLORED) {
                if (min_cn[idx] == SIMILAR) {
                    ++similar_degree[v];
                    if (IsDefiniteCoreVertex(v)) { cores.emplace_back(v); }
                } else {
                    --effective_degree[v];
                }
            }
        }
    }

    // mark u as already explored, to avoid duplicated in cores
    effective_degree[u] = ALREADY_EXPLORED;
    return i;
}

bool Graph::IsDefiniteCoreVertex(int u) {
    return similar_degree[u] >= min_u;
}

// core vertices connected component
void Graph::ClusterCore(int u, int index_i) {
    // for these already checked, either from check or cross link
    for (auto idx : dst_vertices) {
        // u and v similar, and v is also a core vertex
        if (min_cn[idx] == SIMILAR && IsDefiniteCoreVertex(out_edges[idx])) {
            disjoint_set_ptr->Union(u, out_edges[idx]);
        }
    }

    // for these may have not checked
    for (int i = index_i; i < dst_vertices.size(); ++i) {
        ui edge_idx = dst_vertices[i];
        int v = out_edges[edge_idx];

        if (IsSimilarityUnKnow(edge_idx) && IsDefiniteCoreVertex(v) && !disjoint_set_ptr->IsSameSet(u, v)) {
            min_cn[edge_idx] = EvalDensity(u, edge_idx);
            UpdateViaCrossLink(edge_idx);
            if (min_cn[edge_idx] == SIMILAR) { disjoint_set_ptr->Union(u, v); }
        }
    }
}

// 2nd phase:  non-core vertices clustering
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
                if (!IsDefiniteCoreVertex(out_edges[j])) {
                    if (IsSimilarityUnKnow(j)) { min_cn[j] = EvalDensity(i, j); }
                    if (min_cn[j] == SIMILAR) {
                        // root must be parent since already disjoint_set_ptr->FindRoot(i) previously
                        noncore_cluster.emplace_back(cluster_dict[disjoint_set_ptr->parent[i]], out_edges[j]);
                    }
                }
            }
        }
    }
}

void Graph::pSCAN() {
    // 1st
    cores.reserve(n);
    auto prune_start = high_resolution_clock::now();
    PruneAndCrossLink();
    auto prune_end = high_resolution_clock::now();
    cout << "prune and cross link execution time:" << duration_cast<milliseconds>(prune_end - prune_start).count() << " ms\n";

    // 2nd: explore vertex in effective-degree non-increasing order
    auto start = high_resolution_clock::now();

    dst_vertices.reserve(n);
    MaxPriorityQueue max_priority_queue(n, &effective_degree, min_u);

    while (true) {
        int u;
        // deal with already known cores and then deal with unexplored vertices
        if (!cores.empty()) {
            u = cores.back();
            cores.pop_back();
        } else {
            // find-max, if there delete-max
            u = max_priority_queue.pop();
        }

        if (u == INVALID_VERTEX_IDX) { break; }

        int index_i = CheckCore(u);

        if (IsDefiniteCoreVertex(u)) { ClusterCore(u, index_i); }
    }

    auto tmp = std::move(cores);
    auto end = high_resolution_clock::now();
    cout << "core clustering time:" << duration_cast<milliseconds>(end - start).count() << " ms\n";

    // 3rd: non-core clustering
    ClusterNonCores();
}