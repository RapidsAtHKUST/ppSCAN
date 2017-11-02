//
// Created by yche on 8/6/17.
//

#ifndef PPSCAN_DISJOINTSET_H
#define PPSCAN_DISJOINTSET_H

#include <vector>

using ui=unsigned int;
using namespace std;

// disjoint-set related, see CLRS chapter 22 for detail
class DisjointSet {
    void MakeDisjointSetBatch(ui capacity);

public:
    vector<int> parent;
    vector<int> rank;

    explicit DisjointSet(ui capacity);

    int FindRoot(int u);

    void Union(int u, int v);

    bool IsSameSet(int u, int v);
};

#endif //PPSCAN_DISJOINTSET_H
