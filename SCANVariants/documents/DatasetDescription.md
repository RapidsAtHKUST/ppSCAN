# Dataset

## real-world type 0:

from snap, represented in edge list, use [EdgeListToBinsConverter.cpp](../converter/EdgeListToBinsConverter.cpp) to convert into two binary files,
i.e., degree file and adjacent list file, need to sort src vertices

* https://snap.stanford.edu/data/com-LiveJournal.html
* https://snap.stanford.edu/data/com-Orkut.html
* https://snap.stanford.edu/data/com-Friendster.html

graph | double edge num | adj file md5 | deg file md5
--- | --- | --- | ---
livejournal | 69,362,378    | 477e213277b0049e1f7792dd122954f7 | d926c1be74e3b1f6fcc4f285c0cdf7a5
orkut       | 234,370,166   | 6d115ff0e121e0eb6bb8e0655e5810ce | 72b791c021d465e3cf625007c414e940
friendster  | 3,612,134,270 | 1b692bd658f8e49fd837872db2c942cb | ca239d7c726238e8d752ce1999f6a35f   

## real-world type 1:   

from law.di.unimi.it, represented in BVGraph, two files `xxx.graph`, `xxx.properties`,

please check: https://github.com/GraphProcessor/WebGraph-1  , in order to get edge list first.

middle file edge list: need to 1st: remove self-loop (i,i), 2nd: unique (i,j), (j,i) duplicates

add support as follows in [EdgeListToBinsConverter.cpp](../converter/EdgeListToBinsConverter.cpp)

```cpp
            // 1st case first == second: skip these self loop, (i,i)
            // 2nd case first > second: unique (i,j), (j,i)
            if (first >= second) {
                continue;
            }
```

* http://law.di.unimi.it/webdata/webbase-2001/
* http://law.di.unimi.it/webdata/uk-2002/
* http://law.di.unimi.it/webdata/twitter-2010/

graph | double edge num | adj file md5 | deg file md5
--- | --- | --- | ---
uk      | 298,113,762     | 34d8e58aab0ca3524a8aa37d82ae8ad7 | 9bfb9c1c0a6ecaebdef1d7d092618dd7
webbase | 1,019,903,190   | 2325487c38f792d337729446bd45121a | b95c2facaf26aa8a420872ebb64c2040
twitter | 1,468,365,182   | e7aa5c70b082384e1b0d8181ed0b1502 | 6fedb2f1b6b7205e9cf84525e3cb4904   

## synthetic

lfr synthetic, undirected and unweighted, `-k 15 -maxk 50 -mu 0.1 -minc 20 -maxc 50`, `-N 1million`/`10million`/`100million `

* https://github.com/GraphProcessor/CommunityDetectionCodes/tree/master/Benchmark/2009-LFR-Benchmark/src_refactor_cpp
