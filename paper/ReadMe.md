## Experiments

`7*4=28` figures

* by default, use **KNL**, (overall comparison and performance on ROLL graph, using **CPU**)
* show that different `u`(`2, 5, 10, 15`), they show similar trends in **performance with different `u`**

parameters | varying range
--- | ---
`u` | `5`
`eps` | `0.1 to 0.9`

### Overall

ok. overall comparison script: [comparison](comparison)

* overall comparison on **KNL**(4 real-world graphs, fixed `u=5`, varying `eps`), log time
  * anySCAN(runtime err on webbase, friendster), SCAN-XP, pSCAN, SCAN(TLE in some parameters), ppSCAN
* overall comparison on **CPU**(4 real-world graphs, fixed `u=5`, varying `eps`), log time
  * anySCAN(runtime err on webbase, friendster), SCAN-XP, pSCAN, SCAN(TLE in some parameters), ppSCAN

### Set Intersection 

ok. invocation number script: [workload](workload)

set-intersection script(ppSCAN vs ppSCAN-NO): 

* normalized set-intersection invocation number(4 real-world graphs, fixed `u=5`, varying `eps`)
  * pSCAN and ppSCAN
* set-intersection optimization(4 real-world graphs, fixed `u=5`, varying `eps`, accumulated time)
  * ppSCAN and ppSCAN-NO

### Scalability

ok. scalability on varying threads: [scalability_vary_threads](scalability_vary_threads)

ok. scalability on varying both u and eps script: [scalability_vary_u_and_eps](scalability_vary_u_and_eps)

ok. scalability on roll graph script: [scalability_roll_graph](scalability_roll_graph)

* scalability with different thread numbers(4 real-world graphs, fixed `u=5`, `eps=0.2`), log-log time and thread number
  * ppSCAN 4 stages and total time
* scalability with different `u`(4 real-world graphs)(on KNL), varying `u`(`2, 5, 10, 15`)
      * ppSCAN(with different `u`) shows similar trends
* scalability on ROLL graphs(4 different-degree 1-billion edge ROLL graphs, fixed `u=5` and `eps`)
  * ppSCAN time, speedup namely on **CPU and KNL**, **report time and speedup**
