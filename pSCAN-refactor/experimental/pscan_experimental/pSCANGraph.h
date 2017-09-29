#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <memory>

#include "Utility.h"
#include "../../DisjointSet.h"

using namespace std;

namespace yche {
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args &&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}

class pSCANGraph {
private:
    string dir;
    ui n, m;

    int eps_a2, eps_b2, miu; // eps_a2/eps_b2 = eps^2

    ui *pstart; //offset of neighbors of nodes
    int *edges; //adjacent ids of edges

    ui *reverse_link; //the position of reverse edge in edges
    int *min_cn; //minimum common neighbor: -2 means not similar; -1 means similar; 0 means not sure; > 0 means the minimum common neighbor

    unique_ptr<DisjointSet> disjoint_set;

    int *cid; //cluster id

    int *degree;
    int *similar_degree; //number of adjacent edges with similarity no less than epsilon
    int *effective_degree; //number of adjacent edges not pruned by similarity

    vector<pair<int, int>> noncore_cluster;

    // statistics
#ifdef STATISTICS
    long all_cmp0 = 0;
    long all_cmp1 = 0;
    long all_cmp2 = 0;
    long intersection_times = 0;
    int portion = 0;
#endif
public:
    explicit pSCANGraph(const char *_dir);

    ~pSCANGraph();

    void ReadGraph();

    void pSCAN(const char *eps_s, int miu);

    //eps_s and miu are the parameters (epsilon, miu) for the SCAN algorithm
    void ClusterNonCoreVertices(int eps_a2, int eps_b2, int mu);

    void Output(const char *eps_s, const char *miu);

private:
    ui BinarySearch(const int *array, ui b, ui e, int val);

    int similar_check_OP(int u, ui idx, int eps_a, int eps_b);

    int EvalSimilarity(int u, int v, int c);

    int ComputeMinCommonNeighbor(int u, int v, int eps_a2, int eps_b2);

    void PruneCrossLink(int eps_a2, int eps_b2, int miu, int *cores, int &cores_e);

    void GetEps(const char *eps_s);
};

#endif
