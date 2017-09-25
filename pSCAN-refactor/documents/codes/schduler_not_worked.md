too much cost in iteration

```cpp
        auto v_start = 0;
        auto v_end = static_cast<int>(cores.size());
        auto comp_workload = 0;
        for (auto v_i = 0; v_i < v_end; v_i++) {
            for (auto j = out_edge_start[v_i]; j < out_edge_start[v_i + 1]; j++) {
                auto v = out_edges[j];
                if (!IsDefiniteCoreVertex(v)) {
                    comp_workload += degree[v_i] + degree[v];
                    if (comp_workload > 256 * 1024) {
                        comp_workload = 0;
                        pool.enqueue([this](int i_start, int i_end) {
                            for (auto i = i_start; i < i_end; i++) {
                                auto u = cores[i];
                                ClusterNonCoreFirstPhase(u);
                            }
                        }, v_start, v_i + 1);
                        v_start = v_i + 1;

                    }
                }
            }
        }

        pool.enqueue([this](int i_start, int i_end) {
            for (auto i = i_start; i < i_end; i++) {
                auto u = cores[i];
                ClusterNonCoreFirstPhase(u);
            }
        }, v_start, v_end);
```

no significant improvement without knowing core-non-core similarity detail
 
```cpp
        auto v_start = 0;
        auto v_end = static_cast<int>(cores.size());
        auto comp_workload = 0;
        for (auto v_i = 0; v_i < v_end; v_i++) {
            comp_workload += degree[v_i];
            if (comp_workload > 64 * 1024) {
                comp_workload = 0;
                pool.enqueue([this](int i_start, int i_end) {
                    for (auto i = i_start; i < i_end; i++) {
                        auto u = cores[i];
                        ClusterNonCoreFirstPhase(u);
                    }
                }, v_start, v_i + 1);
                v_start = v_i + 1;

            }
        }

        pool.enqueue([this](int i_start, int i_end) {
            for (auto i = i_start; i < i_end; i++) {
                auto u = cores[i];
                ClusterNonCoreFirstPhase(u);
            }
        }, v_start, v_end);
```