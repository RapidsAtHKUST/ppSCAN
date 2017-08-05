//
// Created by yche on 8/4/17.
//

#include "InputOutput.h"

#include <iostream>
#include <cstring>
#include <algorithm>

InputOutput::InputOutput(const string &dir) : dir(dir) {}

void InputOutput::read_degree() {
    FILE *f = open_file((dir + string("/b_degree.bin")).c_str(), "rb");

    int tt;
    fread(&tt, sizeof(int), 1, f);
    if (tt != (int) sizeof(int)) {
        cout << "sizeof int is different: edge.bin(" << tt << "), machine(" << (int) sizeof(int) << "\n";
        return;
    }
    fread(&n, sizeof(int), 1, f);
    fread(&m, sizeof(int), 1, f);

    degree = new int[n];
    fread(degree, sizeof(int), static_cast<size_t>(n), f);

    fclose(f);
}

void InputOutput::read_adjacency_list() {
    FILE *f = open_file((dir + string("/b_adj.bin")).c_str(), "rb");

    pstart = new ui[n + 1];
    edges = new int[m];
    reverse_ = new ui[m];

    auto *buf = new int[n];

    pstart[0] = 0;
    for (ui i = 0; i < n; i++) {
        if (degree[i] > 0) {
            fread(buf, sizeof(int), static_cast<size_t>(degree[i]), f);
        }
        for (ui j = 0; j < degree[i]; j++) {
            edges[pstart[i] + j] = buf[j];
        }
        pstart[i + 1] = pstart[i] + degree[i];
        ++degree[i];
    }

    delete[] buf;
    fclose(f);
}

void InputOutput::check_input_graph() {
    for (ui i = 0; i < n; i++) {
        for (ui j = pstart[i]; j < pstart[i + 1]; j++) {
            if (edges[j] == i) {
                cout << "Self loop\n";
            }
            if (j > pstart[i] && edges[j] <= edges[j - 1]) {
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

void InputOutput::output(const char *eps_s, const char *min_u, vector<pair<int, int>> noncore_cluster,
                         vector<int> &similar_degree, vector<int> &cid, vector<int> &pa) {
    cout << "\t*** Start write result into disk!\n";
    string out_name = dir + "/result-" + string(eps_s) + "-" + string(min_u) + ".txt";
    ofstream ofs(out_name);
    ofs << "c/n vertex_id cluster_id\n";

    int mu = atoi(min_u);
    for (ui i = 0; i < n; i++) {
        if (similar_degree[i] >= mu) {
            ofs << "c " << i << " " << cid[pa[i]] << "\n";
        }
    }

    sort(noncore_cluster.begin(), noncore_cluster.end());
    noncore_cluster.erase(unique(noncore_cluster.begin(), noncore_cluster.end()), noncore_cluster.end());
    for (auto &i : noncore_cluster) {
        ofs << "n " << i.second << " " << i.first << "\n";
    }

    ofs.close();
}