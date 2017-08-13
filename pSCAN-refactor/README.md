# pSCAN

pSCAN: Fast and Exact Structural Graph Clustering

# Before Experiments

clear page cache

```zsh
sudo -i
echo 3 > /proc/sys/vm/drop_caches
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

# Dataset 

already

* https://snap.stanford.edu/data/com-LiveJournal.html
* https://snap.stanford.edu/data/com-Orkut.html
* https://snap.stanford.edu/data/com-Friendster.html 

not yet

* http://law.di.unimi.it/webdata/uk-2002/
* http://law.di.unimi.it/webdata/twitter-2010/

# Usage

```zsh
build/pSCAN ../dataset 0.3 5 output
```

# Citation

> If you are using this code, please kindly cite the following paper.

> Lijun Chang, Wei Li, Xuemin Lin, Lu Qin, Wenjie Zhang,
"pSCAN: Fast and Exact Structural Graph Clustering"
Proceedings of the 31st International Conference on Data Engineering (ICDE’16), 2016
