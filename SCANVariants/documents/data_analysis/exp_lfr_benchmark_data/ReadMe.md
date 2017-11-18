## 10Million - AvgDeg 15 

### Desktop

pscan 

```zsh
**** Graph Clustering (Release): dataset/lfr_benchmark/10million_avgdeg15_maxdeg50_Cdefault, 0.3, 5 *** 
        n = 10000001; m = 152826874
        *** Finished loading graph!
intersection times:39674475
cmp0:159121647
cmp1:644629816
equal cmp:107880042
max portion:47
Total time without IO: 33063765
```

statistics

```zsh
int size:4
n:10000001, m:152826874
read degree file time:27 ms
read adjacency list file time:438 ms
check input graph file time:180 ms

Total input cost:844 ms
with google perf start------------
1st: prune execution time:2458 ms
2nd: check core first-phase bsp time:21559 ms
2nd: check core second-phase bsp time:6948 ms
3rd: core clustering time:5544 ms
4th: non-core clustering time:1664 ms

prune0 definitely not reachable:0
prune1 definitely reachable:0
intersection times:76413437
cmp0:247555261
cmp1:1283030908
equal cmp:318549472
max portion:47
with google perf end--------------
PROFILE: interrupts/evictions/bytes = 3809/2432/156528
Total time without IO:38180 ms
Total output cost:2288 ms
```

parallel

```zsh
int size:4
n:10000001, m:152826874
read degree file time:30 ms
read adjacency list file time:447 ms
check input graph file time:183 ms

Total input cost:856 ms
with google perf start------------
1st: prune execution time:561 ms
2nd: check core first-phase bsp time:3126 ms
2nd: check core second-phase bsp time:1626 ms
3rd: core clustering time:5619 ms
4th: non-core clustering time:1668 ms
with google perf end--------------
PROFILE: interrupts/evictions/bytes = 4832/358/42896
Total time without IO:12604 ms
Total output cost:2310 ms
```

### Cluster: gpu-23

pscan

```zsh
**** Graph Clustering (Release): dataset/lfr_benchmark/10million_avgdeg15_maxdeg50_Cdefault, 0.3, 5 *** 
        n = 10000001; m = 152826874
        *** Finished loading graph!
intersection times:39674475
cmp0:159121647
cmp1:644629816
equal cmp:107880042
max portion:47
Total time without IO: 32655852
        *** Start write result into disk!
```

statistics

```zsh
int size:4
n:10000001, m:152826874
read degree file time:40 ms
read adjacency list file time:5182 ms
check input graph file time:305 ms

Total input cost:5987 ms
with google perf start------------
1st: prune execution time:2324 ms
2nd: check core first-phase bsp time:22796 ms
2nd: check core second-phase bsp time:6089 ms
3rd: core clustering time:5117 ms
4th: non-core clustering time:1293 ms

prune0 definitely not reachable:0
prune1 definitely reachable:0
intersection times:76413437
cmp0:247555261
cmp1:1283030908
equal cmp:318549472
max portion:47
with google perf end--------------
PROFILE: interrupts/evictions/bytes = 3761/2892/161248
Total time without IO:37634 ms
Total output cost:9691 ms
```

parallel

```zsh
int size:4
n:10000001, m:152826874
read degree file time:98 ms
read adjacency list file time:5996 ms
check input graph file time:280 ms

Total input cost:6823 ms
with google perf start------------
1st: prune execution time:206 ms
2nd: check core first-phase bsp time:1156 ms
2nd: check core second-phase bsp time:571 ms
3rd: core clustering time:7311 ms
4th: non-core clustering time:1797 ms
with google perf end--------------
PROFILE: interrupts/evictions/bytes = 7203/2435/252904
Total time without IO:11070 ms
Total output cost:9596 ms
```