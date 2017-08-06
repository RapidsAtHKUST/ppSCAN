#include "Graph.h"

#include <cstring>
#include <cmath>
#include <chrono>
#include <iostream>
#include <algorithm>

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

// 1st phase: core clustering
ui Graph::BinarySearch(vector<int> &array, ui offset_beg, ui offset_end, int val) {
    --offset_end;
    if (array[offset_end] < val) { return offset_end + 1; }
    while (offset_beg < offset_end) {
        ui mid = offset_beg + (offset_end - offset_beg) / 2;
        if (array[mid] >= val) {
            offset_end = mid;
        } else {
            offset_beg = mid + 1;
        }
    }
    return offset_end;
}

int Graph::ComputeCnLowerBound(int du, int dv) {
    auto c = (int) (sqrtl((((long double) du) * ((long double) dv) * eps_a2) / eps_b2));

    if (((long long) c) * ((long long) c) * eps_b2 < ((long long) du) * ((long long) dv) * eps_a2) { ++c; }
    return c;
}

void Graph::PruneAndCrossLink() {
    auto start = high_resolution_clock::now();

    //must iterate from 0 to n-1
    for (ui i = 0; i < n; i++) {
        for (ui j = out_edge_start[i]; j < out_edge_start[i + 1]; j++) {
            if (out_edges[j] < i) {
                // edge should already be explored
                if (min_cn[j] == NOT_SURE) { min_cn[j] = NOT_SIMILAR; }
            } else {
                int v = out_edges[j];
                // use pruning rule
                int a = degree[i], b = degree[v];
                if (a > b) { swap(a, b); }
                if (((long long) a) * eps_b2 < ((long long) b) * eps_a2) {
                    // can be pruned
                    min_cn[j] = NOT_SIMILAR;
                    --effective_degree[i];
                    --effective_degree[v];
                } else {
                    // cannot be pruned
                    int c = ComputeCnLowerBound(a, b);
                    if (c <= 2) {
                        min_cn[j] = SIMILAR;
                        ++similar_degree[i];
                        if (similar_degree[i] == min_u) { cores.emplace_back(i); }
                        ++similar_degree[v];
                        if (similar_degree[v] == min_u) { cores.emplace_back(v); }
                    } else {
                        min_cn[j] = c;
                    }
                }

                // find edge, in order to build cross link
                if (min_cn[j] != NOT_SIMILAR) {
                    ui r_id = BinarySearch(out_edges, out_edge_start[v], out_edge_start[v + 1], i);
                    reverse_edge_idx[j] = r_id;
                    reverse_edge_idx[r_id] = j;

                    // build cross link
                    min_cn[r_id] = min_cn[j];
                }
            }
        }
    }

    auto end1 = high_resolution_clock::now();
    cout << "prune and cross link execution time:" << duration_cast<milliseconds>(end1 - start).count() << " ms\n";
}

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

            // 1st: compute density
            min_cn[idx] = min_cn[reverse_edge_idx[idx]] = SimilarityCheck(u, idx);

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
                    if (similar_degree[v] == min_u) { cores.emplace_back(v); }
                } else {
                    --effective_degree[v];
                }
            }
        }
    }

    // mark u as already explored
    effective_degree[u] = ALREADY_EXPLORED;
    return i;
}

bool Graph::IsCoreVertex(int u) {
    return similar_degree[u] >= min_u;
}

void Graph::ClusterCore(int u, int index_i) {
    for (auto idx : dst_vertices) {
        // u and v similar, and v is also a core vertex
        if (min_cn[idx] == SIMILAR && similar_degree[out_edges[idx]] >= min_u) {
            disjoint_set_ptr->Union(u, out_edges[idx]);
        }
    }

    for (int i = index_i; i < dst_vertices.size(); ++i) {
        ui idx = dst_vertices[i];
        int v = out_edges[idx];
        if (min_cn[idx] >= 0 && IsCoreVertex(v) && !disjoint_set_ptr->IsSameSet(u, v)) {
            min_cn[idx] = min_cn[reverse_edge_idx[idx]] = SimilarityCheck(u, idx);

            if (effective_degree[v] != ALREADY_EXPLORED) {
                if (min_cn[idx] == SIMILAR) {
                    ++similar_degree[v];
                    if (similar_degree[v] == min_u) { cores.emplace_back(v); }
                } else {
                    --effective_degree[v];
                }
            }

            if (min_cn[idx] == SIMILAR) { disjoint_set_ptr->Union(u, v); }
        }
    }
}

// 2nd phase:  non-core vertices clustering
int Graph::CheckCnMerge(int u, int v, int min_c) {
    int cn = 2;

    if (degree[u] > degree[v]) { swap(u, v); }

    // merge-operation for two sorted edge-list
    int du = degree[u] + 1, dv = degree[v] + 1;
    ui i = out_edge_start[u], j = out_edge_start[v];
    while (i < out_edge_start[u + 1] && j < out_edge_start[v + 1] && cn < min_c && du >= min_c && dv >= min_c) {
        if (out_edges[i] < out_edges[j]) {
            --du;
            ++i;
        } else if (out_edges[i] > out_edges[j]) {
            --dv;
            ++j;
        } else {
            ++cn;
            ++i;
            ++j;
        }
    }

    if (cn >= min_c) { return SIMILAR; }
    return NOT_SIMILAR;
}

int Graph::SimilarityCheck(int u, ui idx) {
    int v = out_edges[idx];

    if (min_cn[idx] == NOT_SURE) {
        int du = degree[u], dv = degree[v];
        int c = ComputeCnLowerBound(du, dv);

        if (c <= 2) { return SIMILAR; }

        min_cn[idx] = min_cn[reverse_edge_idx[idx]] = c;
    }

    return CheckCnMerge(u, v, min_cn[idx]);
}

void Graph::ClusterNonCores() {
    noncore_cluster = vector<pair<int, int>>();
    noncore_cluster.reserve(n);

    cluster_dict = vector<int>(n);
    std::fill(cluster_dict.begin(), cluster_dict.end(), n);

    for (auto i = 0; i < n; i++) {
        if (IsCoreVertex(i)) {
            int x = disjoint_set_ptr->FindRoot(i);
            if (i < cluster_dict[x]) { cluster_dict[x] = i; }
        }
    }

    for (auto i = 0; i < n; i++) {
        if (similar_degree[i] >= min_u) {
            for (auto j = out_edge_start[i]; j < out_edge_start[i + 1]; j++) {
                // observation 3: check non-core neighbors
                if (!IsCoreVertex(out_edges[j])) {
                    if (min_cn[j] >= 0) {
                        min_cn[j] = SimilarityCheck(i, j);
                        min_cn[reverse_edge_idx[j]] = min_cn[j];
                        if (min_cn[j] == SIMILAR) {
                            ++similar_degree[i];
                            ++similar_degree[out_edges[j]];
                        }
                    }

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
    PruneAndCrossLink();

    // 2nd: explore vertex in effective-degree non-increasing order
    auto start = high_resolution_clock::now();

    dst_vertices.reserve(n);
    // bin_head, bin_next adopted to mimic the max-priority-queue(heap)
    auto bin_next = vector<int>(n);
    auto bin_head = vector<int>(n);
    std::fill(bin_head.begin(), bin_head.end(), INVALID_VERTEX_IDX);

    int max_ed = 0;
    for (auto i = 0; i < n; i++) {
        if (effective_degree[i] >= min_u) {
            int ed = effective_degree[i];
            if (ed > max_ed) { max_ed = ed; }
            bin_next[i] = bin_head[ed];
            bin_head[ed] = i;
        }
    }

    while (true) {
        int u = INVALID_VERTEX_IDX;
        // deal with already known cores and then deal with unexplored vertices
        if (!cores.empty()) {
            u = cores.back();
            cores.pop_back();
        } else {
            // find-max, if there delete-max
            while (max_ed >= min_u && u == INVALID_VERTEX_IDX) {
                for (int x = bin_head[max_ed]; x != -1;) {
                    int tmp = bin_next[x];
                    int ed = effective_degree[x];
                    // dynamically maintain the max-priority-queue(heap)
                    if (ed == max_ed) {
                        u = x;
                        bin_head[max_ed] = bin_next[x];
                        break;
                    }
                    // lazy update
                    if (ed >= min_u) {
                        bin_next[x] = bin_head[ed];
                        bin_head[ed] = x;
                    }
                    x = tmp;
                }
                // reach the tail of linked list, within underlying array
                if (u == INVALID_VERTEX_IDX) {
                    bin_head[max_ed] = -1;
                    --max_ed;
                }
            }
        }

        if (u == INVALID_VERTEX_IDX) { break; }

        int index_i = CheckCore(u);

        if (IsCoreVertex(u)) { ClusterCore(u, index_i); }
    }

    auto tmp = std::move(cores);
    auto end = high_resolution_clock::now();
    cout << "core clustering time:" << duration_cast<milliseconds>(end - start).count() << " ms\n";

    // 3rd: non-core clustering
    ClusterNonCores();
}


