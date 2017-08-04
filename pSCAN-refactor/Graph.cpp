#include "Graph.h"

Graph::Graph(const char *dir_string) {
    // helper
    io_helper_ptr = make_unique<InputOutput>(dir_string);
    io_helper_ptr->read_graph();

    // 1st: graph
    n = io_helper_ptr->n;
    degree = io_helper_ptr->degree;
    pstart = io_helper_ptr->pstart;
    edges = io_helper_ptr->edges;
    reverse = io_helper_ptr->reverse_;
    min_cn = io_helper_ptr->min_cn;

    // 2nd: algorithm related: sd, ed
    similar_degree = new int[n];
    memset(similar_degree, 0, sizeof(int) * n);

    effective_degree = new int[n];
    for (ui i = 0; i < n; i++) effective_degree[i] = degree[i] - 1;

    // 3rd: disjoint-set
    pa = new int[n];
    rank = new int[n];
    for (ui i = 0; i < n; i++) {
        pa[i] = i;
        rank[i] = 0;
    }
}

Graph::~Graph() {
    delete[] pstart;
    delete[] edges;
    delete[] reverse;
    delete[] min_cn;
    if (cid != nullptr) {
        delete[] cid;
        cid = nullptr;
    }
    delete[] degree;
    delete[] effective_degree;
    delete[] similar_degree;
    delete[] pa;
    delete[] rank;
}

void Graph::output(const char *eps_s, const char *miu) {
    io_helper_ptr->output(eps_s, miu, noncore_cluster, similar_degree, cid, pa);
}

ui Graph::binary_search(const int *array, ui b, ui e, int val) {
    --e;
    if (array[e] < val) return e + 1;
    while (b < e) {
        ui mid = b + (e - b) / 2;
        if (array[mid] >= val) e = mid;
        else b = mid + 1;
    }
    return e;
}

void Graph::cluster_noncore_vertices(int eps_a2, int eps_b2, int mu) {
    cid = new int[n];
    for (ui i = 0; i < n; i++) cid[i] = n;

    for (ui i = 0; i < n; i++)
        if (similar_degree[i] >= mu) {
            int x = find_root(i);
            if (i < cid[x]) cid[x] = i;
        }

    noncore_cluster.clear();
    noncore_cluster.reserve(n);
    for (ui i = 0; i < n; i++)
        if (similar_degree[i] >= mu) {
            for (ui j = pstart[i]; j < pstart[i + 1]; j++)
                if (similar_degree[edges[j]] < mu) {
                    if (min_cn[j] >= 0) {
                        min_cn[j] = similar_check_OP(i, j, eps_a2, eps_b2);
                        if (reverse[reverse[j]] != j)cout << "WA cluster_noncore\n";
                        min_cn[reverse[j]] = min_cn[j];
                        if (min_cn[j] == -1) {
                            ++similar_degree[i];
                            ++similar_degree[edges[j]];
                        }
                    }

                    if (min_cn[j] == -1) noncore_cluster.emplace_back(cid[pa[i]], edges[j]);
                }
        }
}


void Graph::pSCAN(const char *eps_s, int _miu) {
    auto eps_pair = get_eps(eps_s);
    eps_a2 = eps_pair.first;
    eps_b2 = eps_pair.second;
    miu = _miu;

    using namespace std::chrono;
    auto start = high_resolution_clock::now();

    auto *edge_buf = new ui[n];
    auto *cores = new int[n];
    int cores_n = 0;

    prune_and_cross_link(eps_a2, eps_b2, miu, cores, cores_n);

    auto end1 = high_resolution_clock::now();
    cout << "prune and cross link execution time:" << duration_cast<milliseconds>(end1 - start).count() << " ms"
         << endl;

    auto *bin_head = new int[n];
    auto *bin_next = new int[n];
    for (ui i = 0; i < n; i++) bin_head[i] = -1;

    int max_ed = 0;
    for (ui i = 0; i < n; i++)
        if (effective_degree[i] >= miu) {
            int ed = effective_degree[i];
            if (ed > max_ed) max_ed = ed;
            bin_next[i] = bin_head[ed];
            bin_head[ed] = i;
        }

    while (true) {
        int u = -1;
        if (cores_n) u = cores[--cores_n];
        else {
            while (max_ed >= miu && u == -1) {
                for (int x = bin_head[max_ed]; x != -1;) {
                    int tmp = bin_next[x];
                    int ed = effective_degree[x];
                    if (ed == max_ed) {
                        u = x;
                        bin_head[max_ed] = bin_next[x];
                        break;
                    } else if (ed >= miu) {
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
        if (u == -1) break;

        int edge_buf_n = 0;
        for (ui j = pstart[u]; j < pstart[u + 1]; j++) {
            if (min_cn[j] == -2) continue;

            if (similar_degree[u] < miu || find_root(u) != find_root(edges[j])) edge_buf[edge_buf_n++] = j;
        }

        int i = 0;
        while (similar_degree[u] < miu && effective_degree[u] >= miu && i < edge_buf_n) {
            ui idx = edge_buf[i];
            if (min_cn[idx] != -1) {
                int v = edges[idx];

                min_cn[idx] = min_cn[reverse[idx]] = similar_check_OP(u, idx, eps_a2, eps_b2);

                if (min_cn[idx] == -1) ++similar_degree[u];
                else --effective_degree[u];

                if (effective_degree[v] >= 0) {
                    if (min_cn[idx] == -1) {
                        ++similar_degree[v];

                        if (similar_degree[v] == miu) cores[cores_n++] = v;
                    } else --effective_degree[v];
                }
            }

            ++i;
        }

        effective_degree[u] = -1;

        if (similar_degree[u] < miu) continue;

        for (int j = 0; j < edge_buf_n; j++) {
            ui idx = edge_buf[j];
            if (min_cn[idx] == -1 && similar_degree[edges[idx]] >= miu) my_union(u, edges[idx]);
        }

        while (i < edge_buf_n) {
            ui idx = edge_buf[i];
            int v = edges[idx];
            if (min_cn[idx] < 0 || similar_degree[v] < miu || find_root(u) == find_root(v)) {
                ++i;
                continue;
            }

            min_cn[idx] = min_cn[reverse[idx]] = similar_check_OP(u, idx, eps_a2, eps_b2);

            if (effective_degree[v] >= 0) {
                if (min_cn[idx] == -1) {
                    ++similar_degree[v];

                    if (similar_degree[v] == miu) cores[cores_n++] = v;
                } else --effective_degree[v];
            }

            if (min_cn[idx] == -1) my_union(u, v);

            ++i;
        }
    }

    delete[] edge_buf;
    delete[] cores;
    delete[] bin_head;
    delete[] bin_next;

    auto end = high_resolution_clock::now();
    cout << "other time:" << duration_cast<milliseconds>(end - end1).count() << " ms";
    cluster_noncore_vertices(eps_a2, eps_b2, miu);
}

int Graph::check_common_neighbor(int u, int v, int c) {
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

    if (cn >= c) return -1;
    return -2;
}

int Graph::similar_check_OP(int u, ui idx, int eps_a2, int eps_b2) {
    int v = edges[idx];

    if (min_cn[idx] == 0) {
        int du = degree[u], dv = degree[v];
        int c = compute_common_neighbor_lowerbound(du, dv, eps_a2, eps_b2);

        if (c <= 2) return -1;

        min_cn[idx] = min_cn[reverse[idx]] = c;
    }

    return check_common_neighbor(u, v, min_cn[idx]);
}

int Graph::compute_common_neighbor_lowerbound(int du, int dv, int eps_a2, int eps_b2) {
    auto c = (int) (sqrtl((((long double) du) * ((long double) dv) * eps_a2) / eps_b2));

    if (((long long) c) * ((long long) c) * eps_b2 < ((long long) du) * ((long long) dv) * eps_a2) ++c;
    return c;
}

void Graph::prune_and_cross_link(int eps_a2, int eps_b2, int miu, int *cores, int &cores_e) {
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
                int c = compute_common_neighbor_lowerbound(a, b, eps_a2, eps_b2);

                if (c <= 2) {
                    min_cn[j] = -1;

                    ++similar_degree[i];
                    ++similar_degree[v];

                    if (similar_degree[i] == miu) cores[cores_e++] = i;
                    if (similar_degree[v] == miu) cores[cores_e++] = v;
                } else min_cn[j] = c;
            }

            if (min_cn[j] != -2) {
                //else {
                ui r_id = binary_search(edges, pstart[v], pstart[v + 1], i);
                reverse[j] = r_id;
                reverse[r_id] = j;

                min_cn[r_id] = min_cn[j];
            }
        }
    }
}

int Graph::find_root(int u) {
    int x = u;
    while (pa[x] != x) x = pa[x];

    while (pa[u] != x) {
        int tmp = pa[u];
        pa[u] = x;
        u = tmp;
    }

    return x;
}

void Graph::my_union(int u, int v) {
    int ru = find_root(u);
    int rv = find_root(v);

    if (ru == rv) return;

    if (rank[ru] < rank[rv]) pa[ru] = rv;
    else if (rank[ru] > rank[rv]) pa[rv] = ru;
    else {
        pa[rv] = ru;
        ++rank[ru];
    }
}

