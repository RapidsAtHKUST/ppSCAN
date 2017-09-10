# pScan Optimizing

description | folder link
--- | ---
pSCAN-fork-optimization | [pSCAN-refactor](pSCAN-refactor)
pSCAN-fork-statistics | [pSCAN-statistics](pSCAN-statistics)
python scripts during study | [python_playground](python_playground)
python experiments | [python_experiments](python_experiments)
related projects | [related_projects](related_projects)

## pSCAN Overview

pScan: extension of spatial dbScan to graph, in order to explore structural clusters, current research focus on **unweighted, undirected not dynamic** graph.

* two parameters: density threshold, min number of neighbors, input graph representation: csr representation. density function: `number of common neighbors` / `(sqrt(du * dv))`

* pScan basic concepts:
  * important: introduce `min_cn`, alternative values: reachable, not_reachable, min_cn for guaranteeing a core
  * not super userful: introduce `similar_degree`, `effective_degree` to help checking core, if a vertex is a core iff `sd <= number of reachable neighbors <=ed`, which can be used as a pruning rule

* pruning techniques:
  * important: pre-processing, `PruneAndCrossLink`, prune not reachable `if (((long long) a) * eps_b2 < ((long long) b) * eps_a2)`, which is easy to deduced when the max of`number of common neighbors` is `min(du,dv)`, and utilize cross-link
  * important: `common neighbor lower bound <=2`, must be similar, self and neighbor vertices, compute common neighbor with early exit, via utilizing `du` and `dv`
  * not super useful: other tricks to reduce the number of evaluations of reachability, like `max-priority-queue`, `similar_degree`, `effective_degree`



