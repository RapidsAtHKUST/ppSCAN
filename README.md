# pScan Optimizing

**Project**: pScan (Graph Clustering) Optimization.

**Target**: Exact, Efficient(Time/Space), Scalable(Parallel).

## My Work

Please have a look at [pSCAN-refactor](pSCAN-refactor) and find some useful things(source codes and documents) for you. :smile:

Experiments in detail are [here](python_experiments). 

Detail: [scalability-exp](python_experiments/figures/figures-case-study0), [robust-scalability-exp](python_experiments/figures/figures-case-study2-robust), [workload-exp](python_experiments/figures/figures-case-study1).

### Pscan and Pscan+ Overview

* pScan: extension of spatial dbScan to graph, in order to explore structural clusters, current research focus on **unweighted, undirected not dynamic** graph.

* two parameters: **density threshold**, **min number of neighbors**, input graph representation: csr representation.

* density function: `number of common neighbors` / `(sqrt(du * dv))`

* pScan basic concepts:
  * :thumbsup: : introduce `min_cn`, alternative values: reachable, not_reachable, min_cn for guaranteeing a core
  * not super userful: introduce `similar_degree`, `effective_degree` to help checking core, if a vertex is a core iff `sd <= number of reachable neighbors <=ed`, which can be used as a pruning rule

* pruning techniques:
  * :thumbsup: : pre-processing, `PruneAndCrossLink`, prune not reachable `if (((long long) a) * eps_b2 < ((long long) b) * eps_a2)`, which is easy to deduced when the max of`number of common neighbors` is `min(du,dv)`, and utilize cross-link
  * :thumbsup: : `common neighbor lower bound <=2`, must be similar, self and neighbor vertices, compute common neighbor with early exit, via utilizing `du` and `dv`
  * not super useful: other tricks to reduce the number of evaluations of reachability, like `max-priority-queue`, `similar_degree`, `effective_degree`

* **set-intersection** is the bottleneck in the current pScan design.

* detail: check the following table

description | folder link
--- | ---
pSCAN-fork-optimization | [pSCAN-refactor](pSCAN-refactor)
pSCAN-fork-statistics | [pSCAN-statistics](pSCAN-statistics)
python scripts during study | [python_playground](python_playground)
python experiments | [python_experiments](python_experiments)