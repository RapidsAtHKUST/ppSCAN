//
// Created by yche on 8/4/17.
//

#ifndef PSCAN_INPUTOUTPUT_H
#define PSCAN_INPUTOUTPUT_H

#include <fstream>
#include <vector>
#include "Utility.h"

using namespace std;

class InputOutput {
private:
    string dir;

    void read_degree();

    void check_input_graph();

    void read_adjacency_list();

public:
    explicit InputOutput(const string &dir);

    int n;
    int m;

    ui *pstart;
    int *edges;

    int *degree;

    ui *reverse_;

    void read_graph();

    void output(const char *eps_s, const char *min_u, vector<pair<int, int>> noncore_cluster,
                vector<int> &similar_degree, vector<int> &cid, vector<int> &pa);
};


#endif //PSCAN_INPUTOUTPUT_H
