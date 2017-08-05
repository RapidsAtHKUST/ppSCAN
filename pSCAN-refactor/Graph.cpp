#include "Graph.h"

#include <cstring>
#include <cmath>
#include <chrono>
#include <iostream>

Graph::Graph(const char *dir_string, const char *eps_s, int miu) {
    std::tie(eps_a2, eps_b2) = get_eps(eps_s);
    this->miu = miu;

    // helper
    io_helper_ptr = make_unique<InputOutput>(dir_string);
    io_helper_ptr->read_graph();

    // 1st: graph
    n = static_cast<ui>(io_helper_ptr->n);
    degree = io_helper_ptr->degree;
    pstart = io_helper_ptr->pstart;
    edges = io_helper_ptr->edges;
    reverse = io_helper_ptr->reverse_;
    min_cn = io_helper_ptr->min_cn;

    // 2nd: algorithm related: sd, ed
    similar_degree = vector<int>(n);
    std::fill(similar_degree.begin(), similar_degree.end(), 0);

    effective_degree = vector<int>(n);
    for (ui i = 0; i < n; i++) {
        effective_degree[i] = degree[i] - 1;
    }

    // 3rd: disjoint-set, make-set at the beginning
    make_disjoint_set_batch();
}

void Graph::output(const char *eps_s, const char *miu) {
    io_helper_ptr->output(eps_s, miu, noncore_cluster, similar_degree, cid, parent);
}

Graph::~Graph() {
    // 1st: graph
    delete[] pstart;
    delete[] edges;
    delete[] reverse;
    delete[] min_cn;

    // 2nd: algorithm related: sd, ed
    delete[] degree;
}

// data structure related: union-find disjoint set
void Graph::make_disjoint_set_batch() {
    parent = vector<int>(n);
    rank = vector<int>(n);
    for (ui i = 0; i < n; i++) {
        parent[i] = i;
    }
    std::fill(rank.begin(), rank.end(), 0);
}

int Graph::find_root(int u) {
    int x = u;
    while (parent[x] != x) {
        x = parent[x];
    }

    while (parent[u] != x) {
        int tmp = parent[u];
        parent[u] = x;
        u = tmp;
    }
    return x;
}

void Graph::union_if_not_same_set(int u, int v) {
    int ru = find_root(u);
    int rv = find_root(v);

    if (ru == rv) {
        return;
    }

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

// 1st phase core implementation: prune_and_cross_link related, for cores clustering:
ui Graph::binary_search(const int *array, ui b, ui e, int val) {
    --e;
    if (array[e] < val) {
        return e + 1;
    }
    while (b < e) {
        ui mid = b + (e - b) / 2;
        if (array[mid] >= val) {
            e = mid;
        } else {
            b = mid + 1;
        }
    }
    return e;
}

int Graph::compute_common_neighbor_lowerbound(int du, int dv) {
    auto c = (int) (sqrtl((((long double) du) * ((long double) dv) * eps_a2) / eps_b2));

    if (((long long) c) * ((long long) c) * eps_b2 < ((long long) du) * ((long long) dv) * eps_a2) {
        ++c;
    }
    return c;
}

void Graph::prune_and_cross_link(vector<int> &cores, int &cores_e) {
    for (ui i = 0; i < n; i++) { //must be iterating from 0 to n-1
        for (ui j = pstart[i]; j < pstart[i + 1]; j++) {
            if (edges[j] < i) {
                if (min_cn[j] == NOT_SURE) {
                    min_cn[j] = NOT_SIMILAR;
                }
                continue; //this edge has already been checked
            }

            int v = edges[j];
            int a = degree[i], b = degree[v];
            if (a > b) {
                swap(a, b);
            }

            if (((long long) a) * eps_b2 < ((long long) b) * eps_a2) {
                min_cn[j] = NOT_SIMILAR;

                --effective_degree[i];
                --effective_degree[v];
            } else {
                int c = compute_common_neighbor_lowerbound(a, b);

                if (c <= 2) {
                    min_cn[j] = SIMILAR;

                    ++similar_degree[i];
                    ++similar_degree[v];

                    if (similar_degree[i] == miu) {
                        cores[cores_e++] = i;
                    }
                    if (similar_degree[v] == miu) {
                        cores[cores_e++] = v;
                    }
                } else {
                    min_cn[j] = c;
                }
            }

            if (min_cn[j] != -2) {
                ui r_id = binary_search(edges, pstart[v], pstart[v + 1], i);
                reverse[j] = r_id;
                reverse[r_id] = j;

                min_cn[r_id] = min_cn[j];
            }
        }
    }
}

// 2nd phase core implementation:  non-core vertices clustering related:
int Graph::check_common_neighbor(int u, int v, int c) {
    int cn = 2;

    if (degree[u] > degree[v]) {
        swap(u, v);
    }

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

    if (cn >= c) {
        return SIMILAR;
    }
    return NOT_SIMILAR;
}

int Graph::similar_check_OP(int u, ui idx) {
    int v = edges[idx];

    if (min_cn[idx] == NOT_SURE) {
        int du = degree[u], dv = degree[v];
        int c = compute_common_neighbor_lowerbound(du, dv);

        if (c <= 2) {
            return SIMILAR;
        }

        min_cn[idx] = min_cn[reverse[idx]] = c;
    }

    return check_common_neighbor(u, v, min_cn[idx]);
}

void Graph::cluster_noncore_vertices() {
    cid.resize(n);
    std::fill(cid.begin(), cid.end(), n);

    for (ui i = 0; i < n; i++)
        if (similar_degree[i] >= miu) {
            int x = find_root(i);
            if (i < cid[x]) {
                cid[x] = i;
            }
        }

    noncore_cluster.clear();
    noncore_cluster.reserve(n);
    for (ui i = 0; i < n; i++)
        if (similar_degree[i] >= miu) {
            for (ui j = pstart[i]; j < pstart[i + 1]; j++) {
                if (similar_degree[edges[j]] < miu) {
                    if (min_cn[j] >= 0) {
                        min_cn[j] = similar_check_OP(i, j);
                        if (reverse[reverse[j]] != j) {
                            cout << "WA cluster_noncore\n";
                        }
                        min_cn[reverse[j]] = min_cn[j];
                        if (min_cn[j] == SIMILAR) {
                            ++similar_degree[i];
                            ++similar_degree[edges[j]];
                        }
                    }

                    if (min_cn[j] == SIMILAR) {
                        noncore_cluster.emplace_back(cid[parent[i]], edges[j]);
                    }
                }
            }
        }
}

void Graph::pSCAN() {
    using namespace std::chrono;
    auto start = high_resolution_clock::now();

    auto edge_buf = vector<ui>(n);
    auto cores = vector<int>(n);
    int cores_n = 0;

    prune_and_cross_link(cores, cores_n);

    auto end1 = high_resolution_clock::now();
    cout << "prune and cross link execution time:" << duration_cast<milliseconds>(end1 - start).count() << " ms\n";

    auto bin_head = vector<int>(n);
    std::fill(bin_head.begin(), bin_head.end(), -1);
    auto bin_next = vector<int>(n);

    int max_ed = 0;
    for (ui i = 0; i < n; i++) {
        if (effective_degree[i] >= miu) {
            int ed = effective_degree[i];
            if (ed > max_ed) {
                max_ed = ed;
            }
            bin_next[i] = bin_head[ed];
            bin_head[ed] = i;
        }
    }

    while (true) {
        int u = -1;
        if (cores_n != 0) {
            u = cores[--cores_n];
        } else {
            while (max_ed >= miu && u == -1) {
                for (int x = bin_head[max_ed]; x != -1;) {
                    int tmp = bin_next[x];
                    int ed = effective_degree[x];
                    if (ed == max_ed) {
                        u = x;
                        bin_head[max_ed] = bin_next[x];
                        break;
                    }
                    if (ed >= miu) {
                        bin_next[x] = bin_head[ed];
                        bin_head[ed] = x;
                    }
                    x = tmp;
                }
                if (u == -1) {
                    bin_head[max_ed] = -1;
                    --max_ed;
                }
            }
        }
        if (u == -1) {
            break;
        }

        int edge_buf_n = 0;
        for (ui j = pstart[u]; j < pstart[u + 1]; j++) {
            if (min_cn[j] == NOT_SIMILAR) {
                continue;
            }
            if (similar_degree[u] < miu || find_root(u) != find_root(edges[j])) {
                edge_buf[edge_buf_n++] = j;
            }
        }

        int i = 0;
        while (similar_degree[u] < miu && effective_degree[u] >= miu && i < edge_buf_n) {
            ui idx = edge_buf[i];
            if (min_cn[idx] != SIMILAR) {
                int v = edges[idx];

                min_cn[idx] = min_cn[reverse[idx]] = similar_check_OP(u, idx);

                if (min_cn[idx] == SIMILAR) {
                    ++similar_degree[u];
                } else {
                    --effective_degree[u];
                }

                if (effective_degree[v] >= 0) {
                    if (min_cn[idx] == SIMILAR) {
                        ++similar_degree[v];

                        if (similar_degree[v] == miu) {
                            cores[cores_n++] = v;
                        }
                    } else {
                        --effective_degree[v];
                    }
                }
            }

            ++i;
        }

        effective_degree[u] = -1;

        if (similar_degree[u] < miu) {
            continue;
        }

        for (int j = 0; j < edge_buf_n; j++) {
            ui idx = edge_buf[j];
            if (min_cn[idx] == SIMILAR && similar_degree[edges[idx]] >= miu) {
                union_if_not_same_set(u, edges[idx]);
            }
        }

        while (i < edge_buf_n) {
            ui idx = edge_buf[i];
            int v = edges[idx];
            if (min_cn[idx] < 0 || similar_degree[v] < miu || find_root(u) == find_root(v)) {
                ++i;
                continue;
            }

            min_cn[idx] = min_cn[reverse[idx]] = similar_check_OP(u, idx);

            if (effective_degree[v] >= 0) {
                if (min_cn[idx] == SIMILAR) {
                    ++similar_degree[v];

                    if (similar_degree[v] == miu) {
                        cores[cores_n++] = v;
                    }
                } else {
                    --effective_degree[v];
                }
            }

            if (min_cn[idx] == SIMILAR) {
                union_if_not_same_set(u, v);
            }
            ++i;
        }
    }

    auto end = high_resolution_clock::now();
    cout << "before non-core clustering time:" << duration_cast<milliseconds>(end - end1).count() << " ms\n";
    cluster_noncore_vertices();
}
