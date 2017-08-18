## Attention

All data are collected on gpu-23, cpu 2.3Ghz.

## Snap - Dblp

pscan

```zsh
**** Graph Clustering (Release): ../dataset/small_snap_dblp, 0.3, 5 *** 
        n = 425957; m = 2099732
        *** Finished loading graph!
intersection times:648554
cmp0:3240962
cmp1:10910275
equal cmp:1872596
max portion:163
Total time without IO: 595454
        *** Start write result into disk!
```

pscan+

```zsh
int size:4
n:425957, m:2099732
read degree file time:3 ms
read adjacency list file time:96 ms
check input graph file time:13 ms

Total input cost:127 ms
1st: prune execution time:68 ms
2nd: check core first-phase bsp time:325 ms
2nd: check core second-phase bsp time:117 ms
3rd: core clustering time:46 ms
4th: non-core clustering time:38 ms

prune0 definitely not reachable:0
prune1 definitely reachable:0
intersection times:773778
cmp0:10965872
cmp1:4267180
equal cmp:2890571
max portion:163
Total time without IO:596 ms
Total output cost:277 ms
```

## Snap - Livejournal

pscan

```zsh
**** Graph Clustering (Release): dataset/snap_livejournal, 0.3, 5 *** 
        n = 4036538; m = 69362378
        *** Finished loading graph!
intersection times:25120657
cmp0:715186245
cmp1:1810587638
equal cmp:154770513
max portion:875
Total time without IO: 22516338
        *** Start write result into disk!
```

pscan+

```zsh
```zsh
int size:4
n:4036538, m:69362378

Total input cost:3030 ms
1st: prune execution time:1140 ms
2nd: check core first-phase bsp time:17765 ms
2nd: check core second-phase bsp time:2676 ms
3rd: core clustering time:379 ms
4th: non-core clustering time:311 ms

prune0 definitely not reachable:3369420
prune1 definitely reachable:1001839
intersection times:30309930
cmp0:1776486141
cmp1:994831588
equal cmp:297801180
max portion:875
Total time without IO:22273 ms
Total output cost:2416 ms
```

## Snap - Orkut

pscan

```zsh
**** Graph Clustering (Release): dataset/snap_orkut, 0.3, 5 *** 
        n = 3072627; m = 234370166
        *** Finished loading graph!
intersection times:96851867
cmp0:8115508516
cmp1:17562443634
equal cmp:1033761851
max portion:2009
Total time without IO: 170264073
        *** Start write result into disk!
```

pscan+

```zsh
int size:4
n:3072627, m:234370166

Total input cost:10121 ms
1st: prune execution time:2719 ms
2nd: check core first-phase bsp time:160385 ms
2nd: check core second-phase bsp time:13169 ms
3rd: core clustering time:600 ms
4th: non-core clustering time:603 ms

prune0 definitely not reachable:8445671
prune1 definitely reachable:18560
intersection times:108720852
cmp0:16230884108
cmp1:11146323371
equal cmp:1375991912
max portion:2009
Total time without IO:177478 ms
Total output cost:1358 ms
```

## Snap - Friendster

pscan

```zsh
**** Graph Clustering (Release): dataset/snap_friendster, 0.3, 5 ***
        n = 124836180; m = 3612134270
        *** Finished loading graph!
intersection times:1547097583
cmp0:189028022634
cmp1:399383776139
equal cmp:7711211516
max portion:2745
Total time without IO: 4004564621
        *** Start write result into disk!
```

pscan+

```zsh
int size:4
n:124836180, m:3612134270
read degree file time:925 ms
read adjacency list file time:130188 ms
check input graph file time:4324 ms

Total input cost:162256 ms      
1st: prune execution time:79398 ms
2nd: check core first-phase bsp time:3406565 ms
2nd: check core second-phase bsp time:459172 ms                                                                                                                 
3rd: core clustering time:1790 ms
4th: non-core clustering time:4058 ms

prune0 definitely not reachable:217839645
prune1 definitely reachable:5427415
intersection times:1582800075
cmp0:344733547542
cmp1:245393023242
equal cmp:7835707712
max portion:2745
Total time without IO:3950985 ms
Total output cost:10898 ms
```

## Webgraph - Uk

pscan

```zsh
**** Graph Clustering (Release): dataset/webgraph_uk, 0.3, 5 *** 
        n = 18520343; m = 301136554
        *** Finished loading graph!
intersection times:36719739
cmp0:232732652
cmp1:3562856847
equal cmp:315608563
max portion:7218
Total time without IO: 20339757
        *** Start write result into disk!
```

pscan+

```zsh
int size:4
n:18520343, m:301136554
read degree file time:76 ms
read adjacency list file time:10435 ms
check input graph file time:703 ms

Total input cost:11825 ms
1st: prune execution time:1928 ms
2nd: check core first-phase bsp time:28831 ms
2nd: check core second-phase bsp time:3065 ms
3rd: core clustering time:940 ms
4th: non-core clustering time:632 ms

prune0 definitely not reachable:67656923
prune1 definitely reachable:4997008
intersection times:77914346
cmp0:588466397
cmp1:8695675975
equal cmp:2111915924
max portion:7218
Total time without IO:35398 ms
Total output cost:11359 ms
```

## Webgraph - Webbase

pscan

```zsh
**** Graph Clustering (Release): dataset/webgraph_webbase, 0.3, 5 *** 
        n = 118142143; m = 1050026736
        *** Finished loading graph!
intersection times:150869130
cmp0:1020798101
cmp1:7772394167
equal cmp:847450383
max portion:20398
Total time without IO: 69590352
        *** Start write result into disk!
```

pscan+

```zsh
int size:4
n:118142143, m:1050026736
read degree file time:469 ms
read adjacency list file time:38215 ms
check input graph file time:2177 ms

Total input cost:42694 ms
1st: prune execution time:7448 ms
2nd: check core first-phase bsp time:67899 ms
2nd: check core second-phase bsp time:12014 ms
3rd: core clustering time:3403 ms
4th: non-core clustering time:2755 ms

prune0 definitely not reachable:203886616
prune1 definitely reachable:46119741
intersection times:275007011
cmp0:2061408067
cmp1:15685059958
equal cmp:6698684510
max portion:20398
Total time without IO:93521 ms
Total output cost:65509 ms
```

## Webgraph - Twitter

pscan

```zsh
**** Graph Clustering (Release): dataset/webgraph_twitter, 0.3, 5 ***
        n = 41652230; m = 1369000750
        *** Finished loading graph!
intersection times:306051972
cmp0:143132655618
cmp1:391170393366
equal cmp:29341665477
max portion:22493
Total time without IO: 2616628725
        *** Start write result into disk!   
```

pscan+

```zsh
int size:4
n:41652230, m:1369000750
read degree file time:167 ms
read adjacency list file time:36128 ms
check input graph file time:2174 ms

Total input cost:40450 ms
1st: prune execution time:12652 ms
2nd: check core first-phase bsp time:2765647 ms
2nd: check core second-phase bsp time:117539 ms
3rd: core clustering time:431 ms
4th: non-core clustering time:965 ms

prune0 definitely not reachable:346551512
prune1 definitely reachable:4856528
intersection times:333092335
cmp0:305687955207
cmp1:270084254384
equal cmp:40209354722
max portion:22493
Total time without IO:2897236 ms
Total output cost:3042 ms
```