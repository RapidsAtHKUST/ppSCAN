# pScan Optimizing

Project: pScan (Graph Clustering) Optimization

## My Work

* all my works

description | folder link
--- | ---
pSCAN-fork-optimization | [pSCAN-refactor](pSCAN-refactor)
python scripts during study | [python](python)
simd study codes | [simd_study](simd_study)

* pSCAN organization

pSCAN package further usage must follow [GPLv3 license](pSCAN-refactor/LICENSE).

file | utility
--- | ---
[ThreadPool.h](pSCAN-refactor/ThreadPool.h) | third-party thread pool, a simple impl
[MaxPriorityQueue.h](pSCAN-refactor/MaxPriorityQueue.h) | use array implementation to mimic heap
[DisjointSet.h](pSCAN-refactor/DisjointSet.h) | see CLRS for detail, for connected componet
[Graph.h](pSCAN-refactor/Graph.h) | graph representation and algorithm  related
[InputOutput.h](pSCAN-refactor/InputOutput.h) | read binary degree/adjacent edges utility

* some profiling information

see [run_pscan_gperftools.sh](pSCAN-refactor/run_pscan_gperftools.sh), [run_pscan_perf.sh](pSCAN-refactor/run_pscan_perf.sh), [run_pscan_valgrind.sh](pSCAN-refactor/run_pscan_valgrind.sh).

**set-intersection** is the bottleneck in the current pScan design.

## Related Projects

description | folder link
--- | ---
boost.simd repository | [boost.simd](boost.simd)
spatial dbscan sc2012 project | [dbscan-v1.0.0](dbscan-v1.0.0)
spatial dbscan another | [markus.goetz-hpdbscan](markus.goetz-hpdbscan)
parameter-free scan | [gSkeletonClu](gSkeletonClu)
pSCAN original codes | [pSCAN](pSCAN)

## Submodules Init

```
git submodule init
git submodule update
```
