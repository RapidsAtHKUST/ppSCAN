# ppSCAN

Parallel pSCAN algorithm with vectorized set intersection, compared with SCAN, pSCAN, SCAN++, anySCAN, SCAN-XP (an order of magnitude faster than the-state-of-the-arts), 250x faster than pSCAN on KNL (1.8 billion-edge-grap friendster). Please check out our ICPP'18 paper for further details. (ppSCAN is tested on both Multi-core Intel Xeon CPU and Intel Xeon Phi Processor Knights Landing)

## ppSCAN Codes RoadMap

content | directory
--- | ---
c++ parallel codes | see [ppSCAN](https://github.com/GraphProcessor/ppSCAN/tree/master/pSCAN-refactor)
vectorization codes (AVX2 / AVX512) | see [Graph.cpp](https://github.com/GraphProcessor/ppSCAN/blob/master/pSCAN-refactor/Graph.cpp)
experimental scripts and case studies | see [python_experiments](https://github.com/GraphProcessor/ppSCAN/tree/master/python_experiments)
python scripts for generating figures in paper | see [python paper-figures](https://github.com/GraphProcessor/ppSCAN/tree/master/paper)
SCAN variants codes | see [SCANVariants](SCANVariants): SCAN, pSCAN, SCAN++, anySCAN, SCAN-XP
python scripts during study | see [python_playground](python_playground): union-find, galloping-search, pSCAN

## Paper to Present

* Yulin Che, Shixuan Sun, Qiong Luo. 2018. Parallelizing Pruning-based Graph
Structural Clustering. In ICPP 2018: 47th International Conference on Parallel
Processing, August 13–16, 2018, Eugene, OR, USA. ACM, New York, NY, USA,
10 pages.

If you use our codes in your research, please kindly cite our paper. The BibTeX is as follows.

```
@inproceedings{DBLP:conf/icpp/CheSL18,
  author    = {Yulin Che and
               Shixuan Sun and
               Qiong Luo},
  title     = {Parallelizing Pruning-based Graph Structural Clustering},
  booktitle = {Proceedings of the 47th International Conference on Parallel Processing,
               {ICPP} 2018, Eugene, OR, USA, August 13-16, 2018},
  pages     = {77:1--77:10},
  year      = {2018},
  crossref  = {DBLP:conf/icpp/2018},
  url       = {http://doi.acm.org/10.1145/3225058.3225063},
  doi       = {10.1145/3225058.3225063},
  timestamp = {Thu, 09 Aug 2018 08:53:38 +0200},
  biburl    = {https://dblp.org/rec/bib/conf/icpp/CheSL18},
  bibsource = {dblp computer science bibliography, https://dblp.org}
}
```

## Other Information

[SCAN Preliminaries](scan_preliminary.md), [our ppSCAN overview](ppscan_overview.md) including design and optimizations.

## Refrences

* SCAN

Xu X, Yuruk N, Feng Z, et al. Scan: a structural clustering algorithm for networks[C]//Proceedings of the 13th ACM SIGKDD international conference on Knowledge discovery and data mining. ACM, 2007: 824-833.

* SCAN++

Shiokawa H, Fujiwara Y, Onizuka M. SCAN++: efficient algorithm for finding clusters, hubs and outliers on large-scale graphs[J]. Proceedings of the VLDB Endowment, 2015, 8(11): 1178-1189.

* pSCAN

Chang L, Li W, Qin L, et al. $\mathsf {pSCAN} $: Fast and Exact Structural Graph Clustering[J]. IEEE Transactions on Knowledge and Data Engineering, 2017, 29(2): 387-401.

* anySCAN

Mai S T, Dieu M S, Assent I, et al. Scalable and Interactive Graph Clustering Algorithm on Multicore CPUs[C]//Data Engineering (ICDE), 2017 IEEE 33rd International Conference on. IEEE, 2017: 349-360.

* SCAN-XP

Takahashi T, Shiokawa H, Kitagawa H. SCAN-XP: Parallel Structural Graph Clustering Algorithm on Intel Xeon Phi Coprocessors[C]//Proceedings of the 2nd International Workshop on Network Data Analytics. ACM, 2017: 6.

* GPU-SCAN (TPDS, journal)

Stovall T R, Kockara S, Avci R. GPUSCAN: GPU-Based Parallel Structural Clustering Algorithm for Networks[J]. IEEE Transactions on Parallel and Distributed Systems, 2015, 26(12): 3381-3393.
