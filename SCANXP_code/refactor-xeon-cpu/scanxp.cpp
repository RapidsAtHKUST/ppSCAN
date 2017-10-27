/*
Xeon (AVX2) by http://www.kde.cs.tsukuba.ac.jp/~shihakata
*/

#include "scanxp.h"

#include <chrono>

void Usage() {
    cout << "Usage: [1]exe [2]graph-dir [3]similarity-threshold "
            "[4]density-threshold [5 thread_num] [6 optional]output\n";
}

static double EPSILON;
static int MY_U;

int main(int argc, char *argv[]) {
    using namespace chrono;
    if (argc < 4) {
        Usage();
        return 0;
    }
    cout << "****scan-xp on cpu: " << argv[1] << ", " << argv[2] << ", " << argv[3] << " *** \n";

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

    cout << "Number_of_threads" << NUMT << endl;
    cout << g.nodemax << endl << g.edgemax << endl;
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

/*
Step1 'core_detection' detects all core nodes in graph.
Firstly, this method calculates set intersection of all edges.
Next, this method calculates structural similarity based on result of set intersection.
Finally, this method determines whether all nodes are core or not.
*/

#ifdef NAIVE
inline int compute_cn(Graph *g, int edge_idx) {
    auto u = g->edge_src[edge_idx];
    auto v = g->edge_dst[edge_idx];
    auto cn_count = 0;
    auto offset_nei_u = g->node_off[u], offset_nei_v = g->node_off[v];
    auto off_u_end = g->node_off[u + 1], off_v_end = g->node_off[v + 1];
    while (true) {
        while (g->edge_dst[offset_nei_u] < g->edge_dst[offset_nei_v]) {
            ++offset_nei_u;
            if (offset_nei_u >= off_u_end) {
                return cn_count;
            }
        }

        while (g->edge_dst[offset_nei_u] > g->edge_dst[offset_nei_v]) {
            ++offset_nei_v;
            if (offset_nei_v >= off_v_end) {
                return cn_count;
            }
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
#endif

inline void core_detection(Graph *g) {
    int countplus[PARA] = {1, 1, 1, 1, 1, 1, 1, 1};
    __m256i sse_countplus = _mm256_load_si256((__m256i *) (countplus));
    __m256i sj = _mm256_set_epi32(1, 1, 1, 1, 0, 0, 0, 0);
    __m256i st = _mm256_set_epi32(3, 2, 1, 0, 3, 2, 1, 0);

#pragma omp parallel for num_threads(NUMT) schedule(dynamic, 6000)
    for (int i = 0; i < g->edgemax; i++) {
#ifdef NAIVE
        g->common_node_num[i] += compute_cn(g, i);
#else
        int cnv[PARA] = {0, 0, 0, 0, 0, 0, 0, 0};

        __m256i sse_cnv = _mm256_load_si256((__m256i *) (cnv));
        int j, t, j2, t2;

        if (g->node_off[g->edge_src[i] + 1] - g->node_off[g->edge_src[i]] <
            g->node_off[g->edge_dst[i] + 1] - g->node_off[g->edge_dst[i]]) {
            j = g->node_off[g->edge_src[i]];
            t = g->node_off[g->edge_dst[i]];
            j2 = g->node_off[g->edge_src[i] + 1];
            t2 = g->node_off[g->edge_dst[i] + 1];
        } else {
            t = g->node_off[g->edge_src[i]];
            j = g->node_off[g->edge_dst[i]];
            t2 = g->node_off[g->edge_src[i] + 1];
            j2 = g->node_off[g->edge_dst[i] + 1];
        }

        int size_ratio;
        size_ratio = (t2 - t) / (j2 - j);

        if (size_ratio > 2) {
            __m256i jnode = _mm256_set1_epi32(g->edge_dst[j]);
            __m256i tnode = _mm256_loadu_si256((__m256i *) (g->edge_dst + t));
            __m256i ssecnv = _mm256_load_si256((__m256i *) (cnv));

            int vsize = ((t2 - t) / PARA) * PARA;
            int to = t;

            while (j < j2 && t < to + vsize) {
                __m256i mask = _mm256_cmpeq_epi32(jnode, tnode);
                mask = _mm256_and_si256(sse_countplus, mask);
                ssecnv = _mm256_add_epi32(ssecnv, mask);

                if (g->edge_dst[j] > g->edge_dst[t + 7]) {
                    t += PARA;
                    tnode = _mm256_loadu_si256((__m256i *) (g->edge_dst + t));
                } else {
                    j++;
                    jnode = _mm256_set1_epi32(g->edge_dst[j]);
                }
            }
            _mm256_store_si256((__m256i *) cnv, ssecnv);

            for (int cnvplus : cnv) {
                g->common_node_num[i] += cnvplus;
            }

            t = (to + vsize);

            while (j < j2 && t < t2) {
                if (g->edge_dst[j] == g->edge_dst[t]) {
                    g->common_node_num[i]++;
                    j++;
                    t++;
                } else if (g->edge_dst[j] > g->edge_dst[t]) {
                    t++;
                } else {
                    j++;
                }
            }
        } else {
            if (t2 - t < j2 - j) {
                j = g->node_off[g->edge_dst[i]];
                t = g->node_off[g->edge_src[i]];
                j2 = g->node_off[g->edge_dst[i] + 1];
                t2 = g->node_off[g->edge_src[i] + 1];
            }
            int jsize = ((j2 - j) / 2) * 2 + j;
            int tsize = ((t2 - t) / 4) * 4 + t;

            __m256i jnode, tnode;

            jnode = _mm256_loadu_si256((__m256i *) (g->edge_dst + j));
            tnode = _mm256_loadu_si256((__m256i *) (g->edge_dst + t));

            while (j < jsize && t < tsize) {
                __m256i jnodeA = _mm256_permutevar8x32_epi32(jnode, sj);
                __m256i tnodeA = _mm256_permutevar8x32_epi32(tnode, st);
                __m256i mask = _mm256_cmpeq_epi32(jnodeA, tnodeA);
                mask = _mm256_and_si256(sse_countplus, mask);
                sse_cnv = _mm256_add_epi32(sse_cnv, mask);

                if (g->edge_dst[j + 1] == g->edge_dst[t + 3]) {
                    j += 2;
                    t += 4;
                    jnode = _mm256_loadu_si256((__m256i *) (g->edge_dst + j));
                    tnode = _mm256_loadu_si256((__m256i *) (g->edge_dst + t));
                } else if (g->edge_dst[j + 1] > g->edge_dst[t + 3]) {
                    t += 4;

                    tnode = _mm256_loadu_si256((__m256i *) (g->edge_dst + t));
                    _mm256_permutevar8x32_epi32(tnode, st);
                } else {
                    j += 2;
                    jnode = _mm256_loadu_si256((__m256i *) (g->edge_dst + j));
                }
            }

            _mm256_store_si256((__m256i *) cnv, sse_cnv);

            for (int cnvplus : cnv) {
                g->common_node_num[i] += cnvplus;
            }

            if (j >= jsize) {
                j = jsize;
            } else {
                t = tsize;
            }

            while (j < j2 && t < t2) {
                if (g->edge_dst[j] == g->edge_dst[t]) {
                    g->common_node_num[i]++;
                    j++;
                    t++;
                } else if (g->edge_dst[j] > g->edge_dst[t]) {
                    t++;

                } else {
                    j++;
                }
            }
        }
#endif
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