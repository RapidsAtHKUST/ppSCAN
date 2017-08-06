//
// Created by yche on 8/6/17.
//

#include "DisjointSet.h"

void DisjointSet::MakeDisjointSetBatch(ui capacity) {
    parent = vector<int>(capacity);
    rank = vector<int>(capacity);
    for (ui i = 0; i < capacity; i++) { parent[i] = i; }
    std::fill(rank.begin(), rank.end(), 0);
}

DisjointSet::DisjointSet(ui capacity) {
    MakeDisjointSetBatch(capacity);
}

int DisjointSet::FindRoot(int u) {
    int x = u;
    while (parent[x] != x) { x = parent[x]; }

    while (parent[u] != x) {
        // heuristic: path compression
        int tmp = parent[u];
        parent[u] = x;
        u = tmp;
    }
    return x;
}

bool DisjointSet::IsSameSet(int u, int v) {
    return FindRoot(u) == FindRoot(v);
}

void DisjointSet::Union(int u, int v) {
    int ru = FindRoot(u);
    int rv = FindRoot(v);

    if (ru == rv) { return; }

    // heuristic: union by rank
    // Link: ru and rv
    if (rank[ru] < rank[rv]) {
        parent[ru] = rv;
    } else if (rank[ru] > rank[rv]) {
        parent[rv] = ru;
    } else {
        parent[rv] = ru;
        ++rank[ru];
    }
}


