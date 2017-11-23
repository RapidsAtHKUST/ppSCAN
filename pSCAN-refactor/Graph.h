#ifndef PPSCAN_GRAPH_H_
#define PPSCAN_GRAPH_H_

#include <memory>
#include <future>

#include "InputOutput.h"
#include "Util.h"

using namespace std;

// Graph instance: fast consumption object
class Graph {
private:
    unique_ptr<InputOutput> io_helper_ptr;
    // parameter1: e.g eps: 0.13, eps_a:13, eps_b:100;
    // parameter2: min_u: 5, 5 nearest neighbor as threshold
    int eps_a2, eps_b2, min_u;

    // compressed spare row graph
    ui n;
    vector<ui> out_edge_start;
    vector<int> out_edges;

    // edge properties
    int *min_cn; //minimum common neighbor: -2 means not similar; -1 means similar; 0 means not sure; > 0 means the minimum common neighbor

    // vertex properties
    vector<int> degree;
    vector<char> core_status_lst;

    // clusters: core and non-core(hubs)
    int *cluster_dict;    // observation 2: core vertex clusters are disjoint

    // first: cluster id(min core-vertex id in cluster), second: non-core vertex id
    vector<pair<int, int>> noncore_cluster; // observation 1: clusters may overlap, observation 3: non-core uniquely determined by core

    // disjoint-set: used for core-vertex induced connected components
    unique_ptr<DisjointSets> disjoint_set_ptr;

    vector<int> cores;

private:
    // easy-computation pruning optimization: common-neighbor check pruning, as a pre-processing phase
    int ComputeCnLowerBound(int u, int v);

    int IntersectNeighborSets(int u, int v, int min_cn_num);

    int IntersectNeighborSetsSSE(int u, int v, int min_cn_num);

#if defined(ENABLE_AVX2)
    int IntersectNeighborSetsAVX2(int u, int v, int min_cn_num);
#endif

#if defined(ENABLE_AVX2_MERGE)
    int IntersectNeighborSetsAVX2MergePopCnt(int u, int v, int min_cn_num);
#endif

#if defined(ENABLE_AVX512)
    int IntersectNeighborSetsAVX512(int u, int v, int min_cn_num);
#endif

#if defined(ENABLE_AVX512_NO_DU_DV)
    int IntersectNeighborSetsAVX512NoDuDv(int u, int v, int min_cn_num);
#endif

#if defined(ENABLE_AVX512_MERGE)
    int IntersectNeighborSetsAVX512MergePopCnt(int u, int v, int min_cn_num);
#endif

    int EvalSimilarity(int u, ui edge_idx);

    // avoiding redundant computation optimization: find reverse edge index, e.g, (i,j) index know, compute (j,i) index
    ui BinarySearch(vector<int> &array, ui offset_beg, ui offset_end, int val);

    bool IsDefiniteCoreVertex(int u);

private:
    // vertex computations in each phase
    void PruneDetail(int u);

    void CheckCoreFirstBSP(int u);

    void CheckCoreSecondBSP(int u);

    void ClusterCoreFirstPhase(int u);

    void ClusterCoreSecondPhase(int u);

    void ClusterNonCoreDetail(int u, vector<pair<int, int>> &tmp_cluster);

private:
    // computation stages
    void pSCANFirstPhasePrune();

    void pSCANSecondPhaseCheckCore();

    void pSCANThirdPhaseClusterCore();

    void MarkClusterMinEleAsId();

    void pSCANFourthPhaseClusterNonCore();

public:
    explicit Graph(const char *dir_string, const char *eps_s, int min_u);

    void pSCAN();

    void Output(const char *eps_s, const char *miu);

    virtual ~Graph();
};

#endif
