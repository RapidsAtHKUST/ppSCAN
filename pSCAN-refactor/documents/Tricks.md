## Reduce Num of Binary Search

compute all possible definite things, prune0/prune1, instead of using binary search

```cpp
void Graph::Prune() {
    for (auto u = 0; u < n; u++) {
        for (auto j = out_edge_start[u]; j < out_edge_start[u + 1]; j++) {
            auto v = out_edges[j];
            int deg_a = degree[u], b = degree[v];
            if (deg_a > b) { swap(deg_a, b); };
            if (u <= v) {
                if (((long long) deg_a) * eps_b2 < ((long long) b) * eps_a2) {
                    // can be pruned
#ifdef STATISTICS
                    ++prune0;
#endif
                    min_cn[j] = NOT_DIRECT_REACHABLE;
                } else {
                    // can be pruned, when c <= 2
                    int c = ComputeCnLowerBound(deg_a, b);
                    if (c <= 2) {
#ifdef STATISTICS
                        ++prune1;
#endif
                        min_cn[j] = DIRECT_REACHABLE;
                    } else {
                        min_cn[j] = c;
                    }
                }
            } else {
                if (((long long) deg_a) * eps_b2 < ((long long) b) * eps_a2) {
                    // can be pruned
                    min_cn[j] = NOT_DIRECT_REACHABLE;
                } else {
                    // can be pruned, when c <= 2
                    int c = ComputeCnLowerBound(deg_a, b);
                    if (c <= 2) {
                        min_cn[j] = DIRECT_REACHABLE;
                    } else {
                        min_cn[j] = c;
                    }
                }
            }
        }
    }
}
```

check if it is not sure currently

```cpp
            if (min_cn[edge_idx] == NOT_SURE) {
                min_cn[edge_idx] = min_cn[BinarySearch(out_edges, out_edge_start[v], out_edge_start[v + 1], u)];
            }
```

## Reduce Num of Eval

* update cross link, need to figure out where is redundant comp

```cpp
void Graph::CheckCoreFirstBSP(int u) {
    auto sd = 0;
    auto ed = degree[u] - 1;
    for (auto edge_idx = out_edge_start[u]; edge_idx < out_edge_start[u + 1]; edge_idx++) {
        if (min_cn[edge_idx] > 0 && u <= out_edges[edge_idx]) {
            min_cn[edge_idx] = EvalReachable(u, edge_idx);
            auto v = out_edges[edge_idx];
            min_cn[BinarySearch(out_edges, out_edge_start[v], out_edge_start[v + 1], u)] = min_cn[edge_idx];
            if (min_cn[edge_idx] == DIRECT_REACHABLE) {
                ++sd;
                if (sd >= min_u) {
                    is_core_lst[u] = true;
                    return;
                }
            } else {
                --ed;
                if (ed < min_u) {
                    is_non_core_lst[u] = true;
                    return;
                }
            }
        }
    }
}
```

* use sd and ed boundary, sd: lower bound, ed: upper bound

```cpp
void Graph::CheckCoreFirstBSP(int u) {
    auto sd = 0;
    for (auto edge_idx = out_edge_start[u]; edge_idx < out_edge_start[u + 1]; edge_idx++) {
        if (min_cn[edge_idx] > 0 && u <= out_edges[edge_idx]) {
            min_cn[edge_idx] = EvalReachable(u, edge_idx);
            if (min_cn[edge_idx] == DIRECT_REACHABLE) {
                ++sd;
                if (sd >= min_u) {
                    is_core_lst[u] = true;
                    return;
                }
            }
        }
    }
}

void Graph::CheckCoreSecondBSP(int u) {
    if (!is_core_lst[u]) {
        auto sd = 0;
        auto ed = degree[u];
        for (auto edge_idx = out_edge_start[u]; edge_idx < out_edge_start[u + 1]; edge_idx++) {
            auto v = out_edges[edge_idx];
            if (min_cn[edge_idx] == NOT_SURE) {
                min_cn[edge_idx] = min_cn[BinarySearch(out_edges, out_edge_start[v], out_edge_start[v + 1], u)];
            }

            if (min_cn[edge_idx] == DIRECT_REACHABLE) {
                ++sd;
                if (sd >= min_u) {
                    is_core_lst[u] = true;
                    return;
                }
            }
            if (min_cn[edge_idx] == NOT_DIRECT_REACHABLE) {
                --ed;
                if (ed < min_u) {
                    return;
                }
            }
        }

        for (auto edge_idx = out_edge_start[u]; edge_idx < out_edge_start[u + 1]; edge_idx++) {
            if (min_cn[edge_idx] > 0) {
                min_cn[edge_idx] = EvalReachable(u, edge_idx);
                if (min_cn[edge_idx] == DIRECT_REACHABLE) {
                    ++sd;
                    if (sd >= min_u) {
                        is_core_lst[u] = true;
                        return;
                    }
                } else {
                    --ed;
                    if (ed < min_u) {
                        return;
                    }
                }
            }
        }
    }
}
```

for correctness 

```cpp
void Graph::ClusterNonCores() {
    noncore_cluster = vector<pair<int, int>>();
    noncore_cluster.reserve(n);

    MarkClusterMinEleAsId();
    for (auto i = 0; i < n; i++) {
        if (is_core_lst[i]) {
            for (auto j = out_edge_start[i]; j < out_edge_start[i + 1]; j++) {
                // observation 3: check non-core neighbors
                auto v = out_edges[j];
                if (!is_core_lst[v]) {
                    if (min_cn[j] == 0) {
                        min_cn[j] = min_cn[BinarySearch(out_edges, out_edge_start[v], out_edge_start[v + 1], i)];
                    }
                    if (min_cn[j] > 0) {
                        min_cn[j] = EvalReachable(i, j);
                    }
                    if (min_cn[j] == DIRECT_REACHABLE) {
                        noncore_cluster.emplace_back(cluster_dict[disjoint_set_ptr->FindRoot(i)], out_edges[j]);
                    }
                }
            }
        }
    }
}
```

* use disjoint-set pruning in clustering-core

improved one 

```cpp
    for (auto candidate:union_candidates) { ClusterCoreFirstPhase(candidate); }
    for (auto candidate:union_candidates) { ClusterCoreSecondPhase(candidate); }
```

```cpp
void Graph::ClusterCoreFirstPhase(int u) {
    for (auto j = out_edge_start[u]; j < out_edge_start[u + 1]; j++) {
        auto v = out_edges[j];
        if (u < v && is_core_lst[v] && !disjoint_set_ptr->IsSameSet(u, v)) {
            if (min_cn[j] == DIRECT_REACHABLE) {
                disjoint_set_ptr->Union(u, out_edges[j]);
            }
        }
    }
}

void Graph::ClusterCoreSecondPhase(int u) {
    for (auto j = out_edge_start[u]; j < out_edge_start[u + 1]; j++) {
        auto v = out_edges[j];
        if (u < v && is_core_lst[v] && !disjoint_set_ptr->IsSameSet(u, v)) {
            if (min_cn[j] > 0) {
                min_cn[j] = EvalReachable(u, j);
                if (min_cn[j] == DIRECT_REACHABLE) {
                    disjoint_set_ptr->Union(u, out_edges[j]);
                }
            }
        }
    }
}
```

previous one

```cpp
void Graph::ClusterCoreSecondPhase(int u) {
    for (auto j = out_edge_start[u]; j < out_edge_start[u + 1]; j++) {
        auto v = out_edges[j];
        if (u < v && is_core_lst[v] && !disjoint_set_ptr->IsSameSet(u, v)) {
            if (min_cn[j] > 0) {
                min_cn[j] = EvalReachable(u, j);
            }
            if (min_cn[j] == DIRECT_REACHABLE) {
                disjoint_set_ptr->Union(u, out_edges[j]);
            }
        }
    }
}
```

