#include <fstream>
#include <vector>
#include <iostream>

#include "pretty_print.h"

//
// Created by yche on 8/2/17.
//

using ui=unsigned int;
ui n, m;

using namespace std;
// csr
vector<ui> offset_out_edges;
vector<int> out_edges;

// vertex property
vector<int> degree;

// edge property
vector<ui> reverse_;

using namespace std;

void ReadDegreeBin(const string &dir) {
    ifstream deg_file(dir + string("/b_degree.bin"), ios::binary);
    int int_size;
    deg_file.read(reinterpret_cast<char *>(&int_size), 4);
    cout << "int size:" << int_size << "\n";

    deg_file.read(reinterpret_cast<char *>(&n), 4);
    deg_file.read(reinterpret_cast<char *>(&m), 4);
    cout << "n:" << n << ", m:" << m << "\n";

    degree.resize(static_cast<unsigned long>(n));
    deg_file.read(reinterpret_cast<char *>(&degree.front()), sizeof(int) * n);
    cout << "degree list:" << degree << "\n";
}

void ReadAdjBin(const string &dir) {
    ifstream adj_file(dir + string("/b_adj.bin"), ios::binary);
    offset_out_edges.resize(n + 1);
    out_edges.resize(m);
    reverse_.resize(m);

    offset_out_edges[0] = 0;
    for (auto i = 0; i < n; i++) { offset_out_edges[i + 1] = offset_out_edges[i] + degree[i]; }
    for (auto i = 0; i < n; i++) {
        if (degree[i] > 0) {
            adj_file.read(reinterpret_cast<char *>(&out_edges[offset_out_edges[i]]), degree[i] * sizeof(int));
        }
    }
    cout << "out_edges dst vertices:" << out_edges << endl;
}

void CheckGraph() {
    for (auto i = 0; i < n; i++) {
        for (auto j = offset_out_edges[i]; j < offset_out_edges[i + 1]; j++) {
            if (out_edges[j] == i) { cout << "Self loop\n"; }
            if (j > offset_out_edges[i] && out_edges[j] <= out_edges[j - 1]) {
                cout << "Edges not sorted in increasing id order!\nThe program may not run properly!\n";
            }
        }
    }
}

void ReadGraph(const string &dir) {
    ReadDegreeBin(dir);
    ReadAdjBin(dir);
    CheckGraph();
}

int main(int argc, char **argv) {
    ReadGraph(argv[1]);
}