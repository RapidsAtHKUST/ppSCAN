#ifndef PPSCAN_UNION_FIND_H
#define PPSCAN_UNION_FIND_H

struct UnionFind {
    int *parent;

    explicit UnionFind(int size) {
        parent = new int[size];

#pragma omp parallel for
        for (int i = 0; i < size; i++)parent[i] = i;
    }

    ~UnionFind() {
        delete[] parent;
    }

    int FindRoot(int x) {
        return (parent[x] < 0 || parent[x] == x) ? x : parent[x] = FindRoot(parent[x]);
    }

    void UnionThreadSafe(int u, int v) {
        int rx, ry;
        do {
            rx = FindRoot(u);
            ry = FindRoot(v);
            int index;
            if (rx < ry) {
                index = rx;
                rx = ry;
                ry = index;
            }
            if (rx == ry)break;
        } while (!__sync_bool_compare_and_swap(&(parent[rx]), rx, ry));
    }
};

#endif
