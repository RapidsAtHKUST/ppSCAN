## Problem

* urgent: need to interpret the performance degradation in webbase.

* must: need to elaborate on why it is not worthwhile to parallelize clustering phase(core clustering and non-core clustering), mention io cost

* must: need to interpret the speedup difference among different input graphs

* small problem: need to elaborate on comparison difference and union-find operation difference, implement more about statistics, give detailed information for workloads

* small problem: need to analyze why lfr benchmark incurs more cost in union-find disjoint-set operation

## Problem Elaboration

### Webbase Performance Degradation

overview | speedup
--- | ---
![webbase-overview](../figures/scalability/webgraph_webbase-eps:0.3-min_pts:5-overview.png) | ![webbase-speedup](../figures/scalability/webgraph_webbase-eps:0.3-min_pts:5-runtime-speedup.png)

### Not Worthwhile Parallelize Union-Find

e.g, lfr-10million-avg15 synthetic graph, i/o cost is much bigger than 3rd/4th serial part

```zsh
int size:4
n:10000001, m:152826874
read degree file time:40 ms
read adjacency list file time:5120 ms
check input graph file time:298 ms

Total input cost:5938 ms
1st: prune execution time:194 ms
2nd: check core first-phase bsp time:1181 ms
2nd: check core second-phase bsp time:623 ms
3rd: core clustering time:5292 ms
4th: non-core clustering time:1289 ms
Total time without IO:8580 ms
Total output cost:10649 ms
```

### LFR Benchmark More Workload in Union-Find

overview | speedup
--- | ---
![lfr-10million-avg15-overview](../figures/scalability/lfr_10million_avgdeg15-eps:0.3-min_pts:5-overview.png) | ![lfr-10million-avg15-speedup](../figures/scalability/lfr_10million_avgdeg15-eps:0.3-min_pts:5-runtime-speedup.png)

reason: zero-pruning in both prune0 and prune1

```
int size:4
n:10000001, m:152826874
read degree file time:210 ms
read adjacency list file time:6301 ms
check input graph file time:305 ms

Total input cost:7336 ms
with google perf start------------
1st: prune execution time:2250 ms
2nd: check core first-phase bsp time:22887 ms
2nd: check core second-phase bsp time:6086 ms
3rd: core clustering time:5106 ms
4th: non-core clustering time:1288 ms

prune0 definitely not reachable:0
prune1 definitely reachable:0
intersection times:76413437
cmp0:247555261
cmp1:1283030908
equal cmp:318549472
max portion:47
with google perf end--------------
Total time without IO:37632 ms
Total output cost:9880 ms
```