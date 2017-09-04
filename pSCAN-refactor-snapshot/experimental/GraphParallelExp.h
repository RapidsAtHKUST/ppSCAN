#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <memory>
#include <atomic>
#include <mutex>

#include "../InputOutput.h"
#include "../DisjointSet.h"

using namespace std;

constexpr int NOT_DIRECT_REACHABLE = -2;
constexpr int DIRECT_REACHABLE = -1;
constexpr int NOT_SURE = 0;

namespace yche {
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args &&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}

// GraphExp instance: fast consumption object
class GraphExp {
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
    vector<int> min_cn; //minimum common neighbor: -2 means not similar; -1 means similar; 0 means not sure; > 0 means the minimum common neighbor

    // vertex properties
    vector<int> degree;
    vector<int> similar_degree;

    // clusters: core and non-core(hubs)
    vector<int> cluster_dict;    // observation 2: core vertex clusters are disjoint
    // first: cluster id(min core-vertex id in cluster), second: non-core vertex id
    vector<pair<int, int>> noncore_cluster; // observation 1: clusters may overlap, observation 3: non-core uniquely determined by core

    // disjoint-set: used for core-vertex induced connected components
    unique_ptr<DisjointSet> disjoint_set_ptr;

    ui thread_num_;
private:
    // 1st optimization: cross-link
    // find reverse edge index, e.g, (i,j) index know, compute (j,i) index
    ui BinarySearch(vector<int> &array, ui offset_beg, ui offset_end, int val);

    // 2nd optimization: common-neighbor check pruning, as a pre-processing phase
    int ComputeCnLowerBound(int u, int v);

    void Prune();

    // density-eval related
    int IntersectNeighborSets(int u, int v, int min_cn_num);

    int EvalReachable(int u, ui edge_idx);

    // 1st phase computation: core check and cluster
    bool IsDefiniteCoreVertex(int u);

    void CheckCoreFirstBSP(int u);

    void CheckCoreSecondBSP(int u);

    void ClusterCore(int u);

    // 2nd phase computation
    void MarkClusterMinEleAsId();

    void ClusterNonCores();

public:
    explicit GraphExp(const char *dir_string, const char *eps_s, int min_u, ui thread_num);

    void pSCAN();

    void Output(const char *eps_s, const char *miu);
};

#endif
