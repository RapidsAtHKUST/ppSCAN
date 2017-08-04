//
// Created by yche on 8/4/17.
//

#ifndef PSCAN_INPUTOUTPUT_H
#define PSCAN_INPUTOUTPUT_H


#include "Utility.h"
#include <fstream>

using namespace std;

class InputOutput {
public:
    explicit InputOutput(const string &dir);

    string dir;
    int *degree;
    int n;
    int m;

    ui *pstart;
    int *edges;
    ui *reverse_;
    int *min_cn;

    void read_graph();

    void output(const char *eps_s, const char *miu, vector<pair<int, int>> noncore_cluster,
                const int *similar_degree, int *cid, const int *pa);
};


#endif //PSCAN_INPUTOUTPUT_H
