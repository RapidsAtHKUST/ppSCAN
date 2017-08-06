#include "Graph.h"

#include <cstring>
#include <cmath>
#include <chrono>
#include <iostream>

using namespace std::chrono;

pair<int, int> Graph::ParseEps(const char *eps_s) {
    int i = 0, eps_numerator = 0, eps_denominator = 1;
    while (eps_s[i] != '\0' && eps_s[i] != '.') {
        eps_numerator = eps_numerator * 10 + (eps_s[i] - '0');
        ++i;
    }
    if (eps_s[i] == '.') {
        ++i;
        while (eps_s[i] != '\0') {
            eps_numerator = eps_numerator * 10 + (eps_s[i] - '0');
            eps_denominator *= 10;
            ++i;
        }
    }
    if (eps_numerator > eps_denominator || eps_denominator > 100 || eps_numerator <= 0) {
        cout << "eps err";
        exit(1);
    }
    return make_pair(eps_numerator * eps_numerator, eps_denominator * eps_denominator);
}

Graph::Graph(const char *dir_string, const char *eps_s, int miu) {
    // 1st: parameter
    std::tie(eps_a2, eps_b2) = ParseEps(eps_s);
    this->min_u = miu;

    // 2nd: graph
    io_helper_ptr = make_unique<InputOutput>(dir_string);
    io_helper_ptr->ReadGraph();

    // csr representation
    n = static_cast<ui>(io_helper_ptr->n);
    out_edge_start = std::move(io_helper_ptr->offset_out_edges);
    out_edges = std::move(io_helper_ptr->out_edges);

    // edge properties
    reverse_edge_idx = std::move(io_helper_ptr->reverse_edge_idx);
    min_cn = vector<int>(static_cast<ui>(io_helper_ptr->m));
    std::fill(min_cn.begin(), min_cn.end(), NOT_SURE);

    // vertex properties
    degree = std::move(io_helper_ptr->degree);
    similar_degree = vector<int>(n);
    std::fill(similar_degree.begin(), similar_degree.end(), 0);
    effective_degree = vector<int>(n);
    for (ui i = 0; i < n; i++) { effective_degree[i] = degree[i] - 1; }

    // 3rd: disjoint-set, make-set at the beginning
    MakeDisjointSetBatch();
}

void Graph::Output(const char *eps_s, const char *miu) {
    io_helper_ptr->Output(eps_s, miu, noncore_cluster, similar_degree, cid, parent);
}

// data structure related: union-find disjoint set
void Graph::MakeDisjointSetBatch() {
    parent = vector<int>(n);
    rank = vector<int>(n);
    for (ui i = 0; i < n; i++) { parent[i] = i; }
    std::fill(rank.begin(), rank.end(), 0);
}

int Graph::FindRoot(int u) {
    int x = u;
    while (parent[x] != x) { x = parent[x]; }

    while (parent[u] != x) {
        int tmp = parent[u];
        parent[u] = x;
        u = tmp;
    }
    return x;
}

void Graph::Union(int u, int v) {
    int ru = FindRoot(u);
    int rv = FindRoot(v);

    if (ru == rv) { return; }

    // link: ru and rv
    if (rank[ru] < rank[rv]) {
        parent[ru] = rv;
    } else if (rank[ru] > rank[rv]) {
        parent[rv] = ru;
    } else {
        parent[rv] = ru;
        ++rank[ru];
    }
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
                if (min_cn[j] == NOT_SURE) { min_cn[j] = NOT_SIMILAR; }
            } else {
                int v = out_edges[j];

                // use pruning rule
                int a = degree[i], b = degree[v];
                if (a > b) { swap(a, b); }
                if (((long long) a) * eps_b2 < ((long long) b) * eps_a2) {
                    min_cn[j] = NOT_SIMILAR;

                    --effective_degree[i];
                    --effective_degree[v];
                } else {
                    // cannot be pruned
                    int c = ComputeCnLowerBound(a, b);

                    if (c <= 2) {
                        min_cn[j] = SIMILAR;

                        ++similar_degree[i];
                        ++similar_degree[v];

                        if (similar_degree[i] == min_u) {
                            cores.emplace_back(i);
                        }
                        if (similar_degree[v] == min_u) {
                            cores.emplace_back(v);
                        }
                    } else {
                        min_cn[j] = c;
                    }
                }

                // find edge, in order to build cross link
                if (min_cn[j] != NOT_SIMILAR) {
                    ui r_id = BinarySearch(out_edges, out_edge_start[v], out_edge_start[v + 1], i);
                    reverse_edge_idx[j] = r_id;
                    reverse_edge_idx[r_id] = j;

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
    for (ui j = out_edge_start[u]; j < out_edge_start[u + 1]; j++) {
        if (min_cn[j] != NOT_SIMILAR && (similar_degree[u] < min_u || FindRoot(u) != FindRoot(out_edges[j]))) {
            dst_vertices.emplace_back(j);
        }
    }

    int i = 0;
    for (; similar_degree[u] < min_u && effective_degree[u] >= min_u && i < dst_vertices.size(); ++i) {
        ui idx = dst_vertices[i];
        if (min_cn[idx] != SIMILAR) {
            int v = out_edges[idx];

            // 1st: compute density
            min_cn[idx] = min_cn[reverse_edge_idx[idx]] = SimilarCheckOpt(u, idx);

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

void Graph::ClusterCore(int u, int index_i) {
    for (auto idx : dst_vertices) {
        // u and v similar, and v is also a core vertex
        if (min_cn[idx] == SIMILAR && similar_degree[out_edges[idx]] >= min_u) {
            Union(u, out_edges[idx]);
        }
    }

    for (int i = index_i; i < dst_vertices.size(); ++i) {
        ui idx = dst_vertices[i];
        int v = out_edges[idx];
        if (!(min_cn[idx] < 0 || similar_degree[v] < min_u || FindRoot(u) == FindRoot(v))) {
            min_cn[idx] = min_cn[reverse_edge_idx[idx]] = SimilarCheckOpt(u, idx);

            if (effective_degree[v] != ALREADY_EXPLORED) {
                if (min_cn[idx] == SIMILAR) {
                    ++similar_degree[v];
                    if (similar_degree[v] == min_u) { cores.emplace_back(v); }
                } else {
                    --effective_degree[v];
                }
            }

            if (min_cn[idx] == SIMILAR) {
                Union(u, v);
            }
        }
    }
}

// 2nd phase:  non-core vertices clustering
int Graph::CheckCn(int u, int v, int c) {
    int cn = 2;

    if (degree[u] > degree[v]) { swap(u, v); }

    // merge-operation for two sorted edge-list
    int du = degree[u] + 1, dv = degree[v] + 1;
    ui i = out_edge_start[u], j = out_edge_start[v];
    while (i < out_edge_start[u + 1] && j < out_edge_start[v + 1] && cn < c && du >= c && dv >= c) {
        if (out_edges[i] < out_edges[j]) {
            --du;
            ++i;
        } else if (out_edges[i] > out_edges[j]) {
            --dv;
            ++j;
        } else {
            ++cn;
            if (cn >= c) { return SIMILAR; }
            ++i;
            ++j;
        }
    }
    return NOT_SIMILAR;
}

int Graph::SimilarCheckOpt(int u, ui idx) {
    int v = out_edges[idx];

    if (min_cn[idx] == NOT_SURE) {
        int du = degree[u], dv = degree[v];
        int c = ComputeCnLowerBound(du, dv);

        if (c <= 2) { return SIMILAR; }

        min_cn[idx] = min_cn[reverse_edge_idx[idx]] = c;
    }

    return CheckCn(u, v, min_cn[idx]);
}

void Graph::ClusterNonCores() {
    // cluster related
    noncore_cluster = vector<pair<int, int>>();
    noncore_cluster.reserve(n);
    cid = vector<int>(n);
    std::fill(cid.begin(), cid.end(), n);

    for (ui i = 0; i < n; i++) {
        if (similar_degree[i] >= min_u) {
            int x = FindRoot(i);
            if (i < cid[x]) { cid[x] = i; }
        }
    }

    for (ui i = 0; i < n; i++) {
        if (similar_degree[i] >= min_u) {
            for (ui j = out_edge_start[i]; j < out_edge_start[i + 1]; j++) {
                if (similar_degree[out_edges[j]] < min_u) {
                    if (min_cn[j] >= 0) {
                        min_cn[j] = SimilarCheckOpt(i, j);
                        if (reverse_edge_idx[reverse_edge_idx[j]] != j) { cout << "WA cluster_noncore\n"; }
                        min_cn[reverse_edge_idx[j]] = min_cn[j];
                        if (min_cn[j] == SIMILAR) {
                            ++similar_degree[i];
                            ++similar_degree[out_edges[j]];
                        }
                    }

                    if (min_cn[j] == SIMILAR) { noncore_cluster.emplace_back(cid[parent[i]], out_edges[j]); }
                }
            }
        }
    }
}

void Graph::pSCAN() {
    cores.reserve(n);
    // 1st
    PruneAndCrossLink();

    auto start = high_resolution_clock::now();

    dst_vertices.reserve(n);
    auto bin_next = vector<int>(n);
    auto bin_head = vector<int>(n);
    std::fill(bin_head.begin(), bin_head.end(), INVALID_VERTEX_IDX);

    int max_ed = 0;
    for (ui i = 0; i < n; i++) {
        if (effective_degree[i] >= min_u) {
            int ed = effective_degree[i];
            if (ed > max_ed) { max_ed = ed; }
            bin_next[i] = bin_head[ed];
            bin_head[ed] = i;
        }
    }

    // 2nd: explore vertex in effective-degree non-increasing order
    while (true) {
        int u = INVALID_VERTEX_IDX;
        if (!cores.empty()) {
            u = cores.back();
            cores.pop_back();
        } else {
            while (max_ed >= min_u && u == INVALID_VERTEX_IDX) {
                for (int x = bin_head[max_ed]; x != -1;) {
                    int tmp = bin_next[x];
                    int ed = effective_degree[x];
                    if (ed == max_ed) {
                        u = x;
                        bin_head[max_ed] = bin_next[x];
                        break;
                    }
                    if (ed >= min_u) {
                        bin_next[x] = bin_head[ed];
                        bin_head[ed] = x;
                    }
                    x = tmp;
                }
                if (u == INVALID_VERTEX_IDX) {
                    bin_head[max_ed] = -1;
                    --max_ed;
                }
            }
        }

        if (u == INVALID_VERTEX_IDX) { break; }

        int index_i = CheckCore(u);

        if (similar_degree[u] >= min_u) { ClusterCore(u, index_i); }
    }

    auto tmp = std::move(cores);
    auto end = high_resolution_clock::now();
    cout << "core clustering time:" << duration_cast<milliseconds>(end - start).count() << " ms\n";

    // 3rd: non-core clustering
    ClusterNonCores();
}


