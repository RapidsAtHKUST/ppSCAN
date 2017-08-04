#include "Graph.h"

#include <chrono>

Graph::Graph(const char *_dir) {
    dir = string(_dir);

    n = m = 0;

    eps_a2 = eps_b2 = miu = 0;

    pstart = nullptr;
    edges = nullptr;
    reverse = nullptr;
    min_cn = nullptr;

    cid = nullptr;

    degree = nullptr;
    effective_degree = nullptr;
    similar_degree = nullptr;

    pa = nullptr;
    rank = nullptr;
}

Graph::~Graph() {
    if (pstart != nullptr) {
        delete[] pstart;
        pstart = nullptr;
    }
    if (edges != nullptr) {
        delete[] edges;
        edges = nullptr;
    }
    if (reverse != nullptr) {
        delete[] reverse;
        reverse = nullptr;
    }
    if (min_cn != nullptr) {
        delete[] min_cn;
        min_cn = nullptr;
    }
    if (cid != nullptr) {
        delete[] cid;
        cid = nullptr;
    }
    if (degree != nullptr) {
        delete[] degree;
        degree = nullptr;
    }
    if (effective_degree != nullptr) {
        delete[] effective_degree;
        effective_degree = nullptr;
    }
    if (similar_degree != nullptr) {
        delete[] similar_degree;
        similar_degree = nullptr;
    }
    if (pa != nullptr) {
        delete[] pa;
        pa = nullptr;
    }
    if (rank != nullptr) {
        delete[] rank;
        rank = nullptr;
    }
}

void Graph::read_graph() {
    FILE *f = open_file((dir + string("/b_degree.bin")).c_str(), "rb");

    int tt;
    fread(&tt, sizeof(int), 1, f);
    if (tt != (int) sizeof(int)) {
        cout << "sizeof int is different: edge.bin(" << tt << "), machine(" << (int) sizeof(int) << "\n";
        return;
    }
    fread(&n, sizeof(int), 1, f);
    fread(&m, sizeof(int), 1, f);

    degree = new int[n];
    fread(degree, sizeof(int), n, f);

    fclose(f);

    f = open_file((dir + string("/b_adj.bin")).c_str(), "rb");

    if (pstart == nullptr) pstart = new ui[n + 1];
    if (edges == nullptr) edges = new int[m];
    if (reverse == nullptr) reverse = new ui[m];
    if (min_cn == nullptr) min_cn = new int[m];
    memset(min_cn, 0, sizeof(int) * m);

    auto *buf = new int[n];

    pstart[0] = 0;
    for (ui i = 0; i < n; i++) {
        //printf("%d %d\n", i, degree[i]);
        if (degree[i] > 0) fread(buf, sizeof(int), degree[i], f);

        for (ui j = 0; j < degree[i]; j++) edges[pstart[i] + j] = buf[j];

        pstart[i + 1] = pstart[i] + degree[i];

        ++degree[i];
    }

    delete[] buf;

    fclose(f);

    for (ui i = 0; i < n; i++) {
        for (ui j = pstart[i]; j < pstart[i + 1]; j++) {
            if (edges[j] == i)cout << "Self loop\n";
            if (j > pstart[i] && edges[j] <= edges[j - 1])
                cout << "Edges not sorted in increasing id order!\nThe program may not run properly!\n";
        }
    }
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
    if (cid == nullptr) cid = new int[n];
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

void Graph::output(const char *eps_s, const char *miu) {
    printf("\t*** Start write result into disk!\n");

    string out_name = dir + "/result-" + string(eps_s) + "-" + string(miu) + ".txt";
    FILE *fout = open_file(out_name.c_str(), "w");

    fprintf(fout, "c/n vertex_id cluster_id\n");

    int mu = atoi(miu);
    for (ui i = 0; i < n; i++)
        if (similar_degree[i] >= mu) {
            fprintf(fout, "c %d %d\n", i, cid[pa[i]]);
        }

    sort(noncore_cluster.begin(), noncore_cluster.end());
    noncore_cluster.erase(unique(noncore_cluster.begin(), noncore_cluster.end()), noncore_cluster.end());
    for (auto &i : noncore_cluster) {
        fprintf(fout, "n %d %d\n", i.second, i.first);
    }

    fclose(fout);
}

void Graph::pSCAN(const char *eps_s, int _miu) {
    get_eps(eps_s);
    miu = _miu;

    if (similar_degree == nullptr) similar_degree = new int[n];
    memset(similar_degree, 0, sizeof(int) * n);

    if (effective_degree == nullptr) effective_degree = new int[n];
    for (ui i = 0; i < n; i++) effective_degree[i] = degree[i] - 1;

    if (pa == nullptr) pa = new int[n];
    if (rank == nullptr) rank = new int[n];
    for (ui i = 0; i < n; i++) {
        pa[i] = i;
        rank[i] = 0;
    }

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

void Graph::get_eps(const char *eps_s) {
    int i = 0, eps_a = 0, eps_b = 1;
    while (eps_s[i] != '\0' && eps_s[i] != '.') {
        eps_a = eps_a * 10 + (eps_s[i] - '0');
        ++i;
    }

    if (eps_s[i] == '.') {
        ++i;
        while (eps_s[i] != '\0') {
            eps_a = eps_a * 10 + (eps_s[i] - '0');
            eps_b *= 10;
            ++i;
        }
    }

    if (eps_a > eps_b || eps_b > 100 || eps_a <= 0) {
        cout << "??? Wrong eps format: " << eps_a << "," << eps_b << "," << eps_s << "\n";
        exit(1);
    }

    eps_a2 = eps_a * eps_a;
    eps_b2 = eps_b * eps_b;
}
