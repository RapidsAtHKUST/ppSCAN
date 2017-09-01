## Speedup Overview

computation and io portion

with 40 full logical threads | with best thread num
--- | ---
![portion with full logical threads](../scalability_overview_robust/eps:0.3-min_pts:5-withalllogicalcores-comp-io-portion.png) | ![io portition](../scalability_overview_robust/eps:0.3-min_pts:5-withbestlogicalthreadnum-comp-io-portion.png)

with 40 full logical threads | with best thread num
--- | ---
![with full logical threads](../scalability_overview_robust/eps:0.3-min_pts:5-withalllogicalcores-runtime-speedup.png) | ![with best thread num](../scalability_overview_robust/eps:0.3-min_pts:5-withbestlogicalthreadnum-runtime-speedup.png)

```zsh
best performance thread_num_lst: [16, 32, 40, 40, 40, 32, 40, 40]
```

dataset | edge num | pscan runtime | pscan+ 40 logical-core full speedup | pscan+ best thread_num speedup | thread_num choice
--- | --- | --- | --- | --- | ---
dblp | 2,099,732 | 0.555s | 4.549 | 4.549 | 16
pokec | 30,282,866 | 8.597s | 10.043 | 10.043 | 32
livejournal | 69,362,378 | 21.846s | 8.367 | 8.759 | 40
orkut | 234,370,166 | 164.248s | 16.340 | 16.340 | 40
uk | 301,136,554 | 18.498s | 4.173 | 4.487 | 40
webbase | 1,050,026,736 | 63.705s | 5.093 | 4.510 | 32
twitter | 1,369,000,750 | 2487.317s | 19.187 | 19.187 | 40
friendster | 3,612,134,270 | 3726.302s | 21.729 | 21.729 | 40

## Small: Dblp

name | n | m 
--- | --- | ---
dblp        | 425,957       | 2,099,732

overview | speedup
--- | ---
![dblp-overview](../scalability_robust/small_snap_dblp-eps:0.3-min_pts:5-overview.png) | ![dblp-speedup](../scalability_robust/small_snap_dblp-eps:0.3-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 0.074s | 0.333s | 0.12s | 0.048s | 0.037s | 0.615s | 1.000
2 | 0.035s | 0.16s | 0.061s | 0.059s | 0.046s | 0.364s | 1.690
4 | 0.02s | 0.084s | 0.032s | 0.052s | 0.037s | 0.227s | 2.709
8 | 0.011s | 0.046s | 0.017s | 0.048s | 0.037s | 0.162s | 3.796
16 | 0.009s | 0.024s | 0.01s | 0.043s | 0.037s | 0.124s | 4.960
24 | 0.01s | 0.026s | 0.008s | 0.053s | 0.044s | 0.143s | 4.301
32 | 0.011s | 0.022s | 0.01s | 0.045s | 0.036s | 0.125s | 4.920
40 | 0.01s | 0.016s | 0.011s | 0.045s | 0.038s | 0.122s | 5.041

## Medium: Pokec

name | n | m 
--- | --- | ---
pokec       | 1,632,804     | 30,282,866

overview | speedup
--- | ---
![pokec-overview](../scalability_robust/snap_pokec-eps:0.3-min_pts:5-overview.png) | ![pokec-speedup](../scalability_robust/snap_pokec-eps:0.3-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 0.912s | 7.355s | 1.752s | 0.065s | 0.094s | 10.181s | 1.000
2 | 0.502s | 4.308s | 1.006s | 0.067s | 0.094s | 5.979s | 1.703
4 | 0.236s | 2.386s | 0.456s | 0.065s | 0.085s | 3.23s | 3.152
8 | 0.118s | 1.392s | 0.226s | 0.065s | 0.098s | 1.901s | 5.356
16 | 0.075s | 0.934s | 0.112s | 0.073s | 0.101s | 1.297s | 7.850
24 | 0.093s | 0.917s | 0.098s | 0.08s | 0.104s | 1.293s | 7.874
32 | 0.062s | 0.656s | 0.123s | 0.045s | 0.059s | 0.946s | 10.762
40 | 0.059s | 0.543s | 0.117s | 0.055s | 0.079s | 0.856s | 11.894

## Medium: LiveJournal

name | n | m 
--- | --- | ---
livejournal | 4,036,538     | 69,362,378

overview | speedup
--- | ---
![livejournal-overview](../scalability_robust/snap_livejournal-eps:0.3-min_pts:5-overview.png) | ![livejournal-speedup](../scalability_robust/snap_livejournal-eps:0.3-min_pts:5-runtime-speedup.png)


thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 1.435s | 18.931s | 3.64s | 0.7s | 0.402s | 25.11s | 1.000
2 | 0.957s | 9.775s | 1.786s | 0.823s | 0.436s | 13.78s | 1.822
4 | 0.523s | 5.6s | 0.821s | 0.654s | 0.402s | 8.002s | 3.138
8 | 0.255s | 3.247s | 0.555s | 0.724s | 0.403s | 5.187s | 4.841
16 | 0.158s | 2.132s | 0.408s | 0.431s | 0.36s | 3.491s | 7.193
24 | 0.124s | 1.582s | 0.257s | 0.368s | 0.306s | 2.638s | 9.519
32 | 0.12s | 1.226s | 0.227s | 0.531s | 0.387s | 2.494s | 10.068
40 | 0.121s | 1.344s | 0.224s | 0.51s | 0.41s | 2.611s | 9.617

## Medium: Orkut

name | n | m 
--- | --- | ---
orkut       | 3,072,627     | 234,370,166

overview | speedup
--- | ---
![orkut-overview](../scalability_robust/snap_orkut-eps:0.3-min_pts:5-overview.png) | ![orkut-speedup](../scalability_robust/snap_orkut-eps:0.3-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 3.861s | 163.193s | 16.555s | 0.992s | 0.753s | 185.356s | 1.000
2 | 2.201s | 83.07s | 7.35s | 1.006s | 0.752s | 94.381s | 1.964
4 | 1.284s | 43.99s | 4.188s | 1.004s | 0.738s | 51.207s | 3.620
8 | 0.726s | 23.727s | 2.366s | 1.036s | 0.745s | 28.601s | 6.481
16 | 0.553s | 13.226s | 1.524s | 1.067s | 0.761s | 17.133s | 10.819
24 | 0.473s | 9.969s | 1.331s | 1.095s | 0.673s | 13.543s | 13.686
32 | 0.267s | 8.335s | 1.103s | 0.932s | 0.739s | 11.379s | 16.289
40 | 0.315s | 7.491s | 0.839s | 0.734s | 0.671s | 10.052s | 18.440

## Medium: Uk

name | n | m 
--- | --- | ---
uk          | 18,520,343    | 301,136,554

overview | speedup
--- | ---
![uk-overview](../scalability_robust/webgraph_uk-eps:0.3-min_pts:5-overview.png) | ![uk-speedup](../scalability_robust/webgraph_uk-eps:0.3-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 2.655s | 26.275s | 3.271s | 1.545s | 0.734s | 34.484s | 1.000
2 | 1.73s | 13.995s | 2.14s | 1.692s | 0.783s | 20.345s | 1.695
4 | 1.006s | 7.244s | 1.256s | 1.609s | 0.629s | 11.749s | 2.935
8 | 0.558s | 4.387s | 0.686s | 1.687s | 0.769s | 8.093s | 4.261
16 | 0.272s | 2.762s | 0.356s | 1.721s | 0.769s | 5.882s | 5.863
24 | 0.271s | 2.472s | 0.249s | 1.147s | 0.631s | 4.772s | 7.226
32 | 0.238s | 2.073s | 0.209s | 0.974s | 0.626s | 4.123s | 8.364
40 | 0.204s | 2.209s | 0.197s | 1.111s | 0.71s | 4.433s | 7.779

## Large: Webbase

name | n | m 
--- | --- | ---
webbase     | 118,142,143   | 1,050,026,736

overview | speedup
--- | ---
![webbase-overview](../scalability_robust/webgraph_webbase-eps:0.3-min_pts:5-overview.png) | ![webbase-speedup](../scalability_robust/webgraph_webbase-eps:0.3-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 8.377s | 62.338s | 13.973s | 4.375s | 2.749s | 91.815s | 1.000
2 | 4.542s | 31.736s | 7.266s | 4.303s | 2.732s | 50.581s | 1.815
4 | 2.667s | 16.27s | 3.728s | 4.206s | 2.737s | 29.61s | 3.101
8 | 1.829s | 8.924s | 2.155s | 3.685s | 2.741s | 19.336s | 4.748
16 | 1.017s | 5.286s | 1.205s | 4.231s | 2.748s | 14.489s | 6.337
24 | 0.776s | 4.393s | 1.057s | 3.541s | 2.738s | 12.508s | 7.341
32 | 0.731s | 4.535s | 1.116s | 4.333s | 2.74s | 13.456s | 6.823
40 | 0.685s | 4.939s | 1.094s | 4.28s | 3.125s | 14.125s | 6.500

## Large: Twitter

name | n | m 
--- | --- | ---
twitter     | 41,652,230    | 1,369,000,750

overview | speedup
--- | ---
![twitter-overview](../scalability_robust/webgraph_twitter-eps:0.3-min_pts:5-overview.png) | ![twitter-speedup](../scalability_robust/webgraph_twitter-eps:0.3-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 17.415s | 2813.298s | 129.6s | 0.945s | 1.1s | 2962.36s | 1.000
2 | 7.805s | 1409.66s | 65.557s | 0.442s | 0.973s | 1484.439s | 1.996
4 | 4.792s | 744.448s | 36.408s | 0.935s | 1.102s | 787.69s | 3.761
8 | 3.287s | 393.054s | 19.357s | 1.003s | 1.137s | 417.839s | 7.090
16 | 2.658s | 218.43s | 12.158s | 0.522s | 0.971s | 234.741s | 12.620
24 | 2.494s | 168.6s | 11.007s | 0.819s | 1.115s | 184.037s | 16.097
32 | 1.71s | 130.77s | 8.241s | 0.81s | 1.014s | 142.547s | 20.782
40 | 2.056s | 117.446s | 7.985s | 0.9s | 1.245s | 129.635s | 22.852

## Large: Fridenster

name | n | m 
--- | --- | ---
friendster  | 124,836,180   | 3,612,134,270

overview | speedup
--- | ---
![fridenster-overview](../scalability_robust/snap_friendster-eps:0.3-min_pts:5-overview.png) | ![friendster-speedup](../scalability_robust/snap_friendster-eps:0.3-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 129.931s | 3602.674s | 676.785s | 2.655s | 4.076s | 4416.148s | 1.000
2 | 52.277s | 1748.684s | 272.627s | 2.281s | 4.102s | 2079.982s | 2.123
4 | 28.604s | 901.12s | 153.828s | 2.304s | 4.056s | 1089.915s | 4.052
8 | 14.981s | 489.896s | 83.917s | 2.25s | 4.081s | 595.126s | 7.421
16 | 8.928s | 265.88s | 51.357s | 2.285s | 4.414s | 332.866s | 13.267
24 | 7.065s | 197.572s | 28.773s | 1.887s | 4.472s | 239.771s | 18.418
32 | 6.021s | 152.678s | 27.147s | 2.151s | 4.24s | 192.24s | 22.972
40 | 4.967s | 133.181s | 27.079s | 1.902s | 4.361s | 171.492s | 25.751

## LFR: 10million-avgdeg15-maxdeg50

name | n | m
--- | --- | --- 
lfr-avg15-max50 | 10,000,001  | 152,826,874 

overview | speedup
--- | ---
![lfr10million-avgdeg15-maxdeg50-overview](../scalability_robust/lfr_10million_avgdeg15-eps:0.3-min_pts:5-overview.png) | ![lfr10million-avgdeg15-maxdeg50-speedup](../scalability_robust/lfr_10million_avgdeg15-eps:0.3-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 3.109s | 30.622s | 8.391s | 5.484s | 1.261s | 48.87s | 1.000
2 | 1.816s | 13.301s | 3.452s | 5.491s | 1.255s | 25.318s | 1.930
4 | 1.096s | 7.159s | 2.019s | 5.339s | 1.264s | 16.878s | 2.895
8 | 0.64s | 3.883s | 1.316s | 4.973s | 1.266s | 12.082s | 4.045
16 | 0.352s | 2.381s | 0.999s | 5.38s | 1.255s | 10.37s | 4.713
24 | 0.264s | 1.847s | 0.725s | 5.422s | 1.265s | 9.526s | 5.130
32 | 0.211s | 1.492s | 0.585s | 4.991s | 1.266s | 8.547s | 5.718
40 | 0.217s | 1.317s | 0.529s | 4.931s | 1.253s | 8.249s | 5.924