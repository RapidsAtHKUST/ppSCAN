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
    cout << "****scan-xp on knl: " << argv[1] << ", " << argv[2] << ", " << argv[3] << " *** \n";

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
    auto start = high_resolution_clock::now();
    g.MarkClusterMinEleAsId(&uf);
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
    auto end = high_resolution_clock::now();
    cout << "STEP4, prepare results: " << duration_cast<milliseconds>(end - start).count() << " ms\n";

    g.Output(argv[2], argv[3], &uf);
}

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
    __m512i st = _mm512_set_epi32(3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0);
    __m512i ssecountplus = _mm512_set1_epi32(1);

#pragma omp parallel for num_threads(NUMT) schedule(dynamic, 6000)
    for (int i = 0; i < g->edgemax; i++) {
#ifdef NAIVE
        g->common_node_num[i] += compute_cn(g, i);
#else
        int cnv[PARA] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        int to, jo;
        __m512i ssecnv = _mm512_set1_epi32(0);
        int j, t, j2, t2;

        if ((*g).node_off[(*g).edge_src[i] + 1] - (*g).node_off[(*g).edge_src[i]] <
            (*g).node_off[(*g).edge_dst[i] + 1] - (*g).node_off[(*g).edge_dst[i]]) {
            j = (*g).node_off[(*g).edge_src[i]];
            t = (*g).node_off[(*g).edge_dst[i]];
            j2 = (*g).node_off[(*g).edge_src[i] + 1];
            t2 = (*g).node_off[(*g).edge_dst[i] + 1];
        } else {
            t = (*g).node_off[(*g).edge_src[i]];
            j = (*g).node_off[(*g).edge_dst[i]];
            t2 = (*g).node_off[(*g).edge_src[i] + 1];
            j2 = (*g).node_off[(*g).edge_dst[i] + 1];
        }
        // /*
        int size1;
        size1 = (t2 - t) / (j2 - j);

        if (size1 > 2) {

            __m512i jnode = _mm512_set1_epi32((*g).edge_dst[j]);
            __m512i tnode = _mm512_loadu_si512((__m512i *) ((*g).edge_dst + t));

            int vsize = ((t2 - t) / PARA) * PARA + t;

            while (j < j2 && t < vsize) {

                __mmask16 mask = _mm512_cmpeq_epi32_mask(jnode, tnode);
                ssecnv = _mm512_mask_add_epi32(ssecnv, mask, ssecnv, ssecountplus);

                if ((*g).edge_dst[j] > (*g).edge_dst[t + 15]) {
                    t += PARA;
                    tnode = _mm512_loadu_si512((__m512i *) ((*g).edge_dst + t));
                } else {
                    j++;
                    jnode = _mm512_set1_epi32((*g).edge_dst[j]);
                }
            }

            _mm512_store_si512((__m512i *) cnv, ssecnv);

            for (int cnvplus = 0; cnvplus < PARA; cnvplus++) {
                (*g).common_node_num[i] += cnv[cnvplus];
            }


            t = vsize;

        } else {
            int jsize = ((j2 - j) / 4) * 4 + j;
            int tsize = ((t2 - t) / 4) * 4 + t;

            __m512i jnode, tnode;

            jnode = _mm512_loadu_si512((__m512i *) ((*g).edge_dst + j));
            tnode = _mm512_loadu_si512((__m512i *) ((*g).edge_dst + t));

            __mmask16 mask;
            while (j < jsize && t < tsize) {

                __m512i jnodeA = _mm512_permutevar_epi32(st, jnode);
                __m512i tnodeA = _mm512_permute4f128_epi32(tnode, 0b00000000);
                __mmask16 mask = _mm512_cmpeq_epi32_mask(jnodeA, tnodeA);
                ssecnv = _mm512_mask_add_epi32(ssecnv, mask, ssecnv, ssecountplus);

                if ((*g).edge_dst[j + 3] == (*g).edge_dst[t + 3]) {
                    j += 4;
                    t += 4;
                    jnode = _mm512_loadu_si512((__m512i *) ((*g).edge_dst + j));
                    tnode = _mm512_loadu_si512((__m512i *) ((*g).edge_dst + t));
                } else if ((*g).edge_dst[j + 3] > (*g).edge_dst[t + 3]) {
                    t += 4;

                    tnode = _mm512_loadu_si512((__m512i *) ((*g).edge_dst + t));
                    tnodeA = _mm512_permute4f128_epi32(tnode, 0b00000000);
                } else {
                    j += 4;
                    jnode = _mm512_loadu_si512((__m512i *) ((*g).edge_dst + j));
                }

            }

            _mm512_store_si512((__m512i *) cnv, ssecnv);

            for (int cnvplus = 0; cnvplus < PARA; cnvplus++) {
                (*g).common_node_num[i] += cnv[cnvplus];
            }

            if (j >= jsize) {
                j = jsize;
            } else {
                t = tsize;
            }

        }
        // */
        while (j < j2 && t < t2) {

            if ((*g).edge_dst[j] == (*g).edge_dst[t]) {
                (*g).common_node_num[i]++;
                j++;
                t++;
            } else if ((*g).edge_dst[j] > (*g).edge_dst[t]) {
                t++;

            } else {
                j++;

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