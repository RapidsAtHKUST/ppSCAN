//
// Created by yche on 8/4/17.
//

#ifndef PSCAN_INPUTOUTPUT_H
#define PSCAN_INPUTOUTPUT_H

#include <fstream>
#include <vector>
#include <atomic>
#include <unordered_set>

#include "ThreadSafeDisjointSet.h"
#include "Util.h"

using ui=unsigned int;
using namespace std;

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
    vector<int> out_edges;

    // vertex property
    vector<int> degree;

    void ReadGraph();

    pair<int, int> ParseEps(const char *eps_s);

    // possible duplicates in noncore_cluster, sd_lst need to be checked with a cmp
    void Output(const char *eps_s, const char *min_u, vector<pair<int, int>> &noncore_cluster,
                vector<int> &sd_lst, vector<int> &cid, vector<int> &parent);

    void Output(const char *eps_s, const char *min_u, vector<pair<int, int>> &noncore_cluster,
                vector<bool> &is_core_lst, vector<int> &cid, vector<int> &parent);

    // unique: noncore_cluster_set
    void Output(const char *eps_s, const char *min_u, unordered_set<pair<int, int>> &noncore_cluster_set,
                vector<bool> &is_core_lst, vector<int> &cid, vector<int> &parent);

    void Output(const char *eps_s, const char *min_u, vector<pair<int, int>> &noncore_cluster,
                vector<char> &is_core_lst, vector<int> &cid, vector<int> &parent);

    // accept disjoint sets directly
    void Output(const char *eps_s, const char *min_u, vector<pair<int, int>> &noncore_cluster,
                vector<char> &is_core_lst, vector<int> &cid, DisjointSets &disjoint_sets);

    // cid has all core-induced cluster info
    void Output(const char *eps_s, const char *min_u, vector<pair<int, int>> &noncore_cluster,
                vector<bool> &is_core_lst, vector<int> &cid);
};

#endif //PSCAN_INPUTOUTPUT_H