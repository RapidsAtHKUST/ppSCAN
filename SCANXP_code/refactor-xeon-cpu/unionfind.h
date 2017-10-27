#ifndef PPSCAN_UNION_FIND_H
#define PPSCAN_UNION_FIND_H

struct UnionFind {
    int *parent;

    inline UnionFind(int size) {
        parent = new int[size];

#pragma omp parallel for
        for (int i = 0; i < size; i++)parent[i] = i;
    }

    inline int root(int x) {
        return (parent[x] < 0 || parent[x] == x) ? x : parent[x] = root(parent[x]);
    }
};

#endif
