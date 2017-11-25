# Breakdown Time

## Case Study

dataset: livejournal(`n:4036538`, `m:69362378`(double of edge number)) , input parameters: `eps:0.2`, `min_pts:5`

### Breakdown Time Table

algorithm | similarity | binary search/easy-comp-pruning | other | total
--- | --- | --- | --- | ---
SCAN0   | 38.41 | 0.00  | 6.28  | 44.69
SCAN1   | 20.17 | 0.68  | 5.24  | 26.09
SCAN2   | 15.02 | 0.67  | 5.01  | 20.70
pSCAN0  | 17.79 | 4.13  | 7.94  | 29.86
pSCAN1  | 15.25 | 4.06  | 6.08  | 25.39
ppSCAN0 | 14.64 | 0.70  | 4.33  | 19.67
ppSCAN1 | 11.65 | 0.70  | 4.26  | 16.61

### Similarity Evaluation Instruction Table

algorithm | similarity instruction | sim-cpi | all instruction | all-cpi
--- | --- | --- | --- | ---
SCAN0     | 166.8 billion | 0.685      | 178.7 billion  | 0.767
SCAN1     | 82.3  billion | 0.733      | 95.8 billion   | 0.868
SCAN2     | 39.4  billion | 1.128      | 52.7 billion   | 1.279
pSCAN0    | 70.2  billion | 0.750      | 88.8 billion   | 1.078
pSCAN1    | 32.7  billion | 1.384      | 50.6 billion   | 1.667
ppSCAN0   | 55.6  billion | 0.783      | 74.9 billion   | 0.867
ppSCAN1   | 26.5  billion | 1.307      | 45.5 billion   | 1.226

### Output

SCAN0: SCAN super naive

```zsh
**** original SCAN with no optimization: /ghome/yche/projects/dataset/snap_livejournal, 0.2, 5 ***                                                                                            
int size:4                                                                                                                                                                                    
n:4036538, m:69362378                                                                                                                                                                         
read degree file time:15 ms                                                                                                                                                                   
read adjacency list file time:1887 ms                                                                                                                                                         
check input graph file time:134 ms                                                                                                                                                            
other construct time:209 ms                                                                                                                                                                   
Total input cost:2248 ms                                                                                                                                                                      

Total time without IO:44688 ms                                                                                                                                                                

Total output cost:2027 ms       
```

SCAN1: SCAN easy-comp-pruning/cross link, naive set-intersection

```zsh
**** original SCAN with easy-comp/cross-link: /ghome/yche/projects/dataset/snap_livejournal, 0.2, 5 ***
int size:4
n:4036538, m:69362378
read degree file time:16 ms
read adjacency list file time:1852 ms
check input graph file time:133 ms
other construct time:161 ms
Total input cost:2164 ms

Total time without IO:26085 ms

Total output cost:1937 ms
```

SCAN2: SCAN easy-comp-pruning/cross link, best set-intersection

```zsh
**** original SCAN with easy-comp/cross-link: /ghome/yche/projects/dataset/snap_livejournal, 0.2, 5 ***
int size:4
n:4036538, m:69362378
read degree file time:14 ms
read adjacency list file time:1877 ms
check input graph file time:135 ms
other construct time:204 ms
Total input cost:2232 ms

Total time without IO:20703 ms

Total output cost:2004 ms

```

pSCAN0: pSCAN naive set-intersection

```zsh                                                                                                                                  
**** Graph Clustering (Release): /ghome/yche/projects/dataset/snap_livejournal, 0.2, 5 ***                                                                                                    
int size:4                                                                                                                                                                                    
n:4036538, m:69362378                                                                                                                                                                         
read degree file time:16 ms                                                                                                                                                                   
read adjacency list file time:1856 ms                                                                                                                                                         
check input graph file time:134 ms                                                                                                                                                            
other construct time:442 ms                                                                                                                                                                   
Total input cost:2450 ms                                                                                                                                                                      

Prune cost:5141 ms                                                                                                                                                                            
Total time without IO:29864 ms                                                                                                                                                                
non core label num:2538560                                                                                                                                                                    

Total output cost:2082 ms                                                                                                                                                                     
```

pSCAN1: pSCAN best set-intersection

```zsh
**** Graph Clustering (Release): /ghome/yche/projects/dataset/snap_livejournal, 0.2, 5 ***
int size:4
n:4036538, m:69362378
read degree file time:16 ms
read adjacency list file time:1866 ms
check input graph file time:135 ms
other construct time:469 ms
Total input cost:2489 ms

Prune cost:5080 ms
Total time without IO:25974 ms
non core label num:2538560

Total output cost:2229 ms
```

ppSCAN0: serial ppSCAN naive set-intersection

```zsh
int size:4
n:4036538, m:69362378
read degree file time:14 ms
read adjacency list file time:1853 ms
check input graph file time:133 ms
other construct time:236 ms

Total input cost:2238 ms
new algorithm ppSCAN serial experimental
1st: prune execution time:1817 ms
2nd: check core first-phase bsp time:12638 ms
2nd: check core second-phase bsp time:2772 ms
3rd: prepare time: 635 ms
3rd: core clustering time:1084 ms
4th: eval cost in cluster-non-core:914 ms
4th: non-core clustering time:1360 ms
Total time without IO:19674 ms
Total output cost:2290 ms
```

ppSCAN1: serial ppSCAN best set-intersection

```zsh
int size:4                                                                                                                                                                                    
n:4036538, m:69362378                                                                                                                                                                         
read degree file time:15 ms                                                                                                                                                                   
read adjacency list file time:1852 ms                                                                                                                                                         
check input graph file time:131 ms                                                                                                                                                            
other construct time:233 ms                                                                                                                                                                   

Total input cost:2233 ms                                                                                                                                                                      
new algorithm ppSCAN serial experimental                                                                                                                                                      
1st: prune execution time:1826 ms                                                                                                                                                             
2nd: check core first-phase bsp time:10197 ms                                                                                                                                                 
2nd: check core second-phase bsp time:2221 ms                                                                                                                                                 
3rd: prepare time: 633 ms                                                                                                                                                                     
3rd: core clustering time:1075 ms           
3rd: core clustering time:1075 ms
4th: eval cost in cluster-non-core:843 ms
4th: non-core clustering time:1291 ms
Total time without IO:16613 ms
Total output cost:2133 ms
```
