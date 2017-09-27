## eps:0.1

overview | speedup
--- | ---
![webgraph_twitter-overview](../../figures/scalability_new2_opt_scheduler/webgraph_twitter-eps:0.1-min_pts:5-overview.png) | ![webgraph_twitter-runtime-speedup](../../figures/scalability_new2_opt_scheduler/webgraph_twitter-eps:0.1-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 34.452s | 587.953s | 441.488s | 16.103s | 21.398s | 1101.665s | 1.000
4 | 9.711s | 156.993s | 114.798s | 15.681s | 10.871s | 308.242s | 3.574
8 | 5.379s | 83.758s | 62.399s | 15.815s | 9.412s | 176.951s | 6.226
16 | 3.208s | 49.415s | 39.733s | 15.147s | 8.543s | 116.233s | 9.478
24 | 2.46s | 39.289s | 32.773s | 15.121s | 7.899s | 97.728s | 11.273
32 | 2.149s | 30.692s | 27.185s | 15.737s | 8.017s | 83.967s | 13.120
40 | 1.991s | 26.85s | 22.899s | 15.099s | 8.27s | 75.297s | 14.631

## eps:0.2

overview | speedup
--- | ---
![webgraph_twitter-overview](../../figures/scalability_new2_opt_scheduler/webgraph_twitter-eps:0.2-min_pts:5-overview.png) | ![webgraph_twitter-runtime-speedup](../../figures/scalability_new2_opt_scheduler/webgraph_twitter-eps:0.2-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 49.666s | 1496.492s | 469.479s | 8.093s | 17.489s | 2041.952s | 1.000
4 | 8.712s | 399.669s | 122.119s | 8.015s | 7.404s | 546.101s | 3.739
8 | 4.885s | 211.861s | 65.072s | 7.828s | 5.833s | 295.661s | 6.906
16 | 3.022s | 117.932s | 37.761s | 7.448s | 4.849s | 171.196s | 11.928
24 | 2.256s | 91.496s | 30.413s | 7.848s | 4.267s | 136.463s | 14.963
32 | 2.1s | 71.496s | 23.68s | 7.193s | 4.212s | 108.86s | 18.758
40 | 1.89s | 62.53s | 21.088s | 7.295s | 4.15s | 97.135s | 21.022

## eps:0.3

overview | speedup
--- | ---
![webgraph_twitter-overview](../../figures/scalability_new2_opt_scheduler/webgraph_twitter-eps:0.3-min_pts:5-overview.png) | ![webgraph_twitter-runtime-speedup](../../figures/scalability_new2_opt_scheduler/webgraph_twitter-eps:0.3-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 28.076s | 1682.042s | 132.079s | 3.73s | 10.506s | 1856.567s | 1.000
4 | 7.812s | 445.339s | 34.07s | 3.779s | 4.269s | 495.402s | 3.748
8 | 4.57s | 234.624s | 18.06s | 3.684s | 3.079s | 264.148s | 7.029
16 | 3.006s | 130.512s | 10.036s | 3.759s | 2.081s | 149.529s | 12.416
24 | 1.998s | 99.7s | 7.704s | 2.996s | 2.551s | 115.081s | 16.133
32 | 2.076s | 76.949s | 6.336s | 3.556s | 2.465s | 91.516s | 20.287
40 | 1.756s | 69.383s | 5.558s | 3.025s | 2.071s | 81.925s | 22.662

## eps:0.4

overview | speedup
--- | ---
![webgraph_twitter-overview](../../figures/scalability_new2_opt_scheduler/webgraph_twitter-eps:0.4-min_pts:5-overview.png) | ![webgraph_twitter-runtime-speedup](../../figures/scalability_new2_opt_scheduler/webgraph_twitter-eps:0.4-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 28.331s | 1269.974s | 28.989s | 3.048s | 5.405s | 1335.857s | 1.000
4 | 7.466s | 336.046s | 7.345s | 2.972s | 2.269s | 356.207s | 3.750
8 | 4.173s | 175.503s | 4.182s | 3.072s | 1.634s | 188.673s | 7.080
16 | 2.602s | 97.353s | 2.204s | 2.277s | 1.791s | 106.336s | 12.563
24 | 2.283s | 73.854s | 1.717s | 2.923s | 1.817s | 82.705s | 16.152
32 | 1.865s | 58.014s | 1.349s | 2.836s | 1.761s | 65.934s | 20.261
40 | 1.702s | 51.247s | 1.172s | 2.271s | 1.744s | 58.245s | 22.935

## eps:0.5

overview | speedup
--- | ---
![webgraph_twitter-overview](../../figures/scalability_new2_opt_scheduler/webgraph_twitter-eps:0.5-min_pts:5-overview.png) | ![webgraph_twitter-runtime-speedup](../../figures/scalability_new2_opt_scheduler/webgraph_twitter-eps:0.5-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 30.524s | 828.787s | 11.82s | 0.832s | 2.407s | 874.891s | 1.000
4 | 6.911s | 218.534s | 3.133s | 0.719s | 1.115s | 230.549s | 3.795
8 | 4.13s | 115.344s | 1.672s | 0.692s | 0.915s | 122.887s | 7.119
16 | 2.619s | 63.253s | 0.916s | 0.759s | 0.86s | 68.541s | 12.764
24 | 1.633s | 47.465s | 0.635s | 0.437s | 0.82s | 51.091s | 17.124
32 | 1.628s | 37.666s | 0.557s | 0.53s | 0.911s | 41.403s | 21.131
40 | 1.577s | 32.623s | 0.517s | 0.383s | 0.826s | 36.026s | 24.285

## eps:0.6

overview | speedup
--- | ---
![webgraph_twitter-overview](../../figures/scalability_new2_opt_scheduler/webgraph_twitter-eps:0.6-min_pts:5-overview.png) | ![webgraph_twitter-runtime-speedup](../../figures/scalability_new2_opt_scheduler/webgraph_twitter-eps:0.6-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 25.047s | 488.907s | 14.836s | 0.462s | 1.098s | 530.554s | 1.000
4 | 6.742s | 128.249s | 3.327s | 0.483s | 0.459s | 139.471s | 3.804
8 | 3.832s | 66.928s | 1.751s | 0.503s | 0.426s | 73.651s | 7.204
16 | 2.383s | 37.056s | 1.061s | 0.459s | 0.307s | 41.426s | 12.807
24 | 1.824s | 28.2s | 0.765s | 0.492s | 0.389s | 31.88s | 16.642
32 | 1.618s | 22.366s | 0.644s | 0.262s | 0.358s | 25.362s | 20.919
40 | 1.583s | 19.141s | 0.589s | 0.398s | 0.293s | 22.162s | 23.940

## eps:0.7

overview | speedup
--- | ---
![webgraph_twitter-overview](../../figures/scalability_new2_opt_scheduler/webgraph_twitter-eps:0.7-min_pts:5-overview.png) | ![webgraph_twitter-runtime-speedup](../../figures/scalability_new2_opt_scheduler/webgraph_twitter-eps:0.7-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 23.646s | 256.536s | 15.734s | 0.34s | 0.604s | 297.054s | 1.000
4 | 6.258s | 67.128s | 4.372s | 0.344s | 0.25s | 78.557s | 3.781
8 | 3.542s | 35.803s | 2.281s | 0.365s | 0.233s | 42.431s | 7.001
16 | 2.593s | 18.544s | 1.285s | 0.342s | 0.142s | 23.104s | 12.857
24 | 1.829s | 14.936s | 0.992s | 0.357s | 0.174s | 18.501s | 16.056
32 | 1.552s | 11.725s | 0.734s | 0.143s | 0.087s | 14.338s | 20.718
40 | 1.446s | 10.281s | 0.662s | 0.161s | 0.176s | 12.823s | 23.166

## eps:0.8

overview | speedup
--- | ---
![webgraph_twitter-overview](../../figures/scalability_new2_opt_scheduler/webgraph_twitter-eps:0.8-min_pts:5-overview.png) | ![webgraph_twitter-runtime-speedup](../../figures/scalability_new2_opt_scheduler/webgraph_twitter-eps:0.8-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 22.506s | 121.79s | 6.692s | 0.17s | 0.253s | 151.619s | 1.000
4 | 5.932s | 32.113s | 1.8s | 0.159s | 0.113s | 40.296s | 3.763
8 | 3.484s | 16.956s | 1.0s | 0.159s | 0.1s | 21.902s | 6.923
16 | 2.288s | 9.459s | 0.551s | 0.16s | 0.094s | 12.755s | 11.887
24 | 1.567s | 7.414s | 0.417s | 0.159s | 0.088s | 9.845s | 15.401
32 | 1.457s | 5.756s | 0.334s | 0.07s | 0.057s | 7.771s | 19.511
40 | 1.343s | 4.97s | 0.294s | 0.066s | 0.057s | 6.825s | 22.215

## eps:0.9

overview | speedup
--- | ---
![webgraph_twitter-overview](../../figures/scalability_new2_opt_scheduler/webgraph_twitter-eps:0.9-min_pts:5-overview.png) | ![webgraph_twitter-runtime-speedup](../../figures/scalability_new2_opt_scheduler/webgraph_twitter-eps:0.9-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 18.855s | 34.172s | 2.575s | 0.127s | 0.04s | 56.495s | 1.000
4 | 5.591s | 9.156s | 0.657s | 0.123s | 0.02s | 15.744s | 3.588
8 | 3.227s | 5.245s | 0.295s | 0.124s | 0.017s | 9.103s | 6.206
16 | 2.166s | 2.911s | 0.193s | 0.129s | 0.018s | 5.615s | 10.061
24 | 1.75s | 2.063s | 0.152s | 0.052s | 0.008s | 4.121s | 13.709
32 | 1.36s | 2.315s | 0.117s | 0.056s | 0.008s | 3.951s | 14.299
40 | 1.311s | 1.477s | 0.1s | 0.052s | 0.011s | 3.045s | 18.553

