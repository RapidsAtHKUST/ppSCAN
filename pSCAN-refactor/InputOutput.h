//
// Created by yche on 8/4/17.
//

#ifndef PSCAN_INPUTOUTPUT_H
#define PSCAN_INPUTOUTPUT_H

#include <fstream>
#include <vector>

using ui=unsigned int;
using namespace std;

class InputOutput {
private:
    string dir;

    void read_degree();

    void check_input_graph();

    void read_adjacency_list();

public:
    explicit InputOutput(const string &dir);

    ui n;
    ui m;

// csr
    vector<ui> offset_out_edges;
    vector<int> out_edges;

// vertex property

    vector<int> degree;

// edge property
    vector<ui> reverse_edge_idx;

    void read_graph();

    void output(const char *eps_s, const char *min_u, vector<pair<int, int>> &noncore_cluster,
                vector<int> &similar_degree, vector<int> &cid, vector<int> &pa);
};


#endif //PSCAN_INPUTOUTPUT_H
