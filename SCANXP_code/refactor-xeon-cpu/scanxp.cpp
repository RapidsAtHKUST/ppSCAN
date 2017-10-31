/*
Xeon (AVX2) by http://www.kde.cs.tsukuba.ac.jp/~shihakata
*/

#include "scanxp.h"

#include <chrono>

using namespace std::chrono;

SCAN_XP::SCAN_XP(int thread_num, int min_u, double epsilon, char *dir) : thread_num_(thread_num), min_u_(min_u),
                                                                         epsilon_(epsilon), g(Graph(dir)),
                                                                         uf_ptr(nullptr) {
    cout << "thread num:" << thread_num_ << endl;
    cout << g.nodemax << endl << g.edgemax << endl;
    uf_ptr = new UnionFind(g.nodemax);

    // pre-processing
#pragma omp parallel for num_threads(thread_num_) schedule(dynamic, 100000)
    for (int i = 0u; i < g.nodemax; i++) {
        g.core_count[i] = 0u;
        g.label[i] = UNCLASSIFIED;
        for (int n = g.node_off[i]; n < g.node_off[i + 1]; n++) {
            g.edge_src[n] = i;
            g.common_node_num[n] = 2;
        }
    }
}

SCAN_XP::~SCAN_XP() {
    delete uf_ptr;
}

void SCAN_XP::CheckCore(Graph *g) {
#pragma omp parallel for num_threads(thread_num_) schedule(dynamic, 6000)
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

#pragma omp parallel for num_threads(thread_num_)
    for (int i = 0u; i < g->edgemax; i++) {
        long double du = g->node_off[g->edge_src[i] + 1] - g->node_off[g->edge_src[i]] + 1;
        long double dv = g->node_off[(g->edge_dst[i] + 1)] - g->node_off[g->edge_dst[i]] + 1;
        g->sim_values[i] = static_cast<double>((long double) g->common_node_num[i] / sqrt(du * dv));
    }

#pragma omp parallel for num_threads(thread_num_)
    for (int i = 0u; i < g->edgemax; i++) {
        if (g->sim_values[i] >= epsilon_) {
            g->core_count[g->edge_src[i]]++;
            g->similarity[i] = true;
        } else {
            g->similarity[i] = false;
        }
    }

#pragma omp parallel for num_threads(thread_num_)
    for (int i = 0u; i < g->nodemax; i++) {
        if (g->core_count[i] >= min_u_) {
            g->label[i] = CORE;
        };
    }
}

void SCAN_XP::ClusterCore() {
#pragma omp parallel for num_threads(thread_num_) schedule(dynamic, 2000)
    for (int i = 0u; i < g.nodemax; i++) {
        if (g.label[i] == CORE) {
            for (auto edge_idx = g.node_off[i]; edge_idx < g.node_off[i + 1]; edge_idx++) {
                // yche: fix bug, only union when g.edge_dst[edge_idx] is a core vertex
                if (g.label[g.edge_dst[edge_idx]] == CORE && g.similarity[edge_idx]) {
                    uf_ptr->UnionThreadSafe(i, g.edge_dst[edge_idx]);
                }
            }
        }
    }
}

bool SCAN_XP::CheckHub(Graph *g, UnionFind *uf, int a) {
    set<int> c;

    for (int i = g->node_off[a]; i < g->node_off[a + 1]; i++) {
        if (g->label[g->edge_dst[i]] != CORE)continue;
        c.insert((*uf).FindRoot(g->edge_dst[i]));
    }
    return c.size() >= 2;
}

void SCAN_XP::LabelNonCore() {
    int core_num = 0u;
#pragma omp parallel for num_threads(thread_num_) schedule(dynamic, 1000), reduction(+:core_num)
    for (int i = 0u; i < g.nodemax; i++) {
        if (g.label[i] == CORE) {
            core_num++;
            continue;
        }

        if (CheckHub(&g, uf_ptr, i)) {
            g.label[i] = HUB;
        }
    }
    cout << "Core:" << core_num << endl;
}

void SCAN_XP::PostProcess() {
    int cluster_num;
    int hub_num = 0u;
    int out_num = 0u;

    set<int> c;
    for (int i = 0u; i < g.nodemax; i++) {
        if (g.label[i] == CORE) {
            c.emplace(uf_ptr->FindRoot(i));
        }
    }
    cluster_num = static_cast<int>(c.size());

#pragma omp parallel for num_threads(thread_num_) reduction(+:hub_num, out_num)
    for (int i = 0u; i < g.nodemax; i++) {
        if (g.label[i] == HUB) {
            hub_num++;
        } else if (g.label[i] == UNCLASSIFIED) {
            out_num++;
        }
    }
    cout << "Detect_cluster: " << cluster_num << endl << "Hub: " << hub_num << endl
         << "Outlier: " << out_num << endl;
}

void SCAN_XP::PrepareResultOutput() {
    // prepare output
    auto start = high_resolution_clock::now();
    g.MarkClusterMinEleAsId(uf_ptr);
    for (auto i = 0u; i < g.nodemax; i++) {
        if (g.label[i] == CORE) {
            for (auto j = g.node_off[i]; j < g.node_off[i + 1]; j++) {
                auto v = g.edge_dst[j];
                if (g.label[v] != CORE && g.similarity[j]) {
                    g.noncore_cluster.emplace_back(g.cluster_dict[uf_ptr->FindRoot(i)], v);
                }
            }
        }
    }
    auto end = high_resolution_clock::now();
    cout << "STEP4, prepare results: " << duration_cast<milliseconds>(end - start).count() << " ms\n";

    auto epsilon_str = to_string(epsilon_);
    epsilon_str.erase(epsilon_str.find_last_not_of("0u") + 1);
    g.Output(epsilon_str.c_str(), to_string(min_u_).c_str(), uf_ptr);
}

void SCAN_XP::Execute() {
    //step1 CheckCore
    double s1 = omp_get_wtime();
    CheckCore(&g);
    double e1 = omp_get_wtime();
    cout << "STEP1, CheckCore:" << e1 - s1 << endl;

    //step2 ClusterCore
    double s2 = omp_get_wtime();
    ClusterCore();
    double e2 = omp_get_wtime();
    cout << "STEP2, ClusterCore:" << e2 - s2 << endl;

    //step3 LabelNonCore
    double s3 = omp_get_wtime();
    LabelNonCore();
    double e3 = omp_get_wtime();
    cout << "STEP3, LabelNonCore:" << e3 - s3 << endl;

    // post-processing, prepare result and output
    PostProcess();
    PrepareResultOutput();
}