#ifndef _UTILITY_H_
#define _UTILITY_H_


#include <cstdio>
#include <utility>

typedef unsigned int ui;
using namespace std;

FILE *open_file(const char *file_name, const char *mode);

pair<int, int> get_eps(const char *eps_s);

#endif
