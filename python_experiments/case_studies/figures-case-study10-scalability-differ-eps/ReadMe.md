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
![portion with full logical threads-withalllogicalcores-comp-io-portion](../../figures/scalability_new3_all_in_parallel/eps:0.1-min_pts:5-withalllogicalcores-comp-io-portion.png) | ![io portition-withbestlogicalthreadnum-comp-io-portion](../../figures/scalability_new3_all_in_parallel/eps:0.1-min_pts:5-withbestlogicalthreadnum-comp-io-portion.png)

with 40 full logical threads | with best thread num
--- | ---
![with full logical threads-withalllogicalcores-runtime-speedup](../../figures/scalability_new3_all_in_parallel/eps:0.1-min_pts:5-withalllogicalcores-runtime-speedup.png) | ![with best thread num-withbestlogicalthreadnum-runtime-speedup](../../figures/scalability_new3_all_in_parallel/eps:0.1-min_pts:5-withbestlogicalthreadnum-runtime-speedup.png)

```zsh
best performance thread_num_lst:[8, 24, 16, 40, 16, 8, 40, 40]
```

dataset | edge num | pscan runtime | pscan+ 40 logical-core full speedup | pscan+ best thread_num speedup | thread_num choice
--- | --- | --- | --- | --- | ---
small_snap_dblp | 2,099,732 | 0.318s | 2.427 | 3.118 | 8
snap_pokec | 30,282,866 | 6.925s | 8.592 | 8.656 | 24
snap_livejournal | 69,362,378 | 16.088s | 8.786 | 10.005 | 16
snap_orkut | 234,370,166 | 149.207s | 34.230 | 34.230 | 40
webgraph_uk | 301,136,554 | 37.3s | 6.491 | 6.777 | 16
webgraph_webbase | 1,050,026,736 | 129.071s | 5.097 | 6.636 | 8
webgraph_twitter | 1,369,000,750 | 2223.316s | 38.738 | 38.738 | 40
snap_friendster | 3,612,134,270 | 4959.734s | 35.192 | 35.192 | 40

## eps:0.2

with 40 full logical threads | with best thread num
--- | ---
![portion with full logical threads-withalllogicalcores-comp-io-portion](../../figures/scalability_new3_all_in_parallel/eps:0.2-min_pts:5-withalllogicalcores-comp-io-portion.png) | ![io portition-withbestlogicalthreadnum-comp-io-portion](../../figures/scalability_new3_all_in_parallel/eps:0.2-min_pts:5-withbestlogicalthreadnum-comp-io-portion.png)

with 40 full logical threads | with best thread num
--- | ---
![with full logical threads-withalllogicalcores-runtime-speedup](../../figures/scalability_new3_all_in_parallel/eps:0.2-min_pts:5-withalllogicalcores-runtime-speedup.png) | ![with best thread num-withbestlogicalthreadnum-runtime-speedup](../../figures/scalability_new3_all_in_parallel/eps:0.2-min_pts:5-withbestlogicalthreadnum-runtime-speedup.png)

```zsh
best performance thread_num_lst:[16, 40, 32, 40, 16, 8, 40, 40]
```

dataset | edge num | pscan runtime | pscan+ 40 logical-core full speedup | pscan+ best thread_num speedup | thread_num choice
--- | --- | --- | --- | --- | ---
small_snap_dblp | 2,099,732 | 0.504s | 4.000 | 4.990 | 16
snap_pokec | 30,282,866 | 10.004s | 12.397 | 12.397 | 40
snap_livejournal | 69,362,378 | 23.072s | 12.933 | 14.043 | 32
snap_orkut | 234,370,166 | 179.55s | 29.835 | 29.835 | 40
webgraph_uk | 301,136,554 | 26.048s | 5.743 | 6.224 | 16
webgraph_webbase | 1,050,026,736 | 81.475s | 3.837 | 5.526 | 8
webgraph_twitter | 1,369,000,750 | 2973.869s | 33.029 | 33.029 | 40
snap_friendster | 3,612,134,270 | 4673.88s | 30.259 | 30.259 | 40

## eps:0.3

with 40 full logical threads | with best thread num
--- | ---
![portion with full logical threads-withalllogicalcores-comp-io-portion](../../figures/scalability_new3_all_in_parallel/eps:0.3-min_pts:5-withalllogicalcores-comp-io-portion.png) | ![io portition-withbestlogicalthreadnum-comp-io-portion](../../figures/scalability_new3_all_in_parallel/eps:0.3-min_pts:5-withbestlogicalthreadnum-comp-io-portion.png)

with 40 full logical threads | with best thread num
--- | ---
![with full logical threads-withalllogicalcores-runtime-speedup](../../figures/scalability_new3_all_in_parallel/eps:0.3-min_pts:5-withalllogicalcores-runtime-speedup.png) | ![with best thread num-withbestlogicalthreadnum-runtime-speedup](../../figures/scalability_new3_all_in_parallel/eps:0.3-min_pts:5-withbestlogicalthreadnum-runtime-speedup.png)

```zsh
best performance thread_num_lst:[8, 40, 24, 40, 40, 8, 40, 40]
```

dataset | edge num | pscan runtime | pscan+ 40 logical-core full speedup | pscan+ best thread_num speedup | thread_num choice
--- | --- | --- | --- | --- | ---
small_snap_dblp | 2,099,732 | 0.555s | 4.744 | 5.187 | 8
snap_pokec | 30,282,866 | 8.597s | 14.257 | 14.257 | 40
snap_livejournal | 69,362,378 | 21.846s | 9.376 | 13.419 | 24
snap_orkut | 234,370,166 | 164.248s | 30.787 | 30.787 | 40
webgraph_uk | 301,136,554 | 18.498s | 5.032 | 5.032 | 40
webgraph_webbase | 1,050,026,736 | 63.705s | 3.755 | 4.922 | 8
webgraph_twitter | 1,369,000,750 | 2487.317s | 32.272 | 32.272 | 40
snap_friendster | 3,612,134,270 | 3726.302s | 30.960 | 30.960 | 40

## eps:0.4

with 40 full logical threads | with best thread num
--- | ---
![portion with full logical threads-withalllogicalcores-comp-io-portion](../../figures/scalability_new3_all_in_parallel/eps:0.4-min_pts:5-withalllogicalcores-comp-io-portion.png) | ![io portition-withbestlogicalthreadnum-comp-io-portion](../../figures/scalability_new3_all_in_parallel/eps:0.4-min_pts:5-withbestlogicalthreadnum-comp-io-portion.png)

with 40 full logical threads | with best thread num
--- | ---
![with full logical threads-withalllogicalcores-runtime-speedup](../../figures/scalability_new3_all_in_parallel/eps:0.4-min_pts:5-withalllogicalcores-runtime-speedup.png) | ![with best thread num-withbestlogicalthreadnum-runtime-speedup](../../figures/scalability_new3_all_in_parallel/eps:0.4-min_pts:5-withbestlogicalthreadnum-runtime-speedup.png)

```zsh
best performance thread_num_lst:[16, 40, 32, 40, 16, 16, 40, 40]
```

dataset | edge num | pscan runtime | pscan+ 40 logical-core full speedup | pscan+ best thread_num speedup | thread_num choice
--- | --- | --- | --- | --- | ---
small_snap_dblp | 2,099,732 | 0.491s | 4.505 | 4.960 | 16
snap_pokec | 30,282,866 | 6.82s | 12.653 | 12.653 | 40
snap_livejournal | 69,362,378 | 18.37s | 13.791 | 14.385 | 32
snap_orkut | 234,370,166 | 135.772s | 29.036 | 29.036 | 40
webgraph_uk | 301,136,554 | 14.356s | 4.568 | 4.743 | 16
webgraph_webbase | 1,050,026,736 | 52.978s | 4.010 | 5.243 | 16
webgraph_twitter | 1,369,000,750 | 1712.815s | 31.097 | 31.097 | 40
snap_friendster | 3,612,134,270 | 2801.607s | 29.120 | 29.120 | 40

## eps:0.5

with 40 full logical threads | with best thread num
--- | ---
![portion with full logical threads-withalllogicalcores-comp-io-portion](../../figures/scalability_new3_all_in_parallel/eps:0.5-min_pts:5-withalllogicalcores-comp-io-portion.png) | ![io portition-withbestlogicalthreadnum-comp-io-portion](../../figures/scalability_new3_all_in_parallel/eps:0.5-min_pts:5-withbestlogicalthreadnum-comp-io-portion.png)

with 40 full logical threads | with best thread num
--- | ---
![with full logical threads-withalllogicalcores-runtime-speedup](../../figures/scalability_new3_all_in_parallel/eps:0.5-min_pts:5-withalllogicalcores-runtime-speedup.png) | ![with best thread num-withbestlogicalthreadnum-runtime-speedup](../../figures/scalability_new3_all_in_parallel/eps:0.5-min_pts:5-withbestlogicalthreadnum-runtime-speedup.png)

```zsh
best performance thread_num_lst:[16, 40, 32, 40, 40, 8, 40, 40]
```

dataset | edge num | pscan runtime | pscan+ 40 logical-core full speedup | pscan+ best thread_num speedup | thread_num choice
--- | --- | --- | --- | --- | ---
small_snap_dblp | 2,099,732 | 0.441s | 4.900 | 5.250 | 16
snap_pokec | 30,282,866 | 5.709s | 13.958 | 13.958 | 40
snap_livejournal | 69,362,378 | 14.317s | 12.072 | 14.579 | 32
snap_orkut | 234,370,166 | 105.36s | 24.996 | 24.996 | 40
webgraph_uk | 301,136,554 | 11.721s | 4.540 | 4.540 | 40
webgraph_webbase | 1,050,026,736 | 45.006s | 4.126 | 4.211 | 8
webgraph_twitter | 1,369,000,750 | 1112.027s | 31.903 | 31.903 | 40
snap_friendster | 3,612,134,270 | 2019.334s | 29.510 | 29.510 | 40

## eps:0.6

with 40 full logical threads | with best thread num
--- | ---
![portion with full logical threads-withalllogicalcores-comp-io-portion](../../figures/scalability_new3_all_in_parallel/eps:0.6-min_pts:5-withalllogicalcores-comp-io-portion.png) | ![io portition-withbestlogicalthreadnum-comp-io-portion](../../figures/scalability_new3_all_in_parallel/eps:0.6-min_pts:5-withbestlogicalthreadnum-comp-io-portion.png)

with 40 full logical threads | with best thread num
--- | ---
![with full logical threads-withalllogicalcores-runtime-speedup](../../figures/scalability_new3_all_in_parallel/eps:0.6-min_pts:5-withalllogicalcores-runtime-speedup.png) | ![with best thread num-withbestlogicalthreadnum-runtime-speedup](../../figures/scalability_new3_all_in_parallel/eps:0.6-min_pts:5-withbestlogicalthreadnum-runtime-speedup.png)

```zsh
best performance thread_num_lst:[8, 40, 40, 40, 40, 40, 40, 40]
```

dataset | edge num | pscan runtime | pscan+ 40 logical-core full speedup | pscan+ best thread_num speedup | thread_num choice
--- | --- | --- | --- | --- | ---
small_snap_dblp | 2,099,732 | 0.346s | 4.325 | 5.014 | 8
snap_pokec | 30,282,866 | 4.122s | 13.127 | 13.127 | 40
snap_livejournal | 69,362,378 | 10.49s | 14.901 | 14.901 | 40
snap_orkut | 234,370,166 | 73.887s | 23.286 | 23.286 | 40
webgraph_uk | 301,136,554 | 9.737s | 4.057 | 4.057 | 40
webgraph_webbase | 1,050,026,736 | 37.624s | 4.286 | 4.286 | 40
webgraph_twitter | 1,369,000,750 | 671.103s | 31.156 | 31.156 | 40
snap_friendster | 3,612,134,270 | 1382.493s | 31.684 | 31.684 | 40

## eps:0.7

with 40 full logical threads | with best thread num
--- | ---
![portion with full logical threads-withalllogicalcores-comp-io-portion](../../figures/scalability_new3_all_in_parallel/eps:0.7-min_pts:5-withalllogicalcores-comp-io-portion.png) | ![io portition-withbestlogicalthreadnum-comp-io-portion](../../figures/scalability_new3_all_in_parallel/eps:0.7-min_pts:5-withbestlogicalthreadnum-comp-io-portion.png)

with 40 full logical threads | with best thread num
--- | ---
![with full logical threads-withalllogicalcores-runtime-speedup](../../figures/scalability_new3_all_in_parallel/eps:0.7-min_pts:5-withalllogicalcores-runtime-speedup.png) | ![with best thread num-withbestlogicalthreadnum-runtime-speedup](../../figures/scalability_new3_all_in_parallel/eps:0.7-min_pts:5-withbestlogicalthreadnum-runtime-speedup.png)

```zsh
best performance thread_num_lst:[16, 32, 40, 40, 32, 40, 40, 40]
```

dataset | edge num | pscan runtime | pscan+ 40 logical-core full speedup | pscan+ best thread_num speedup | thread_num choice
--- | --- | --- | --- | --- | ---
small_snap_dblp | 2,099,732 | 0.245s | 3.889 | 4.298 | 16
snap_pokec | 30,282,866 | 3.098s | 11.735 | 12.908 | 32
snap_livejournal | 69,362,378 | 7.404s | 13.486 | 13.486 | 40
snap_orkut | 234,370,166 | 48.46s | 19.885 | 19.885 | 40
webgraph_uk | 301,136,554 | 8.152s | 4.058 | 4.107 | 32
webgraph_webbase | 1,050,026,736 | 31.675s | 4.361 | 4.361 | 40
webgraph_twitter | 1,369,000,750 | 378.662s | 29.388 | 29.388 | 40
snap_friendster | 3,612,134,270 | 881.861s | 27.381 | 27.381 | 40

## eps:0.8

with 40 full logical threads | with best thread num
--- | ---
![portion with full logical threads-withalllogicalcores-comp-io-portion](../../figures/scalability_new3_all_in_parallel/eps:0.8-min_pts:5-withalllogicalcores-comp-io-portion.png) | ![io portition-withbestlogicalthreadnum-comp-io-portion](../../figures/scalability_new3_all_in_parallel/eps:0.8-min_pts:5-withbestlogicalthreadnum-comp-io-portion.png)

with 40 full logical threads | with best thread num
--- | ---
![with full logical threads-withalllogicalcores-runtime-speedup](../../figures/scalability_new3_all_in_parallel/eps:0.8-min_pts:5-withalllogicalcores-runtime-speedup.png) | ![with best thread num-withbestlogicalthreadnum-runtime-speedup](../../figures/scalability_new3_all_in_parallel/eps:0.8-min_pts:5-withbestlogicalthreadnum-runtime-speedup.png)

```zsh
best performance thread_num_lst:[8, 40, 40, 40, 40, 16, 40, 40]
```

dataset | edge num | pscan runtime | pscan+ 40 logical-core full speedup | pscan+ best thread_num speedup | thread_num choice
--- | --- | --- | --- | --- | ---
small_snap_dblp | 2,099,732 | 0.183s | 3.211 | 4.159 | 8
snap_pokec | 30,282,866 | 2.009s | 13.305 | 13.305 | 40
snap_livejournal | 69,362,378 | 4.797s | 11.700 | 11.700 | 40
snap_orkut | 234,370,166 | 27.708s | 20.449 | 20.449 | 40
webgraph_uk | 301,136,554 | 7.054s | 3.997 | 3.997 | 40
webgraph_webbase | 1,050,026,736 | 25.399s | 3.969 | 4.207 | 16
webgraph_twitter | 1,369,000,750 | 183.012s | 27.368 | 27.368 | 40
snap_friendster | 3,612,134,270 | 509.733s | 25.529 | 25.529 | 40

## eps:0.9

with 40 full logical threads | with best thread num
--- | ---
![portion with full logical threads-withalllogicalcores-comp-io-portion](../../figures/scalability_new3_all_in_parallel/eps:0.9-min_pts:5-withalllogicalcores-comp-io-portion.png) | ![io portition-withbestlogicalthreadnum-comp-io-portion](../../figures/scalability_new3_all_in_parallel/eps:0.9-min_pts:5-withbestlogicalthreadnum-comp-io-portion.png)

with 40 full logical threads | with best thread num
--- | ---
![with full logical threads-withalllogicalcores-runtime-speedup](../../figures/scalability_new3_all_in_parallel/eps:0.9-min_pts:5-withalllogicalcores-runtime-speedup.png) | ![with best thread num-withbestlogicalthreadnum-runtime-speedup](../../figures/scalability_new3_all_in_parallel/eps:0.9-min_pts:5-withbestlogicalthreadnum-runtime-speedup.png)

```zsh
best performance thread_num_lst:[16, 32, 40, 40, 40, 32, 40, 40]
```

dataset | edge num | pscan runtime | pscan+ 40 logical-core full speedup | pscan+ best thread_num speedup | thread_num choice
--- | --- | --- | --- | --- | ---
small_snap_dblp | 2,099,732 | 0.121s | 2.574 | 3.270 | 16
snap_pokec | 30,282,866 | 1.453s | 11.092 | 12.008 | 32
snap_livejournal | 69,362,378 | 2.753s | 10.548 | 10.548 | 40
snap_orkut | 234,370,166 | 12.535s | 14.293 | 14.293 | 40
webgraph_uk | 301,136,554 | 5.814s | 3.703 | 3.703 | 40
webgraph_webbase | 1,050,026,736 | 20.091s | 3.634 | 3.939 | 32
webgraph_twitter | 1,369,000,750 | 61.578s | 19.980 | 19.980 | 40
snap_friendster | 3,612,134,270 | 245.702s | 19.425 | 19.425 | 40

