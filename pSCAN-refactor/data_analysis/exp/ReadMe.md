# Experiments

if possible, page cache are all dropped first 

```zsh
echo 3 > /proc/sys/vm/drop_caches
```

## Machine Configuration

Desktop

hardware | detail
--- | ---   
disk read | 190MB/s
memory | 32GB
cpu | 8 logical threads, hyper-threading, 3.4GHz

Cluster: luocpu8

hardware | detail
--- | ---
memory | 47GB
cpu | 16 logical threads, hyper-threading, 2.4GHz, 2 NUMA nodes

Cluster: gpu-23

hardware | detail
--- | ---
memory | 64GB
cpu | 40 logical threads, hyper-threading, 2.3GHz, 2 NUMA nodes 2 sockets

## Dataset 0: LiveJournal

### Desktop

sequential

```zsh
int size:4
n:4036538, m:69362378

Total input cost:2129 ms
with google perf start------------
1st: prune execution time:703 ms
2nd: check core first-phase bsp time:15718 ms
2nd: check core second-phase bsp time:2574 ms
3rd: core clustering time:366 ms
4th: non-core clustering time:339 ms

prune0 definitely not reachable:3369420
prune1 definitely reachable:1001839
intersection times:30309930
cmp0:1776486141
cmp1:994831588
equal cmp:297801180
max portion:875
with google perf end--------------
PROFILE: interrupts/evictions/bytes = 1964/1523/98272
Total time without IO:19704 ms
Total output cost:540 ms
```

parallel 

```zsh
int size:4
n:4036538, m:69362378

Total input cost:2229 ms
with google perf start------------
1st: prune execution time:164 ms
2nd: check core first-phase bsp time:3654 ms
2nd: check core second-phase bsp time:611 ms
3rd: core clustering time:390 ms
4th: non-core clustering time:339 ms
with google perf end--------------
PROFILE: interrupts/evictions/bytes = 2620/241/29512
Total time without IO:5163 ms
Total output cost:501 ms
```

### Cluster: luocpu8

sequential

```zsh
int size:4                                                                                                                                                                                    
n:4036538, m:69362378                                                                                                                                                                         
                                                                                                                                                                                              
Total input cost:2198 ms                                                                                                                                                                      
with google perf start------------                                                                                                                                                            
1st: prune execution time:1230 ms                                                                                                                                                             
2nd: check core first-phase bsp time:23029 ms                                                                                                                                                 
2nd: check core second-phase bsp time:3111 ms                                                                                                                                                 
3rd: core clustering time:506 ms                                                                                                                                                              
4th: non-core clustering time:429 ms                                                                                                                                                          

prune0 definitely not reachable:3369420        
prune1 definitely reachable:1001839            
intersection times:30309930                    
cmp0:1776486141                                
cmp1:994831588                                 
equal cmp:297801180                            
max portion:875                                
with google perf end--------------             
PROFILE: interrupts/evictions/bytes = 2830/2387/153344                                         
Total time without IO:28309 ms                 
Total output cost:838 ms    
```

parallel

```zsh
Total input cost:2173 ms
with google perf start------------
1st: prune execution time:157 ms
2nd: check core first-phase bsp time:4388 ms
2nd: check core second-phase bsp time:456 ms
3rd: core clustering time:522 ms
4th: non-core clustering time:453 ms
with google perf end--------------
PROFILE: interrupts/evictions/bytes = 2990/2287/238480
Total time without IO:5982 ms
Total output cost:938 ms
```

### Cluster: gpu23

sequential

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

parallel

```zsh
int size:4
n:4036538, m:69362378

Total input cost:3452 ms
1st: prune execution time:99 ms
2nd: check core first-phase bsp time:2510 ms
2nd: check core second-phase bsp time:245 ms
3rd: core clustering time:736 ms
4th: non-core clustering time:407 ms
Total time without IO:3999 ms
Total output cost:2334 ms
```

## Dataset 1: Orkut

### Desktop

sequential

```zsh
int size:4
n:3072627, m:234370166

Total input cost:6879 ms
with google perf start------------
1st: prune execution time:2253 ms
2nd: check core first-phase bsp time:137746 ms
2nd: check core second-phase bsp time:14660 ms
3rd: core clustering time:540 ms
4th: non-core clustering time:592 ms

prune0 definitely not reachable:8445671
prune1 definitely reachable:18560
intersection times:108720852
cmp0:16230884108
cmp1:11146323371
equal cmp:1375991912
max portion:2009
with google perf end--------------
PROFILE: interrupts/evictions/bytes = 15545/13770/881672
Total time without IO:155811 ms
Total output cost:291 ms
```

parallel

```zsh
int size:4
n:3072627, m:234370166

Total input cost:6990 ms
with google perf start------------
1st: prune execution time:475 ms
2nd: check core first-phase bsp time:23224 ms
2nd: check core second-phase bsp time:3143 ms
3rd: core clustering time:552 ms
4th: non-core clustering time:598 ms
with google perf end--------------
PROFILE: interrupts/evictions/bytes = 21156/1426/144544
Total time without IO:28004 ms
Total output cost:314 ms
```

### Cluster: luocpu8

sequential

```zsh
Total input cost:6525 ms                       
with google perf start------------             
1st: prune execution time:3800 ms              
2nd: check core first-phase bsp time:205698 ms 
2nd: check core second-phase bsp time:14668 ms 
3rd: core clustering time:713 ms               
4th: non-core clustering time:808 ms           

prune0 definitely not reachable:8445671        
prune1 definitely reachable:18560              
intersection times:108720852                   
cmp0:16230884108       
cmp1:11146323371       
equal cmp:1375991912   
max portion:2009       
with google perf end--------------             
PROFILE: interrupts/evictions/bytes = 22567/19354/1238784                                      
Total time without IO:225705 ms                
Total output cost:493 ms           
```

parallel

```zsh
int size:4                                                                                     
n:3072627, m:234370166                         
                                               
Total input cost:6492 ms                       
with google perf start------------             
1st: prune execution time:501 ms               
2nd: check core first-phase bsp time:20914 ms  
2nd: check core second-phase bsp time:2235 ms  
3rd: core clustering time:825 ms               
4th: non-core clustering time:921 ms                                                           
with google perf end--------------             
PROFILE: interrupts/evictions/bytes = 22420/12654/1318448                                      
Total time without IO:25424 ms                 
Total output cost:521 ms               
```

### Cluster: gpu23

sequential

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

parallel

```zsh
int size:4
n:3072627, m:234370166

Total input cost:9557 ms
1st: prune execution time:276 ms
2nd: check core first-phase bsp time:9208 ms
2nd: check core second-phase bsp time:1118 ms
3rd: core clustering time:596 ms
4th: non-core clustering time:596 ms
Total time without IO:11795 ms
Total output cost:1346 ms
```

## Dataset2: friendster

### Desktop

```zsh
int size:4
n:124836180, m:3612134270
read degree file time:3368 ms
read adjacency list file time:96129 ms
check input graph file time:4825 ms

Total input cost:124244 ms
with google perf start------------
1st: prune execution time:21597 ms
2nd: check core first-phase bsp time:479283 ms
2nd: check core second-phase bsp time:105028 ms
3rd: core clustering time:1539 ms
4th: non-core clustering time:35237 ms
with google perf end--------------
PROFILE: interrupts/evictions/bytes = 479436/42017/4041832
Total time without IO:643497 ms
Total output cost:2757 ms
```