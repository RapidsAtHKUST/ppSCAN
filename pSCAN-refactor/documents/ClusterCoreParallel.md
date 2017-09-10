## Related Computations

```cpp
void Graph::ClusterCoreFirstPhase(int u) {
    for (auto j = out_edge_start[u]; j < out_edge_start[u + 1]; j++) {
        auto v = out_edges[j];
        if (u < v && IsDefiniteCoreVertex(v) && !disjoint_set_ptr->IsSameSet(u, v)) {
            if (min_cn[j] == DIRECT_REACHABLE) {
                disjoint_set_ptr->Union(u, v);
            }
        }
    }
}

void Graph::ClusterCoreSecondPhase(int u) {
    for (auto edge_idx = out_edge_start[u]; edge_idx < out_edge_start[u + 1]; edge_idx++) {
        auto v = out_edges[edge_idx];
        if (u < v && IsDefiniteCoreVertex(v) && !disjoint_set_ptr->IsSameSet(u, v)) {
            if (min_cn[edge_idx] > 0) {
                min_cn[edge_idx] = EvalReachable(u, edge_idx);
                if (min_cn[edge_idx] == DIRECT_REACHABLE) {
                    disjoint_set_ptr->Union(u, v);
                }
            }
        }
    }
}
```

## Parallelism

parallelism for `disjoint_set` is non-trivial

* `FindRoot` invoked by `disjoint_set_ptr->IsSameSet(u, v)` should also be thread-safe first for the correctness, due to the optimization technique of compression-by-path

* take `ClusterCoreFirstPhase(int u)` for example, inspired by `a new scalable parallel DBSCAN algorithm using the disjoint-set data sturcture`

```cpp
void Graph::ClusterCoreFirstPhase(int u) {
    for (auto j = out_edge_start[u]; j < out_edge_start[u + 1]; j++) {
        auto v = out_edges[j];
        if (u < v && IsDefiniteCoreVertex(v) && !disjoint_set_ptr->IsSameSet(u, v)) {
            if (min_cn[j] == DIRECT_REACHABLE) {
                Preparations for our parallelism..................                   
            }
        }
    }
}
```

```cpp
Here is the barrier
```

```cpp
Loop over our intermedaite things(thread local), do thread-safe-union
```

* take `ClusterCoreSecondPhase(int u)` for example

```cpp
void Graph::ClusterCoreSecondPhase(int u) {
    for (auto edge_idx = out_edge_start[u]; edge_idx < out_edge_start[u + 1]; edge_idx++) {
        auto v = out_edges[edge_idx];
        if (u < v && IsDefiniteCoreVertex(v) && !disjoint_set_ptr->IsSameSet(u, v)) {
            if (min_cn[edge_idx] > 0) {
                min_cn[edge_idx] = EvalReachable(u, edge_idx);
                if (min_cn[edge_idx] == DIRECT_REACHABLE) {
                    Preparations for our parallelism..................        
                }
            }
        }
    }
}
```

```cpp
Here is the barrier
```

```cpp
Loop over our intermedaite things(thread local), do thread-safe-union
```
