## Concepts

data structure | meaning
--- | ---
super node |  represented by vertex `u`, actually is `eps-neighborhood(u)`
disjoint clusters | disjoint-sets where each tree is a cluster, element: core vertex

## Vertex Status

* vertex status unknown

type | detail
--- | ---
untouched | initial state
unprocessed-border | `sd >=1`, core-status unknown

* definite cores

type | detail
--- | ---
unprocessed-core | `sd >= min_pts`, `eps-neighborhood(u)` unknown, `nei[u]>=min_pts`
processed-core | `sd >= min_pts`, `eps-neighborhood(u)` known, `size of eps-neighborhood(u)>=min_pts `

* definite non-core that can be merged into a single status: non-core

type | detail
--- | ---
unprocessed-noise | `neighbor number < min_pts`, definite non-core
processed-noise | `sd < min_pts`, and none of its neighbors is core
processed-border | `sd < min_pts`, and at least one of its neighbors is core, `nei[u]>=1 and nei[u]<min_pts`                                                                            

## Computation Steps

### Summarize

* pre-processing

```cpp
    // mark unprocessed noise
    for (auto v_cur = 0; v_cur < n; v_cur++) {
        if (out_edge_start[v_cur + 1] - out_edge_start[v_cur] < min_u) {
            vertex_status[v_cur] = anySCAN::UNPROCESSED_NOISE;
        }
    }
```

* alpha untouched vertices as a block, check core or non-core
    * into processed-core, get eps-neighborhood 
    * into processed-noise, also get eps-neighborhood

```cpp
            for (auto u = v_beg; u < v_cur + 1; u++) {
                // compute eps-neighborhood for u
                auto sd = 0;
                for (auto j = out_edge_start[u]; j < out_edge_start[u + 1]; j++) {
                    auto &eps_neighborhood_vec = eps_neighborhood[u];
                    if (EvalSimilarity(u, j) == SIMILAR) {
                        eps_neighborhood_vec.emplace_back(out_edges[j]);
                        sd++;
                    }
                }
                // check core status from sd
                if (sd >= min_u) {
                    vertex_status[u] = anySCAN::PROCESSED_CORE; // definite core, eps-neighborhood known
                } else {
                    vertex_status[u] = anySCAN::PROCESSED_NOISE; // definite non-core, eps-neighborhood known
                }
            }
```

update `nei[u]`

```cpp
               for (auto w: eps_neighborhood[u]) {
                    eps_neighbor_num[w]++;
                }
```

* neighbor vertex status transition

```cpp
            for (auto u = v_beg; u < v_cur + 1; u++) {
                if (vertex_status[u] == anySCAN::PROCESSED_CORE) {
                    for (auto w: eps_neighborhood[u]) {
                        if (vertex_status[w] == anySCAN::PROCESSED_NOISE) {
                            vertex_status[w] = anySCAN::PROCESSED_BORDER;   // in a cluster
                        } else if (vertex_status[w] != anySCAN::PROCESSED_CORE) {
                            if (eps_neighbor_num[w] >= min_u) {
                                vertex_status[w] = anySCAN::UNPROCESSED_CORE;
                            } else {
                                vertex_status[w] = anySCAN::UNPROCESSED_BORDER;
                            }
                        }
                    }
                }
            }
```

* union processed-core and cores

### Merge Strongly Related Super Nodes

* filter unprocessed-border in at least two super nodes and sort them by `nei[u]`

* fetch all its super nodes, if all the same, do nothing,
    * how to fetch all `u's` super nodes? Probably via cross link technique to record.
    
    
* otherwise if it turns into unprocessed-core, then union all its super nodes

### Merge Weakly Related Super Nodes

* checking core, after which core or non-core can be determined for unprocessed-border, 
i.e, no unprocessed-border after that

* clustering core, after which core-induced cluster is complete

### Cluster Non-Core(Assign Labels)

* processed core, from `eps-neighborhood`

* unprocessed core, filter non-cores in its neighborhood and evaluated, 
some pre-computations can be stored but not complete in its `eps-neighborhood`