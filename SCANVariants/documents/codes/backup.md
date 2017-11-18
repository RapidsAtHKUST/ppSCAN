```cpp
//    {
//        ThreadPool pool(std::thread::hardware_concurrency());
//
//        auto batch_size = 64u;
//        for (auto outer_i = 0; outer_i < cores.size(); outer_i += batch_size) {
//            int my_start = outer_i;
//            int my_end = min(static_cast<ui>(cores.size()), my_start + batch_size);
//            pool.enqueue([this](int i_start, int i_end) {
//                for (auto i = i_start; i < i_end; i++) {
//                    auto u = cores[i];
//                    ClusterCoreFirstPhase(u);
//                }
//            }, my_start, my_end);
//        }
//    }

//    {
//        ThreadPool pool(std::thread::hardware_concurrency());
//
//        auto batch_size = 64u;
//        for (auto v_i = 0; v_i < cores.size(); v_i += batch_size) {
//            int my_start = v_i;
//            int my_end = min(static_cast<ui>(cores.size()), my_start + batch_size);
//            pool.enqueue([this](int i_start, int i_end) {
//                for (auto i = i_start; i < i_end; i++) {
//                    auto u = cores[i];
//                    ClusterCoreSecondPhase(u);
//                }
//            }, my_start, my_end);
//        }
//    }
```

```cpp
auto thread_num = std::thread::hardware_concurrency();
    {
        ThreadPool pool(thread_num);
        auto batch_size = 64u;
        for (auto v_i = 0; v_i < cores.size(); v_i += batch_size) {
            int my_start = v_i;
            int my_end = min(static_cast<ui>(cores.size()), my_start + batch_size);
            pool.enqueue([this](int i_start, int i_end) {
                for (auto i = i_start; i < i_end; i++) {
                    auto u = cores[i];
                    ClusterNonCoreFirstPhase(u);
                }
            }, my_start, my_end);
        }
    }
```

```cpp
 {
        ThreadPool pool(thread_num);

        auto batch_size = 8192u;
        vector<future<vector<pair<int, int >>>> future_vec;
        for (auto v_i = 0; v_i < cores.size(); v_i += batch_size) {
            int my_start = v_i;
            int my_end = min(static_cast<ui>(cores.size()), my_start + batch_size);
            future_vec.emplace_back(
                    pool.enqueue([this](int i_start, int i_end) -> vector<pair<int, int>> {
                        auto tmp_cluster = vector<pair<int, int>>();
                        for (auto i = i_start; i < i_end; i++) {
                            auto u = cores[i];
                            for (auto j = out_edge_start[u]; j < out_edge_start[u + 1]; j++) {
                                auto v = out_edges[j];
                                if (!IsDefiniteCoreVertex(v)) {
                                    auto root_of_u = disjoint_set_ptr->FindRoot(static_cast<uint32_t>(u));
                                    if (min_cn[j] == SIMILAR) {
                                        tmp_cluster.emplace_back(cluster_dict[root_of_u], v);
                                    }
                                }
                            }
                        }
                        return tmp_cluster;
                    }, my_start, my_end));
        }

        for (auto &future: future_vec) {
            for (auto ele:future.get()) {
                noncore_cluster.emplace_back(ele);
            };
        }
    }
```

