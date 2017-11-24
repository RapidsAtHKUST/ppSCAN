## Experiments

`6*4=24` figures

parameters | varying range
--- | ---
`u` | `2, 5, 10, 15`
`eps` | `0.1 to 0.9`

* overall comparison on KNL(4 real-world graph2s, fixed `u=5`, varying `eps`), log time
  * anySCAN(runtime err on webbase, friendster), SCAN-XP, pSCAN, SCAN(TLE in some parameters), ppSCAN
* overall comparison on CPU(4 real-world graphs, fixed `u=5`, varying `eps`), log time
  * anySCAN(runtime err on webbase, friendster), SCAN-XP, pSCAN, SCAN(TLE in some parameters), ppSCAN
* normalized set-intersection invocation number(4 real-world graphs, fixed `u=5`, varying `eps`)
  * pSCAN and ppSCAN
* set-intersection optimization(4 real-world graphs, fixed `u=5`, varying `eps`, accumulated time)
  * ppSCAN and ppSCAN-NO
* scalability with different thread numbers(4 real-world graphs, fixed `u=5`, `eps=0.2`), log-log time and thread number
  * ppSCAN 4 stages and total time
* performance on ROLL graphs(4 different degree 1-billion edge ROLL graphs, varying `u` and `eps`)
  * ppSCAN time, speedup namely on **CPU and KNL**, **report time and speedup**
