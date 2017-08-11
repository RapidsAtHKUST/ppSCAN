# Problem

race condition not solved in pre-processing `PruneAndCrossLink()` parallel try, only `min_cn[j]` and cross link creation and update okay by correctness guaranteed by `i <= v`

* `if (IsDefiniteCoreVertex(i)) { cores.emplace_back(i); }`
* `--effective_degree[i]`

```cpp
void Graph::PruneAndCrossLink() {
    auto thread_num = std::thread::hardware_concurrency();
    cout << "thread num:" << thread_num << "\n";
    auto batch_num = 16u * thread_num;
    auto batch_size = n / batch_num;
    //must iterate from 0 to n-1
    ThreadPool pool(thread_num);
    for (auto v_i = 0; v_i < n; v_i += batch_size) {
        int my_start = v_i;
        int my_end = min(n, my_start + batch_size);

        // correctness guaranteed by i <= v
        pool.enqueue([this](int i_start, int i_end) {
            for (auto i = i_start; i < i_end; i++)
                for (auto j = out_edge_start[i]; j < out_edge_start[i + 1]; j++) {
                    auto v = out_edges[j];
                    //edge (i,v)
                    if (i <= v) {
                        // use pruning rule
                        int a = degree[i], b = degree[v];
                        if (a > b) { swap(a, b); }
                        if (((long long) a) * eps_b2 < ((long long) b) * eps_a2) {
                            // can be pruned
                            min_cn[j] = NOT_DIRECT_REACHABLE;
                            --effective_degree[i];
                            --effective_degree[v];
                        } else {
                            int c = ComputeCnLowerBound(a, b);
                            // can be pruned
                            if (c <= 2) {
                                min_cn[j] = DIRECT_REACHABLE;
                                ++similar_degree[i];
                                if (IsDefiniteCoreVertex(i)) { cores.emplace_back(i); }
                                ++similar_degree[v];
                                if (IsDefiniteCoreVertex(v)) { cores.emplace_back(v); }
                            } else {
                                // need to refine later
                                min_cn[j] = c;
                            }
                        }

                        // find edge, in order to build cross link
                        auto r_id = BinarySearch(out_edges, out_edge_start[v], out_edge_start[v + 1], i);
                        InitCrossLink(j, r_id);
                        UpdateViaCrossLink(j);
                    }
                }
        }, my_start, my_end);
    }
}
```

# Changes

remove ed 

```cpp
//    vector<int> effective_degree;

//    effective_degree.reserve(n);
//    std::transform(degree.begin(), degree.end(), back_inserter(effective_degree),
//                   [](int degree_val) { return degree_val - 1; });

//        else {
//            --effective_degree[u];
//        }
```

```cpp
#ifdef STATISTICS
    vector<int> candidates;
    for (auto i = 0; i < n; i++) {
//        if (effective_degree[i] >= min_u) {
            CheckCore(i);
            if (IsDefiniteCoreVertex(i)) { candidates.emplace_back(i); }
//        }
    }
#else
    auto thread_num = std::thread::hardware_concurrency();
    auto batch_num = 16u * thread_num;
    auto batch_size = n / batch_num;
    vector<std::future<vector<int>>> future_vec;
    {
        ThreadPool pool(thread_num);

        for (auto v_i = 0; v_i < n; v_i += batch_size) {
            int my_start = v_i;
            int my_end = min(n, my_start + batch_size);
            future_vec.emplace_back(pool.enqueue([this](int i_start, int i_end) -> vector<int> {
                auto candidates = vector<int>();
                for (auto i = i_start; i < i_end; i++) {
//                    if (effective_degree[i] >= min_u) {
                        CheckCore(i);
                        if (IsDefiniteCoreVertex(i)) { candidates.emplace_back(i); }
//                    }
                }
                return candidates;
            }, my_start, my_end));
        }
    }
#endif
```

remove useless based on the change

```cpp
            for (auto j = out_edge_start[i]; j < out_edge_start[i + 1]; j++) {
                // observation 3: check non-core neighbors
                if (!IsDefiniteCoreVertex(out_edges[j])) {
//                    if (IsReachableUnKnow(j)) { min_cn[j] = EvalReachable(i, j); }
                    if (min_cn[j] == DIRECT_REACHABLE) {
                        // root must be parent since already disjoint_set_ptr->FindRoot(i) previously
                        noncore_cluster.emplace_back(cluster_dict[disjoint_set_ptr->FindRoot(i)], out_edges[j]);
                    }
                }
            }
```

remove early stop check cores, seem to be useless

```cpp
//    for (; IsCoreStatusUnKnow(u) && early_stop_idx < reachable_candidate_vertices.size(); ++early_stop_idx) {
    for (; early_stop_idx < reachable_candidate_vertices.size(); ++early_stop_idx) {
        auto idx = reachable_candidate_vertices[early_stop_idx];

        // correctness guaranteed by reachable_candidate_vertices[early_stop_idx] are all in not-similar status
        if (min_cn[idx] != DIRECT_REACHABLE) {
            int v = out_edges[idx];
            // 1st: compute density, build cross link
            min_cn[idx] = EvalReachable(u, idx);
            UpdateViaCrossLink(idx);

            // 2nd: update sd and ed for u
            if (min_cn[idx] == DIRECT_REACHABLE) {
                ++similar_degree[u];
                ++similar_degree[v];
            } else {
                --effective_degree[u];
                --effective_degree[v];
            }
        }
    }
    return early_stop_idx;
```

```cpp
    // for these may have not checked
//    for (int i = early_start_idx; i < reachable_candidate_vertices.size(); ++i) {
//        ui edge_idx = reachable_candidate_vertices[i];
//        int v = out_edges[edge_idx];
//
//        // !disjoint_set_ptr->IsSameSet(u, v) adopted to reduce the number of EvalReachable check
//        if (IsReachableUnKnow(edge_idx) && IsDefiniteCoreVertex(v) && !disjoint_set_ptr->IsSameSet(u, v)) {
//            min_cn[edge_idx] = EvalReachable(u, edge_idx);
//            UpdateViaCrossLink(edge_idx);
//            if (min_cn[edge_idx] == DIRECT_REACHABLE) { disjoint_set_ptr->Union(u, v); }
//        }
//    }
```

# Removes

remove useless state update

```cpp
    // mark u as already explored, to avoid duplicated in cores
//    effective_degree[u] = ALREADY_EXPLORED;
```

remove useless swap in intersection

```cpp
//    if (degree[u] > degree[v]) { swap(u, v); }

```

remove `cores` member

```cpp
//    vector<ui> cores;

```

```cpp
//    cores.reserve(n);

    while (true) {
        int u;
        // deal with already known cores and then deal with unexplored vertices
//        if (!cores.empty()) {
//            u = cores.back();
//            cores.pop_back();
//        } else {
        // find-max, if there delete-max
        u = max_priority_queue.pop();
//        }

        if (u == INVALID_VERTEX_IDX) { break; }

        int index_i = CheckCore(u);

        if (IsDefiniteCoreVertex(u)) { ClusterCore(u, index_i); }
    }

//    {
//        auto tmp = std::move(cores);
//    }
```

core change in pre-processing phase

```cpp
                    if (c <= 2) {
                        min_cn[j] = DIRECT_REACHABLE;
                        ++similar_degree[i];
//                        if (IsDefiniteCoreVertex(i)) { cores.emplace_back(i); }
                        ++similar_degree[v];
//                        if (IsDefiniteCoreVertex(v)) { cores.emplace_back(v); }
                    } 
```

```cpp
//    void ReactAfterEvalDensity(int v, ui edge_idx);


//void Graph::ReactAfterEvalDensity(int v, ui edge_idx) {
//    if (effective_degree[v] != ALREADY_EXPLORED) {
//        if (min_cn[edge_idx] == DIRECT_REACHABLE) {
//            ++similar_degree[v];
//            if (IsDefiniteCoreVertex(v)) { cores.emplace_back(v); }
//        } else {
//            --effective_degree[v];
//        }
//    }
//}

```

CheckCore

```cpp
            // 3rd: update sd and ed for v
//            ReactAfterEvalDensity(v, idx);
```

ClusterCore

```cpp

            // 3rd: update sd and ed for v
//            ReactAfterEvalDensity(v, edge_idx);
```

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
                        min_cn[j] = EvalReachable(i, j);

// it seems to be useless
//                        UpdateViaCrossLink(j);
//                        if (min_cn[j] == DIRECT_REACHABLE) {
//                            ++similar_degree[i];
//                            ++similar_degree[out_edges[j]];
//                        }
                    }
                    if (min_cn[j] == DIRECT_REACHABLE) {
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
//                if (min_cn[j] == NOT_SURE) { min_cn[j] = NOT_DIRECT_REACHABLE; }
//            }
//            else {
            if (i <= v) {
                // use pruning rule
                int a = degree[i], b = degree[v];
                if (a > b) { swap(a, b); }
                if (((long long) a) * eps_b2 < ((long long) b) * eps_a2) {
                    // can be pruned
                    min_cn[j] = NOT_DIRECT_REACHABLE;
                    --effective_degree[i];
                    --effective_degree[v];
                } else {
                    // cannot be pruned
                    int c = ComputeCnLowerBound(a, b);
                    if (c <= 2) {
                        min_cn[j] = DIRECT_REACHABLE;
                        ++similar_degree[i];
                        if (IsDefiniteCoreVertex(i)) { cores.emplace_back(i); }
                        ++similar_degree[v];
                        if (IsDefiniteCoreVertex(v)) { cores.emplace_back(v); }
                    } else {
                        min_cn[j] = c;
                    }
                }

                // find edge, in order to build cross link
//                if (min_cn[j] != NOT_DIRECT_REACHABLE) {
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
int Graph::EvalReachable(int u, ui edge_idx) {
    // check density for edge (u,v)
    int v = out_edges[edge_idx];
    
// it seems useless
//    if (min_cn[edge_idx] == NOT_SURE) {
//        int c = ComputeCnLowerBound(degree[u], degree[v]);
//        if (c <= 2) { return DIRECT_REACHABLE; }
//        min_cn[edge_idx] = c;
//        UpdateViaCrossLink(edge_idx);
//    }
    return IntersectNeighborSets(u, v, min_cn[edge_idx]);
}
```

```cpp
void Graph::ClusterCore(int u, int index_i) {
    for (auto idx : reachable_candidate_vertices) {
        // u and v similar, and v is also a core vertex
        if (min_cn[idx] == DIRECT_REACHABLE && IsDefiniteCoreVertex(out_edges[idx])) {
            disjoint_set_ptr->Union(u, out_edges[idx]);
        }
    }

    for (int i = index_i; i < reachable_candidate_vertices.size(); ++i) {
        ui edge_idx = reachable_candidate_vertices[i];
        int v = out_edges[edge_idx];
        if (IsReachableUnKnow(edge_idx) && IsDefiniteCoreVertex(v) && !disjoint_set_ptr->IsSameSet(u, v)) {
            min_cn[edge_idx] = EvalReachable(u, edge_idx);
            UpdateViaCrossLink(edge_idx);
            // core vertices connected component
            if (min_cn[edge_idx] == DIRECT_REACHABLE) { disjoint_set_ptr->Union(u, v); }

//            if (effective_degree[v] != ALREADY_EXPLORED) {
//                if (min_cn[edge_idx] == DIRECT_REACHABLE) {
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

```cpp
    MaxPriorityQueueWithLazyUpdate max_priority_queue(n, &effective_degree, min_u);

    // bin_head, bin_next adopted to mimic the max-priority-queue(heap)
//    auto bin_next = vector<int>(n);
//    auto bin_head = vector<int>(n);
//    std::fill(bin_head.begin(), bin_head.end(), INVALID_VERTEX_IDX);
//    int cur_max_ed = 0;
//    for (auto i = 0; i < n; i++) {
//        if (effective_degree[i] >= min_u) {
//            int ed = effective_degree[i];
//            if (ed > cur_max_ed) { cur_max_ed = ed; }
//            bin_next[i] = bin_head[ed];
//            bin_head[ed] = i;
//        }
//    }

    while (true) {
        int u = INVALID_VERTEX_IDX;
        // deal with already known cores and then deal with unexplored vertices
        if (!cores.empty()) {
            u = cores.back();
            cores.pop_back();
        } else {
            // find-max, if there delete-max
           u= max_priority_queue.pop();
//            while (cur_max_ed >= min_u && u == INVALID_VERTEX_IDX) {
//                for (int x = bin_head[cur_max_ed]; x != -1;) {
//                    int tmp = bin_next[x];
//                    int ed = effective_degree[x];
//                    // dynamically maintain the max-priority-queue(heap)
//                    if (ed == cur_max_ed) {
//                        u = x;
//                        bin_head[cur_max_ed] = bin_next[x];
//                        break;
//                    }
//                    // lazy update
//                    if (ed >= min_u) {
//                        bin_next[x] = bin_head[ed];
//                        bin_head[ed] = x;
//                    }
//                    x = tmp;
//                }
//                // reach the tail of linked list, within underlying array
//                if (u == INVALID_VERTEX_IDX) {
//                    bin_head[cur_max_ed] = -1;
//                    --cur_max_ed;
//                }
//            }
        }
```