#ifndef _GraphParallelExp_H_
#define _GraphParallelExp_H_

#include <memory>
#include <future>

#include "../InputOutput.h"
#include "../Util.h"

using namespace std;

constexpr int NOT_SIMILAR = -2;
constexpr int SIMILAR = -1;

namespace yche {
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args &&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}

// GraphParallelExp instance: fast consumption object
class GraphParallelExp {
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
    vector<char> is_core_lst;
    vector<char> is_non_core_lst;

    // clusters: core and non-core(hubs)
    vector<int> cluster_dict;    // observation 2: core vertex clusters are disjoint

    // first: cluster id(min core-vertex id in cluster), second: non-core vertex id
    vector<pair<int, int>> noncore_cluster; // observation 1: clusters may overlap, observation 3: non-core uniquely determined by core

    // disjoint-set: used for core-vertex induced connected components
    unique_ptr<DisjointSets> disjoint_set_ptr;

    vector<int> cores;

    ui thread_num_;
private:
    // easy-computation pruning optimization: common-neighbor check pruning, as a pre-processing phase
    int ComputeCnLowerBound(int u, int v);

    int IntersectNeighborSets(int u, int v, int min_cn_num);

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

    void ClusterNonCoreFirstPhase(int u);

private:
    // computation stages
    void pSCANFirstPhasePrune();

    void pSCANSecondPhaseCheckCore();

    void pSCANThirdPhaseClusterCore();

    void MarkClusterMinEleAsId();

    void pSCANFourthPhaseClusterNonCore();

public:
    explicit GraphParallelExp(const char *dir_string, const char *eps_s, int min_u, ui thread_num);

    void pSCAN();

    void Output(const char *eps_s, const char *miu);

    virtual ~GraphParallelExp();
};

#endif
