## Observations

* webbase requires much fewer computations from **eval comparison table** and **`cmp`/`intersection_times` table** 

* dblp, webbase, check core 1st bsp(parallel eval) speedup degradation in 40 threads, but the runtime is still acceptable, since much less than i/o time

* webbase, check core 2nd bsp(binary search) speedup degradation in 40 threads, but the runtime is still acceptable, since much less than i/o time

* parallel prune speedup scales well vertically

## Need to Explanation

### Problems to Solve

dataset | detail
--- | ---
**dblp** | 16->40, prune not improve any more, check core 1st phase bsp becomes slower
pokec | 32->40, prune and check core 2nd phase becomes slower
livejournal | 24->32->40, check core 2nd phase bsp slower and faster
orkut | 32->40, check core 2nd phase bsp slower
**webbase** | 24->32->40, check core 1st and 2nd bsp slower

### Explanations

* dblp: casued by ``, from vtune report, system call cost, probably due to small tasks influencing behaviors of the thread pool, same problem with webbase

* webbase: task grain differs a lot, it seems 64 vertex as a task too small for check core 1st phase comp

* pokec, livejournal, orkut: maybe the same, we are required to assign tasks based on real work load, considering pruning, different degree of vertices, different degree of dst vertices

### Trade-off Problem

* Task too large: load imbalance, cache locality issue

* Task too small: system call overheads, queue access concurrency issue, memory cost for more function objects(maybe okay)