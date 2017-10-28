//
// Created by yche on 10/27/17.
//

#include "scan_xp_galloping.h"
#include <chrono>

void Usage() {
    cout << "Usage: [1]exe [2]graph-dir [3]similarity-threshold "
            "[4]density-threshold [5 thread_num] [6 optional]output\n";
}

static double EPSILON;
static int MY_U;

int GallopingSearch(int *array, int offset_beg, int offset_end, int val) {
    if (array[offset_end - 1] < val) {
        return offset_end;
    }

    // galloping
    if (array[offset_beg] >= val) {
        return offset_beg;
    }
    if (array[offset_beg + 1] >= val) {
        return offset_beg + 1;
    }
    if (array[offset_beg + 2] >= val) {
        return offset_beg + 2;
    }

    auto jump_idx = 4u;
    bool is_working = true;
    while (is_working) {
        auto peek_idx = offset_beg + jump_idx;
        if (peek_idx >= offset_end) {
            peek_idx = offset_end - 1;
            is_working = false;
        }
        if (array[peek_idx] == val) {
            return peek_idx;
        }
        if (array[peek_idx] > val) {
            return GallopingSearch(array, jump_idx / 2 + offset_beg + 1, peek_idx + 1, val);
        }
        jump_idx <<= 1;
    }
}

int main(int argc, char *argv[]) {
    using namespace chrono;
    if (argc < 4) {
        Usage();
        return 0;
    }
    cout << "****scan-xp galloping on cpu: " << argv[1] << ", " << argv[2] << ", " << argv[3] << " *** \n";

    // input:
    int cluster_num = 0;
    int hub_num = 0;
    int out_num = 0;

    // parse parameters
    cout << "graph dir" << argv[1] << endl;
    Graph g(argv[1]);
    EPSILON = strtod(argv[2], nullptr);
    MY_U = atoi(argv[3]);
    NUMT = atoi(argv[4]);

    UnionFind uf(g.nodemax);

    // pre-processing
#pragma omp parallel for num_threads(NUMT) schedule(dynamic, 100000)
    for (int i = 0; i < g.nodemax; i++) {
        g.core_count[i] = 0;
        g.label[i] = UNCLASSIFIED;
        for (int n = g.node_off[i]; n < g.node_off[i + 1]; n++) {
            g.edge_src[n] = i;
            g.common_node_num[n] = 2;
        }
    }

    //step1 core_detection
    cout << "STEP1" << endl;
    double s1 = omp_get_wtime();
    core_detection(&g);
    double e1 = omp_get_wtime();

    //step2 cluster_construction
    cout << "STEP2" << endl;
    double s2 = omp_get_wtime();
#pragma omp parallel for num_threads(NUMT) schedule(dynamic, 2000)
    for (int i = 0; i < g.nodemax; i++) {
        int rx, ry, index;
        if (g.label[i] == CORE) {
            for (int n = g.node_off[i]; n < g.node_off[i + 1]; n++) {
                // yche: fix bug, only union when g.edge_dst[n] is a core vertex
                if (g.label[g.edge_dst[n]] != CORE)continue;
                if (!g.similarity[n])continue;
                do {
                    rx = uf.root(i);
                    ry = uf.root(g.edge_dst[n]);
                    if (rx < ry) {
                        index = rx;
                        rx = ry;
                        ry = index;
                    }
                    if (rx == ry)break;
                } while (!__sync_bool_compare_and_swap(&(uf.parent[rx]), rx, ry));
            }
        }
    }
    double e2 = omp_get_wtime();

    //step3 Hub Outlier detection
    cout << "STEP3" << endl;
    double s3 = omp_get_wtime();
    atomic_int core_num(0);
#pragma omp parallel for num_threads(NUMT) schedule(dynamic, 1000)
    for (int i = 0; i < g.nodemax; i++) {
        if (g.label[i] == CORE) {
            core_num++;
            continue;
        }

        if (hub_check_uf(&g, &uf, i)) {
            g.label[i] = HUB;
        }
    }
    double e3 = omp_get_wtime();

    // post-processing
    g.MarkClusterMinEleAsId(&uf);

    set<int> c;
    for (int i = 0; i < g.nodemax; i++) {
        if (g.label[i] == CORE) {
            c.emplace(uf.root(i));
        }
    }
    cluster_num = static_cast<int>(c.size());

#pragma omp parallel for num_threads(NUMT) reduction(+:hub_num, out_num)
    for (int i = 0; i < g.nodemax; i++) {
        if (g.label[i] == HUB) {
            hub_num++;
        } else if (g.label[i] == UNCLASSIFIED) {
            out_num++;
        }
    }

    cout << "Detect_cluster: " << cluster_num << endl << "Core:" << core_num << endl << "Hub: " << hub_num << endl
         << "Outlier: " << out_num << endl;
    cout << "STEP1, core_detection:" << e1 - s1 << endl;
    cout << "STEP2, cluster_construction:" << e2 - s2 << endl;
    cout << "STEP3, hub_and_outlier_detection:" << e3 - s3 << endl;

    // prepare output
    for (auto i = 0; i < g.nodemax; i++) {
        if (g.label[i] == CORE) {
            for (auto j = g.node_off[i]; j < g.node_off[i + 1]; j++) {
                auto v = g.edge_dst[j];
                if (g.label[v] != CORE && g.similarity[j]) {
                    g.noncore_cluster.emplace_back(g.cluster_dict[uf.root(i)], v);
                }
            }
        }
    }
    cout << "prepare finish" << endl;

    g.Output(argv[2], argv[3], &uf);
}

inline int compute_cn(Graph *g, int edge_idx) {
    auto u = g->edge_src[edge_idx];
    auto v = g->edge_dst[edge_idx];
    auto cn_count = 0;
    auto offset_nei_u = g->node_off[u], offset_nei_v = g->node_off[v];
    auto off_u_end = g->node_off[u + 1], off_v_end = g->node_off[v + 1];
    while (true) {
        offset_nei_u = GallopingSearch(g->edge_dst, offset_nei_u, off_u_end, g->edge_dst[offset_nei_v]);
        if (offset_nei_u >= off_u_end) {
            return cn_count;
        }

        offset_nei_v = GallopingSearch(g->edge_dst, offset_nei_v, off_v_end, g->edge_dst[offset_nei_u]);
        if (offset_nei_v >= off_v_end) {
            return cn_count;
        }

        if (g->edge_dst[offset_nei_u] == g->edge_dst[offset_nei_v]) {
            cn_count++;
            ++offset_nei_u;
            ++offset_nei_v;
            if (offset_nei_u >= off_u_end || offset_nei_v >= off_v_end) {
                return cn_count;
            }
        }
    }
}

inline void core_detection(Graph *g) {
#pragma omp parallel for num_threads(NUMT) schedule(dynamic, 6000)
    for (int i = 0; i < g->edgemax; i++) {
        g->common_node_num[i] += compute_cn(g, i);
    }

#pragma omp parallel for num_threads(NUMT)
    for (int i = 0; i < g->edgemax; i++) {
        g->sim_values[i] =
                g->common_node_num[i] / sqrt(((g->node_off[g->edge_src[i] + 1] - g->node_off[g->edge_src[i]]) + 1) *
                                             ((g->node_off[(g->edge_dst[i] + 1)] - g->node_off[g->edge_dst[i]]) + 1));
    }

#pragma omp parallel for num_threads(NUMT)
    for (int i = 0; i < g->edgemax; i++) {
        if (g->sim_values[i] >= EPSILON) {
            g->core_count[g->edge_src[i]]++;
            g->similarity[i] = true;
        } else {
            g->similarity[i] = false;
        }
    }

#pragma omp parallel for num_threads(NUMT)
    for (int i = 0; i < g->nodemax; i++) {
        if (g->core_count[i] >= MY_U) {
            g->label[i] = CORE;
        };
    }
}

inline bool hub_check_uf(Graph *g, UnionFind *uf, int a) {
    set<int> c;

    for (int i = g->node_off[a]; i < g->node_off[a + 1]; i++) {
        if (g->label[g->edge_dst[i]] != CORE)continue;
        c.insert((*uf).root(g->edge_dst[i]));
    }
    return c.size() >= 2;
}