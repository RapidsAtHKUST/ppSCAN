# pScan Optimizing

**Project**: pScan (Graph Clustering) Optimization.

**Target**: Exact, Efficient(Time/Space), Scalable(Parallel).

## My Work

Please have a look at [pSCAN-refactor](pSCAN-refactor) and find some useful things(source codes and documents) for you. :smile:

Experiments in detail are [here](python_experiments).

### Pscan Overview

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
python scripts during study | [python](python)
simd study codes | [simd_study](simd_study)

### Package Organization, [pSCAN-refactor](pSCAN-refactor)

pSCAN package further usage must follow [GPLv3 license](pSCAN-refactor/LICENSE).

* pSCAN-fork-optimization implementations follow modern cpp rules, to make things easier without loss of performance.

file | utility
--- | ---
[ThreadPool.h](pSCAN-refactor/ThreadPool.h) | third-party thread pool, a simple impl
[DisjointSet.h](pSCAN-refactor/DisjointSet.h), [DisjointSet.cpp](pSCAN-refactor/DisjointSet.cpp) | see CLRS for detail, for connected componet
[Graph.h](pSCAN-refactor/Graph.h), [Graph.cpp](pSCAN-refactor/Graph.cpp) | graph representation and algorithm  related
[InputOutput.h](pSCAN-refactor/InputOutput.h), [InputOutput.cpp](pSCAN-refactor/InputOutput.cpp) | read binary degree/adjacent edges utility

* utils and plays

file | utility
--- | ---
[pretty_print.h](pSCAN-refactor/playground/pretty_print.h) | third-party pretty print utilities
[graph_io.cpp](pSCAN-refactor/playground/graph_io.cpp) | play ground about i/o
[play.cpp](pSCAN-refactor/playground/play.cpp) | play ground others

* experimental(parallel version with adjustable thread num)

see [pSCAN-refactor/experimental](pSCAN-refactor/experimental).

* deprecated

file | utility
--- | ---
[MaxPriorityQueue.h](pSCAN-refactor/playground/MaxPriorityQueue.h)，[MaxPriorityQueue.cpp](pSCAN-refactor/playground/MaxPriorityQueue.cpp) | not useful for pruning, deprecated now
[ThreadSafeDisjointSet.h](pSCAN-refactor/playground/ThreadSafeDisjointSet.h) | not useful after change some codes to make it simpler, deprecated now

* profiling scrips

see [run_pscan_gperftools.sh](pSCAN-refactor/run_pscan_gperftools.sh), [run_pscan_perf.sh](pSCAN-refactor/run_pscan_perf.sh), [run_pscan_valgrind.sh](pSCAN-refactor/run_pscan_valgrind.sh) and this post [profiler tutorial](http://gernotklingler.com/blog/gprof-valgrind-gperftools-evaluation-tools-application-level-cpu-profiling-linux/).


## Related Projects

description | folder link
--- | ---
spatial dbscan sc2012 project | [dbscan-v1.0.0](related_projects/dbscan-v1.0.0)
spatial dbscan another | [markus.goetz-hpdbscan](related_projects/markus.goetz-hpdbscan)
parameter-free scan | [gSkeletonClu](related_projects/gSkeletonClu)
pSCAN original codes | [pSCAN](related_projects/pSCAN)

* Submodules Init

```
git submodule init
git submodule update
```
