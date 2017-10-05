# Synthetic Graph Case Study

## LFR Benchmark Graph Specification


see [benchmark_gen.py](https://github.com/GraphProcessor/CommunityDetectionCodes/blob/master/Benchmark/2009-LFR-Benchmark/src_refactor_cpp/benchmark_gen.py)

two types: varying average degree, varying clustering coefficient

each type: 7 graphs.

all generate 10,000,000 vertices, `-mu` is `0.1`

* varying average degree: from `10` to `70`, max degree `100`, clustering coefficient `-C` is `0.4`, others are all by default

```python
def generate_different_avg(vertex_num):
    cluster_coefficient = 0.4
    max_degree = 100
    avg_degree_lst = [i * 10 for i in xrange(1, 8)]

    for avg_degree in avg_degree_lst:
        param_lst = map(str, ['build/undirected_graph/lfr_undir_net', '-N', vertex_num, '-k', avg_degree, '-maxk',
                              max_degree, '-mu', 0.1, '-C', cluster_coefficient])
        os.system(' '.join(param_lst))
        dir_name = 'output_vary_deg' + os.sep + '_'.join(
            map(str, ['v', vertex_num, 'avgk', avg_degree, 'maxk', max_degree, 'C', cluster_coefficient]))
        os.system('mkdir -p ' + dir_name)
        os.system('mv *.dat ' + dir_name)
```

* varying coefficient: from `0.1` to `0.7`, avg degree `40`, max degree `100`, clustering coefficient `-C` is `0.4`, others are all by default

```python
def generate_different_coefficient(vertex_num):
    cluster_coefficient_lst = [i * 0.1 for i in xrange(1, 8)]
    avg_degree = 40
    max_degree = 100

    for cluster_coefficient in cluster_coefficient_lst:
        param_lst = map(str, ['build/undirected_graph/lfr_undir_net', '-N', vertex_num, '-k', avg_degree, '-maxk',
                              max_degree, '-mu', 0.1, '-C', cluster_coefficient])
        os.system(' '.join(param_lst))
        dir_name = 'output_vary_coefficient' + os.sep + '_'.join(
            map(str, ['v', vertex_num, 'avgk', avg_degree, 'maxk', max_degree, 'C', cluster_coefficient]))
        os.system('mkdir -p ' + dir_name)
        os.system('mv *.dat ' + dir_name)
```

## ROLL Graph

clone and build

```zsh
git clone https://github.com/alihadian/ROLL.git
cd ROLL
mvn package
```

usage, `-s` should specify `roll-tree` which is the fastest model, other models are orders of magnitude slower

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

please check the original https://github.com/alihadian/ROLL.git for further information
