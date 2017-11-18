```zsh
// lfr benchmark graphs
        if (n == 10000001) {
            auto batch_size = 256u;
            for (auto v_i = 0; v_i < n; v_i += batch_size) {
                int my_start = v_i;
                int my_end = min(n, my_start + batch_size);
                pool.enqueue([this](int i_start, int i_end) {
                    for (auto i = i_start; i < i_end; i++) { CheckCoreFirstBSP(i); }
                }, my_start, my_end);
            }
        } else {
            auto task_count = 0u;

            // 50,000,000
            if (min_cn.size() > 50000000) {
                // medium, large datasets
                // > 1000,000,000 large datsets, `min_cn.size() / n < 20`: non-social-network
                auto threshold_sum =
                        min_cn.size() < 1000000000 ? 1024 * 16 : (min_cn.size() / n < 20 ? 64 * 1024 : 1024 * 32);

                auto prev_start = 0;
                int cur_end;
                for (auto v_i = 0; v_i < n; ++v_i) {
                    cur_end = v_i + 1;

                    auto tmp_sum = 0u;
                    auto cmp0 = degree[v_i];
                    for (auto edge_idx = out_edge_start[v_i]; edge_idx < out_edge_start[v_i + 1]; edge_idx++) {
                        auto dst_idx = out_edges[edge_idx];
                        if (v_i < dst_idx && min_cn[edge_idx] > 0) {
                            tmp_sum += cmp0 + degree[dst_idx];
                        }
                    }

                    if (tmp_sum > threshold_sum) {
                        pool.enqueue([this](int i_start, int i_end) {
                            for (auto i = i_start; i < i_end; i++) { CheckCoreFirstBSP(i); }
                        }, prev_start, cur_end);

                        prev_start = cur_end;
                        ++task_count;
                    }
                }
                pool.enqueue([this](int i_start, int i_end) {
                    for (auto i = i_start; i < i_end; i++) { CheckCoreFirstBSP(i); }
                }, prev_start, cur_end);
                cout << "total task count:" << task_count << endl;
            } else {
                // small datasets
                auto batch_size = 256u;
                for (auto v_i = 0; v_i < n; v_i += batch_size) {
                    int my_start = v_i;
                    int my_end = min(n, my_start + batch_size);
                    pool.enqueue([this](int i_start, int i_end) {
                        for (auto i = i_start; i < i_end; i++) { CheckCoreFirstBSP(i); }
                    }, my_start, my_end);
                }
            }
        }
    }
```