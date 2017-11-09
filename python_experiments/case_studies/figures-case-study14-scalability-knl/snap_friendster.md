## eps:0.1

overview | speedup
--- | ---
![snap_friendster-overview](../../figures/scalability_simd_paper/snap_friendster-eps:0.1-min_pts:5-overview.png) | ![snap_friendster-runtime-speedup](../../figures/scalability_simd_paper/snap_friendster-eps:0.1-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 770.994s | 4612.198s | 1248.038s | 596.451s | 280.265s | 7507.956s | 1.000
2 | 390.378s | 2316.805s | 627.369s | 307.199s | 151.554s | 3793.316s | 1.979
4 | 201.092s | 1155.001s | 311.549s | 157.04s | 80.004s | 1904.695s | 3.942
8 | 100.088s | 576.029s | 154.545s | 79.658s | 44.674s | 955.006s | 7.862
16 | 50.299s | 291.005s | 78.099s | 40.617s | 29.63s | 489.661s | 15.333
32 | 25.631s | 146.274s | 39.751s | 21.287s | 20.159s | 253.114s | 29.662
64 | 13.749s | 75.206s | 20.388s | 12.526s | 11.352s | 133.236s | 56.351
128 | 8.827s | 42.469s | 11.494s | 8.847s | 10.82s | 82.469s | 91.040
256 | 6.755s | 32.983s | 9.231s | 6.685s | 13.341s | 69.006s | 108.801

