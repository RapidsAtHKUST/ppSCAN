# Parallel Version Challenges

* load balance, especially twitter dataset

    * 1st bsp, `batch size` important
    
    * 2nd bsp, `batch size` important
    
    * so how to effectively set `batch_num`, while efficiently estimating task grain and pruning effects
    
    * case study: webbase 40, 32, 24
    
    ```zsh
    build/pSCANParallelExp0 ../dataset/webgraph_webbase 0.3 5 40
    
    int size:4                                                                                     
    n:118142143, m:1050026736                                                                      
    read degree file time:464 ms                                                                  
    read adjacency list file time:37771 ms                                                    
    check input graph file time:2235 ms                                                            
                                                                                                   
    Total input cost:42346 ms                                                                      
    with google perf start------------                                                             
    1st: prune execution time:677 ms                                                               
    2nd: check core first-phase bsp time:20021 ms                                                  
    2nd: check core second-phase bsp time:1548 ms                                                  
    3rd: core clustering time:5593 ms                                                              
    4th: non-core clustering time:2754 ms                                                          
    with google perf end--------------                                                             
    PROFILE: interrupts/evictions/bytes = 30669/18839/1906040                                      
    Total time without IO:30628 ms                                                                 
    Total output cost:0 ms     
    ```
    
    ```zsh
    build/pSCANParallelExp0 ../dataset/webgraph_webbase 0.3 5 32

    int size:4
    n:118142143, m:1050026736
    read degree file time:486 ms
    read adjacency list file time:37577 ms
    check input graph file time:2240 ms
    
    Total input cost:42159 ms
    with google perf start------------
    1st: prune execution time:783 ms
    2nd: check core first-phase bsp time:9461 ms
    2nd: check core second-phase bsp time:924 ms
    3rd: core clustering time:3538 ms
    4th: non-core clustering time:2756 ms
    with google perf end--------------
    PROFILE: interrupts/evictions/bytes = 23411/15952/1624992
    Total time without IO:17491 ms
    Total output cost:0 ms

    ```
    
    ```zsh
     build/pSCANParallelExp0 ../dataset/webgraph_webbase 0.3 5 24            
    
    int size:4                                                                                                                                                                                    
    n:118142143, m:1050026736                                                                                                                                                                     
    read degree file time:468 ms                                                                                                                                                                  
    read adjacency list file time:37794 ms                                                                                                                                                        
    check input graph file time:2164 ms                                                                                                                                                           
                                                                                                                                                                                                  
    Total input cost:42295 ms                                                                                                                                                                     
    with google perf start------------                                                                                                                                                            
    1st: prune execution time:750 ms                                                                                                                                                              
    2nd: check core first-phase bsp time:6139 ms                                                                                                                                                  
    2nd: check core second-phase bsp time:1301 ms
    3rd: core clustering time:3954 ms
    4th: non-core clustering time:3106 ms
    with google perf end--------------
    PROFILE: interrupts/evictions/bytes = 18170/12484/1268568
    Total time without IO:15282 ms
    Total output cost:0 ms
    ```
    
    ```zsh
    build/pSCANParallelExp0 ../dataset/webgraph_webbase 0.3 5 16
    
    int size:4                                                                                                                                                                                    
    n:118142143, m:1050026736                                                                                                                                                                     
    read degree file time:467 ms                                                                                                                                                                  
    read adjacency list file time:38443 ms                                                                                                                                                        
    check input graph file time:2305 ms                                                                                                                                                           
                                                                                                                                                                                                  
    Total input cost:43124 ms                                                                                                                                                                     
    with google perf start------------                                                                                                                                                            
    1st: prune execution time:1188 ms                                                                                                                                                             
    2nd: check core first-phase bsp time:6302 ms                                                                                                                                                  
    2nd: check core second-phase bsp time:1668 ms                                                                                                                                                 
    3rd: core clustering time:4283 ms                                                                                                                                                             
    4th: non-core clustering time:3024 ms                                                                                                                                                         
    with google perf end--------------                                                                                                                                                            
    PROFILE: interrupts/evictions/bytes = 14527/9653/973008                                                                                                                                       
    Total time without IO:16488 ms                                                                                                                                                                
    Total output cost:0 ms      
    ```
    
    
* how to deal with workload increment in some datasets, from less pruning 

    * interpret how will workload increment influence the running time