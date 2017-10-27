#ifndef PPSCAN_UNION_FIND_H
#define PPSCAN_UNION_FIND_H

struct UnionFind {
    int *parent;

    inline UnionFind(int size) {
        parent = new int[size];

#pragma omp parallel for
        for (int i = 0; i < size; i++)parent[i] = i;
    }

//    bool unionSet(int x, int y) {
//        x = root(x);
//        y = root(y);
//        if (x != y) {
//            if (y < x) swap(x, y);
//            // parent[x] = x;
//            parent[y] = x;
//        }
//        return x != y;
//    }

    inline bool findSet(int x, int y) {
        return root(x) == root(y);
    }

    inline int root(int x) {
        return (parent[x] < 0 || parent[x] == x) ? x : parent[x] = root(parent[x]);
    }

    inline int size(int x) {
        return -parent[root(x)];
    }
};

#endif
