class DisjointSet:
    def __init__(self, capacity):
        self.parent = range(capacity)
        self.rank = [0] * capacity

    def find_root(self, u):
        x = u
        while self.parent[x] != x:
            x = self.parent[x]

        # heuristics: path compression
        while self.parent[u] != x:
            tmp = self.parent[u]
            self.parent[u] = x
            u = tmp
        return x

    def union(self, u, v):
        ru = self.find_root(u)
        rv = self.find_root(v)
        if ru == rv:
            return

        # heuristics: union by rank
        if self.rank[ru] < self.rank[rv]:
            self.parent[ru] = rv
        elif self.rank[ru] > self.rank[rv]:
            self.parent[rv] = ru
        else:
            self.parent[rv] = ru
            self.rank[ru] += 1
