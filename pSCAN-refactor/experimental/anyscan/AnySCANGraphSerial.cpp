//
// Created by yche on 10/3/17.
//

#include "AnySCANGraphSerial.h"

#include <cmath>
#include <chrono>
#include <queue>
#include <algorithm>

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
    candidate_super_nodes = vector<vector<int>>(n);

    disjoint_set_ptr = yche::make_unique<DisjointSet>(n);

    alpha_block_size = 32768;
    beta_block_size = 32768;
    checking_lst.reserve(n);
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
#ifdef STATISTICS
    intersection_times++;
#endif
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
    if (deg_a > deg_b) { swap(deg_a, deg_b); }
    if (((long long) deg_a) * eps_b2 < ((long long) deg_b) * eps_a2) {
        return NOT_SIMILAR;
    } else {
        int c = ComputeCnLowerBound(deg_a, deg_b);
        return c <= 2 ? SIMILAR : IntersectNeighborSets(u, v, c);
    }
}

// get complete core-induced clusters, but requiring further merging. further expansion obeys to connectivity
void AnySCANGraph::Summarize() {
    // mark unprocessed noise
    for (auto v_cur = 0; v_cur < n; v_cur++) {
        if (out_edge_start[v_cur + 1] - out_edge_start[v_cur] < min_u) {
            vertex_status[v_cur] = anySCAN::UNPROCESSED_NOISE;
        }
    }

    // summarize and mark status
    for (auto v_beg = 0, v_cur = 0, untouched_num = 0; v_cur < n; v_cur++) {
        if (vertex_status[v_cur] == untouched_num) {
            untouched_num++;
        }
        if (untouched_num >= alpha_block_size || v_cur == n - 1) {
            // 1.1 examining all untouched vertices
            for (auto u = v_beg; u < v_cur + 1; u++) {
                // compute eps-neighborhood for u
                auto sd = 0;
                for (auto j = out_edge_start[u]; j < out_edge_start[u + 1]; j++) {
                    auto &eps_neighborhood_vec = eps_neighborhood[u];
                    if (EvalSimilarity(u, j) == SIMILAR) {
                        eps_neighborhood_vec.emplace_back(out_edges[j]);
                        sd++;
                    }
                }
                // check core status
                if (sd >= min_u) {
                    vertex_status[u] = anySCAN::PROCESSED_CORE; // definite core, eps-neighborhood known
                } else {
                    vertex_status[u] = anySCAN::PROCESSED_NOISE; // definite non-core, eps-neighborhood known
                }
            }

            // 1.2 update eps-neighborhood property, update vertex_status
            for (auto u = v_beg; u < v_cur + 1; u++) {
                for (auto w: eps_neighborhood[u]) {
                    eps_neighbor_num[w]++;
                }
            }
            for (auto u = v_beg; u < v_cur + 1; u++) {
                if (vertex_status[u] == anySCAN::PROCESSED_CORE) {
                    for (auto w: eps_neighborhood[u]) {
                        if (vertex_status[w] == anySCAN::PROCESSED_NOISE) {
                            vertex_status[w] = anySCAN::PROCESSED_BORDER;   // in a cluster
                        } else if (vertex_status[w] != anySCAN::PROCESSED_CORE) {
                            if (eps_neighbor_num[w] >= min_u) {
                                vertex_status[w] = anySCAN::UNPROCESSED_CORE;
                            } else {
                                vertex_status[w] = anySCAN::UNPROCESSED_BORDER;
                            }
                        }
                    }
                }
            }

            // 1.3 merge core-induced clusters related to vertices in the current block
            for (auto u = v_beg; u < v_cur + 1; u++) {
                // u is either anySCAN::PROCESSED_CORE or PROCESSED_NOISE
                if (vertex_status[u] == anySCAN::PROCESSED_CORE) {
                    for (auto w: eps_neighborhood[u]) {
                        if (vertex_status[w] == anySCAN::PROCESSED_CORE ||
                            vertex_status[w] == anySCAN::UNPROCESSED_CORE) {
                            disjoint_set_ptr->Union(u, w);
                        }
                    }
                }
            }

            v_beg = v_cur + 1;
        }
    }
}

void AnySCANGraph::MergeStronglyRelatedCluster() {
    // 1st: init information for finding super nodes of unprocessed-borders
    for (auto u = 0; u < n; u++) {
        if (vertex_status[u] == anySCAN::PROCESSED_CORE) {
            for (auto w: eps_neighborhood[u]) {
                if (w == anySCAN::UNPROCESSED_BORDER) {
                    candidate_super_nodes[w].emplace_back(u);
                }
            }
        }
    }
    for (auto u = 0; u < n; u++) {
        if (vertex_status[u] == anySCAN::UNPROCESSED_BORDER && candidate_super_nodes[u].size() >= 2) {
            checking_lst.emplace_back(u);
        }
    }
    sort(begin(checking_lst), end(checking_lst), [this](int u, int v) -> bool {
        return candidate_super_nodes[u].size() > candidate_super_nodes[v].size();
    });

    // 2nd: check core and conditionally cluster core
    for (auto i = 0; i < checking_lst.size(); i += beta_block_size) {
        for (auto cur = i, end = min(static_cast<int>(checking_lst.size()), i + beta_block_size); cur < end; cur++) {
            auto u = checking_lst[cur];
            // all-same prune
            auto first_v = candidate_super_nodes[u][0];
            auto is_same = true;
            for (auto j = 1; j < candidate_super_nodes[u].size(); j++) {
                if (!disjoint_set_ptr->IsSameSet(first_v, candidate_super_nodes[u][j])) {
                    is_same = false;
                    break;
                }
            }
            if (eps_neighbor_num[u] >= min_u) {            // easy to determine core
                vertex_status[u] = anySCAN::UNPROCESSED_CORE;
            } else {
                // check core
                auto sd = 0;
                for (auto j = out_edge_start[u]; j < out_edge_start[u + 1]; j++) {
                    auto &eps_neighborhood_vec = eps_neighborhood[u];
                    if (EvalSimilarity(u, j) == SIMILAR) {
                        eps_neighborhood_vec.emplace_back(out_edges[j]);
                        sd++;
                    }
                }
                // check core status
                if (sd >= min_u) {
                    vertex_status[u] = anySCAN::PROCESSED_CORE;
                } else {
                    vertex_status[u] = anySCAN::PROCESSED_BORDER;
                }
            }

            if (!is_same && vertex_status[u] == anySCAN::UNPROCESSED_CORE) {
                // cluster cores
                for (auto v: candidate_super_nodes[u]) {
                    disjoint_set_ptr->Union(u, v);
                }
            }
        }
    }
}

void AnySCANGraph::MergeWeaklyRelatedCluster() {
    // 1st: init checking list of possible cores and definite cores
    checking_lst.clear();
    for (auto u = 0; u < n; u++) {
        if (vertex_status[u] == anySCAN::UNPROCESSED_BORDER || vertex_status[u] == anySCAN::UNPROCESSED_CORE ||
            vertex_status[u] == anySCAN::PROCESSED_CORE) {
            checking_lst.emplace_back(u);
        }
    }
    sort(begin(checking_lst), end(checking_lst), [this](int u, int v) -> bool {
        return degree[u] > degree[v];
    });

    // 2nd: check core and conditionally cluster core
    for (auto i = 0; i < checking_lst.size(); i += beta_block_size) {
        for (auto cur = i, end = min(static_cast<int>(checking_lst.size()), i + beta_block_size); cur < end; cur++) {
            auto u = checking_lst[cur];
            // check core if unknown
            if (vertex_status[u] == anySCAN::UNPROCESSED_BORDER) {
                if (eps_neighbor_num[u] >= min_u) {
                    vertex_status[u] = anySCAN::UNPROCESSED_CORE;
                } else {
                    auto sd = 0;
                    for (auto j = out_edge_start[u]; j < out_edge_start[u + 1]; j++) {
                        auto &eps_neighborhood_vec = eps_neighborhood[u];
                        if (EvalSimilarity(u, j) == SIMILAR) {
                            eps_neighborhood_vec.emplace_back(out_edges[j]);
                            sd++;
                        }
                    }
                    // check core status
                    if (sd >= min_u) {
                        vertex_status[u] = anySCAN::PROCESSED_CORE;
                    } else {
                        vertex_status[u] = anySCAN::PROCESSED_BORDER;
                    }
                }
            }
            if (vertex_status[u] == anySCAN::PROCESSED_CORE) {
                for (auto v: eps_neighborhood[u]) {
                    if (vertex_status[v] == anySCAN::PROCESSED_CORE || vertex_status[v] == anySCAN::UNPROCESSED_CORE) {
                        disjoint_set_ptr->Union(u, v);
                    }
                }
            } else if (vertex_status[u] == anySCAN::UNPROCESSED_CORE) {
                for (auto j = out_edge_start[u]; j < out_edge_start[u + 1]; j++) {
                    auto v = out_edges[j];
                    if (vertex_status[v] == anySCAN::PROCESSED_CORE || vertex_status[v] == anySCAN::UNPROCESSED_CORE) {
                        if (!disjoint_set_ptr->IsSameSet(u, v)) {
                            if (EvalSimilarity(u, j) == SIMILAR) {
                                disjoint_set_ptr->Union(u, v);
                            }
                        }
                    }
                }
            }
        }
    }
}

void AnySCANGraph::MarkClusterMinEleAsId() {
    cluster_dict = vector<int>(n);
    std::fill(cluster_dict.begin(), cluster_dict.end(), n);

    for (auto i = 0u; i < n; i++) {
        if (vertex_status[i] == anySCAN::PROCESSED_CORE || vertex_status[i] == anySCAN::UNPROCESSED_CORE) {
            int x = disjoint_set_ptr->FindRoot(i);
            if (i < cluster_dict[x]) { cluster_dict[x] = i; }
        }
    }
}

void AnySCANGraph::ClusterNonCore() {
    for (auto u = 0; u < n; u++) {
        if (vertex_status[u] == anySCAN::PROCESSED_CORE) {
            auto cluster_id = cluster_dict[disjoint_set_ptr->FindRoot(u)];
            for (auto v: eps_neighborhood[u]) {
                if (vertex_status[v] != anySCAN::PROCESSED_CORE && vertex_status[u] != anySCAN::UNPROCESSED_CORE) {
                    noncore_cluster.emplace_back(cluster_id, v);
                }
            }
        } else if (vertex_status[u] == anySCAN::UNPROCESSED_CORE) {
            auto cluster_id = cluster_dict[disjoint_set_ptr->FindRoot(u)];
            for (auto j = out_edge_start[u]; j < out_edge_start[u + 1]; j++) {
                auto v = out_edges[j];
                if (vertex_status[v] != anySCAN::PROCESSED_CORE && vertex_status[u] != anySCAN::UNPROCESSED_CORE) {
                    if (EvalSimilarity(u, j) == SIMILAR) {
                        noncore_cluster.emplace_back(cluster_id, v);
                    }
                }
            }
        }
    }
}

void AnySCANGraph::PostProcessing() {
    // 1st: get min core element in a cluster
    MarkClusterMinEleAsId();

    // 2nd: cluster non-core
    ClusterNonCore();
}

void AnySCANGraph::anySCAN() {
    Summarize();
    MergeStronglyRelatedCluster();
    MergeWeaklyRelatedCluster();
    PostProcessing();
#ifdef STATISTICS
    cout << "intersection times:" << intersection_times << "\n\n";
#endif
}

void AnySCANGraph::Output(const char *eps_s, const char *miu) {
    io_helper_ptr->OutputAnySCAN(eps_s, miu, noncore_cluster, vertex_status, cluster_dict, disjoint_set_ptr->parent);
}