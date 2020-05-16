* eps: 0.6, mu: 3, on a 11-vertex and 21-edge toy graph with two clusters

* input parameters

```zsh
cmake -DUSE_LOG_DEBUG=ON ..
make -j
./pSCANParallel ../datasets/truss_ppscan 0.6 3 output
```

* pruning effect:

```
[-1, -1, -1, 4, 4, -1, -1, 3, -1, -1, -1, -1, -2, -1, -1, -1, -1, 4, -1, -1, -1, 4, -1, -1, -1, 4, -2, -1, -2, -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1]
```

* final results

```
c/n vertex_id cluster_id
c 0 0
c 2 0
c 3 0
c 4 0
c 5 0
c 8 8
c 10 8
n 1 0
n 6 0
n 7 8
n 9 8
```