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

void core_detection(Graph *g);

bool hub_check_uf(Graph *g, UnionFind *uf, int a);

static int NUMT;

using namespace std;
