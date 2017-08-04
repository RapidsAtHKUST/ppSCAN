#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <queue>
#include <set>

typedef unsigned int ui;
using namespace std;

FILE *open_file(const char *file_name, const char *mode);

pair<int, int> get_eps(const char *eps_s);

#endif
