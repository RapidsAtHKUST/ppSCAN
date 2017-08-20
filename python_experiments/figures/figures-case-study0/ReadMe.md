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

![dblp-overview](../small_snap_dblp-eps:0.3-min_pts:5-overview.png)

![dblp-speedup](../small_snap_dblp-eps:0.3-min_pts:5-runtime-speedup.png)

## Medium: Pokec

![pokec-overview](../snap_pokec-eps:0.3-min_pts:5-overview.png)

![pokec-speedup](../snap_pokec-eps:0.3-min_pts:5-runtime-speedup.png)

## Medium: LiveJournal

![livejournal-overview](../snap_livejournal-eps:0.3-min_pts:5-overview.png)

![livejournal-speedup](../snap_livejournal-eps:0.3-min_pts:5-runtime-speedup.png)

## Medium: Orkut

![orkut-overview](../snap_orkut-eps:0.3-min_pts:5-overview.png)

![orkut-speedup](../snap_orkut-eps:0.3-min_pts:5-runtime-speedup.png)

## Medium: Uk

![uk-overview](../webgraph_uk-eps:0.3-min_pts:5-overview.png)

![uk-speedup](../webgraph_uk-eps:0.3-min_pts:5-runtime-speedup.png)

## Large: Webbase

![webbase-overview](../webgraph_webbase-eps:0.3-min_pts:5-overview.png)

![webbase-speedup](../webgraph_webbase-eps:0.3-min_pts:5-runtime-speedup.png)

## Large: Twitter

![twitter-overview](../webgraph_twitter-eps:0.3-min_pts:5-overview.png)

![twitter-speedup](../webgraph_twitter-eps:0.3-min_pts:5-runtime-speedup.png)

## Large: Fridenster

![fridenster-overview](../snap_friendster-eps:0.3-min_pts:5-overview.png)

![friendster-speedup](../snap_friendster-eps:0.3-min_pts:5-runtime-speedup.png)

## LFR: 10million-avgdeg15-maxdeg50

![lfr10million-avgdeg15-maxdeg50-overview](../lfr_10million_avgdeg15-eps:0.3-min_pts:5-overview.png)

![lfr10million-avgdeg15-maxdeg50-speedup](../lfr_10million_avgdeg15-eps:0.3-min_pts:5-runtime-speedup.png)