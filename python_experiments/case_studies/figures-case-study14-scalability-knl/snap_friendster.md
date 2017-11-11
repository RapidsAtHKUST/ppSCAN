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

## eps:0.2

overview | speedup
--- | ---
![snap_friendster-overview](../../figures/scalability_simd_paper/snap_friendster-eps:0.2-min_pts:5-overview.png) | ![snap_friendster-runtime-speedup](../../figures/scalability_simd_paper/snap_friendster-eps:0.2-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 759.385s | 6308.925s | 414.088s | 123.25s | 203.527s | 7809.179s | 1.000
2 | 397.049s | 3154.587s | 206.981s | 64.43s | 107.247s | 3930.3s | 1.987
4 | 199.469s | 1580.95s | 103.738s | 33.024s | 55.101s | 1972.287s | 3.959
8 | 99.847s | 789.311s | 51.661s | 16.952s | 29.473s | 987.249s | 7.910
16 | 50.115s | 396.879s | 25.821s | 8.739s | 18.407s | 499.966s | 15.619
32 | 25.705s | 203.231s | 13.541s | 5.077s | 11.467s | 259.026s | 30.148
64 | 13.631s | 102.372s | 7.04s | 3.669s | 6.915s | 133.633s | 58.438
128 | 8.878s | 56.663s | 4.247s | 2.677s | 4.901s | 77.371s | 100.932
256 | 6.696s | 43.754s | 3.163s | 2.837s | 4.304s | 60.758s | 128.529

## eps:0.3

overview | speedup
--- | ---
![snap_friendster-overview](../../figures/scalability_simd_paper/snap_friendster-eps:0.3-min_pts:5-overview.png) | ![snap_friendster-runtime-speedup](../../figures/scalability_simd_paper/snap_friendster-eps:0.3-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 744.535s | 5429.591s | 101.873s | 17.765s | 53.064s | 6346.832s | 1.000
2 | 386.753s | 2715.207s | 50.976s | 9.486s | 27.516s | 3189.943s | 1.990
4 | 194.127s | 1360.522s | 25.392s | 5.16s | 14.144s | 1599.349s | 3.968
8 | 98.764s | 681.863s | 12.62s | 3.091s | 7.938s | 804.28s | 7.891
16 | 48.811s | 339.348s | 6.436s | 1.818s | 5.051s | 401.468s | 15.809
32 | 24.258s | 174.648s | 3.494s | 1.477s | 3.05s | 206.931s | 30.671
64 | 13.293s | 88.096s | 2.109s | 1.106s | 1.921s | 106.527s | 59.580
128 | 8.679s | 48.882s | 1.666s | 1.356s | 1.542s | 62.128s | 102.157
256 | 6.185s | 37.33s | 1.593s | 1.532s | 1.6s | 48.243s | 131.560

## eps:0.4

overview | speedup
--- | ---
![snap_friendster-overview](../../figures/scalability_simd_paper/snap_friendster-eps:0.4-min_pts:5-overview.png) | ![snap_friendster-runtime-speedup](../../figures/scalability_simd_paper/snap_friendster-eps:0.4-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 716.118s | 4148.362s | 72.63s | 2.878s | 7.638s | 4947.628s | 1.000
2 | 371.966s | 2076.83s | 36.698s | 1.787s | 3.999s | 2491.284s | 1.986
4 | 186.898s | 1040.229s | 17.987s | 1.204s | 2.137s | 1248.457s | 3.963
8 | 93.428s | 517.115s | 8.999s | 0.934s | 1.275s | 621.753s | 7.958
16 | 47.749s | 263.916s | 4.662s | 0.833s | 0.917s | 318.08s | 15.555
32 | 23.623s | 133.169s | 2.427s | 0.775s | 0.612s | 160.608s | 30.806
64 | 12.92s | 68.268s | 1.689s | 0.789s | 0.444s | 84.114s | 58.821
128 | 8.47s | 37.716s | 1.565s | 0.813s | 0.598s | 49.165s | 100.633
256 | 6.082s | 28.208s | 1.563s | 0.907s | 0.879s | 37.643s | 131.436

## eps:0.5

overview | speedup
--- | ---
![snap_friendster-overview](../../figures/scalability_simd_paper/snap_friendster-eps:0.5-min_pts:5-overview.png) | ![snap_friendster-runtime-speedup](../../figures/scalability_simd_paper/snap_friendster-eps:0.5-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 677.199s | 3003.765s | 64.607s | 1.091s | 2.003s | 3748.668s | 1.000
2 | 348.532s | 1497.121s | 32.029s | 0.851s | 1.035s | 1879.571s | 1.994
4 | 175.977s | 751.257s | 15.99s | 0.75s | 0.581s | 944.559s | 3.969
8 | 88.142s | 373.888s | 8.0s | 0.69s | 0.361s | 471.083s | 7.958
16 | 44.118s | 188.181s | 4.148s | 0.664s | 0.241s | 237.356s | 15.793
32 | 22.17s | 95.454s | 2.319s | 0.663s | 0.194s | 120.803s | 31.031
64 | 12.082s | 48.857s | 1.645s | 0.663s | 0.21s | 63.461s | 59.070
128 | 8.221s | 27.09s | 1.367s | 0.692s | 0.288s | 37.662s | 99.534
256 | 5.644s | 20.006s | 1.434s | 0.718s | 0.398s | 28.202s | 132.922

## eps:0.6

overview | speedup
--- | ---
![snap_friendster-overview](../../figures/scalability_simd_paper/snap_friendster-eps:0.6-min_pts:5-overview.png) | ![snap_friendster-runtime-speedup](../../figures/scalability_simd_paper/snap_friendster-eps:0.6-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 616.208s | 2034.604s | 55.846s | 0.752s | 1.175s | 2708.588s | 1.000
2 | 311.557s | 1023.508s | 28.066s | 0.677s | 0.597s | 1364.408s | 1.985
4 | 160.53s | 512.556s | 14.086s | 0.654s | 0.329s | 688.157s | 3.936
8 | 80.471s | 252.734s | 6.992s | 0.63s | 0.219s | 341.05s | 7.942
16 | 40.01s | 127.062s | 3.722s | 0.63s | 0.147s | 171.574s | 15.787
32 | 21.755s | 66.179s | 2.169s | 0.632s | 0.112s | 90.85s | 29.814
64 | 11.306s | 33.466s | 1.465s | 0.633s | 0.142s | 47.014s | 57.612
128 | 7.434s | 18.411s | 1.149s | 0.656s | 0.198s | 27.851s | 97.253
256 | 5.522s | 12.867s | 1.356s | 0.69s | 0.255s | 20.693s | 130.894

## eps:0.7

overview | speedup
--- | ---
![snap_friendster-overview](../../figures/scalability_simd_paper/snap_friendster-eps:0.7-min_pts:5-overview.png) | ![snap_friendster-runtime-speedup](../../figures/scalability_simd_paper/snap_friendster-eps:0.7-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 545.494s | 1261.849s | 46.577s | 0.662s | 0.981s | 1855.567s | 1.000
2 | 282.671s | 632.786s | 23.242s | 0.631s | 0.494s | 939.827s | 1.974
4 | 142.415s | 316.901s | 11.597s | 0.62s | 0.269s | 471.806s | 3.933
8 | 71.372s | 157.281s | 5.79s | 0.615s | 0.192s | 235.254s | 7.888
16 | 36.048s | 78.77s | 3.047s | 0.612s | 0.124s | 118.604s | 15.645
32 | 17.85s | 40.39s | 1.858s | 0.62s | 0.101s | 60.822s | 30.508
64 | 10.083s | 20.711s | 1.276s | 0.629s | 0.13s | 32.833s | 56.515
128 | 7.017s | 11.773s | 1.076s | 0.642s | 0.191s | 20.702s | 89.632
256 | 5.152s | 8.325s | 1.241s | 0.676s | 0.252s | 15.649s | 118.574

## eps:0.8

overview | speedup
--- | ---
![snap_friendster-overview](../../figures/scalability_simd_paper/snap_friendster-eps:0.8-min_pts:5-overview.png) | ![snap_friendster-runtime-speedup](../../figures/scalability_simd_paper/snap_friendster-eps:0.8-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 475.792s | 687.779s | 36.669s | 0.63s | 0.928s | 1201.801s | 1.000
2 | 246.993s | 343.995s | 18.288s | 0.611s | 0.465s | 610.355s | 1.969
4 | 124.112s | 172.497s | 9.213s | 0.608s | 0.238s | 306.671s | 3.919
8 | 62.812s | 85.984s | 4.679s | 0.611s | 0.144s | 154.232s | 7.792
16 | 31.7s | 42.92s | 2.376s | 0.609s | 0.116s | 77.724s | 15.462
32 | 15.589s | 21.893s | 1.478s | 0.611s | 0.089s | 39.664s | 30.300
64 | 8.688s | 11.383s | 1.02s | 0.622s | 0.1s | 21.816s | 55.088
128 | 6.458s | 6.531s | 1.156s | 0.635s | 0.196s | 14.98s | 80.227
256 | 5.088s | 4.442s | 1.292s | 0.666s | 0.252s | 11.743s | 102.342

## eps:0.9

overview | speedup
--- | ---
![snap_friendster-overview](../../figures/scalability_simd_paper/snap_friendster-eps:0.9-min_pts:5-overview.png) | ![snap_friendster-runtime-speedup](../../figures/scalability_simd_paper/snap_friendster-eps:0.9-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 414.199s | 283.01s | 24.293s | 0.611s | 0.902s | 723.018s | 1.000
2 | 215.501s | 141.761s | 12.154s | 0.604s | 0.453s | 370.476s | 1.952
4 | 108.847s | 70.633s | 6.073s | 0.604s | 0.231s | 186.392s | 3.879
8 | 55.256s | 35.246s | 3.154s | 0.606s | 0.165s | 94.43s | 7.657
16 | 27.832s | 17.789s | 1.738s | 0.613s | 0.104s | 48.08s | 15.038
32 | 13.725s | 9.043s | 1.166s | 0.61s | 0.104s | 24.651s | 29.330
64 | 7.826s | 4.869s | 0.941s | 0.616s | 0.11s | 14.365s | 50.332
128 | 5.744s | 2.928s | 1.106s | 0.63s | 0.157s | 10.568s | 68.416
256 | 4.44s | 4.999s | 1.114s | 0.666s | 0.285s | 11.508s | 62.827

