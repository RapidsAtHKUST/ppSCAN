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
            for (auto edge_idx = g.node_off[i]; edge_idx < g.node_off[i + 1]; edge_idx++) {
                // yche: fix bug, only union when g.edge_dst[edge_idx] is a core vertex
                if (g.label[g.edge_dst[edge_idx]] != CORE)continue;
                if (!g.similarity[edge_idx])continue;
                uf.UnionThreadSafe(i, g.edge_dst[edge_idx]);
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
            c.emplace(uf.FindRoot(i));
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
                    g.noncore_cluster.emplace_back(g.cluster_dict[uf.FindRoot(i)], v);
                }
            }
        }
    }
    auto end = high_resolution_clock::now();
    cout << "STEP4, prepare results: " << duration_cast<milliseconds>(end - start).count() << " ms\n";

    g.Output(argv[2], argv[3], &uf);
}

void core_detection(Graph *g) {
#pragma omp parallel for num_threads(NUMT) schedule(dynamic, 6000)
    for (auto i = 0u; i < g->edgemax; i++) {
#if defined(NAIVE)
        g->common_node_num[i] += ComputeCNNaive(g, i);
#elif defined(GALLOPING_SINGLE)
        g->common_node_num[i] += ComputeCNGallopingSingleDir(g, i);
#elif defined(GALLOPING_DOUBLE)
        g->common_node_num[i] += ComputeCNGallopingDoubleDir(g, i);
#else
        g->common_node_num[i] += ComputeCNAVX2(g, i);
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

bool hub_check_uf(Graph *g, UnionFind *uf, int a) {
    set<int> c;

    for (int i = g->node_off[a]; i < g->node_off[a + 1]; i++) {
        if (g->label[g->edge_dst[i]] != CORE)continue;
        c.insert((*uf).FindRoot(g->edge_dst[i]));
    }
    return c.size() >= 2;
}