#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<cstdlib>
#include<time.h>

using namespace std;

struct GRAPH{
  int nodemax;
  int edgemax;
  int *node; 
  int *edge; 
  int *edgef; 
  bool *core; 
  bool *similarity;
  int *common_node; 
  double *ss; 
  int *core_count;
  int *label;
  GRAPH(char *infilename);
};


