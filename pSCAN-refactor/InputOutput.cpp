//
// Created by yche on 8/4/17.
//

#include "InputOutput.h"

InputOutput::InputOutput(const string &dir) : dir(dir) {}

void InputOutput::read_graph() {
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
    fread(degree, sizeof(int), n, f);

    fclose(f);

    f = open_file((dir + string("/b_adj.bin")).c_str(), "rb");

    if (pstart == nullptr) pstart = new ui[n + 1];
    if (edges == nullptr) edges = new int[m];
    if (reverse_ == nullptr) reverse_ = new ui[m];
    if (min_cn == nullptr) min_cn = new int[m];
    memset(min_cn, 0, sizeof(int) * m);

    auto *buf = new int[n];

    pstart[0] = 0;
    for (ui i = 0; i < n; i++) {
        if (degree[i] > 0) fread(buf, sizeof(int), degree[i], f);

        for (ui j = 0; j < degree[i]; j++) edges[pstart[i] + j] = buf[j];

        pstart[i + 1] = pstart[i] + degree[i];

        ++degree[i];
    }

    delete[] buf;

    fclose(f);

    for (ui i = 0; i < n; i++) {
        for (ui j = pstart[i]; j < pstart[i + 1]; j++) {
            if (edges[j] == i)cout << "Self loop\n";
            if (j > pstart[i] && edges[j] <= edges[j - 1])
                cout << "Edges not sorted in increasing id order!\nThe program may not run properly!\n";
        }
    }
}


void InputOutput::output(const char *eps_s, const char *miu, vector<pair<int, int>> noncore_cluster,
                         const int *similar_degree, int *cid, const int *pa) {
    cout << "\t*** Start write result into disk!\n";
    string out_name = dir + "/result-" + string(eps_s) + "-" + string(miu) + ".txt";
    ofstream ofs(out_name);
    ofs << "c/n vertex_id cluster_id\n";

    int mu = atoi(miu);
    for (ui i = 0; i < n; i++)
        if (similar_degree[i] >= mu) {
            ofs << "c " << i << " " << cid[pa[i]] << "\n";
        }

    sort(noncore_cluster.begin(), noncore_cluster.end());
    noncore_cluster.erase(unique(noncore_cluster.begin(), noncore_cluster.end()), noncore_cluster.end());
    for (auto &i : noncore_cluster) {
        ofs << "n " << i.second << " " << i.first << "\n";
    }

    ofs.close();
}