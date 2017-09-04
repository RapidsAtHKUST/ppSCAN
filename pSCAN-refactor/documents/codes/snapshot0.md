```cpp
ui Graph::BinarySearch(vector<int> &array, ui offset_beg, ui offset_end, int val) {
    auto mid = static_cast<ui>((static_cast<unsigned long>(offset_beg) + offset_end) / 2);
    if (array[mid] == val) { return mid; }
    return val < array[mid] ? BinarySearch(array, offset_beg, mid, val) : BinarySearch(array, mid + 1, offset_end, val);
}

int Graph::ComputeCnLowerBound(int du, int dv) {
    auto c = (int) (sqrtl((((long double) du) * ((long double) dv) * eps_a2) / eps_b2));
    if (((long long) c) * ((long long) c) * eps_b2 < ((long long) du) * ((long long) dv) * eps_a2) { ++c; }
    return c;
}

int Graph::IntersectNeighborSets(int u, int v, int min_cn_num) {
    int cn = 2; // count for self and v, count for self and u
    int du = degree[u] + 1, dv = degree[v] + 1; // count for self and v, count for self and u
#ifdef STATISTICS
    intersection_times++;
    auto tmp0 = 0;
    auto tmp1 = 0;
#endif

    auto offset_nei_u_end = out_edge_start[u + 1];
    auto offset_nei_v_end = out_edge_start[v + 1];
    for (auto offset_nei_u = out_edge_start[u], offset_nei_v = out_edge_start[v];
         offset_nei_u < offset_nei_u_end && offset_nei_v < offset_nei_v_end &&
         cn < min_cn_num && du >= min_cn_num && dv >= min_cn_num;) {
        if (out_edges[offset_nei_u] < out_edges[offset_nei_v]) {
            --du;
            ++offset_nei_u;
#ifdef STATISTICS
            ++all_cmp0;
            ++tmp0;
#endif
        } else if (out_edges[offset_nei_u] > out_edges[offset_nei_v]) {
            --dv;
            ++offset_nei_v;
#ifdef STATISTICS
            ++all_cmp1;
            ++tmp1;
#endif
        } else {
            ++cn;
            ++offset_nei_u;
            ++offset_nei_v;
#ifdef STATISTICS
            ++all_cmp2;
            ++tmp0;
            ++tmp1;
#endif
        }
    }
#ifdef STATISTICS
    int max_val = max(tmp0, tmp1);
    int min_val = min(tmp0, tmp1);
    int local_portion = min_val == 0 ? max_val : max_val / min_val;
    portion = max(portion, local_portion);
#endif
    return cn >= min_cn_num ? DIRECT_REACHABLE : NOT_DIRECT_REACHABLE;
}

int Graph::EvalReachable(int u, ui edge_idx) {
    int v = out_edges[edge_idx];
    return IntersectNeighborSets(u, v, min_cn[edge_idx]);
}
```

```cpp
void Graph::Prune() {
    for (auto u = 0; u < n; u++) {
        for (auto j = out_edge_start[u]; j < out_edge_start[u + 1]; j++) {
            auto v = out_edges[j];
            int deg_a = degree[u], deg_b = degree[v];
            if (deg_a > deg_b) { swap(deg_a, deg_b); };

            if (((long long) deg_a) * eps_b2 < ((long long) deg_b) * eps_a2) {
                // can be pruned
#ifdef STATISTICS
                if (u <= v) {
                    ++prune0;
                }
#endif
                min_cn[j] = NOT_DIRECT_REACHABLE;
            } else {
                // can be pruned, when c <= 2
                int c = ComputeCnLowerBound(deg_a, deg_b);
                if (c <= 2) {
#ifdef STATISTICS
                    if (u <= v) {
                        ++prune1;
                    }
#endif
                    min_cn[j] = DIRECT_REACHABLE;
                } else {
                    min_cn[j] = c;
                }
            }
        }
    }
}
```

```cpp
void Graph::CheckCoreFirstBSP(int u) {
    auto sd = 0;
    auto ed = degree[u] - 1;
    for (auto edge_idx = out_edge_start[u]; edge_idx < out_edge_start[u + 1]; edge_idx++) {
        auto v = out_edges[edge_idx];
        if (min_cn[edge_idx] > 0 && u <= v) {
            min_cn[edge_idx] = EvalReachable(u, edge_idx);
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

```cpp
void Graph::CheckCoreSecondBSP(int u) {
    if (!is_core_lst[u] && !is_non_core_lst[u]) {
        auto sd = 0;
        auto ed = degree[u] - 1;
        for (auto edge_idx = out_edge_start[u]; edge_idx < out_edge_start[u + 1]; edge_idx++) {
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
            auto v = out_edges[edge_idx];
            if (min_cn[edge_idx] == NOT_SURE) {
                min_cn[edge_idx] = min_cn[BinarySearch(out_edges, out_edge_start[v], out_edge_start[v + 1], u)];
            }
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

void Graph::ClusterCore(int u) {
    for (auto edge_idx = out_edge_start[u]; edge_idx < out_edge_start[u + 1]; edge_idx++) {
        auto v = out_edges[edge_idx];
        if (u < v && is_core_lst[v] && !disjoint_set_ptr->IsSameSet(u, v)) {
            if (min_cn[edge_idx] == NOT_SURE) {
                min_cn[edge_idx] = min_cn[BinarySearch(out_edges, out_edge_start[v], out_edge_start[v + 1], u)];
            }
            if (min_cn[edge_idx] > 0) {
                min_cn[edge_idx] = EvalReachable(u, edge_idx);
                if (min_cn[edge_idx] == DIRECT_REACHABLE) {
                    disjoint_set_ptr->Union(u, out_edges[edge_idx]);
                }
            }
        }
    }
}
```

```cpp
void Graph::MarkClusterMinEleAsId() {
    cluster_dict = vector<int>(n);
    std::fill(cluster_dict.begin(), cluster_dict.end(), n);

    for (auto i = 0; i < n; i++) {
        if (is_core_lst[i]) {
            // after this, root must be left nodes' parent since disjoint_set_ptr->FindRoot(i)
            int x = disjoint_set_ptr->FindRoot(i);
            if (i < cluster_dict[x]) { cluster_dict[x] = i; }
        }
    }
}

void Graph::ClusterNonCores() {
    noncore_cluster = vector<pair<int, int>>();
    noncore_cluster.reserve(n);

    MarkClusterMinEleAsId();
    for (auto i = 0; i < n; i++) {
        if (is_core_lst[i]) {
            for (auto j = out_edge_start[i]; j < out_edge_start[i + 1]; j++) {
                auto v = out_edges[j];
                if (!is_core_lst[v]) {
                    if (min_cn[j] == NOT_SURE) {
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

```cpp
void Graph::pSCAN() {
    // 1 prune
    cout << "new algo" << endl;
    auto prune_start = high_resolution_clock::now();
    Prune();
    auto prune_end = high_resolution_clock::now();
    cout << "1st: prune execution time:" << duration_cast<milliseconds>(prune_end - prune_start).count() << " ms\n";

    // 2.1 check-core 1st bsp
    auto find_core_start = high_resolution_clock::now();
    vector<int> candidates;
    for (auto i = 0; i < n; i++) {
        CheckCoreFirstBSP(i);
    }
    auto first_bsp_end = high_resolution_clock::now();
    cout << "2nd: check core first-phase bsp time:"
         << duration_cast<milliseconds>(first_bsp_end - find_core_start).count() << " ms\n";

    // 2.2 check-core 2nd bsp
    for (auto i = 0; i < n; i++) {
        CheckCoreSecondBSP(i);
        if (is_core_lst[i]) { candidates.emplace_back(i); }
    }
    auto second_bsp_end = high_resolution_clock::now();
    cout << "2nd: check core second-phase bsp time:"
         << duration_cast<milliseconds>(second_bsp_end - first_bsp_end).count() << " ms\n";

    // 3 cluster core
    for (auto candidate:candidates) { ClusterCoreFirstPhase(candidate); }
    for (auto candidate:candidates) { ClusterCore(candidate); }

    auto end_core_cluster = high_resolution_clock::now();
    cout << "3rd: core clustering time:" << duration_cast<milliseconds>(end_core_cluster - second_bsp_end).count()
         << " ms\n";

    // 4 non-core clustering
    ClusterNonCores();
    auto all_end = high_resolution_clock::now();
    cout << "4th: non-core clustering time:" << duration_cast<milliseconds>(all_end - end_core_cluster).count()
         << " ms\n";

    // output statistics
#ifdef STATISTICS
    cout << "\nprune0 definitely not reachable:" << prune0 << "\nprune1 definitely reachable:" << prune1 << "\n";
    cout << "intersection times:" << intersection_times << "\ncmp0:" << all_cmp0 << "\ncmp1:" << all_cmp1
         << "\nequal cmp:" << all_cmp2 << "\n";
    cout << "max portion:" << portion << endl;
#endif
}
```
