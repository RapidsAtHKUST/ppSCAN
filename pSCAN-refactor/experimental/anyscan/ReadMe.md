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
processed-core | `sd >= min_pts`, `eps-neighborhood(u)` known

* definite non-core that can be merged

type | detail
--- | ---
unprocessed-noise | `neighbor number < min_pts`, definite non-core
processed-noise | `sd < min_pts`, and none of its neighbors is core
processed-border | `sd < min_pts`, and at least one of its neighbors is core, `nei[u]>=1 and nei[u]<min_pts`                                                                            
                  