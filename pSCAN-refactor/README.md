# pSCAN
pSCAN: Fast and Exact Structural Graph Clustering

# Compile
make

# Usage
./pscan graph_directory epsilon mu Output[optional]

# Note
If you are using this code, please kindly cite the following paper.

Lijun Chang, Wei Li, Xuemin Lin, Lu Qin, Wenjie Zhang,
"pSCAN: Fast and Exact Structural Graph Clustering"
Proceedings of the 31st International Conference on Data Engineering (ICDE’16), 2016

# Others(Changes)

```cpp
void Graph::ClusterNonCores() {
    noncore_cluster = vector<pair<int, int>>();
    noncore_cluster.reserve(n);

    MarkClusterMinEleAsId();
    for (auto i = 0; i < n; i++) {
        if (IsDefiniteCoreVertex(i)) {
            for (auto j = out_edge_start[i]; j < out_edge_start[i + 1]; j++) {
                // observation 3: check non-core neighbors
                if (!IsDefiniteCoreVertex(out_edges[j])) {
                    if (min_cn[j] >= 0) {
                        min_cn[j] = EvalDensity(i, j);

// it seems to be useless
//                        UpdateViaCrossLink(j);
//                        if (min_cn[j] == SIMILAR) {
//                            ++similar_degree[i];
//                            ++similar_degree[out_edges[j]];
//                        }
                    }
                    if (min_cn[j] == SIMILAR) {
                        // root must be parent since already disjoint_set_ptr->FindRoot(i) previously
                        noncore_cluster.emplace_back(cluster_dict[disjoint_set_ptr->parent[i]], out_edges[j]);
                    }
                }
            }
        }
    }
}
```