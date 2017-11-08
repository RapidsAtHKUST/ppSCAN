## eps:0.1

overview | speedup
--- | ---
![snap_orkut-overview](../../figures/scalability_simd_paper/snap_orkut-eps:0.1-min_pts:5-overview.png) | ![snap_orkut-runtime-speedup](../../figures/scalability_simd_paper/snap_orkut-eps:0.1-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 43.462s | 110.792s | 44.49s | 30.4s | 12.301s | 241.448s | 1.000
2 | 23.846s | 55.918s | 22.572s | 16.88s | 6.945s | 126.163s | 1.914
4 | 11.577s | 27.959s | 11.263s | 8.31s | 3.265s | 62.377s | 3.871
8 | 5.823s | 13.997s | 5.721s | 4.19s | 1.858s | 31.591s | 7.643
16 | 3.03s | 7.117s | 2.799s | 2.261s | 1.179s | 16.388s | 14.733
32 | 1.645s | 3.559s | 1.649s | 1.308s | 0.908s | 9.071s | 26.618
64 | 1.052s | 2.221s | 1.095s | 0.862s | 0.604s | 5.837s | 41.365
128 | 0.7s | 1.619s | 0.802s | 0.766s | 0.946s | 4.836s | 49.927
256 | 0.85s | 1.342s | 0.609s | 1.11s | 1.039s | 4.953s | 48.748

## eps:0.2

overview | speedup
--- | ---
![snap_orkut-overview](../../figures/scalability_simd_paper/snap_orkut-eps:0.2-min_pts:5-overview.png) | ![snap_orkut-runtime-speedup](../../figures/scalability_simd_paper/snap_orkut-eps:0.2-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 42.823s | 210.546s | 35.03s | 18.17s | 13.601s | 320.173s | 1.000
2 | 23.557s | 106.086s | 17.643s | 10.022s | 7.338s | 164.648s | 1.945
4 | 11.827s | 53.32s | 8.864s | 5.074s | 3.67s | 82.757s | 3.869
8 | 5.7s | 26.703s | 4.368s | 2.349s | 1.862s | 40.985s | 7.812
16 | 2.879s | 13.543s | 2.34s | 1.333s | 1.218s | 21.315s | 15.021
32 | 1.727s | 6.7s | 1.284s | 0.814s | 0.831s | 11.359s | 28.187
64 | 1.034s | 3.803s | 0.823s | 0.582s | 0.578s | 6.822s | 46.932
128 | 0.74s | 2.494s | 0.649s | 0.589s | 0.711s | 5.185s | 61.750
256 | 0.795s | 1.887s | 0.69s | 0.908s | 0.923s | 5.206s | 61.501

## eps:0.3

overview | speedup
--- | ---
![snap_orkut-overview](../../figures/scalability_simd_paper/snap_orkut-eps:0.3-min_pts:5-overview.png) | ![snap_orkut-runtime-speedup](../../figures/scalability_simd_paper/snap_orkut-eps:0.3-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 42.165s | 226.166s | 20.991s | 9.169s | 10.849s | 309.342s | 1.000
2 | 23.108s | 114.138s | 10.539s | 4.96s | 5.742s | 158.489s | 1.952
4 | 11.351s | 57.451s | 5.39s | 2.487s | 2.968s | 79.65s | 3.884
8 | 5.635s | 28.73s | 2.739s | 1.309s | 1.668s | 40.084s | 7.717
16 | 2.963s | 14.395s | 1.593s | 0.797s | 0.862s | 20.612s | 15.008
32 | 1.525s | 7.229s | 1.007s | 0.408s | 0.686s | 10.857s | 28.492
64 | 1.069s | 4.05s | 0.544s | 0.363s | 0.505s | 6.533s | 47.351
128 | 0.873s | 2.616s | 0.49s | 0.48s | 0.465s | 4.926s | 62.798
256 | 0.711s | 2.035s | 0.748s | 0.598s | 0.735s | 4.828s | 64.072

## eps:0.4

overview | speedup
--- | ---
![snap_orkut-overview](../../figures/scalability_simd_paper/snap_orkut-eps:0.4-min_pts:5-overview.png) | ![snap_orkut-runtime-speedup](../../figures/scalability_simd_paper/snap_orkut-eps:0.4-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 40.591s | 207.086s | 12.841s | 3.837s | 6.492s | 270.849s | 1.000
2 | 22.485s | 104.762s | 6.379s | 2.021s | 3.366s | 139.015s | 1.948
4 | 11.118s | 52.466s | 3.17s | 1.01s | 1.71s | 69.476s | 3.898
8 | 5.459s | 26.468s | 1.72s | 0.567s | 0.952s | 35.168s | 7.702
16 | 2.787s | 13.328s | 0.897s | 0.358s | 0.548s | 17.921s | 15.113
32 | 1.599s | 6.571s | 0.517s | 0.229s | 0.378s | 9.297s | 29.133
64 | 1.133s | 3.706s | 0.341s | 0.228s | 0.314s | 5.725s | 47.310
128 | 0.713s | 2.479s | 0.379s | 0.316s | 0.346s | 4.235s | 63.955
256 | 0.748s | 1.79s | 0.612s | 0.401s | 0.56s | 4.113s | 65.852

## eps:0.5

overview | speedup
--- | ---
![snap_orkut-overview](../../figures/scalability_simd_paper/snap_orkut-eps:0.5-min_pts:5-overview.png) | ![snap_orkut-runtime-speedup](../../figures/scalability_simd_paper/snap_orkut-eps:0.5-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 38.735s | 168.384s | 7.549s | 0.775s | 2.326s | 217.772s | 1.000
2 | 21.283s | 84.899s | 3.702s | 0.41s | 1.206s | 111.503s | 1.953
4 | 10.49s | 42.729s | 1.833s | 0.225s | 0.656s | 55.934s | 3.893
8 | 5.203s | 21.364s | 1.071s | 0.151s | 0.381s | 28.171s | 7.730
16 | 2.887s | 10.645s | 0.532s | 0.097s | 0.257s | 14.42s | 15.102
32 | 1.71s | 5.311s | 0.311s | 0.095s | 0.176s | 7.605s | 28.635
64 | 1.034s | 3.066s | 0.217s | 0.11s | 0.175s | 4.605s | 47.290
128 | 0.563s | 2.145s | 0.293s | 0.123s | 0.225s | 3.351s | 64.987
256 | 0.963s | 1.499s | 0.313s | 0.168s | 0.363s | 3.308s | 65.832

## eps:0.6

overview | speedup
--- | ---
![snap_orkut-overview](../../figures/scalability_simd_paper/snap_orkut-eps:0.6-min_pts:5-overview.png) | ![snap_orkut-runtime-speedup](../../figures/scalability_simd_paper/snap_orkut-eps:0.6-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 35.672s | 121.983s | 5.276s | 0.132s | 0.499s | 163.564s | 1.000
2 | 19.755s | 62.014s | 2.594s | 0.076s | 0.257s | 84.698s | 1.931
4 | 10.013s | 31.352s | 1.326s | 0.049s | 0.153s | 42.895s | 3.813
8 | 4.661s | 15.649s | 0.72s | 0.045s | 0.081s | 21.158s | 7.731
16 | 2.467s | 7.835s | 0.373s | 0.041s | 0.068s | 10.786s | 15.164
32 | 1.469s | 3.987s | 0.223s | 0.046s | 0.065s | 5.793s | 28.235
64 | 0.945s | 2.349s | 0.195s | 0.05s | 0.092s | 3.633s | 45.022
128 | 0.654s | 1.621s | 0.229s | 0.066s | 0.114s | 2.686s | 60.895
256 | 0.755s | 1.329s | 0.324s | 0.097s | 0.166s | 2.673s | 61.191

## eps:0.7

overview | speedup
--- | ---
![snap_orkut-overview](../../figures/scalability_simd_paper/snap_orkut-eps:0.7-min_pts:5-overview.png) | ![snap_orkut-runtime-speedup](../../figures/scalability_simd_paper/snap_orkut-eps:0.7-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 31.889s | 79.748s | 4.186s | 0.028s | 0.07s | 115.924s | 1.000
2 | 17.354s | 40.491s | 2.026s | 0.025s | 0.044s | 59.941s | 1.934
4 | 8.832s | 20.444s | 1.006s | 0.026s | 0.022s | 30.333s | 3.822
8 | 4.255s | 10.217s | 0.488s | 0.028s | 0.025s | 15.016s | 7.720
16 | 2.297s | 5.157s | 0.323s | 0.031s | 0.022s | 7.832s | 14.801
32 | 1.486s | 2.581s | 0.178s | 0.034s | 0.03s | 4.311s | 26.890
64 | 0.852s | 1.671s | 0.146s | 0.042s | 0.043s | 2.756s | 42.062
128 | 0.455s | 1.146s | 0.198s | 0.055s | 0.07s | 1.927s | 60.158
256 | 0.872s | 1.202s | 0.214s | 0.091s | 0.133s | 2.514s | 46.111

## eps:0.8

overview | speedup
--- | ---
![snap_orkut-overview](../../figures/scalability_simd_paper/snap_orkut-eps:0.8-min_pts:5-overview.png) | ![snap_orkut-runtime-speedup](../../figures/scalability_simd_paper/snap_orkut-eps:0.8-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 26.577s | 44.431s | 3.24s | 0.018s | 0.026s | 74.293s | 1.000
2 | 14.666s | 22.671s | 1.594s | 0.019s | 0.015s | 38.968s | 1.907
4 | 7.481s | 11.474s | 0.821s | 0.019s | 0.012s | 19.809s | 3.750
8 | 3.655s | 5.725s | 0.446s | 0.021s | 0.011s | 9.86s | 7.535
16 | 1.957s | 3.093s | 0.2s | 0.026s | 0.016s | 5.295s | 14.031
32 | 1.14s | 1.516s | 0.143s | 0.036s | 0.026s | 2.864s | 25.940
64 | 0.754s | 0.926s | 0.12s | 0.049s | 0.034s | 1.886s | 39.392
128 | 0.493s | 0.657s | 0.167s | 0.056s | 0.064s | 1.44s | 51.592
256 | 0.59s | 0.763s | 0.212s | 0.101s | 0.122s | 1.792s | 41.458

## eps:0.9

overview | speedup
--- | ---
![snap_orkut-overview](../../figures/scalability_simd_paper/snap_orkut-eps:0.9-min_pts:5-overview.png) | ![snap_orkut-runtime-speedup](../../figures/scalability_simd_paper/snap_orkut-eps:0.9-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 20.732s | 18.108s | 2.028s | 0.017s | 0.023s | 40.909s | 1.000
2 | 11.843s | 9.165s | 1.005s | 0.018s | 0.012s | 22.045s | 1.856
4 | 6.02s | 4.699s | 0.535s | 0.018s | 0.008s | 11.282s | 3.626
8 | 2.819s | 2.363s | 0.276s | 0.02s | 0.012s | 5.494s | 7.446
16 | 1.362s | 1.206s | 0.16s | 0.023s | 0.01s | 2.763s | 14.806
32 | 0.973s | 0.74s | 0.094s | 0.03s | 0.016s | 1.856s | 22.041
64 | 0.588s | 0.429s | 0.092s | 0.038s | 0.033s | 1.182s | 34.610
128 | 0.439s | 0.34s | 0.131s | 0.054s | 0.06s | 1.027s | 39.833
256 | 0.435s | 0.532s | 0.163s | 0.089s | 0.126s | 1.347s | 30.370

