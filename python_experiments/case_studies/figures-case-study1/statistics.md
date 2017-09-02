## Statistics

* **eval comparison table**
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