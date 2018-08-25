//
// Created by yche on 8/4/17.
//

#ifndef PPSCAN_INPUTOUTPUT_H
#define PPSCAN_INPUTOUTPUT_H

#include <fstream>
#include <vector>
#include <atomic>
#include <unordered_set>
#include <hbw_allocator.h>

#include "ThreadSafeDisjointSet.h"
#include "Util.h"
#include "DisjointSet.h"

#ifdef KNL
#include <hbw_allocator.h>
#endif

using ui=unsigned int;
using namespace std;

namespace anySCAN {
    constexpr char UN_TOUCHED = 0;

    // eps-neighborhood unknown
    constexpr char UNPROCESSED_CORE = 1;
    // eps-neighborhood known
    constexpr char PROCESSED_CORE = 2;

    constexpr char UNPROCESSED_BORDER = 3;
    constexpr char PROCESSED_BORDER = 4;
    constexpr char UNPROCESSED_NOISE = 5;
    constexpr char PROCESSED_NOISE = 6;
}

class InputOutput {
private:
    string dir;

    void ReadDegree();

    void CheckInputGraph();

    void ReadAdjacencyList();

public:
    explicit InputOutput(const string &dir);

    ui n;
    ui m;

    // csr
    vector<ui> offset_out_edges;

#ifdef KNL
    vector<int, hbw::allocator<int>> out_edges;
#else
    vector<int> out_edges;
#endif

    // vertex property
    vector<int> degree;

    void ReadGraph();

    pair<int, int> ParseEps(const char *eps_s);

    // possible duplicates in noncore_cluster, sd_lst need to be checked with a cmp
    void Output(const char *eps_s, const char *min_u, vector<pair<int, int>> &noncore_cluster,
                vector<int> &sd_lst, vector<int> &cid, vector<int> &parent);

    void Output(const char *eps_s, const char *min_u, vector<pair<int, int>> &noncore_cluster,
                vector<bool> &is_core_lst, vector<int> &cid, vector<int> &parent);

    // anySCAN
    void OutputAnySCAN(const char *eps_s, const char *min_u, vector<pair<int, int>> &noncore_cluster,
                       vector<char> &core_status, vector<int> &cid, vector<int> &parent);

    // unique: noncore_cluster_set
    void Output(const char *eps_s, const char *min_u, unordered_set<pair<int, int>> &noncore_cluster_set,
                vector<bool> &is_core_lst, vector<int> &cid, vector<int> &parent);

    void Output(const char *eps_s, const char *min_u, vector<pair<int, int>> &noncore_cluster,
                vector<char> &is_core_lst, vector<int> &cid, vector<int> &parent);


    // accept disjoint sets directly
    void Output(const char *eps_s, const char *min_u, vector<pair<int, int>> &noncore_cluster,
                vector<char> &is_core_lst, vector<int> &cid, DisjointSets &disjoint_sets);

    void Output(const char *eps_s, const char *min_u, vector<pair<int, int>> &noncore_cluster,
                vector<char> &is_core_lst, int *cid, DisjointSets &disjoint_sets);

    // cid has all core-induced cluster info
    void Output(const char *eps_s, const char *min_u, vector<pair<int, int>> &noncore_cluster,
                vector<bool> &is_core_lst, vector<int> &cid);
};

#endif //PPSCAN_INPUTOUTPUT_H