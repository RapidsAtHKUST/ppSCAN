#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <memory>

#include "InputOutput.h"

using namespace std;

#define NOT_SIMILAR -2
#define SIMILAR -1
#define NOT_SURE  0

// Graph instance: fast consumption object
class Graph {
private:
    unique_ptr<InputOutput> io_helper_ptr;

    int eps_a2, eps_b2, miu; // eps_a2/eps_b2 = eps^2

    ui n;
    ui *pstart; //offset of neighbors of nodes
    ui *reverse; //the position of reverse edge in edges
    int *edges;

    int *min_cn; //minimum common neighbor: -2 means not similar; -1 means similar; 0 means not sure; > 0 means the minimum common neighbor

    int *degree;
    vector<int> similar_degree;
    vector<int> effective_degree;

    vector<int> parent;
    vector<int> rank;

    vector<int> cid;
    vector<pair<int, int>> noncore_cluster;

public:
    explicit Graph(const char *dir_string, const char *eps_s, int miu);

    ~Graph();

    void pSCAN();


    void output(const char *eps_s, const char *miu);

private:
    // disjoint-set related, see CLRS chapter 22 for detail
    void make_disjoint_set_batch();

    int find_root(int u);

    void union_if_not_same_set(int u, int v);

    // 1st phase core implementation: prune_and_cross_link related, for cores clustering:
    ui binary_search(const int *array, ui b, ui e, int val);

    int compute_common_neighbor_lowerbound(int u, int v);

    void prune_and_cross_link(vector<int> &cores, int &cores_e);

    // 2nd phase core implementation:  non-core vertices clustering related:
    int check_common_neighbor(int u, int v, int c);

    int similar_check_OP(int u, ui idx);

    void cluster_noncore_vertices();

};

#endif
