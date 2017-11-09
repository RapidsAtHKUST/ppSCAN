## eps:0.1

overview | speedup
--- | ---
![snap_livejournal-overview](../../figures/scalability_simd_paper/snap_livejournal-eps:0.1-min_pts:5-overview.png) | ![snap_livejournal-runtime-speedup](../../figures/scalability_simd_paper/snap_livejournal-eps:0.1-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 13.067s | 8.386s | 2.839s | 9.244s | 3.419s | 36.956s | 1.000
2 | 6.864s | 4.246s | 1.471s | 5.03s | 1.988s | 19.603s | 1.885
4 | 3.447s | 2.162s | 0.744s | 2.642s | 1.214s | 10.211s | 3.619
8 | 1.822s | 1.186s | 0.43s | 1.425s | 0.874s | 5.739s | 6.439
16 | 1.068s | 0.563s | 0.22s | 0.673s | 0.554s | 3.08s | 11.999
32 | 0.684s | 0.333s | 0.172s | 0.517s | 0.439s | 2.147s | 17.213
64 | 0.398s | 0.272s | 0.213s | 0.448s | 0.436s | 1.769s | 20.891
128 | 0.412s | 0.299s | 0.167s | 0.394s | 0.525s | 1.799s | 20.543
256 | 0.468s | 0.462s | 0.226s | 0.696s | 0.569s | 2.423s | 15.252

## eps:0.2

overview | speedup
--- | ---
![snap_livejournal-overview](../../figures/scalability_simd_paper/snap_livejournal-eps:0.2-min_pts:5-overview.png) | ![snap_livejournal-runtime-speedup](../../figures/scalability_simd_paper/snap_livejournal-eps:0.2-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 12.797s | 22.119s | 4.529s | 6.654s | 4.312s | 50.414s | 1.000
2 | 6.708s | 11.259s | 2.33s | 3.6s | 2.474s | 26.374s | 1.912
4 | 3.411s | 5.637s | 1.167s | 1.87s | 1.417s | 13.505s | 3.733
8 | 1.683s | 2.843s | 0.581s | 0.975s | 0.921s | 7.006s | 7.196
16 | 0.935s | 1.412s | 0.337s | 0.67s | 0.559s | 3.915s | 12.877
32 | 0.63s | 0.729s | 0.195s | 0.375s | 0.377s | 2.309s | 21.834
64 | 0.341s | 0.58s | 0.192s | 0.36s | 0.357s | 1.832s | 27.519
128 | 0.396s | 0.529s | 0.24s | 0.346s | 0.475s | 1.989s | 25.346
256 | 0.375s | 0.635s | 0.293s | 0.574s | 0.567s | 2.446s | 20.611

## eps:0.3

overview | speedup
--- | ---
![snap_livejournal-overview](../../figures/scalability_simd_paper/snap_livejournal-eps:0.3-min_pts:5-overview.png) | ![snap_livejournal-runtime-speedup](../../figures/scalability_simd_paper/snap_livejournal-eps:0.3-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 12.265s | 26.82s | 3.003s | 4.071s | 3.885s | 50.045s | 1.000
2 | 6.476s | 13.68s | 1.514s | 2.159s | 2.154s | 25.986s | 1.926
4 | 3.244s | 6.922s | 0.756s | 1.204s | 1.183s | 13.312s | 3.759
8 | 1.755s | 3.408s | 0.397s | 0.588s | 0.761s | 6.911s | 7.241
16 | 1.004s | 1.791s | 0.281s | 0.453s | 0.476s | 4.007s | 12.489
32 | 0.573s | 0.977s | 0.147s | 0.308s | 0.35s | 2.357s | 21.232
64 | 0.418s | 0.595s | 0.128s | 0.259s | 0.283s | 1.685s | 29.700
128 | 0.293s | 0.567s | 0.169s | 0.346s | 0.374s | 1.753s | 28.548
256 | 0.452s | 0.678s | 0.212s | 0.408s | 0.46s | 2.212s | 22.624

## eps:0.4

overview | speedup
--- | ---
![snap_livejournal-overview](../../figures/scalability_simd_paper/snap_livejournal-eps:0.4-min_pts:5-overview.png) | ![snap_livejournal-runtime-speedup](../../figures/scalability_simd_paper/snap_livejournal-eps:0.4-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 11.572s | 25.079s | 1.994s | 2.167s | 2.552s | 43.366s | 1.000
2 | 6.118s | 12.806s | 1.002s | 1.15s | 1.412s | 22.489s | 1.928
4 | 3.09s | 6.461s | 0.545s | 0.607s | 0.811s | 11.516s | 3.766
8 | 1.525s | 3.241s | 0.249s | 0.373s | 0.475s | 5.866s | 7.393
16 | 0.97s | 1.678s | 0.145s | 0.247s | 0.346s | 3.388s | 12.800
32 | 0.507s | 0.888s | 0.1s | 0.171s | 0.225s | 1.893s | 22.909
64 | 0.362s | 0.624s | 0.109s | 0.184s | 0.216s | 1.498s | 28.949
128 | 0.404s | 0.389s | 0.142s | 0.27s | 0.304s | 1.512s | 28.681
256 | 0.355s | 0.752s | 0.219s | 0.298s | 0.411s | 2.037s | 21.289

## eps:0.5

overview | speedup
--- | ---
![snap_livejournal-overview](../../figures/scalability_simd_paper/snap_livejournal-eps:0.5-min_pts:5-overview.png) | ![snap_livejournal-runtime-speedup](../../figures/scalability_simd_paper/snap_livejournal-eps:0.5-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 10.735s | 20.249s | 1.455s | 1.001s | 1.261s | 34.703s | 1.000
2 | 5.638s | 10.4s | 0.733s | 0.526s | 0.692s | 17.99s | 1.929
4 | 2.845s | 5.254s | 0.392s | 0.282s | 0.391s | 9.165s | 3.786
8 | 1.469s | 2.706s | 0.179s | 0.182s | 0.266s | 4.805s | 7.222
16 | 0.972s | 1.382s | 0.11s | 0.135s | 0.169s | 2.77s | 12.528
32 | 0.491s | 0.762s | 0.075s | 0.109s | 0.129s | 1.568s | 22.132
64 | 0.376s | 0.548s | 0.1s | 0.132s | 0.133s | 1.292s | 26.860
128 | 0.379s | 0.534s | 0.136s | 0.166s | 0.207s | 1.423s | 24.387
256 | 0.521s | 0.584s | 0.156s | 0.178s | 0.27s | 1.711s | 20.282

## eps:0.6

overview | speedup
--- | ---
![snap_livejournal-overview](../../figures/scalability_simd_paper/snap_livejournal-eps:0.6-min_pts:5-overview.png) | ![snap_livejournal-runtime-speedup](../../figures/scalability_simd_paper/snap_livejournal-eps:0.6-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 9.811s | 14.611s | 1.195s | 0.551s | 0.531s | 26.701s | 1.000
2 | 5.098s | 7.491s | 0.602s | 0.294s | 0.289s | 13.776s | 1.938
4 | 2.562s | 3.787s | 0.299s | 0.173s | 0.166s | 6.989s | 3.820
8 | 1.452s | 1.954s | 0.165s | 0.116s | 0.106s | 3.796s | 7.034
16 | 0.712s | 0.971s | 0.084s | 0.095s | 0.074s | 1.939s | 13.771
32 | 0.482s | 0.612s | 0.077s | 0.086s | 0.067s | 1.327s | 20.121
64 | 0.344s | 0.408s | 0.07s | 0.098s | 0.096s | 1.018s | 26.229
128 | 0.418s | 0.515s | 0.113s | 0.102s | 0.131s | 1.281s | 20.844
256 | 0.446s | 0.532s | 0.136s | 0.165s | 0.201s | 1.482s | 18.017

## eps:0.7

overview | speedup
--- | ---
![snap_livejournal-overview](../../figures/scalability_simd_paper/snap_livejournal-eps:0.7-min_pts:5-overview.png) | ![snap_livejournal-runtime-speedup](../../figures/scalability_simd_paper/snap_livejournal-eps:0.7-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 8.357s | 9.536s | 0.989s | 0.389s | 0.257s | 19.53s | 1.000
2 | 4.443s | 4.91s | 0.506s | 0.212s | 0.14s | 10.212s | 1.912
4 | 2.236s | 2.467s | 0.282s | 0.128s | 0.084s | 5.2s | 3.756
8 | 1.169s | 1.261s | 0.146s | 0.086s | 0.054s | 2.718s | 7.185
16 | 0.748s | 0.621s | 0.072s | 0.078s | 0.049s | 1.569s | 12.447
32 | 0.498s | 0.425s | 0.063s | 0.067s | 0.048s | 1.103s | 17.706
64 | 0.356s | 0.303s | 0.097s | 0.077s | 0.067s | 0.903s | 21.628
128 | 0.381s | 0.265s | 0.104s | 0.089s | 0.099s | 0.94s | 20.777
256 | 0.451s | 0.485s | 0.14s | 0.137s | 0.167s | 1.382s | 14.132

## eps:0.8

overview | speedup
--- | ---
![snap_livejournal-overview](../../figures/scalability_simd_paper/snap_livejournal-eps:0.8-min_pts:5-overview.png) | ![snap_livejournal-runtime-speedup](../../figures/scalability_simd_paper/snap_livejournal-eps:0.8-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 6.925s | 5.474s | 0.783s | 0.312s | 0.182s | 13.679s | 1.000
2 | 3.727s | 2.819s | 0.403s | 0.169s | 0.096s | 7.216s | 1.896
4 | 1.879s | 1.436s | 0.234s | 0.107s | 0.056s | 3.714s | 3.683
8 | 1.073s | 0.76s | 0.119s | 0.074s | 0.039s | 2.068s | 6.615
16 | 0.576s | 0.399s | 0.065s | 0.069s | 0.035s | 1.146s | 11.936
32 | 0.371s | 0.316s | 0.075s | 0.064s | 0.039s | 0.867s | 15.777
64 | 0.281s | 0.256s | 0.076s | 0.079s | 0.058s | 0.752s | 18.190
128 | 0.176s | 0.255s | 0.091s | 0.081s | 0.094s | 0.699s | 19.569
256 | 0.442s | 0.315s | 0.124s | 0.124s | 0.147s | 1.156s | 11.833

## eps:0.9

overview | speedup
--- | ---
![snap_livejournal-overview](../../figures/scalability_simd_paper/snap_livejournal-eps:0.9-min_pts:5-overview.png) | ![snap_livejournal-runtime-speedup](../../figures/scalability_simd_paper/snap_livejournal-eps:0.9-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 5.478s | 2.514s | 0.498s | 0.22s | 0.16s | 8.872s | 1.000
2 | 2.988s | 1.279s | 0.255s | 0.123s | 0.084s | 4.731s | 1.875
4 | 1.512s | 0.64s | 0.155s | 0.083s | 0.048s | 2.44s | 3.636
8 | 0.918s | 0.37s | 0.077s | 0.06s | 0.034s | 1.461s | 6.073
16 | 0.527s | 0.208s | 0.048s | 0.052s | 0.031s | 0.868s | 10.221
32 | 0.299s | 0.133s | 0.063s | 0.059s | 0.035s | 0.592s | 14.986
64 | 0.273s | 0.172s | 0.071s | 0.062s | 0.053s | 0.634s | 13.994
128 | 0.196s | 0.179s | 0.073s | 0.073s | 0.086s | 0.609s | 14.568
256 | 0.247s | 0.228s | 0.098s | 0.111s | 0.147s | 0.833s | 10.651

