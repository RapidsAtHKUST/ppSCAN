#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>

#include <omp.h> //OpenMP

#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <atomic> //CAS

#include "csr_set_intersection.h"

using namespace std;

class SCAN_XP {
private:
    int thread_num_;
    int min_u_;
    double epsilon_;

private:
    Graph g;
    UnionFind *uf_ptr;

private:
    bool CheckHub(Graph *g, UnionFind *uf, int a);

private:
    void CheckCore(Graph *g);

    void ClusterCore();

    void LabelNonCore();

    void PostProcess();

    void PrepareResultOutput();

public:
    SCAN_XP(int thread_num, int min_u, double epsilon, char *dir);

    ~SCAN_XP();

    void Execute();
};

