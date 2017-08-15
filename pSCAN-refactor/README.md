# pSCAN

pSCAN: Fast and Exact Structural Graph Clustering

# Before Experiments

clear page cache    

```zsh
sudo -i
echo 3 > /proc/sys/vm/drop_caches
```

make sure that you have already clear page cache before checking. check disk r/w.

```zsh
sudo time dd if=/dev/sda5  of=/dev/null bs=8k count=1400000
```

my desktop info

```zsh
1400000+0 records in
1400000+0 records out
11468800000 bytes (11 GB, 11 GiB) copied, 59.5983 s, 192 MB/s
0.24user 6.33system 0:59.62elapsed 11%CPU (0avgtext+0avgdata 1928maxresident)k
22399088inputs+0outputs (1major+78minor)pagefaults 0swaps
```

# Compile

g++ should support c++11, e.g, g++ 4.8+

cmake should be 3.6+, allowing for modern cmake

```zsh
mkdir -p build
cd build
cmake .. -DCMAKE_CXX_COMPILER=g++
make
```

# Parameter

* eps influence the computation workloads a lot

* min_pts not influence the workloads

# Dataset

already(from snap, represented in edge list, use [EdgeListToBinsConverter.cpp](converter/EdgeListToBinsConverter.cpp) to convert into two binary files,
i.e., degree file and adjacent list file) 

* https://snap.stanford.edu/data/com-LiveJournal.html
* https://snap.stanford.edu/data/com-Orkut.html
* https://snap.stanford.edu/data/com-Friendster.html

not yet()

* http://law.di.unimi.it/webdata/webbase-2001/
* http://law.di.unimi.it/webdata/uk-2002/
* http://law.di.unimi.it/webdata/twitter-2010/

lfr synthetic, undirected and unweighted, `-k 15 -maxk 50 -mu 0.1 -minc 20 -maxc 50`, `-N 1million/10million/100million`

* https://github.com/GraphProcessor/CommunityDetectionCodes/tree/master/Benchmark/2009-LFR-Benchmark/src_refactor_cpp

# Usage

```zsh
build/pSCAN ../dataset 0.3 5 output
```

# Citation

> If you are using this code, please kindly cite the following paper.

> Lijun Chang, Wei Li, Xuemin Lin, Lu Qin, Wenjie Zhang,
"pSCAN: Fast and Exact Structural Graph Clustering"
Proceedings of the 31st International Conference on Data Engineering (ICDE’16), 2016
