#include <cstdlib>
#include <cmath>
#include <ctime>

#include <iostream>
#include <vector>
#include <fstream>
#include <set>
//#include <aligned_new>
#include <atomic> //CAS
#include <cstdio>

#include"graph.h"
#include"unionfind.h"

#include <immintrin.h>  //AVX

#include <omp.h> //OpenMP

#define UNCLASSIFIED 0
#define CMEMBER 1 //Member of cluster
#define HUB 2

#define PARA 8

#define EPSILON 0.1 //\epsilon
#define MYU 2 //\mu

inline void core_detection(Graph *g);

inline bool hub_check_uf(Graph *g, UnionFind *uf, int a);

static int NUMT;

using namespace std;
