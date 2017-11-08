## eps:0.1

overview | speedup
--- | ---
![webgraph_webbase-overview](../../figures/scalability_simd_paper/webgraph_webbase-eps:0.1-min_pts:5-overview.png) | ![webgraph_webbase-runtime-speedup](../../figures/scalability_simd_paper/webgraph_webbase-eps:0.1-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 16.982s | 21.694s | 2.976s | 13.622s | 10.89s | 66.168s | 1.000
4 | 5.011s | 5.946s | 0.865s | 5.009s | 4.666s | 21.503s | 3.077
8 | 3.055s | 3.295s | 0.474s | 3.246s | 3.666s | 13.745s | 4.814
16 | 1.901s | 1.865s | 0.297s | 2.424s | 2.814s | 9.309s | 7.108
24 | 1.609s | 1.398s | 0.278s | 2.128s | 2.197s | 7.613s | 8.691
32 | 1.395s | 1.216s | 0.297s | 1.912s | 1.747s | 6.57s | 10.071
40 | 1.263s | 1.046s | 0.29s | 1.806s | 1.823s | 6.238s | 10.607

## eps:0.2

overview | speedup
--- | ---
![webgraph_webbase-overview](../../figures/scalability_simd_paper/webgraph_webbase-eps:0.2-min_pts:5-overview.png) | ![webgraph_webbase-runtime-speedup](../../figures/scalability_simd_paper/webgraph_webbase-eps:0.2-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 15.513s | 17.687s | 4.346s | 11.791s | 10.45s | 59.791s | 1.000
4 | 4.617s | 4.836s | 1.304s | 4.279s | 4.111s | 19.159s | 3.121
8 | 2.723s | 2.723s | 0.705s | 2.807s | 3.172s | 12.134s | 4.928
16 | 1.772s | 1.475s | 0.378s | 2.07s | 2.67s | 8.375s | 7.139
24 | 1.398s | 1.1s | 0.277s | 1.374s | 1.41s | 5.562s | 10.750
32 | 1.226s | 0.929s | 0.313s | 1.376s | 1.503s | 5.36s | 11.155
40 | 1.184s | 0.894s | 0.362s | 1.633s | 1.864s | 5.945s | 10.057

## eps:0.3

overview | speedup
--- | ---
![webgraph_webbase-overview](../../figures/scalability_simd_paper/webgraph_webbase-eps:0.3-min_pts:5-overview.png) | ![webgraph_webbase-runtime-speedup](../../figures/scalability_simd_paper/webgraph_webbase-eps:0.3-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 14.256s | 17.264s | 4.591s | 9.648s | 8.541s | 54.313s | 1.000
4 | 4.439s | 4.924s | 1.349s | 3.412s | 3.144s | 17.271s | 3.145
8 | 2.58s | 2.574s | 0.805s | 2.577s | 2.744s | 11.284s | 4.813
16 | 1.874s | 1.453s | 0.405s | 1.828s | 1.731s | 7.294s | 7.446
24 | 1.446s | 1.09s | 0.365s | 1.639s | 1.792s | 6.335s | 8.573
32 | 1.109s | 0.843s | 0.257s | 1.077s | 1.245s | 4.535s | 11.976
40 | 1.183s | 0.704s | 0.264s | 1.131s | 1.334s | 4.618s | 11.761

## eps:0.4

overview | speedup
--- | ---
![webgraph_webbase-overview](../../figures/scalability_simd_paper/webgraph_webbase-eps:0.4-min_pts:5-overview.png) | ![webgraph_webbase-runtime-speedup](../../figures/scalability_simd_paper/webgraph_webbase-eps:0.4-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 13.269s | 16.736s | 4.541s | 8.176s | 7.331s | 50.057s | 1.000
4 | 4.147s | 4.856s | 1.317s | 3.15s | 3.098s | 16.571s | 3.021
8 | 2.429s | 2.782s | 0.752s | 2.341s | 2.513s | 10.82s | 4.626
16 | 1.596s | 1.359s | 0.416s | 1.627s | 1.065s | 6.066s | 8.252
24 | 1.119s | 1.001s | 0.374s | 1.489s | 1.076s | 5.063s | 9.887
32 | 1.042s | 0.833s | 0.235s | 1.309s | 1.376s | 4.798s | 10.433
40 | 1.005s | 0.679s | 0.267s | 0.727s | 0.891s | 3.578s | 13.990

## eps:0.5

overview | speedup
--- | ---
![webgraph_webbase-overview](../../figures/scalability_simd_paper/webgraph_webbase-eps:0.5-min_pts:5-overview.png) | ![webgraph_webbase-runtime-speedup](../../figures/scalability_simd_paper/webgraph_webbase-eps:0.5-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 12.654s | 15.982s | 4.356s | 7.094s | 6.026s | 46.114s | 1.000
4 | 3.944s | 4.496s | 1.171s | 2.871s | 2.721s | 15.206s | 3.033
8 | 2.253s | 2.632s | 0.739s | 2.022s | 1.658s | 9.316s | 4.950
16 | 1.595s | 1.364s | 0.405s | 1.356s | 1.389s | 6.117s | 7.539
24 | 1.435s | 0.998s | 0.38s | 1.19s | 0.736s | 4.754s | 9.700
32 | 1.033s | 0.822s | 0.414s | 1.176s | 1.149s | 4.599s | 10.027
40 | 1.056s | 0.666s | 0.239s | 0.771s | 0.882s | 3.623s | 12.728

## eps:0.6

overview | speedup
--- | ---
![webgraph_webbase-overview](../../figures/scalability_simd_paper/webgraph_webbase-eps:0.6-min_pts:5-overview.png) | ![webgraph_webbase-runtime-speedup](../../figures/scalability_simd_paper/webgraph_webbase-eps:0.6-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 11.713s | 14.928s | 4.317s | 6.055s | 5.138s | 42.154s | 1.000
4 | 3.666s | 4.475s | 1.39s | 2.574s | 2.22s | 14.331s | 2.941
8 | 2.396s | 2.258s | 0.712s | 1.797s | 1.531s | 8.699s | 4.846
16 | 1.419s | 1.237s | 0.622s | 1.392s | 1.085s | 5.765s | 7.312
24 | 1.025s | 0.985s | 0.364s | 1.034s | 1.023s | 4.44s | 9.494
32 | 1.062s | 0.732s | 0.291s | 0.752s | 0.807s | 3.655s | 11.533
40 | 0.911s | 0.636s | 0.27s | 0.679s | 0.588s | 3.093s | 13.629

## eps:0.7

overview | speedup
--- | ---
![webgraph_webbase-overview](../../figures/scalability_simd_paper/webgraph_webbase-eps:0.7-min_pts:5-overview.png) | ![webgraph_webbase-runtime-speedup](../../figures/scalability_simd_paper/webgraph_webbase-eps:0.7-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 11.167s | 14.105s | 4.305s | 5.616s | 4.366s | 39.564s | 1.000
4 | 3.651s | 4.095s | 1.259s | 2.422s | 1.652s | 13.081s | 3.025
8 | 2.221s | 2.228s | 0.683s | 1.733s | 1.213s | 8.087s | 4.892
16 | 1.462s | 1.231s | 0.36s | 0.816s | 0.815s | 4.689s | 8.438
24 | 1.13s | 0.866s | 0.388s | 1.002s | 0.807s | 4.198s | 9.424
32 | 0.946s | 0.701s | 0.341s | 0.85s | 0.626s | 3.47s | 11.402
40 | 0.961s | 0.602s | 0.281s | 0.521s | 0.474s | 2.845s | 13.907

## eps:0.8

overview | speedup
--- | ---
![webgraph_webbase-overview](../../figures/scalability_simd_paper/webgraph_webbase-eps:0.8-min_pts:5-overview.png) | ![webgraph_webbase-runtime-speedup](../../figures/scalability_simd_paper/webgraph_webbase-eps:0.8-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 10.463s | 12.725s | 3.211s | 4.478s | 3.664s | 34.548s | 1.000
4 | 3.355s | 3.983s | 0.991s | 1.827s | 1.472s | 11.63s | 2.971
8 | 2.158s | 2.091s | 0.544s | 1.307s | 1.029s | 7.136s | 4.841
16 | 1.434s | 1.153s | 0.306s | 0.895s | 0.707s | 4.5s | 7.677
24 | 1.084s | 0.775s | 0.307s | 0.923s | 0.487s | 3.58s | 9.650
32 | 0.875s | 0.662s | 0.267s | 0.691s | 0.514s | 3.013s | 11.466
40 | 0.896s | 0.533s | 0.217s | 0.443s | 0.325s | 2.416s | 14.300

## eps:0.9

overview | speedup
--- | ---
![webgraph_webbase-overview](../../figures/scalability_simd_paper/webgraph_webbase-eps:0.9-min_pts:5-overview.png) | ![webgraph_webbase-runtime-speedup](../../figures/scalability_simd_paper/webgraph_webbase-eps:0.9-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total | total speedup
--- | --- | --- | --- | --- | --- | --- | ---
1 | 9.91s | 11.335s | 2.655s | 3.93s | 2.791s | 30.624s | 1.000
4 | 3.074s | 3.421s | 0.802s | 1.786s | 1.2s | 10.29s | 2.976
8 | 1.926s | 1.872s | 0.484s | 1.246s | 0.767s | 6.299s | 4.862
16 | 1.268s | 1.007s | 0.279s | 0.783s | 0.542s | 3.882s | 7.889
24 | 1.102s | 0.736s | 0.268s | 0.73s | 0.458s | 3.297s | 9.288
32 | 0.862s | 0.579s | 0.207s | 0.394s | 0.318s | 2.362s | 12.965
40 | 0.802s | 0.46s | 0.197s | 0.394s | 0.274s | 2.131s | 14.371

