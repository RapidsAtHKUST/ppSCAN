//
// Created by yche on 8/4/17.
//

#include "InputOutput.h"

#include <algorithm>
#include <chrono>

InputOutput::InputOutput(const string &dir) : dir(dir) {}

using namespace std::chrono;
using namespace yche;

void InputOutput::ReadDegree() {
    auto start = high_resolution_clock::now();

    ifstream deg_file(dir + string("/b_degree.bin"), ios::binary);
    int int_size;
    deg_file.read(reinterpret_cast<char *>(&int_size), 4);
    cout << "int size:" << int_size << "\n";

    deg_file.read(reinterpret_cast<char *>(&n), 4);
    deg_file.read(reinterpret_cast<char *>(&m), 4);
    cout << "n:" << n << ", m:" << m << "\n";

    degree.resize(static_cast<unsigned long>(n));
    deg_file.read(reinterpret_cast<char *>(&degree.front()), sizeof(int) * n);

    auto end = high_resolution_clock::now();
    cout << "read degree file time:" << duration_cast<milliseconds>(end - start).count() << " ms\n";
}

void InputOutput::ReadAdjacencyList() {
    auto start = high_resolution_clock::now();

    ifstream adj_file(dir + string("/b_adj.bin"), ios::binary);
    offset_out_edges.resize(n + 1);
    out_edges.resize(m);

    offset_out_edges[0] = 0;
    for (auto i = 0; i < n; i++) { offset_out_edges[i + 1] = offset_out_edges[i] + degree[i]; }
    for (auto i = 0; i < n; i++) {
        if (degree[i] > 0) {
            adj_file.read(reinterpret_cast<char *>(&out_edges[offset_out_edges[i]]), degree[i] * sizeof(int));
        }
        // inclusive
        degree[i]++;
    }

    auto end = high_resolution_clock::now();
    cout << "read adjacency list file time:" << duration_cast<milliseconds>(end - start).count() << " ms\n";
}

void InputOutput::CheckInputGraph() {
    auto start = high_resolution_clock::now();

    for (auto i = 0; i < n; i++) {
        for (auto j = offset_out_edges[i]; j < offset_out_edges[i + 1]; j++) {
            if (out_edges[j] == i) {
                cout << "Self loop\n";
                exit(1);
            }
            if (j > offset_out_edges[i] && out_edges[j] <= out_edges[j - 1]) {
                cout << "Edges not sorted in increasing id order!\nThe program may not run properly!\n";
                exit(1);
            }
        }
    }
    auto end = high_resolution_clock::now();
    cout << "check input graph file time:" << duration_cast<milliseconds>(end - start).count() << " ms\n";
}

void InputOutput::ReadGraph() {
    ReadDegree();
    ReadAdjacencyList();
    CheckInputGraph();
}

pair<int, int> InputOutput::ParseEps(const char *eps_s) {
    int i = 0, eps_numerator = 0, eps_denominator = 1;
    while (eps_s[i] != '\0' && eps_s[i] != '.') {
        eps_numerator = eps_numerator * 10 + (eps_s[i] - '0');
        ++i;
    }
    if (eps_s[i] == '.') {
        ++i;
        while (eps_s[i] != '\0') {
            eps_numerator = eps_numerator * 10 + (eps_s[i] - '0');
            eps_denominator *= 10;
            ++i;
        }
    }
    if (eps_numerator > eps_denominator || eps_denominator > 100 || eps_numerator <= 0) {
        cout << "eps err\n";
        exit(1);
    }
    return make_pair(eps_numerator * eps_numerator, eps_denominator * eps_denominator);
}

// possible duplicates in noncore_cluster, sd_lst need to be checked with a cmp
void InputOutput::Output(const char *eps_s, const char *min_u, vector<pair<int, int>> &noncore_cluster,
                         vector<int> &sd_lst, vector<int> &cid, vector<int> &parent) {
    string out_name = dir + "/result-" + string(eps_s) + "-" + string(min_u) + ".txt";
    ofstream ofs(out_name);
    ofs << "c/n vertex_id cluster_id\n";

    int miu = atoi(min_u);
    // observation 2: unique belonging
    for (auto i = 0; i < n; i++) { if (sd_lst[i] >= miu) { ofs << "c " << i << " " << cid[parent[i]] << "\n"; }}

    // possibly multiple belongings
    cout << "non core label num:" << noncore_cluster.size() << endl;
    sort(noncore_cluster.begin(), noncore_cluster.end());
    auto iter_end = unique(noncore_cluster.begin(), noncore_cluster.end());
    for_each(noncore_cluster.begin(), iter_end,
             [&ofs](pair<int, int> my_pair) { ofs << "n " << my_pair.second << " " << my_pair.first << "\n"; });
}

void InputOutput::Output(const char *eps_s, const char *min_u, vector<pair<int, int>> &noncore_cluster,
                         vector<bool> &is_core_lst, vector<int> &cid, vector<int> &parent) {
    string out_name = dir + "/result-" + string(eps_s) + "-" + string(min_u) + ".txt";
    ofstream ofs(out_name);
    ofs << "c/n vertex_id cluster_id\n";

    // observation 2: unique belonging
    for (auto i = 0; i < n; i++) { if (is_core_lst[i]) { ofs << "c " << i << " " << cid[parent[i]] << "\n"; }}

    // possibly multiple belongings
    sort(noncore_cluster.begin(), noncore_cluster.end());
    auto iter_end = unique(noncore_cluster.begin(), noncore_cluster.end());
    for_each(noncore_cluster.begin(), iter_end,
             [&ofs](pair<int, int> my_pair) { ofs << "n " << my_pair.second << " " << my_pair.first << "\n"; });
}

// unique: noncore_cluster_set
void InputOutput::Output(const char *eps_s, const char *min_u, unordered_set<pair<int, int>> &noncore_cluster_set,
                         vector<bool> &is_core_lst, vector<int> &cid, vector<int> &parent) {
    string out_name = dir + "/result-" + string(eps_s) + "-" + string(min_u) + ".txt";
    ofstream ofs(out_name);
    ofs << "c/n vertex_id cluster_id\n";

    // observation 2: unique belonging
    for (auto i = 0; i < n; i++) { if (is_core_lst[i]) { ofs << "c " << i << " " << cid[parent[i]] << "\n"; }}

    // possibly multiple belongings
    cout << "non core label num:" << noncore_cluster_set.size() << endl;
    vector<pair<int, int>> noncore_cluster;
    noncore_cluster.reserve(n);
    for (auto my_pair: noncore_cluster_set) {
        noncore_cluster.push_back(my_pair);
    };
    sort(noncore_cluster.begin(), noncore_cluster.end());
    for_each(noncore_cluster.begin(), noncore_cluster.end(),
             [&ofs](pair<int, int> my_pair) { ofs << "n " << my_pair.second << " " << my_pair.first << "\n"; });
}

void InputOutput::Output(const char *eps_s, const char *min_u, vector<pair<int, int>> &noncore_cluster,
                         vector<char> &is_core_lst, vector<int> &cid, vector<int> &parent) {
    string out_name = dir + "/result-" + string(eps_s) + "-" + string(min_u) + ".txt";
    ofstream ofs(out_name);
    ofs << "c/n vertex_id cluster_id\n";

    // observation 2: unique belonging
    for (auto i = 0; i < n; i++) { if (is_core_lst[i] == TRUE) { ofs << "c " << i << " " << cid[parent[i]] << "\n"; }}

    // possibly multiple belongings
    sort(noncore_cluster.begin(), noncore_cluster.end());
    auto iter_end = unique(noncore_cluster.begin(), noncore_cluster.end());
    for_each(noncore_cluster.begin(), iter_end,
             [&ofs](pair<int, int> my_pair) { ofs << "n " << my_pair.second << " " << my_pair.first << "\n"; });
}

void InputOutput::Output(const char *eps_s, const char *min_u, vector<pair<int, int>> &noncore_cluster,
                         vector<char> &is_core_lst, vector<int> &cid, DisjointSets &disjoint_sets) {
    string out_name = dir + "/result-" + string(eps_s) + "-" + string(min_u) + ".txt";
    ofstream ofs(out_name);
    ofs << "c/n vertex_id cluster_id\n";

    // observation 2: unique belonging
    for (auto i = 0; i < n; i++) {
        if (is_core_lst[i] == TRUE) {
            ofs << "c " << i << " " << cid[disjoint_sets.FindRoot(static_cast<uint32_t>(i))] << "\n";
        }
    }

    // possibly multiple belongings
    sort(noncore_cluster.begin(), noncore_cluster.end());
    auto iter_end = unique(noncore_cluster.begin(), noncore_cluster.end());
    for_each(noncore_cluster.begin(), iter_end,
             [&ofs](pair<int, int> my_pair) { ofs << "n " << my_pair.second << " " << my_pair.first << "\n"; });
}

// cid has all core-induced cluster info
void InputOutput::Output(const char *eps_s, const char *min_u, vector<pair<int, int>> &noncore_cluster,
                         vector<bool> &is_core_lst, vector<int> &cid) {
    string out_name = dir + "/result-" + string(eps_s) + "-" + string(min_u) + ".txt";
    ofstream ofs(out_name);
    ofs << "c/n vertex_id cluster_id\n";

    // observation 2: unique belonging
    for (auto i = 0; i < n; i++) { if (is_core_lst[i]) { ofs << "c " << i << " " << cid[i] << "\n"; }}

    // possibly multiple belongings
    sort(noncore_cluster.begin(), noncore_cluster.end());
    auto iter_end = unique(noncore_cluster.begin(), noncore_cluster.end());
    for_each(noncore_cluster.begin(), iter_end,
             [&ofs](pair<int, int> my_pair) { ofs << "n " << my_pair.second << " " << my_pair.first << "\n"; });
}
