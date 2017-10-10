## Question & Confusion

`Summarize`, Line 95 to 171, does not prune similarity evaluations from the output

## Compile

```zsh
g++ AnySCANGraphSerial.cpp DisjointSet.cpp InputOutput.cpp main_exp_any_scan_serial.cpp -o anySCANReImpl -O3 -std=c++11
```

## Dataset

dropbox link:

https://www.dropbox.com/sh/i7tm9ey289rbyfe/AADauZDLYr2-7u_wRaW5tg3Ja?dl=0

download into a folder `../dataset/snap_livejournal`

## Input

```zsh
./anySCANReImpl ../dataset/snap_livejournal 0.3 5 output
```

`../dataset/snap_livejournal` is the folder of `b_adj.bin` and `b_degree.bin`

### Output

```zsh
**** original anySCAN with easy-comp: ../dataset/snap_livejournal, 0.3, 5 *** 
int size:4
n:4036538, m:69362378
read degree file time:14 ms
read adjacency list file time:249 ms
check input graph file time:105 ms
other construct time:56 ms
Total input cost:425 ms

1. mark unprocessed noise cost:11 ms
1. summarize cost:30865 ms
2. init check list cost:42 ms
check list size:0
2. exam check list for strongly related cluster cost:0 ms
3. init check list cost:80 ms
check list size:1299935
3. exam check list for weakly related cluster cost:2414 ms
4. mark cost:22 ms
4. cluster non-core cost:22 ms
Total time without IO:33547 ms
intersection times:64111632

Total output cost:474 ms
```