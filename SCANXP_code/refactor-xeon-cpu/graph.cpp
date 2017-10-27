#include"graph.h"

#include <chrono>

#define UNCLASSIFIED 0

using namespace chrono;

Graph::Graph(char *dir_cstr) {
    dir = string(dir_cstr);

    ReadDegree();
    ReadAdjacencyList();
    CheckInputGraph();

    // vertex property
    label = new int[nodemax];
    core = new bool[nodemax];
    core_count = new int[nodemax];

    // edge_dst property
    edge_src = new int[edgemax];
    common_node_num = new int[edgemax];
    sim_values = new double[edgemax];
    similarity = new bool[edgemax];
}

void Graph::ReadDegree() {
    auto start = high_resolution_clock::now();

    ifstream deg_file(dir + string("/b_degree.bin"), ios::binary);
    int int_size;
    deg_file.read(reinterpret_cast<char *>(&int_size), 4);
    cout << "int size:" << int_size << "\n";

    deg_file.read(reinterpret_cast<char *>(&nodemax), 4);
    deg_file.read(reinterpret_cast<char *>(&edgemax), 4);
    cout << "n:" << nodemax << ", m:" << edgemax << "\n";

    degree.resize(static_cast<unsigned long>(nodemax));
    deg_file.read(reinterpret_cast<char *>(&degree.front()), sizeof(int) * nodemax);

    auto end = high_resolution_clock::now();
    cout << "read degree file time:" << duration_cast<milliseconds>(end - start).count() << " ms\n";
}

void Graph::ReadAdjacencyList() {
    auto start = high_resolution_clock::now();
    ifstream adj_file(dir + string("/b_adj.bin"), ios::binary);

    // csr representation
    node_off = new int[nodemax + 1];
    edge_dst = new int[edgemax];

    node_off[0] = 0;
    for (auto i = 0; i < nodemax; i++) { node_off[i + 1] = node_off[i] + degree[i]; }
    for (auto i = 0; i < nodemax; i++) {
        if (degree[i] > 0) {
            adj_file.read(reinterpret_cast<char *>(&edge_dst[node_off[i]]), degree[i] * sizeof(int));
        }
        // inclusive
        degree[i]++;
    }

    auto end = high_resolution_clock::now();
    cout << "read adjacency list file time:" << duration_cast<milliseconds>(end - start).count() << " ms\n";
}

void Graph::CheckInputGraph() {
    auto start = high_resolution_clock::now();

    for (auto i = 0; i < nodemax; i++) {
        for (auto j = node_off[i]; j < node_off[i + 1]; j++) {
            if (edge_dst[j] == i) {
                cout << "Self loop\n";
                exit(1);
            }
            if (j > node_off[i] && edge_dst[j] <= edge_dst[j - 1]) {
                cout << "Edges not sorted in increasing id order!\nThe program may not run properly!\n";
                exit(1);
            }
        }
    }
    auto end = high_resolution_clock::now();
    cout << "check input graph file time:" << duration_cast<milliseconds>(end - start).count() << " ms\n";
}

