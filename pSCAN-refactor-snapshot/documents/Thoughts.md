## Simple Parallel 

* parallelize `PruneAndCrossLink`

* parallelize the computations for `EvalReachable`

* try to reduce the cost of if-branch miss in `IntersectNeighborSets(int u, int v, int min_cn_num)`, 
still keep early quit intersection functionality

## Not Useful

```cpp
pair<bool, ui> Graph::BinarySearch2(vector<int> &array, ui offset_beg, ui offset_end, int val) {
    auto mid = static_cast<ui>((static_cast<unsigned long>(offset_beg) + offset_end) / 2);
    if (array[mid] == val) {
        return make_pair(true, mid);
    }
    if (array[mid] < val) {
        return mid + 1 == offset_end ? make_pair(false, mid) : BinarySearch2(array, mid + 1, offset_end, val);
    }
    return mid == offset_beg ? make_pair(false, offset_beg) : BinarySearch2(array, offset_beg, mid, val);
}

int Graph::IntersectNeighborSets(int u, int v, int min_cn_num) {
    int cn = 2; // count for self and v, count for self and u
    if (degree[u] > degree[v]) { std::swap(u, v); }
    int du = degree[u] + 1, dv = degree[v] + 1; // count for self and v, count for self and u

    if (dv < 10 * du) {
#ifdef STATISTICS
        intersection_times++;
        auto tmp0 = 0;
        auto tmp1 = 0;
#endif
        for (auto offset_nei_u = out_edge_start[u], offset_nei_v = out_edge_start[v];
             offset_nei_u < out_edge_start[u + 1] && offset_nei_v < out_edge_start[v + 1] &&
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
    } else {
        auto new_offset_beg = out_edge_start[v];
        auto offset_end = out_edge_start[v + 1];

        for (auto offset_nei_u = out_edge_start[u];
             offset_nei_u < out_edge_start[u + 1] && cn < min_cn_num && du >= min_cn_num; ++offset_nei_u) {
            bool flag;
            std::tie(flag, new_offset_beg) = BinarySearch2(out_edges, new_offset_beg, offset_end,
                                                           out_edges[offset_nei_u]);
            if (flag) {
                ++cn;
            } else {
                --du;
            }
        }
    }
    return cn >= min_cn_num ? DIRECT_REACHABLE : NOT_DIRECT_REACHABLE;

}
```