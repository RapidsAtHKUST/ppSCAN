## Origin

```cpp
#ifdef ORIGIN
        int edge_buf_n = 0;
        for (ui j = pstart[u]; j < pstart[u + 1]; j++) {
            if (min_cn[j] == -2) continue;

            if (similar_degree[u] < miu || find_root(u) != find_root(edges[j])) edge_buf[edge_buf_n++] = j;
        }

        // 1st: Check Core
        int i = 0;
        while (similar_degree[u] < miu && effective_degree[u] >= miu && i < edge_buf_n) {
            ui idx = edge_buf[i];
            if (min_cn[idx] != -1) {
                int v = edges[idx];

                min_cn[idx] = min_cn[reverse[idx]] = similar_check_OP(u, idx, eps_a2, eps_b2);

                if (min_cn[idx] == -1) ++similar_degree[u];
                else --effective_degree[u];

                if (effective_degree[v] != -1) {
                    if (min_cn[idx] == -1) {
                        ++similar_degree[v];
                        if (similar_degree[v] == miu) {
                            cores[cores_n++] = v;
                        }
                    } else {
                        --effective_degree[v];
                    }
                }
            }

            ++i;
        }

        effective_degree[u] = -1;

        // 2nd: Cluster Core-Core
        if (similar_degree[u] >= miu) {
            for (int j = 0; j < edge_buf_n; j++) {
                ui idx = edge_buf[j];
                if (min_cn[idx] == -1 && similar_degree[edges[idx]] >= miu) {
                    my_union(u, edges[idx]);
                }
            }

            while (i < edge_buf_n) {
                ui idx = edge_buf[i];
                int v = edges[idx];
                // if neighbor v currently is a core, check if u and v can be clustered together
                if (min_cn[idx] >= 0 && similar_degree[v] >= miu && find_root(u) != find_root(v)) {
                    min_cn[idx] = min_cn[reverse[idx]] = similar_check_OP(u, idx, eps_a2, eps_b2);

                    if (min_cn[idx] == -1) { my_union(u, v); }
                }
                ++i;
            }
        }
    }
#else
```