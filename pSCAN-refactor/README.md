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

```cpp
void Graph::PruneAndCrossLink() {
    auto start = high_resolution_clock::now();

    //must iterate from 0 to n-1
    for (auto i = 0; i < n; i++) {
        for (auto j = out_edge_start[i]; j < out_edge_start[i + 1]; j++) {
            auto v = out_edges[j];

// it seems to be useless
//            if (v < i) {
//                // edge should already be explored
//                if (min_cn[j] == NOT_SURE) { min_cn[j] = NOT_SIMILAR; }
//            }
//            else {
            if (i <= v) {
                // use pruning rule
                int a = degree[i], b = degree[v];
                if (a > b) { swap(a, b); }
                if (((long long) a) * eps_b2 < ((long long) b) * eps_a2) {
                    // can be pruned
                    min_cn[j] = NOT_SIMILAR;
                    --effective_degree[i];
                    --effective_degree[v];
                } else {
                    // cannot be pruned
                    int c = ComputeCnLowerBound(a, b);
                    if (c <= 2) {
                        min_cn[j] = SIMILAR;
                        ++similar_degree[i];
                        if (IsDefiniteCoreVertex(i)) { cores.emplace_back(i); }
                        ++similar_degree[v];
                        if (IsDefiniteCoreVertex(v)) { cores.emplace_back(v); }
                    } else {
                        min_cn[j] = c;
                    }
                }

                // find edge, in order to build cross link
//                if (min_cn[j] != NOT_SIMILAR) {
                auto r_id = BinarySearch(out_edges, out_edge_start[v], out_edge_start[v + 1], i);
                InitCrossLink(j, r_id);
                UpdateViaCrossLink(j);
//                }
            }
        }
    }

    auto end1 = high_resolution_clock::now();
    cout << "prune and cross link execution time:" << duration_cast<milliseconds>(end1 - start).count() << " ms\n";
}
```


```cpp
int Graph::EvalDensity(int u, ui edge_idx) {
    // check density for edge (u,v)
    int v = out_edges[edge_idx];
    
// it seems useless
//    if (min_cn[edge_idx] == NOT_SURE) {
//        int c = ComputeCnLowerBound(degree[u], degree[v]);
//        if (c <= 2) { return SIMILAR; }
//        min_cn[edge_idx] = c;
//        UpdateViaCrossLink(edge_idx);
//    }
    return IntersectNeighborSets(u, v, min_cn[edge_idx]);
}
```

```cpp
void Graph::ClusterCore(int u, int index_i) {
    for (auto idx : dst_vertices) {
        // u and v similar, and v is also a core vertex
        if (min_cn[idx] == SIMILAR && IsDefiniteCoreVertex(out_edges[idx])) {
            disjoint_set_ptr->Union(u, out_edges[idx]);
        }
    }

    for (int i = index_i; i < dst_vertices.size(); ++i) {
        ui edge_idx = dst_vertices[i];
        int v = out_edges[edge_idx];
        if (IsSimilarityUnKnow(edge_idx) && IsDefiniteCoreVertex(v) && !disjoint_set_ptr->IsSameSet(u, v)) {
            min_cn[edge_idx] = EvalDensity(u, edge_idx);
            UpdateViaCrossLink(edge_idx);
            // core vertices connected component
            if (min_cn[edge_idx] == SIMILAR) { disjoint_set_ptr->Union(u, v); }

//            if (effective_degree[v] != ALREADY_EXPLORED) {
//                if (min_cn[edge_idx] == SIMILAR) {
//                    ++similar_degree[v];
//                    if (IsDefiniteCoreVertex(v)) { cores.emplace_back(v); }
//                } else {
//                    --effective_degree[v];
//                }
//            }
        }
    }
}
```