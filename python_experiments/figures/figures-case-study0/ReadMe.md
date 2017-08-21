# Case Study

parameter setting: `eps:0.3`, `min_pts:5`

* real-world

graph type | name
--- | ---
small | dblp
medium |  pokec, livejournal, orkut, uk
large | webbase, twitter, friendster

name | n | m 
--- | --- | ---
dblp        | 425,957       | 2,099,732
pokec       | 1,632,804     | 30,282,866
livejournal | 4,036,538     | 69,362,378
orkut       | 3,072,627     | 234,370,166
uk          | 18,520,343    | 301,136,554
webbase     | 118,142,143   | 1,050,026,736
twitter     | 41,652,230    | 1,369,000,750
friendster  | 124,836,180   | 3,612,134,270

* synthetic

name | n | m
--- | --- | --- 
lfr-avg15-max50 | 10,000,001  | 152,826,874 

## Small: Dblp

name | n | m 
--- | --- | ---
dblp        | 425,957       | 2,099,732

overview | speedup
--- | ---
![dblp-overview](../small_snap_dblp-eps:0.3-min_pts:5-overview.png) | ![dblp-speedup](../small_snap_dblp-eps:0.3-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total
--- | --- | --- | --- | --- | --- | ---
1 | 0.077s | 0.331s | 0.123s | 0.057s | 0.045s | 0.635s
2 | 0.036s | 0.16s | 0.065s | 0.057s | 0.044s | 0.364s
4 | 0.018s | 0.087s | 0.032s | 0.052s | 0.036s | 0.227s
8 | 0.011s | 0.045s | 0.018s | 0.048s | 0.035s | 0.16s
16 | 0.009s | 0.03s | 0.011s | 0.045s | 0.037s | 0.133s
24 | 0.014s | 0.061s | 0.009s | 0.053s | 0.043s | 0.182s
32 | 0.01s | 0.071s | 0.008s | 0.045s | 0.037s | 0.173s
40 | 0.01s | 0.108s | 0.007s | 0.045s | 0.037s | 0.209s

## Medium: Pokec

name | n | m 
--- | --- | ---
pokec       | 1,632,804     | 30,282,866

overview | speedup
--- | ---
![pokec-overview](../snap_pokec-eps:0.3-min_pts:5-overview.png) | ![pokec-speedup](../snap_pokec-eps:0.3-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total
--- | --- | --- | --- | --- | --- | ---
1 | 0.779s | 6.626s | 1.439s | 0.066s | 0.094s | 9.006s
2 | 0.481s | 3.813s | 0.849s | 0.064s | 0.082s | 5.291s
4 | 0.237s | 2.283s | 0.357s | 0.065s | 0.084s | 3.029s
8 | 0.129s | 1.346s | 0.229s | 0.062s | 0.083s | 1.852s
16 | 0.073s | 0.907s | 0.144s | 0.077s | 0.114s | 1.317s
24 | 0.066s | 0.73s | 0.151s | 0.07s | 0.093s | 1.112s
32 | 0.05s | 0.591s | 0.114s | 0.052s | 0.069s | 0.878s
40 | 0.06s | 0.599s | 0.132s | 0.064s | 0.074s | 0.931s

## Medium: LiveJournal

name | n | m 
--- | --- | ---
livejournal | 4,036,538     | 69,362,378

overview | speedup
--- | ---
![livejournal-overview](../snap_livejournal-eps:0.3-min_pts:5-overview.png) | ![livejournal-speedup](../snap_livejournal-eps:0.3-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total
--- | --- | --- | --- | --- | --- | ---
1 | 1.503s | 18.016s | 3.093s | 0.713s | 0.404s | 23.731s
2 | 0.924s | 9.717s | 1.557s | 0.677s | 0.399s | 13.276s
4 | 0.516s | 5.298s | 0.887s | 0.705s | 0.406s | 7.814s
8 | 0.247s | 3.084s | 0.471s | 0.658s | 0.4s | 4.862s
16 | 0.165s | 2.107s | 0.29s | 0.803s | 0.426s | 3.793s
24 | 0.122s | 1.611s | 0.171s | 0.376s | 0.307s | 2.589s
32 | 0.107s | 1.299s | 0.232s | 0.654s | 0.408s | 2.702s
40 | 0.111s | 1.221s | 0.15s | 0.431s | 0.371s | 2.285s

## Medium: Orkut

name | n | m 
--- | --- | ---
orkut       | 3,072,627     | 234,370,166

overview | speedup
--- | ---
![orkut-overview](../snap_orkut-eps:0.3-min_pts:5-overview.png) | ![orkut-speedup](../snap_orkut-eps:0.3-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total
--- | --- | --- | --- | --- | --- | ---
1 | 3.75s | 161.28s | 16.501s | 0.93s | 0.743s | 183.205s
2 | 2.454s | 79.316s | 7.606s | 1.08s | 0.73s | 91.19s
4 | 1.32s | 41.962s | 4.078s | 1.059s | 0.754s | 49.175s
8 | 0.88s | 22.718s | 2.444s | 0.97s | 0.751s | 27.764s
16 | 0.531s | 12.631s | 1.522s | 1.057s | 0.805s | 16.548s
24 | 0.326s | 9.988s | 1.227s | 0.948s | 1.016s | 13.507s
32 | 0.362s | 8.509s | 0.813s | 0.587s | 0.594s | 10.869s
40 | 0.317s | 7.231s | 0.927s | 0.652s | 0.627s | 9.756s

## Medium: Uk

name | n | m 
--- | --- | ---
uk          | 18,520,343    | 301,136,554

overview | speedup
--- | ---
![uk-overview](../webgraph_uk-eps:0.3-min_pts:5-overview.png) | ![uk-speedup](../webgraph_uk-eps:0.3-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total
--- | --- | --- | --- | --- | --- | ---
1 | 2.728s | 25.529s | 3.286s | 1.677s | 0.754s | 33.977s
2 | 1.642s | 13.549s | 2.084s | 1.557s | 0.744s | 19.578s
4 | 0.976s | 7.348s | 1.222s | 1.638s | 0.746s | 11.931s
8 | 0.561s | 4.215s | 0.689s | 1.722s | 0.765s | 7.955s
16 | 0.328s | 2.927s | 0.419s | 1.633s | 0.769s | 6.079s
24 | 0.252s | 2.299s | 0.297s | 1.714s | 0.787s | 5.352s
32 | 0.206s | 1.9s | 0.253s | 0.972s | 0.633s | 3.967s
40 | 0.194s | 1.791s | 0.249s | 1.009s | 0.634s | 3.879s

## Large: Webbase

name | n | m 
--- | --- | ---
webbase     | 118,142,143   | 1,050,026,736

overview | speedup
--- | ---
![webbase-overview](../webgraph_webbase-eps:0.3-min_pts:5-overview.png) | ![webbase-speedup](../webgraph_webbase-eps:0.3-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total
--- | --- | --- | --- | --- | --- | ---
1 | 8.416s | 62.804s | 14.063s | 4.361s | 2.742s | 92.388s
2 | 4.433s | 32.174s | 7.679s | 4.374s | 2.742s | 51.404s
4 | 2.607s | 17.421s | 4.414s | 4.253s | 2.741s | 31.438s
8 | 1.55s | 9.935s | 2.414s | 4.299s | 3.074s | 21.275s
16 | 0.999s | 6.56s | 1.656s | 3.568s | 2.844s | 15.628s
24 | 0.909s | 6.352s | 0.931s | 4.063s | 2.748s | 15.006s
32 | 0.743s | 5.394s | 0.928s | 3.944s | 2.742s | 13.753s
40 | 0.724s | 17.167s | 1.508s | 4.324s | 3.128s | 26.854s

## Large: Twitter

name | n | m 
--- | --- | ---
twitter     | 41,652,230    | 1,369,000,750

overview | speedup
--- | ---
![twitter-overview](../webgraph_twitter-eps:0.3-min_pts:5-overview.png) | ![twitter-speedup](../webgraph_twitter-eps:0.3-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total
--- | --- | --- | --- | --- | --- | ---
1 | 17.42s | 2670.844s | 149.719s | 0.805s | 1.21s | 2840.002s
2 | 8.141s | 1342.299s | 65.191s | 0.959s | 1.074s | 1417.667s
4 | 4.635s | 684.949s | 36.339s | 0.85s | 1.097s | 727.872s
8 | 3.321s | 377.744s | 19.278s | 0.93s | 1.099s | 402.375s
16 | 2.148s | 209.769s | 10.508s | 0.58s | 1.191s | 224.198s
24 | 1.714s | 159.254s | 9.195s | 0.489s | 1.028s | 171.683s
32 | 1.604s | 129.201s | 8.592s | 0.813s | 1.017s | 141.229s
40 | 1.649s | 118.153s | 7.845s | 0.56s | 1.219s | 129.427s

## Large: Fridenster

name | n | m 
--- | --- | ---
friendster  | 124,836,180   | 3,612,134,270

overview | speedup
--- | ---
![fridenster-overview](../snap_friendster-eps:0.3-min_pts:5-overview.png) | ![friendster-speedup](../snap_friendster-eps:0.3-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total
--- | --- | --- | --- | --- | --- | ---
1 | 94.69s | 3300.112s | 505.722s | 2.072s | 5.12s | 3907.722s
2 | 51.892s | 1697.725s | 274.799s | 2.294s | 3.964s | 2030.68s
4 | 28.763s | 907.392s | 153.025s | 2.265s | 4.225s | 1095.677s
8 | 14.744s | 472.934s | 65.818s | 1.996s | 3.502s | 558.998s
16 | 7.252s | 258.949s | 41.999s | 1.887s | 4.156s | 314.246s
24 | 6.421s | 193.434s | 35.944s | 2.349s | 4.177s | 242.329s
32 | 5.172s | 154.929s | 26.779s | 1.883s | 4.134s | 192.901s
40 | 4.617s | 132.803s | 24.353s | 1.707s | 5.382s | 168.866s

## LFR: 10million-avgdeg15-maxdeg50

name | n | m
--- | --- | --- 
lfr-avg15-max50 | 10,000,001  | 152,826,874 

overview | speedup
--- | ---
![lfr10million-avgdeg15-maxdeg50-overview](../lfr_10million_avgdeg15-eps:0.3-min_pts:5-overview.png) | ![lfr10million-avgdeg15-maxdeg50-speedup](../lfr_10million_avgdeg15-eps:0.3-min_pts:5-runtime-speedup.png)

thread_num | prune | check-core 1st bsp | check-core 2nd bsp | cluster-core | cluster-non-core | total
--- | --- | --- | --- | --- | --- | ---
1 | 3.335s | 28.883s | 8.464s | 5.589s | 1.308s | 47.581s
2 | 1.87s | 13.076s | 4.047s | 5.56s | 1.304s | 25.859s
4 | 1.125s | 6.864s | 2.245s | 5.314s | 1.295s | 16.845s
8 | 0.613s | 3.647s | 1.176s | 5.491s | 1.3s | 12.229s
16 | 0.414s | 2.331s | 0.71s | 5.569s | 1.289s | 10.315s
24 | 0.267s | 1.784s | 0.679s | 5.616s | 1.297s | 9.646s
32 | 0.267s | 1.452s | 0.548s | 5.233s | 1.312s | 8.815s
40 | 0.228s | 1.162s | 0.573s | 5.218s | 1.281s | 8.464s