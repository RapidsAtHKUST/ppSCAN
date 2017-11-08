## eps:0.1

overview | speedup
--- | ---
![webgraph_twitter-overview](../../figures/scalability_simd_paper/webgraph_twitter-eps:0.1-min_pts:5-overview.png) | ![webgraph_twitter-runtime-speedup](../../figures/scalability_simd_paper/webgraph_twitter-eps:0.1-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 36.072s | 553.41s | 424.194s | 21.185s | 21.212s | 1056.076s | 1.000
4 | 9.304s | 143.834s | 109.66s | 5.67s | 6.015s | 274.486s | 3.847
8 | 5.26s | 76.812s | 60.239s | 3.484s | 3.716s | 149.514s | 7.063
16 | 3.301s | 43.388s | 37.981s | 2.259s | 2.679s | 89.612s | 11.785
24 | 2.451s | 34.737s | 30.755s | 1.488s | 1.953s | 71.387s | 14.794
32 | 2.124s | 27.106s | 25.679s | 1.246s | 1.654s | 57.813s | 18.267
40 | 2.038s | 21.69s | 20.201s | 0.954s | 1.399s | 46.285s | 22.817

## eps:0.2

overview | speedup
--- | ---
![webgraph_twitter-overview](../../figures/scalability_simd_paper/webgraph_twitter-eps:0.2-min_pts:5-overview.png) | ![webgraph_twitter-runtime-speedup](../../figures/scalability_simd_paper/webgraph_twitter-eps:0.2-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 34.022s | 1455.058s | 487.741s | 8.967s | 16.355s | 2002.144s | 1.000
4 | 8.66s | 377.422s | 127.551s | 2.654s | 4.724s | 521.016s | 3.843
8 | 4.781s | 200.287s | 68.17s | 1.594s | 3.114s | 277.949s | 7.203
16 | 2.866s | 105.793s | 37.024s | 0.897s | 1.544s | 148.127s | 13.516
24 | 2.411s | 77.676s | 28.505s | 0.789s | 1.346s | 110.731s | 18.081
32 | 1.935s | 60.867s | 23.121s | 0.483s | 0.862s | 87.272s | 22.941
40 | 1.881s | 49.217s | 21.186s | 0.408s | 0.992s | 73.687s | 27.171

## eps:0.3

overview | speedup
--- | ---
![webgraph_twitter-overview](../../figures/scalability_simd_paper/webgraph_twitter-eps:0.3-min_pts:5-overview.png) | ![webgraph_twitter-runtime-speedup](../../figures/scalability_simd_paper/webgraph_twitter-eps:0.3-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 27.21s | 1612.194s | 135.397s | 3.58s | 9.484s | 1787.868s | 1.000
4 | 8.163s | 421.989s | 35.08s | 1.217s | 2.892s | 469.343s | 3.809
8 | 4.718s | 223.727s | 18.534s | 0.779s | 1.779s | 249.54s | 7.165
16 | 2.998s | 118.57s | 9.87s | 0.436s | 1.0s | 132.877s | 13.455
24 | 2.025s | 81.206s | 6.724s | 0.243s | 0.561s | 90.762s | 19.698
32 | 1.789s | 65.039s | 5.698s | 0.272s | 0.518s | 73.319s | 24.385
40 | 1.737s | 52.877s | 4.784s | 0.182s | 0.397s | 59.978s | 29.809

## eps:0.4

overview | speedup
--- | ---
![webgraph_twitter-overview](../../figures/scalability_simd_paper/webgraph_twitter-eps:0.4-min_pts:5-overview.png) | ![webgraph_twitter-runtime-speedup](../../figures/scalability_simd_paper/webgraph_twitter-eps:0.4-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 34.174s | 1208.189s | 30.541s | 2.597s | 4.468s | 1279.971s | 1.000
4 | 8.028s | 314.036s | 7.508s | 0.933s | 1.314s | 331.821s | 3.857
8 | 4.233s | 166.639s | 4.156s | 0.485s | 0.73s | 176.245s | 7.262
16 | 2.624s | 87.746s | 2.074s | 0.279s | 0.419s | 93.145s | 13.742
24 | 1.934s | 61.435s | 1.475s | 0.248s | 0.274s | 65.368s | 19.581
32 | 1.774s | 48.068s | 1.265s | 0.3s | 0.241s | 51.651s | 24.781
40 | 1.736s | 39.908s | 0.971s | 0.211s | 0.233s | 43.062s | 29.724

## eps:0.5

overview | speedup
--- | ---
![webgraph_twitter-overview](../../figures/scalability_simd_paper/webgraph_twitter-eps:0.5-min_pts:5-overview.png) | ![webgraph_twitter-runtime-speedup](../../figures/scalability_simd_paper/webgraph_twitter-eps:0.5-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 30.036s | 786.074s | 13.757s | 0.456s | 1.478s | 831.802s | 1.000
4 | 7.89s | 200.967s | 3.064s | 0.216s | 0.51s | 212.649s | 3.912
8 | 4.34s | 108.067s | 1.681s | 0.152s | 0.295s | 114.538s | 7.262
16 | 2.247s | 57.29s | 0.885s | 0.123s | 0.145s | 60.693s | 13.705
24 | 2.01s | 39.936s | 0.658s | 0.119s | 0.185s | 42.91s | 19.385
32 | 1.709s | 31.09s | 0.545s | 0.068s | 0.111s | 33.526s | 24.811
40 | 1.646s | 25.258s | 0.552s | 0.063s | 0.114s | 27.635s | 30.100

## eps:0.6

overview | speedup
--- | ---
![webgraph_twitter-overview](../../figures/scalability_simd_paper/webgraph_twitter-eps:0.6-min_pts:5-overview.png) | ![webgraph_twitter-runtime-speedup](../../figures/scalability_simd_paper/webgraph_twitter-eps:0.6-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 28.83s | 465.494s | 16.866s | 0.299s | 0.57s | 512.062s | 1.000
4 | 7.277s | 119.532s | 4.272s | 0.15s | 0.209s | 131.443s | 3.896
8 | 3.922s | 61.566s | 2.221s | 0.12s | 0.116s | 67.948s | 7.536
16 | 2.697s | 33.352s | 1.42s | 0.118s | 0.117s | 37.706s | 13.580
24 | 2.022s | 23.555s | 0.821s | 0.099s | 0.066s | 26.567s | 19.274
32 | 1.579s | 17.78s | 0.667s | 0.087s | 0.064s | 20.179s | 25.376
40 | 1.445s | 14.767s | 0.731s | 0.05s | 0.053s | 17.047s | 30.038

## eps:0.7

overview | speedup
--- | ---
![webgraph_twitter-overview](../../figures/scalability_simd_paper/webgraph_twitter-eps:0.7-min_pts:5-overview.png) | ![webgraph_twitter-runtime-speedup](../../figures/scalability_simd_paper/webgraph_twitter-eps:0.7-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 27.596s | 243.488s | 21.92s | 0.228s | 0.331s | 293.565s | 1.000
4 | 7.031s | 64.382s | 6.034s | 0.132s | 0.124s | 77.706s | 3.778
8 | 4.154s | 33.852s | 3.489s | 0.111s | 0.074s | 41.682s | 7.043
16 | 2.56s | 18.194s | 1.575s | 0.101s | 0.038s | 22.47s | 13.065
24 | 1.845s | 12.781s | 1.105s | 0.054s | 0.031s | 15.819s | 18.558
32 | 1.611s | 9.895s | 0.954s | 0.048s | 0.034s | 12.545s | 23.401
40 | 1.488s | 7.762s | 0.786s | 0.044s | 0.026s | 10.109s | 29.040

## eps:0.8

overview | speedup
--- | ---
![webgraph_twitter-overview](../../figures/scalability_simd_paper/webgraph_twitter-eps:0.8-min_pts:5-overview.png) | ![webgraph_twitter-runtime-speedup](../../figures/scalability_simd_paper/webgraph_twitter-eps:0.8-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 26.69s | 132.706s | 10.636s | 0.121s | 0.267s | 170.422s | 1.000
4 | 6.743s | 33.433s | 2.598s | 0.09s | 0.094s | 42.961s | 3.967
8 | 3.614s | 17.87s | 1.551s | 0.047s | 0.069s | 23.153s | 7.361
16 | 2.734s | 9.545s | 0.72s | 0.038s | 0.034s | 13.074s | 13.035
24 | 1.679s | 6.617s | 0.53s | 0.081s | 0.032s | 8.942s | 19.059
32 | 1.517s | 5.186s | 0.395s | 0.037s | 0.039s | 7.177s | 23.746
40 | 1.409s | 4.146s | 0.351s | 0.039s | 0.036s | 5.984s | 28.480

## eps:0.9

overview | speedup
--- | ---
![webgraph_twitter-overview](../../figures/scalability_simd_paper/webgraph_twitter-eps:0.9-min_pts:5-overview.png) | ![webgraph_twitter-runtime-speedup](../../figures/scalability_simd_paper/webgraph_twitter-eps:0.9-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 25.5s | 41.314s | 2.661s | 0.098s | 0.058s | 69.633s | 1.000
4 | 6.439s | 10.527s | 0.75s | 0.088s | 0.025s | 17.832s | 3.905
8 | 3.791s | 5.588s | 0.4s | 0.096s | 0.018s | 9.896s | 7.036
16 | 2.263s | 3.076s | 0.213s | 0.089s | 0.013s | 5.656s | 12.311
24 | 1.563s | 2.156s | 0.142s | 0.05s | 0.011s | 3.925s | 17.741
32 | 1.453s | 1.824s | 0.14s | 0.09s | 0.01s | 3.519s | 19.788
40 | 1.344s | 1.312s | 0.088s | 0.042s | 0.017s | 2.806s | 24.816

