detail : see [case-study-synthetic0](case_studies/case-study-synthetic0)

## LFR Benchmark

maximum degree is important to ppSCAN and other SCAN's computations

so we can fix average degree `k`, varying max degree `maxk`, use default `mu: 0.1`, default `minc, maxc` related with max degree to generate meaningful synthetic
data close to social network

example, code can be found [here](https://github.com/GraphProcessor/CommunityDetectionCodes/tree/master/Benchmark/2009-LFR-Benchmark/src_refactor_cpp)

```zsh
build/undirected_graph/lfr_undir_net -N 1000000 -k 30 -maxk 10000 -mu 0.1 
```

## ROLL Graph Generator

cluster coefficient differs a lot from LFR Benchmark

but it can generate quite fast in `roll-tree` sampling model, for generating 10m/100m vertices graph, i.e, billion-edge graph

* clone and build

```zsh
git clone https://github.com/alihadian/ROLL.git
cd ROLL
mvn package
```

* usage, `-s` should specify `roll-tree` which is the fastest model, other models are orders of magnitude slower

parameter tag | meaning
--- | ---
`-n` | number of nodes
`-m` | number of edges per node, i.e. the average degree (default = 2)
`-o` | output file name (default: no output)
`-s` | sampling mode (simple, roll-bucket, roll-tree, SA)
`-m0` | number of initial nodes. Note that m0 >= m (default: =m)
`-d` | only for ROLL-tree: path to save the degree distribution file.

```zsh
java -jar target/ROLL-0.3-SNAPSHOT-jar-with-dependencies.jar -n 1000000 -m 30 -o ~/GitRepos/ScanOptimizing/dataset/roll_graph0/roll.edges -s roll-tree
```