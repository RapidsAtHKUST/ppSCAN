#include <cmath>

#include <chrono>
#include <algorithm>

#include "pSCANGraph.h"
#include "MaxPriorityQueue.h"

pSCANGraph::pSCANGraph(const char *_dir, const char *eps_s, int min_u) {
    using namespace std::chrono;
    io_helper_ptr = yche::make_unique<InputOutput>(_dir);
    io_helper_ptr->ReadGraph();

    auto tmp_start = high_resolution_clock::now();

    // 1st: parameter
    std::tie(eps_a2, eps_b2) = io_helper_ptr->ParseEps(eps_s);
    this->miu = min_u;

    // 2nd: graph
    // csr representation
    n = static_cast<ui>(io_helper_ptr->n);
    pstart = std::move(io_helper_ptr->offset_out_edges);
    edges = std::move(io_helper_ptr->out_edges);

    // edge properties
    min_cn = vector<int>(io_helper_ptr->m);
    reverse_link = vector<ui>(io_helper_ptr->m);

    // vertex properties
    degree = std::move(io_helper_ptr->degree);
    similar_degree = vector<int>(n);
    std::fill(similar_degree.begin(), similar_degree.end(), 0);
    effective_degree.reserve(n);
    std::transform(degree.begin(), degree.end(), back_inserter(effective_degree),
                   [](int degree_val) { return degree_val - 1; });

    // 3rd: disjoint-set, make-set at the beginning
    disjoint_set = yche::make_unique<DisjointSet>(n);
    cores.reserve(n);

    auto all_end = high_resolution_clock::now();
    cout << "other construct time:" << duration_cast<milliseconds>(all_end - tmp_start).count()
         << " ms\n";
}

void pSCANGraph::Output(const char *eps_s, const char *miu) {
    io_helper_ptr->Output(eps_s, miu, noncore_cluster, similar_degree, cid, disjoint_set->parent);
}

int pSCANGraph::ComputeMinCommonNeighbor(int du, int dv) {
    auto c = (int) (sqrtl((((long double) du) * ((long double) dv) * eps_a2) / eps_b2));
    if (((long long) c) * ((long long) c) * eps_b2 < ((long long) du) * ((long long) dv) * eps_a2) ++c;
    return c;
}

int pSCANGraph::EvalSimilarity(int u, int v, int c) {
#ifdef IMPROVED_SET_INTERSECT
    int cn = 2; // count for self and v, count for self and u
    int du = pstart[u + 1] - pstart[u] + 2, dv =
            pstart[v + 1] - pstart[v] + 2; // count for self and v, count for self and u

    auto offset_nei_u = pstart[u], offset_nei_v = pstart[v];

    // correctness guaranteed by two pruning previously in computing min_cn
    while (cn < c) {
        while (edges[offset_nei_u] < edges[offset_nei_v]) {
            --du;
            if (du < c) { return -2; }
            ++offset_nei_u;
        }
        while (edges[offset_nei_u] > edges[offset_nei_v]) {
            --dv;
            if (dv < c) { return -2; }
            ++offset_nei_v;
        }
        if (edges[offset_nei_u] == edges[offset_nei_v]) {
            ++cn;
            ++offset_nei_u;
            ++offset_nei_v;
        }
    }
    return cn >= c ? -1 : -2;
#else
    int cn = 2;
    if (degree[u] > degree[v]) swap(u, v);
    int du = degree[u] + 1, dv = degree[v] + 1;

    ui i = pstart[u], j = pstart[v];
    while (i < pstart[u + 1] && j < pstart[v + 1] && cn < c && du >= c && dv >= c) {
        if (edges[i] < edges[j]) {
            --du;
            ++i;
        } else if (edges[i] > edges[j]) {
            --dv;
            ++j;
        } else {
            ++cn;
            ++i;
            ++j;
        }
    }
    return cn >= c ? -1 : -2;
#endif
}

int pSCANGraph::EvalSimilarityWrapper(int u, ui idx) {
    int v = edges[idx];

    if (min_cn[idx] == 0) {
        int du = degree[u], dv = degree[v];
        int c = ComputeMinCommonNeighbor(du, dv);

        if (c <= 2) return -1;

        min_cn[idx] = min_cn[reverse_link[idx]] = c;
    }

    return EvalSimilarity(u, v, min_cn[idx]);
}

ui pSCANGraph::BinarySearch(vector<int> &array, ui b, ui e, int val) {
    --e;
    if (array[e] < val) return e + 1;
    while (b < e) {
        ui mid = b + (e - b) / 2;
        if (array[mid] >= val) e = mid;
        else b = mid + 1;
    }
    return e;
}

void pSCANGraph::PruneCrossLink() {
    for (ui i = 0; i < n; i++) { //must be iterating from 0 to n-1
        for (ui j = pstart[i]; j < pstart[i + 1]; j++) {
            if (edges[j] < i) {
                if (min_cn[j] == 0) min_cn[j] = -2;
                continue; //this edge has already been checked
            }

            int v = edges[j];
            int a = degree[i], b = degree[v];
            if (a > b) swap(a, b);

            if (((long long) a) * eps_b2 < ((long long) b) * eps_a2) {
                min_cn[j] = -2;

                --effective_degree[i];
                --effective_degree[v];
            } else {
                int c = ComputeMinCommonNeighbor(a, b);

                if (c <= 2) {
                    min_cn[j] = -1;

                    ++similar_degree[i];
                    ++similar_degree[v];

                    if (similar_degree[i] == miu) cores.emplace_back(i);
                    if (similar_degree[v] == miu) cores.emplace_back(v);
                } else min_cn[j] = c;
            }

            if (min_cn[j] != -2) {
                ui r_id = BinarySearch(edges, pstart[v], pstart[v + 1], i);
                reverse_link[j] = r_id;
                reverse_link[r_id] = j;

                min_cn[r_id] = min_cn[j];
            }
        }
    }
}

void pSCANGraph::ClusterNonCoreVertices() {
    cid = vector<int>(n);
    for (ui i = 0; i < n; i++) cid[i] = n;

    for (ui i = 0; i < n; i++)
        if (similar_degree[i] >= miu) {
            int x = disjoint_set->FindRoot(i);
            if (i < cid[x]) cid[x] = i;
        }

    noncore_cluster.clear();
    noncore_cluster.reserve(n);
    for (ui i = 0; i < n; i++)
        if (similar_degree[i] >= miu) {
            for (ui j = pstart[i]; j < pstart[i + 1]; j++)
                if (similar_degree[edges[j]] < miu) {
                    if (min_cn[j] >= 0) {
                        min_cn[j] = EvalSimilarityWrapper(i, j);
                        if (reverse_link[reverse_link[j]] != j) printf("WA cluster_noncore\n");
                        min_cn[reverse_link[j]] = min_cn[j];
                        if (min_cn[j] == -1) {
                            ++similar_degree[i];
                            ++similar_degree[edges[j]];
                        }
                    }

                    if (min_cn[j] == -1) noncore_cluster.emplace_back(cid[disjoint_set->FindRoot(i)], edges[j]);
                }
        }
}

int pSCANGraph::CheckCore(int u) {
    edge_buf.clear();
    for (ui j = pstart[u]; j < pstart[u + 1]; j++) {
        if (min_cn[j] == -2) continue;

        if (similar_degree[u] < miu || !disjoint_set->IsSameSet(u, edges[j]))
            edge_buf.emplace_back(j);
    }

    int i = 0;
    for (; similar_degree[u] < miu && effective_degree[u] >= miu && i < edge_buf.size(); ++i) {
        auto idx = edge_buf[i];
        if (min_cn[idx] != -1) {
            int v = edges[idx];

            min_cn[idx] = min_cn[reverse_link[idx]] = EvalSimilarityWrapper(u, idx);

            if (min_cn[idx] == -1) ++similar_degree[u];
            else --effective_degree[u];

            if (effective_degree[v] >= 0) {
                if (min_cn[idx] == -1) {
                    ++similar_degree[v];

                    if (similar_degree[v] == miu) cores.emplace_back(v);
                } else --effective_degree[v];
            }
        }

    }

    // mark u as already explored, to avoid duplicated in cores
    effective_degree[u] = -1;
    return i;
}

void pSCANGraph::ClusterCore(int u, int early_start_idx) {
    auto i = early_start_idx;
    for (auto idx : edge_buf) {
        if (min_cn[idx] == -1 && similar_degree[edges[idx]] >= miu)
            disjoint_set->Union(u, edges[idx]);
    }

    for (; i < edge_buf.size(); ++i) {
        ui idx = edge_buf[i];
        int v = edges[idx];
        if (min_cn[idx] >= 0 && similar_degree[v] >= miu && !disjoint_set->IsSameSet(u, v)) {
            min_cn[idx] = min_cn[reverse_link[idx]] = EvalSimilarityWrapper(u, idx);

            if (effective_degree[v] >= 0) {
                if (min_cn[idx] == -1) {
                    ++similar_degree[v];

                    if (similar_degree[v] == miu) cores.emplace_back(v);
                } else --effective_degree[v];
            }

            if (min_cn[idx] == -1) disjoint_set->Union(u, v);
        }
    }
}

void pSCANGraph::pSCAN() {
    using namespace std;
    using namespace chrono;
    auto prune_start = high_resolution_clock::now();

    PruneCrossLink();
    auto prune_end = high_resolution_clock::now();
    cout << "Prune cost:" << duration_cast<milliseconds>(prune_end - prune_start).count() << " ms\n";

    MaxPriorityQueue max_priority_queue(n, &effective_degree, miu);

    int u;
    while (true) {
        if (!cores.empty()) {
            u = cores.back();
            cores.pop_back();
        } else {
            u = max_priority_queue.pop();
        }

        if (u == -1) break;

        auto i = CheckCore(u);

        if (similar_degree[u] >= miu) {
            ClusterCore(u, i);
        }
    }

    ClusterNonCoreVertices();
}