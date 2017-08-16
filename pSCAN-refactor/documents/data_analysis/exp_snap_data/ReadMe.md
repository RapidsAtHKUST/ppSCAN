# Snap Datasets

## Friendster

### Desktop

without clearing page-cache, sequential

```zsh
int size:4
n:124836180, m:3612134270
read degree file time:233 ms
read adjacency list file time:20947 ms
check input graph file time:4662 ms

Total input cost:30099 ms
with google perf start------------
1st: prune execution time:83082 ms
2nd: check core first-phase bsp time:2912217 ms
2nd: check core second-phase bsp time:512762 ms
3rd: core clustering time:1470 ms
4th: non-core clustering time:3402 ms

prune0 definitely not reachable:217839645
prune1 definitely reachable:5427415
intersection times:1582800075
cmp0:344733547542
cmp1:245393023242
equal cmp:7835707712
max portion:2745
with google perf end--------------
PROFILE: interrupts/evictions/bytes = 350648/298193/19081752
Total time without IO:3512963 ms
Total output cost:2114 ms
```

without clearing page-cache, parallel

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

parallel with smaller tasks in density eval

```zsh
int size:4
n:124836180, m:3612134270
read degree file time:3257 ms
read adjacency list file time:96345 ms
check input graph file time:4704 ms

Total input cost:108502 ms
with google perf start------------
1st: prune execution time:20213 ms
2nd: check core first-phase bsp time:476118 ms
2nd: check core second-phase bsp time:98080 ms
3rd: core clustering time:1564 ms
4th: non-core clustering time:3324 ms
with google perf end--------------
PROFILE: interrupts/evictions/bytes = 473683/35996/3468416
Total time without IO:599304 ms
Total output cost:1901 ms
```

### Cluster: luocpu8

without clearing page-cache, sequential

```zsh
int size:4                                                                                                                                                                                    
n:124836180, m:3612134270                                                                                                                                                                     
read degree file time:264 ms                                                                                                                                                                  
read adjacency list file time:13269 ms                                                                                                                                                        
check input graph file time:7265 ms                                                                                                                                                           
                                                                                                                                                                                              
Total input cost:30049 ms                                                                                                                                                                     
with google perf start------------                                                                                                                                                            
1st: prune execution time:113179 ms                                                                                                                                                           
2nd: check core first-phase bsp time:4332802 ms                                                                                                                                               
2nd: check core second-phase bsp time:530805 ms                                                                                                                                               
3rd: core clustering time:2419 ms              
4th: non-core clustering time:5549 ms          

prune0 definitely not reachable:217839645      
prune1 definitely reachable:5427415            
intersection times:1582800075                  
cmp0:344733547542                              
cmp1:245393023242                              
equal cmp:7835707712                           
max portion:2745                               
with google perf end--------------             
PROFILE: interrupts/evictions/bytes = 498472/418180/26758648                                   
Total time without IO:4984785 ms               
Total output cost:3828 ms           
```

without clearing page-cache, parallel

```zsh
int size:4                                                                      
n:124836180, m:3612134270                                                       
read degree file time:272 ms                                                    
read adjacency list file time:12872 ms                                          
check input graph file time:7270 ms                                             
                                                                                
Total input cost:29458 ms                                                       
with google perf start------------                                              
1st: prune execution time:15418 ms                                              
2nd: check core first-phase bsp time:405723 ms                                  
2nd: check core second-phase bsp time:64124 ms                                  
3rd: core clustering time:2510 ms                                               
4th: non-core clustering time:6019 ms                                           
with google perf end--------------                                              
PROFILE: interrupts/evictions/bytes = 483439/379885/39541344                    
Total time without IO:493800 ms                                                 
Total output cost:3655 ms                                                       
```

### Cluster: gpu-23

without clearing page-cache, sequential

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

without clearing page-cache, parallel

```zsh
int size:4
n:124836180, m:3612134270
read degree file time:1003 ms
read adjacency list file time:133417 ms
check input graph file time:4377 ms

Total input cost:194094 ms
1st: prune execution time:5565 ms
2nd: check core first-phase bsp time:154054 ms
2nd: check core second-phase bsp time:31065 ms
3rd: core clustering time:2010 ms
4th: non-core clustering time:6660 ms
Total time without IO:199357 ms
Total output cost:10173 ms
```

parallel with smaller tasks in density eval

```zsh
int size:4
n:124836180, m:3612134270
read degree file time:1126 ms
read adjacency list file time:139372 ms
check input graph file time:4364 ms

Total input cost:224349 ms
1st: prune execution time:5463 ms
2nd: check core first-phase bsp time:147574 ms
2nd: check core second-phase bsp time:24656 ms
3rd: core clustering time:2304 ms
4th: non-core clustering time:4060 ms
Total time without IO:184070 ms
Total output cost:9787 ms
```

## Orkut

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

## LiveJournal

### Desktop
__
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

