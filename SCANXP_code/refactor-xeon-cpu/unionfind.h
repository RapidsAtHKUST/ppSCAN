struct UnionFind {
    int *data;

    inline UnionFind(int size) {
        data = new int[size];

#pragma omp parallel for
        for (int i = 0; i < size; i++)data[i] = i;
    }

    bool unionSet(int x, int y) {
        x = root(x);
        y = root(y);
        if (x != y) {
            if (y < x) swap(x, y);
            // data[x] = x;
            data[y] = x;
        }
        return x != y;
    }

    inline bool findSet(int x, int y) {
        return root(x) == root(y);
    }

    inline int root(int x) {
        return (data[x] < 0 || data[x] == x) ? x : data[x] = root(data[x]);
    }

    inline int size(int x) {
        return -data[root(x)];
    }
};
