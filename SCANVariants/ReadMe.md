## Related Projects

description | folder link
--- | ---
spatial dbscan sc2012 project | [dbscan-v1.0.0](./dbscan-v1.0.0)
spatial dbscan another | [markus.goetz-hpdbscan](./markus.goetz-hpdbscan)
parameter-free scan | [gSkeletonClu](./gSkeletonClu)
pSCAN original codes | [pSCAN](./pSCAN)

* Submodules Init

```
git submodule init
git submodule update
```

## Backup

pScan: extension of spatial dbScan to graph, in order to explore structural clusters, current research focus on **unweighted, undirected not dynamic** graph.

* two parameters: density threshold, min number of neighbors, input graph representation: csr representation. density function: `number of common neighbors` / `(sqrt(du * dv))`

* pScan basic concepts:
  * introduce `min_cn`, alternative values: reachable, not_reachable, min_cn for guaranteeing a core
    * pre-processing, `PruneAndCrossLink`, prune not reachable `if (((long long) a) * eps_b2 < ((long long) b) * eps_a2)`, which is easy to deduced when the max of`number of common neighbors` is `min(du,dv)`, and utilize cross-link
    * `common neighbor lower bound <=2`, must be similar, self and neighbor vertices, compute common neighbor with early exit, via utilizing `du` and `dv`

  * introduce `similar_degree`, `effective_degree` to help checking core, if a vertex is a core iff `sd <= number of reachable neighbors <=ed`, which can be used as a pruning rule
    * other tricks to reduce the number of evaluations of reachability, like `max-priority-queue`, `similar_degree`, `effective_degree` along with `disjoint_set`

