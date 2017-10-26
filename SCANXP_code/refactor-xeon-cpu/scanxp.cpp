/*
Xeon (AVX2)
*/

#include "scanxp.h"

int main(int argc, char *argv[]) {
    //Graph_input
    int cluster_num = 0;
    int hub_num = 0;
    int out_num = 0;

    NUMT = atoi(argv[2]);
    GRAPH g(argv[1]);
    UnionFind uf(g.nodemax);
    cout << "graph_name" << argv[1] << endl;


#pragma omp parallel for num_threads(NUMT) schedule(dynamic, 100000)
    for (int i = 0; i < g.nodemax; i++) {
        g.core_count[i] = 0;
        g.label[i] = UNCLASSIFIED;
        g.core[i] = false;
        for (int n = g.node[i]; n < g.node[i + 1]; n++) {
            g.edgef[n] = i;
            g.common_node[n] = 2;
        }
    }


    //step1 core_detection
    cout << "STEP1" << endl;
    double s1 = omp_get_wtime();
    core_detection(&g);
    double e1 = omp_get_wtime();

//step2 cluster_construction
    /*
      This step constructs cluster using Union-Find tree.
      Each threads detects that core and adjacent nodes constructs cluster.
      Clusters are expand by merging other cluster.
     */
    cout << "STEP2" << endl;
    double s2 = omp_get_wtime();
#pragma omp parallel for num_threads(NUMT) schedule(dynamic, 2000)
    for (int i = 0; i < g.nodemax; i++) {

        int rx, ry, index;
        if (g.core[i]) {
            g.label[i] = CMEMBER;
            for (int n = g.node[i]; n < g.node[i + 1]; n++) {
                if (!g.similarity[n])continue;
                g.label[g.edge[n]] = CMEMBER;

                do {
                    rx = uf.root(i);
                    ry = uf.root(g.edge[n]);
                    if (rx < ry) {
                        index = rx;
                        rx = ry;
                        ry = index;
                    }

                    if (rx == ry)break;

                } while (!__sync_bool_compare_and_swap(&(uf.data[rx]), rx, ry));
            }

        }
    }


    double e2 = omp_get_wtime();
    cout << "STEP3" << endl;

    double s3 = omp_get_wtime();

    //STEP3 Hub Outlier detection
#pragma omp parallel for num_threads(NUMT) schedule(dynamic, 1000)
    for (int i = 0; i < g.nodemax; i++) {

        if (g.label[i] == CMEMBER)continue;

        if (hub_check_uf(&g, &uf, i)) {
            g.label[i] = HUB;
        }
    }


    double e3 = omp_get_wtime();


    //Result output
    set<int> c;
    for (int i = 0; i < g.nodemax; i++) {
        if (g.label[i] == CMEMBER) {
            c.insert(uf.root(i));

        }
    }

    cluster_num = c.size();

#pragma omp parallel for num_threads(NUMT) reduction(+:hub_num, out_num)
    for (int i = 0; i < g.nodemax; i++) {
        if (g.label[i] == HUB) {
            hub_num++;
        } else if (g.label[i] == UNCLASSIFIED) {
            out_num++;
        }
    }

    /*
    ofstream ofresult("result.txt",ios::app);
    for(int i = 0; i< g.nodemax; i++){
      if(g.label[i] == CMEMBER){
	ofresult << i << "\t" << uf.root(i) << endl;
      }else{
	ofresult << i << endl;
      }
    }
    // */

    cout << "Detect_cluster: " << cluster_num << endl;
    cout << "Hub: " << hub_num << endl;
    cout << "Outlier: " << out_num << endl;
    cout << "STEP1_core_detection:" << (double) (e1 - s1) << endl;
    cout << "STEP2_cluster_construction:" << (double) (e2 - s2) << endl;
    cout << "STEP3_hub_and_outlier_detection:" << (double) (e3 - s3) << endl;
    cout << "Number_of_threads" << NUMT << endl;
    cout << g.nodemax << endl;
    cout << g.edgemax << endl;


}

/*
Step1 'core_detection' detects all core nodes in graph.
Firstly, this method calclates set intersection of all edges.
Next, this method calclates stractural similarity based on result of set intersection.
Finaly, this method determines whether all nodes are core or not.
*/
inline void core_detection(GRAPH *g) {

    int countplus[PARA] = {1, 1, 1, 1, 1, 1, 1, 1};
    __m256i ssecountplus = _mm256_load_si256((__m256i *) (countplus));
    __m256i sj = _mm256_set_epi32(1, 1, 1, 1, 0, 0, 0, 0);
    __m256i st = _mm256_set_epi32(3, 2, 1, 0, 3, 2, 1, 0);

#pragma omp parallel for num_threads(NUMT) schedule(dynamic, 6000)
    for (int i = 0; i < (*g).edgemax; i++) {
        int cnv[PARA] = {0, 0, 0, 0, 0, 0, 0, 0};
        int to, jo;

        __m256i ssecnv = _mm256_load_si256((__m256i *) (cnv));
        int j, t, j2, t2;

        if ((*g).node[(*g).edgef[i] + 1] - (*g).node[(*g).edgef[i]] <
            (*g).node[(*g).edge[i] + 1] - (*g).node[(*g).edge[i]]) {
            j = (*g).node[(*g).edgef[i]];
            t = (*g).node[(*g).edge[i]];
            j2 = (*g).node[(*g).edgef[i] + 1];
            t2 = (*g).node[(*g).edge[i] + 1];
        } else {
            t = (*g).node[(*g).edgef[i]];
            j = (*g).node[(*g).edge[i]];
            t2 = (*g).node[(*g).edgef[i] + 1];
            j2 = (*g).node[(*g).edge[i] + 1];
        }

        int size1;
        size1 = (t2 - t) / (j2 - j);

        if (size1 > 2) {

            __m256i jnode = _mm256_set1_epi32((*g).edge[j]);
            __m256i tnode = _mm256_loadu_si256((__m256i *) ((*g).edge + t));
            __m256i ssecnv = _mm256_load_si256((__m256i *) (cnv));

            int vsize = ((t2 - t) / PARA) * PARA;
            int to = t;

            while (j < j2 && t < to + vsize) {

                __m256i mask = _mm256_cmpeq_epi32(jnode, tnode);
                mask = _mm256_and_si256(ssecountplus, mask);
                ssecnv = _mm256_add_epi32(ssecnv, mask);


                if ((*g).edge[j] > (*g).edge[t + 7]) {
                    t += PARA;
                    tnode = _mm256_loadu_si256((__m256i *) ((*g).edge + t));


                } else {
                    j++;
                    jnode = _mm256_set1_epi32((*g).edge[j]);
                }


            }
            _mm256_store_si256((__m256i *) cnv, ssecnv);

            for (int cnvplus = 0; cnvplus < PARA; cnvplus++) {
                (*g).common_node[i] += cnv[cnvplus];
            }


            t = (to + vsize);

            while (j < j2 && t < t2) {

                if ((*g).edge[j] == (*g).edge[t]) {
                    (*g).common_node[i]++;
                    j++;
                    t++;
                } else if ((*g).edge[j] > (*g).edge[t]) {
                    t++;
                } else {
                    j++;
                }
            }


        } else {

            if (t2 - t < j2 - j) {
                j = (*g).node[(*g).edge[i]];
                t = (*g).node[(*g).edgef[i]];
                j2 = (*g).node[(*g).edge[i] + 1];
                t2 = (*g).node[(*g).edgef[i] + 1];
            }
            int jsize = ((j2 - j) / 2) * 2 + j;
            int tsize = ((t2 - t) / 4) * 4 + t;


            __m256i jnode, tnode;


            jnode = _mm256_loadu_si256((__m256i *) ((*g).edge + j));
            tnode = _mm256_loadu_si256((__m256i *) ((*g).edge + t));

            __mmask16 mask;
            while (j < jsize && t < tsize) {

                __m256i jnodeA = _mm256_permutevar8x32_epi32(jnode, sj);
                __m256i tnodeA = _mm256_permutevar8x32_epi32(tnode, st);
                __m256i mask = _mm256_cmpeq_epi32(jnodeA, tnodeA);
                mask = _mm256_and_si256(ssecountplus, mask);
                ssecnv = _mm256_add_epi32(ssecnv, mask);

                if ((*g).edge[j + 1] == (*g).edge[t + 3]) {
                    j += 2;
                    t += 4;
                    jnode = _mm256_loadu_si256((__m256i *) ((*g).edge + j));
                    tnode = _mm256_loadu_si256((__m256i *) ((*g).edge + t));
                } else if ((*g).edge[j + 1] > (*g).edge[t + 3]) {
                    t += 4;

                    tnode = _mm256_loadu_si256((__m256i *) ((*g).edge + t));
                    tnodeA = _mm256_permutevar8x32_epi32(tnode, st);
                } else {
                    j += 2;
                    jnode = _mm256_loadu_si256((__m256i *) ((*g).edge + j));
                }


            }

            _mm256_store_si256((__m256i *) cnv, ssecnv);

            for (int cnvplus = 0; cnvplus < PARA; cnvplus++) {
                (*g).common_node[i] += cnv[cnvplus];
            }


            if (j >= jsize) {
                j = jsize;
            } else {
                t = tsize;
            }

            while (j < j2 && t < t2) {

                if ((*g).edge[j] == (*g).edge[t]) {
                    (*g).common_node[i]++;
                    j++;
                    t++;
                } else if ((*g).edge[j] > (*g).edge[t]) {
                    t++;

                } else {
                    j++;

                }
            }
        }

    }


#pragma omp parallel for num_threads(NUMT)
    for (int i = 0; i < (*g).edgemax; i++) {

        (*g).ss[i] = (*g).common_node[i] / sqrt((((*g).node[(*g).edgef[i] + 1] - (*g).node[(*g).edgef[i]]) + 1) *
                                                (((*g).node[((*g).edge[i] + 1)] - (*g).node[(*g).edge[i]]) + 1));
    }

#pragma omp parallel for num_threads(NUMT)
    for (int i = 0; i < (*g).edgemax; i++) {

        if ((*g).ss[i] >= EPSILON) {
            (*g).core_count[(*g).edgef[i]]++;
            (*g).similarity[i] = true;
        } else {
            (*g).similarity[i] = false;
        }

    }


#pragma omp parallel for num_threads(NUMT)
    for (int i = 0; i < (*g).nodemax; i++) {
        (*g).core[i] = (*g).core_count[i] >= MYU;
    }
}

inline bool hub_check_uf(GRAPH *g, UnionFind *uf, int a) {
    int counter = 0;
    set<int> c;
    bool q;

    for (int i = (*g).node[a]; i < (*g).node[a + 1]; i++) {

        if ((*g).label[(*g).edge[i]] != CMEMBER)continue;
        c.insert((*uf).root((*g).edge[i]));
    }

    q = c.size() >= 2;

    return q;
}
