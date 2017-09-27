dataset | scalability link
--- | ---
small_snap_dblp | [small_snap_dblp.md](small_snap_dblp.md)
snap_pokec | [snap_pokec.md](snap_pokec.md)
snap_livejournal | [snap_livejournal.md](snap_livejournal.md)
snap_orkut | [snap_orkut.md](snap_orkut.md)
webgraph_uk | [webgraph_uk.md](webgraph_uk.md)
webgraph_webbase | [webgraph_webbase.md](webgraph_webbase.md)
webgraph_twitter | [webgraph_twitter.md](webgraph_twitter.md)
snap_friendster | [snap_friendster.md](snap_friendster.md)

## eps:0.1

with 40 full logical threads | with best thread num
--- | ---
![portion with full logical threads-withalllogicalcores-comp-io-portion](../../figures/scalability_new2_opt_scheduler/eps:0.1-min_pts:5-withalllogicalcores-comp-io-portion.png) | ![io portition-withbestlogicalthreadnum-comp-io-portion](../../figures/scalability_new2_opt_scheduler/eps:0.1-min_pts:5-withbestlogicalthreadnum-comp-io-portion.png)

with 40 full logical threads | with best thread num
--- | ---
![with full logical threads-withalllogicalcores-runtime-speedup](../../figures/scalability_new2_opt_scheduler/eps:0.1-min_pts:5-withalllogicalcores-runtime-speedup.png) | ![with best thread num-withbestlogicalthreadnum-runtime-speedup](../../figures/scalability_new2_opt_scheduler/eps:0.1-min_pts:5-withbestlogicalthreadnum-runtime-speedup.png)

```zsh
best performance thread_num_lst:[16, 32, 40, 40, 40, 40, 40, 40]
```

dataset | edge num | pscan runtime | pscan+ 40 logical-core full speedup | pscan+ best thread_num speedup | thread_num choice
--- | --- | --- | --- | --- | ---
small_snap_dblp | 2,099,732 | 0.318s | 1.639 | 1.849 | 16
snap_pokec | 30,282,866 | 6.925s | 4.014 | 4.235 | 32
snap_livejournal | 69,362,378 | 16.088s | 5.109 | 5.109 | 40
snap_orkut | 234,370,166 | 149.207s | 16.932 | 16.932 | 40
webgraph_uk | 301,136,554 | 37.3s | 5.580 | 5.580 | 40
webgraph_webbase | 1,050,026,736 | 129.071s | 5.587 | 5.587 | 40
webgraph_twitter | 1,369,000,750 | 2223.316s | 29.527 | 29.527 | 40
snap_friendster | 3,612,134,270 | 4959.734s | 17.358 | 17.358 | 40

## eps:0.2

with 40 full logical threads | with best thread num
--- | ---
![portion with full logical threads-withalllogicalcores-comp-io-portion](../../figures/scalability_new2_opt_scheduler/eps:0.2-min_pts:5-withalllogicalcores-comp-io-portion.png) | ![io portition-withbestlogicalthreadnum-comp-io-portion](../../figures/scalability_new2_opt_scheduler/eps:0.2-min_pts:5-withbestlogicalthreadnum-comp-io-portion.png)

with 40 full logical threads | with best thread num
--- | ---
![with full logical threads-withalllogicalcores-runtime-speedup](../../figures/scalability_new2_opt_scheduler/eps:0.2-min_pts:5-withalllogicalcores-runtime-speedup.png) | ![with best thread num-withbestlogicalthreadnum-runtime-speedup](../../figures/scalability_new2_opt_scheduler/eps:0.2-min_pts:5-withbestlogicalthreadnum-runtime-speedup.png)

```zsh
best performance thread_num_lst:[16, 40, 32, 40, 16, 40, 40, 40]
```

dataset | edge num | pscan runtime | pscan+ 40 logical-core full speedup | pscan+ best thread_num speedup | thread_num choice
--- | --- | --- | --- | --- | ---
small_snap_dblp | 2,099,732 | 0.504s | 2.667 | 2.847 | 16
snap_pokec | 30,282,866 | 10.004s | 6.425 | 6.425 | 40
snap_livejournal | 69,362,378 | 23.072s | 6.596 | 6.954 | 32
snap_orkut | 234,370,166 | 179.55s | 18.693 | 18.693 | 40
webgraph_uk | 301,136,554 | 26.048s | 4.242 | 4.392 | 16
webgraph_webbase | 1,050,026,736 | 81.475s | 4.217 | 4.217 | 40
webgraph_twitter | 1,369,000,750 | 2973.869s | 30.616 | 30.616 | 40
snap_friendster | 3,612,134,270 | 4673.88s | 24.511 | 24.511 | 40

## eps:0.3

with 40 full logical threads | with best thread num
--- | ---
![portion with full logical threads-withalllogicalcores-comp-io-portion](../../figures/scalability_new2_opt_scheduler/eps:0.3-min_pts:5-withalllogicalcores-comp-io-portion.png) | ![io portition-withbestlogicalthreadnum-comp-io-portion](../../figures/scalability_new2_opt_scheduler/eps:0.3-min_pts:5-withbestlogicalthreadnum-comp-io-portion.png)

with 40 full logical threads | with best thread num
--- | ---
![with full logical threads-withalllogicalcores-runtime-speedup](../../figures/scalability_new2_opt_scheduler/eps:0.3-min_pts:5-withalllogicalcores-runtime-speedup.png) | ![with best thread num-withbestlogicalthreadnum-runtime-speedup](../../figures/scalability_new2_opt_scheduler/eps:0.3-min_pts:5-withbestlogicalthreadnum-runtime-speedup.png)

```zsh
best performance thread_num_lst:[16, 32, 24, 32, 24, 32, 40, 40]
```

dataset | edge num | pscan runtime | pscan+ 40 logical-core full speedup | pscan+ best thread_num speedup | thread_num choice
--- | --- | --- | --- | --- | ---
small_snap_dblp | 2,099,732 | 0.555s | 3.000 | 3.153 | 16
snap_pokec | 30,282,866 | 8.597s | 9.950 | 10.640 | 32
snap_livejournal | 69,362,378 | 21.846s | 8.164 | 8.477 | 24
snap_orkut | 234,370,166 | 164.248s | 17.709 | 18.099 | 32
webgraph_uk | 301,136,554 | 18.498s | 3.446 | 3.638 | 24
webgraph_webbase | 1,050,026,736 | 63.705s | 3.793 | 3.845 | 32
webgraph_twitter | 1,369,000,750 | 2487.317s | 30.361 | 30.361 | 40
snap_friendster | 3,612,134,270 | 3726.302s | 29.133 | 29.133 | 40

## eps:0.4

with 40 full logical threads | with best thread num
--- | ---
![portion with full logical threads-withalllogicalcores-comp-io-portion](../../figures/scalability_new2_opt_scheduler/eps:0.4-min_pts:5-withalllogicalcores-comp-io-portion.png) | ![io portition-withbestlogicalthreadnum-comp-io-portion](../../figures/scalability_new2_opt_scheduler/eps:0.4-min_pts:5-withbestlogicalthreadnum-comp-io-portion.png)

with 40 full logical threads | with best thread num
--- | ---
![with full logical threads-withalllogicalcores-runtime-speedup](../../figures/scalability_new2_opt_scheduler/eps:0.4-min_pts:5-withalllogicalcores-runtime-speedup.png) | ![with best thread num-withbestlogicalthreadnum-runtime-speedup](../../figures/scalability_new2_opt_scheduler/eps:0.4-min_pts:5-withbestlogicalthreadnum-runtime-speedup.png)

```zsh
best performance thread_num_lst:[24, 32, 32, 40, 24, 16, 40, 40]
```

dataset | edge num | pscan runtime | pscan+ 40 logical-core full speedup | pscan+ best thread_num speedup | thread_num choice
--- | --- | --- | --- | --- | ---
small_snap_dblp | 2,099,732 | 0.491s | 3.031 | 3.209 | 24
snap_pokec | 30,282,866 | 6.82s | 10.333 | 11.144 | 32
snap_livejournal | 69,362,378 | 18.37s | 9.363 | 10.166 | 32
snap_orkut | 234,370,166 | 135.772s | 19.024 | 19.024 | 40
webgraph_uk | 301,136,554 | 14.356s | 2.980 | 2.985 | 24
webgraph_webbase | 1,050,026,736 | 52.978s | 3.215 | 3.636 | 16
webgraph_twitter | 1,369,000,750 | 1712.815s | 29.407 | 29.407 | 40
snap_friendster | 3,612,134,270 | 2801.607s | 29.569 | 29.569 | 40

## eps:0.5

with 40 full logical threads | with best thread num
--- | ---
![portion with full logical threads-withalllogicalcores-comp-io-portion](../../figures/scalability_new2_opt_scheduler/eps:0.5-min_pts:5-withalllogicalcores-comp-io-portion.png) | ![io portition-withbestlogicalthreadnum-comp-io-portion](../../figures/scalability_new2_opt_scheduler/eps:0.5-min_pts:5-withbestlogicalthreadnum-comp-io-portion.png)

with 40 full logical threads | with best thread num
--- | ---
![with full logical threads-withalllogicalcores-runtime-speedup](../../figures/scalability_new2_opt_scheduler/eps:0.5-min_pts:5-withalllogicalcores-runtime-speedup.png) | ![with best thread num-withbestlogicalthreadnum-runtime-speedup](../../figures/scalability_new2_opt_scheduler/eps:0.5-min_pts:5-withbestlogicalthreadnum-runtime-speedup.png)

```zsh
best performance thread_num_lst:[32, 40, 40, 40, 16, 40, 40, 40]
```

dataset | edge num | pscan runtime | pscan+ 40 logical-core full speedup | pscan+ best thread_num speedup | thread_num choice
--- | --- | --- | --- | --- | ---
small_snap_dblp | 2,099,732 | 0.441s | 3.615 | 3.737 | 32
snap_pokec | 30,282,866 | 5.709s | 13.528 | 13.528 | 40
snap_livejournal | 69,362,378 | 14.317s | 12.031 | 12.031 | 40
snap_orkut | 234,370,166 | 105.36s | 22.643 | 22.643 | 40
webgraph_uk | 301,136,554 | 11.721s | 2.550 | 2.701 | 16
webgraph_webbase | 1,050,026,736 | 45.006s | 3.684 | 3.684 | 40
webgraph_twitter | 1,369,000,750 | 1112.027s | 30.867 | 30.867 | 40
snap_friendster | 3,612,134,270 | 2019.334s | 28.479 | 28.479 | 40

## eps:0.6

with 40 full logical threads | with best thread num
--- | ---
![portion with full logical threads-withalllogicalcores-comp-io-portion](../../figures/scalability_new2_opt_scheduler/eps:0.6-min_pts:5-withalllogicalcores-comp-io-portion.png) | ![io portition-withbestlogicalthreadnum-comp-io-portion](../../figures/scalability_new2_opt_scheduler/eps:0.6-min_pts:5-withbestlogicalthreadnum-comp-io-portion.png)

with 40 full logical threads | with best thread num
--- | ---
![with full logical threads-withalllogicalcores-runtime-speedup](../../figures/scalability_new2_opt_scheduler/eps:0.6-min_pts:5-withalllogicalcores-runtime-speedup.png) | ![with best thread num-withbestlogicalthreadnum-runtime-speedup](../../figures/scalability_new2_opt_scheduler/eps:0.6-min_pts:5-withbestlogicalthreadnum-runtime-speedup.png)

```zsh
best performance thread_num_lst:[32, 32, 40, 40, 32, 16, 40, 40]
```

dataset | edge num | pscan runtime | pscan+ 40 logical-core full speedup | pscan+ best thread_num speedup | thread_num choice
--- | --- | --- | --- | --- | ---
small_snap_dblp | 2,099,732 | 0.346s | 3.977 | 3.977 | 32
snap_pokec | 30,282,866 | 4.122s | 12.416 | 12.762 | 32
snap_livejournal | 69,362,378 | 10.49s | 8.698 | 8.698 | 40
snap_orkut | 234,370,166 | 73.887s | 23.583 | 23.583 | 40
webgraph_uk | 301,136,554 | 9.737s | 2.532 | 2.582 | 32
webgraph_webbase | 1,050,026,736 | 37.624s | 3.415 | 3.536 | 16
webgraph_twitter | 1,369,000,750 | 671.103s | 30.282 | 30.282 | 40
snap_friendster | 3,612,134,270 | 1382.493s | 28.098 | 28.098 | 40

## eps:0.7

with 40 full logical threads | with best thread num
--- | ---
![portion with full logical threads-withalllogicalcores-comp-io-portion](../../figures/scalability_new2_opt_scheduler/eps:0.7-min_pts:5-withalllogicalcores-comp-io-portion.png) | ![io portition-withbestlogicalthreadnum-comp-io-portion](../../figures/scalability_new2_opt_scheduler/eps:0.7-min_pts:5-withbestlogicalthreadnum-comp-io-portion.png)

with 40 full logical threads | with best thread num
--- | ---
![with full logical threads-withalllogicalcores-runtime-speedup](../../figures/scalability_new2_opt_scheduler/eps:0.7-min_pts:5-withalllogicalcores-runtime-speedup.png) | ![with best thread num-withbestlogicalthreadnum-runtime-speedup](../../figures/scalability_new2_opt_scheduler/eps:0.7-min_pts:5-withbestlogicalthreadnum-runtime-speedup.png)

```zsh
best performance thread_num_lst:[16, 40, 40, 40, 16, 32, 40, 40]
```

dataset | edge num | pscan runtime | pscan+ 40 logical-core full speedup | pscan+ best thread_num speedup | thread_num choice
--- | --- | --- | --- | --- | ---
small_snap_dblp | 2,099,732 | 0.245s | 3.451 | 3.712 | 16
snap_pokec | 30,282,866 | 3.098s | 12.645 | 12.645 | 40
snap_livejournal | 69,362,378 | 7.404s | 10.623 | 10.623 | 40
snap_orkut | 234,370,166 | 48.46s | 21.888 | 21.888 | 40
webgraph_uk | 301,136,554 | 8.152s | 2.316 | 2.404 | 16
webgraph_webbase | 1,050,026,736 | 31.675s | 3.294 | 3.322 | 32
webgraph_twitter | 1,369,000,750 | 378.662s | 29.530 | 29.530 | 40
snap_friendster | 3,612,134,270 | 881.861s | 29.523 | 29.523 | 40

## eps:0.8

with 40 full logical threads | with best thread num
--- | ---
![portion with full logical threads-withalllogicalcores-comp-io-portion](../../figures/scalability_new2_opt_scheduler/eps:0.8-min_pts:5-withalllogicalcores-comp-io-portion.png) | ![io portition-withbestlogicalthreadnum-comp-io-portion](../../figures/scalability_new2_opt_scheduler/eps:0.8-min_pts:5-withbestlogicalthreadnum-comp-io-portion.png)

with 40 full logical threads | with best thread num
--- | ---
![with full logical threads-withalllogicalcores-runtime-speedup](../../figures/scalability_new2_opt_scheduler/eps:0.8-min_pts:5-withalllogicalcores-runtime-speedup.png) | ![with best thread num-withbestlogicalthreadnum-runtime-speedup](../../figures/scalability_new2_opt_scheduler/eps:0.8-min_pts:5-withbestlogicalthreadnum-runtime-speedup.png)

```zsh
best performance thread_num_lst:[32, 40, 40, 32, 32, 16, 40, 32]
```

dataset | edge num | pscan runtime | pscan+ 40 logical-core full speedup | pscan+ best thread_num speedup | thread_num choice
--- | --- | --- | --- | --- | ---
small_snap_dblp | 2,099,732 | 0.183s | 3.519 | 3.660 | 32
snap_pokec | 30,282,866 | 2.009s | 13.217 | 13.217 | 40
snap_livejournal | 69,362,378 | 4.797s | 9.730 | 9.730 | 40
snap_orkut | 234,370,166 | 27.708s | 16.156 | 18.301 | 32
webgraph_uk | 301,136,554 | 7.054s | 1.975 | 2.363 | 32
webgraph_webbase | 1,050,026,736 | 25.399s | 3.002 | 3.098 | 16
webgraph_twitter | 1,369,000,750 | 183.012s | 26.815 | 26.815 | 40
snap_friendster | 3,612,134,270 | 509.733s | 20.976 | 25.322 | 32

## eps:0.9

with 40 full logical threads | with best thread num
--- | ---
![portion with full logical threads-withalllogicalcores-comp-io-portion](../../figures/scalability_new2_opt_scheduler/eps:0.9-min_pts:5-withalllogicalcores-comp-io-portion.png) | ![io portition-withbestlogicalthreadnum-comp-io-portion](../../figures/scalability_new2_opt_scheduler/eps:0.9-min_pts:5-withbestlogicalthreadnum-comp-io-portion.png)

with 40 full logical threads | with best thread num
--- | ---
![with full logical threads-withalllogicalcores-runtime-speedup](../../figures/scalability_new2_opt_scheduler/eps:0.9-min_pts:5-withalllogicalcores-runtime-speedup.png) | ![with best thread num-withbestlogicalthreadnum-runtime-speedup](../../figures/scalability_new2_opt_scheduler/eps:0.9-min_pts:5-withbestlogicalthreadnum-runtime-speedup.png)

```zsh
best performance thread_num_lst:[24, 40, 40, 40, 40, 16, 40, 40]
```

dataset | edge num | pscan runtime | pscan+ 40 logical-core full speedup | pscan+ best thread_num speedup | thread_num choice
--- | --- | --- | --- | --- | ---
small_snap_dblp | 2,099,732 | 0.121s | 2.881 | 3.103 | 24
snap_pokec | 30,282,866 | 1.453s | 13.330 | 13.330 | 40
snap_livejournal | 69,362,378 | 2.753s | 7.605 | 7.605 | 40
snap_orkut | 234,370,166 | 12.535s | 15.533 | 15.533 | 40
webgraph_uk | 301,136,554 | 5.814s | 2.425 | 2.425 | 40
webgraph_webbase | 1,050,026,736 | 20.091s | 2.708 | 2.929 | 16
webgraph_twitter | 1,369,000,750 | 61.578s | 20.223 | 20.223 | 40
snap_friendster | 3,612,134,270 | 245.702s | 19.973 | 19.973 | 40

