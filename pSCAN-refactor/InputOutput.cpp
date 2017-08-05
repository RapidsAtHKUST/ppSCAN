//
// Created by yche on 8/4/17.
//

#include "InputOutput.h"

#include <iostream>
#include <algorithm>

InputOutput::InputOutput(const string &dir) : dir(dir) {}

void InputOutput::read_degree() {
    ifstream deg_file(dir + string("/b_degree.bin"), ios::binary);
    int int_size;
    deg_file.read(reinterpret_cast<char *>(&int_size), 4);
    cout << "int size:" << int_size << "\n";

    deg_file.read(reinterpret_cast<char *>(&n), 4);
    deg_file.read(reinterpret_cast<char *>(&m), 4);
    cout << "n:" << n << ", m:" << m << "\n";

    degree.resize(static_cast<unsigned long>(n));
    deg_file.read(reinterpret_cast<char *>(&degree.front()), sizeof(int) * n);
}

void InputOutput::read_adjacency_list() {
    ifstream adj_file(dir + string("/b_adj.bin"), ios::binary);
    offset_out_edges.resize(n + 1);
    out_edges.resize(m);
    reverse_edge_idx.resize(m);

    offset_out_edges[0] = 0;
    for (auto i = 0; i < n; i++) { offset_out_edges[i + 1] = offset_out_edges[i] + degree[i]; }
    for (auto i = 0; i < n; i++) {
        if (degree[i] > 0) {
            adj_file.read(reinterpret_cast<char *>(&out_edges[offset_out_edges[i]]), degree[i] * sizeof(int));
        }
        degree[i]++;
    }
}

void InputOutput::check_input_graph() {
    for (auto i = 0; i < n; i++) {
        for (auto j = offset_out_edges[i]; j < offset_out_edges[i + 1]; j++) {
            if (out_edges[j] == i) { cout << "Self loop\n"; }
            if (j > offset_out_edges[i] && out_edges[j] <= out_edges[j - 1]) {
                cout << "Edges not sorted in increasing id order!\nThe program may not run properly!\n";
            }
        }
    }
}

void InputOutput::read_graph() {
    read_degree();
    read_adjacency_list();
    check_input_graph();
}

void InputOutput::output(const char *eps_s, const char *min_u, vector<pair<int, int>> &noncore_cluster,
                         vector<int> &similar_degree, vector<int> &cid, vector<int> &pa) {
    cout << "\t*** Start write result into disk!\n";
    string out_name = dir + "/result-" + string(eps_s) + "-" + string(min_u) + ".txt";
    ofstream ofs(out_name);
    ofs << "c/n vertex_id cluster_id\n";

    int mu = atoi(min_u);
    for (auto i = 0; i < n; i++) {
        if (similar_degree[i] >= mu) { ofs << "c " << i << " " << cid[pa[i]] << "\n"; }
    }

    sort(noncore_cluster.begin(), noncore_cluster.end());
    noncore_cluster.erase(unique(noncore_cluster.begin(), noncore_cluster.end()), noncore_cluster.end());
    for (auto &i : noncore_cluster) { ofs << "n " << i.second << " " << i.first << "\n"; }
}