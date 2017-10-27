//
// Created by yche on 10/27/17.
//

#ifndef REFACTOR_XEON_CPU_SCAN_XP_GALLOPING_H
#define REFACTOR_XEON_CPU_SCAN_XP_GALLOPING_H

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>

#include <immintrin.h>  //AVX
#include <omp.h> //OpenMP

#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <atomic> //CAS

#include"graph.h"
#include"unionfind.h"

#define PARA 8

inline void core_detection(Graph *g);

inline bool hub_check_uf(Graph *g, UnionFind *uf, int a);

inline int compute_cn(Graph *g, int edge_idx);

int GallopingSearch(int *array, int offset_beg, int offset_end, int val);

static int NUMT;

using namespace std;


#endif //REFACTOR_XEON_CPU_SCAN_XP_GALLOPING_H
