## Twitter

### Desktop

pscan

```zsh
**** Graph Clustering (Release): ../dataset, 0.3, 5 ***
	*** Finished loading graph!
Total time without IO: 2340757763
	*** Start write result into disk!
```

sequential

```zsh
int size:4
n:41652230, m:1369000750
read degree file time:103 ms
read adjacency list file time:3082 ms
check input graph file time:1752 ms

Total input cost:6502 ms
with google perf start------------
1st: prune execution time:12238 ms
2nd: check core first-phase bsp time:2378220 ms
2nd: check core second-phase bsp time:142053 ms
3rd: core clustering time:424 ms
4th: non-core clustering time:936 ms

prune0 definitely not reachable:346551512
prune1 definitely reachable:4856528
intersection times:333092335
cmp0:305687955207
cmp1:270084254384
equal cmp:40209354722
max portion:22493
with google perf end--------------
PROFILE: interrupts/evictions/bytes = 252943/229135/14664480
Total time without IO:2533875 ms
Total output cost:528 ms
```

parallel

```zsh
int size:4
n:41652230, m:1369000750
read degree file time:1300 ms
read adjacency list file time:37902 ms
check input graph file time:1801 ms

Total input cost:42550 ms
with google perf start------------
1st: prune execution time:4700 ms
2nd: check core first-phase bsp time:1853156 ms
2nd: check core second-phase bsp time:49436 ms
3rd: core clustering time:448 ms
4th: non-core clustering time:841 ms
with google perf end--------------
PROFILE: interrupts/evictions/bytes = 265429/18187/1756040
Total time without IO:1908585 ms
Total output cost:503 ms
```

parallel with smaller tasks in density eval

```zsh
int size:4
n:41652230, m:1369000750
read degree file time:97 ms
read adjacency list file time:3044 ms
check input graph file time:1737 ms

Total input cost:6592 ms
with google perf start------------
1st: prune execution time:4322 ms
2nd: check core first-phase bsp time:432875 ms
2nd: check core second-phase bsp time:26933 ms
3rd: core clustering time:415 ms
4th: non-core clustering time:858 ms
with google perf end--------------
PROFILE: interrupts/evictions/bytes = 358331/35396/3410472
Total time without IO:465406 ms
Total output cost:613 ms
```

### Cluster: gpu-23

sequential

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

parallel

```zsh
int size:4
n:41652230, m:1369000750
read degree file time:410 ms
read adjacency list file time:41889 ms
check input graph file time:2071 ms

Total input cost:46358 ms
1st: prune execution time:3073 ms
2nd: check core first-phase bsp time:1972818 ms
2nd: check core second-phase bsp time:27115 ms
3rd: core clustering time:973 ms
4th: non-core clustering time:1141 ms
Total time without IO:2005122 ms
Total output cost:3239 ms
```

parallel with smaller tasks in density eval

```zsh
int size:4
n:41652230, m:1369000750
read degree file time:384 ms
read adjacency list file time:44108 ms
check input graph file time:2180 ms

Total input cost:48724 ms
1st: prune execution time:3314 ms
2nd: check core first-phase bsp time:116536 ms
2nd: check core second-phase bsp time:10997 ms
3rd: core clustering time:634 ms
4th: non-core clustering time:970 ms
Total time without IO:132453 ms
Total output cost:3090 ms
```

## Webbase

### Desktop

pscan

```zsh
**** Graph Clustering (Release): ../dataset, 0.3, 5 ***
	*** Finished loading graph!
Total time without IO: 59182874
	*** Start write result into disk!
```

sequential

```zsh
int size:4
n:118142143, m:1050026736
read degree file time:221 ms
read adjacency list file time:4030 ms
check input graph file time:1728 ms

Total input cost:7543 ms
with google perf start------------
1st: prune execution time:6547 ms
2nd: check core first-phase bsp time:57428 ms
2nd: check core second-phase bsp time:10702 ms
3rd: core clustering time:3496 ms
4th: non-core clustering time:2447 ms

prune0 definitely not reachable:203886616
prune1 definitely reachable:46119741
intersection times:275007011
cmp0:2061408067
cmp1:15685059958
equal cmp:6698684510
max portion:20398
with google perf end--------------
PROFILE: interrupts/evictions/bytes = 8048/7144/456808
Total time without IO:80626 ms
Total output cost:18280 ms
```

parallel

```zsh
int size:4
n:118142143, m:1050026736
read degree file time:251 ms
read adjacency list file time:3930 ms
check input graph file time:1728 ms

Total input cost:7431 ms
with google perf start------------
1st: prune execution time:1427 ms
2nd: check core first-phase bsp time:13011 ms
2nd: check core second-phase bsp time:2002 ms
3rd: core clustering time:3377 ms
4th: non-core clustering time:2469 ms
with google perf end--------------
PROFILE: interrupts/evictions/bytes = 13010/2314/223248
Total time without IO:22290 ms
Total output cost:16729 ms
```

### Cluster: gpu-23

sequential

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

parallel

```zsh
int size:4
n:118142143, m:1050026736
read degree file time:1073 ms
read adjacency list file time:41282 ms
check input graph file time:2166 ms

Total input cost:46372 ms
1st: prune execution time:681 ms
2nd: check core first-phase bsp time:4594 ms
2nd: check core second-phase bsp time:1008 ms
3rd: core clustering time:4341 ms
4th: non-core clustering time:2763 ms
Total time without IO:13388 ms
Total output cost:65331 ms
```

## Uk

### Desktop

pscan

```zsh
**** Graph Clustering (Release): ../dataset, 0.3, 5 *** 
	*** Finished loading graph!
Total time without IO: 18003817
	*** Start write result into disk!
```

sequential

```zsh
int size:4
n:18520343, m:301136554
read degree file time:48 ms
read adjacency list file time:865 ms
check input graph file time:419 ms

Total input cost:1716 ms
with google perf start------------
1st: prune execution time:1518 ms
2nd: check core first-phase bsp time:24966 ms
2nd: check core second-phase bsp time:2698 ms
3rd: core clustering time:994 ms
4th: non-core clustering time:566 ms

prune0 definitely not reachable:67656923
prune1 definitely reachable:4997008
intersection times:77914346
cmp0:588466397
cmp1:8695675975
equal cmp:2111915924
max portion:7218
with google perf end--------------
PROFILE: interrupts/evictions/bytes = 3069/2776/178456
Total time without IO:30745 ms
Total output cost:1980 ms
```

parallel

```zsh
int size:4
n:18520343, m:301136554
read degree file time:51 ms
read adjacency list file time:852 ms
check input graph file time:419 ms

Total input cost:1701 ms
with google perf start------------
1st: prune execution time:350 ms
2nd: check core first-phase bsp time:5796 ms
2nd: check core second-phase bsp time:511 ms
3rd: core clustering time:946 ms
4th: non-core clustering time:556 ms
with google perf end--------------
PROFILE: interrupts/evictions/bytes = 5193/642/69288
Total time without IO:8164 ms
Total output cost:1928 ms
```

### Cluster: gpu-23

sequential

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

parallel

```zsh
int size:4
n:18520343, m:301136554
read degree file time:185 ms
read adjacency list file time:11473 ms
check input graph file time:745 ms

Total input cost:13019 ms
1st: prune execution time:196 ms
2nd: check core first-phase bsp time:2177 ms
2nd: check core second-phase bsp time:205 ms
3rd: core clustering time:1424 ms
4th: non-core clustering time:702 ms
Total time without IO:4706 ms
Total output cost:11012 ms
```