## Observation

### Workload    

* pscan's prunning is powerful in non social networks with `eps:0.1` and `eps:0.2` 

### Scalability

* pscan's prunning(via sd/ed) is more powerful in non social network graphs and lfr benchmark graphs, from [figures/figures-case-study0](../figures/figures-case-study0), 
however at most twice evaluation of density from our experiments is affordable, since evaluation phase, i.e, check-core 1st phase bsp has a good scalability 
except for webbase and small datasets.

* **eval comparison table**, parameter setting: `eps:0.3`, `min_pts:5`
 on 9 graphs.

algorithm | dblp | pokec | livejournal | orkut | uk | webbase | twitter | frienster | lfr10m
--- | --- | --- | --- | --- | --- | --- | --- | --- | ---
pscan   | 0.6177 | 0.8520 | 0.7243 | 0.8265	| **0.2439** | **0.2874** | 0.4471 | 0.8566 | **0.5192**
pscan+  | 0.7370 | 0.9209 | 0.8740 | 0.9278 | **0.5175** | **0.5238** | 0.4866 | 0.8764 | **1.0000**

* **`cmp`/`intersection_times` table** 

algorithm | dblp | pokec | livejournal | orkut | uk | webbase | twitter | frienster | lfr10m
--- | --- | --- | --- | --- | --- | --- | --- | --- | ---
pscan | 24.707 | 70.888 | 106.707 | 275.800 | 111.962 | **63.901** | 1841.663 | 385.317 | **22.978**
pscan+ | 23.422 | 67.641 | 101.258 | 264.468 | 146.264 | **88.889** | 1849.282 | 377.788 | **24.199**

* lfr benchmark synthetic graph: lfr benchmark generator very slow, unable to generate large graph efficiently, consider use 10m, 1m vertices, instead of 100m 

* real-world graph more parameters: use more small real-world datasets to do work-load experiments, with different `eps [0,1, 0.9]` and `min_pts [2, 5, 15, 20]`
