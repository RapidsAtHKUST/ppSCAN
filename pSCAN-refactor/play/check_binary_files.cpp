//
// Created by yche on 8/13/17.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

#include "pretty_print.h"
#include "../InputOutput.h"
#include "../Graph.h"

#ifdef WITHGPERFTOOLS

#include <gperftools/profiler.h>
#include <cassert>

#endif

using namespace std;
using ui=unsigned int;

vector<int> ReadDegree(string &file_path, unsigned int &n, unsigned int &m) {
    cout << file_path << endl;
    ifstream deg_file(file_path, ios::binary);

    int int_size;
    deg_file.read(reinterpret_cast<char *>(&int_size), 4);
    cout << "int size:" << int_size << "\n";

    deg_file.read(reinterpret_cast<char *>(&n), 4);
    deg_file.read(reinterpret_cast<char *>(&m), 4);
    cout << "n:" << n << ", m:" << m << "\n";

    auto degree_arr = vector<int>(n);
    deg_file.read(reinterpret_cast<char *>(&degree_arr.front()), sizeof(int) * n);
    return degree_arr;
}

vector<int> ReadAdjacencyList(string &file_path, size_t offset, int num) {
    ifstream adj_file(file_path, ios::binary);
    adj_file.seekg(offset, ios::beg);
    auto edges = vector<int>(num);
    cout << "start from offset:" << offset << ", num:" << num << endl;
    adj_file.read(reinterpret_cast<char *>(&edges.front()), 4 * num);
    cout << "finished" << endl;
    return edges;
}


ui BinarySearch(vector<int> &array, ui offset_beg, ui offset_end, int val) {
    auto mid = static_cast<ui>((static_cast<unsigned long>(offset_beg) + offset_end) / 2);
    if (offset_beg >= offset_end) {
        cout << "err:" << offset_beg << " " << offset_end << " " << val << endl;
        exit(1);
    }
    if (array[mid] == val) { return mid; }
    return val < array[mid] ? BinarySearch(array, offset_beg, mid, val) : BinarySearch(array, mid + 1, offset_end, val);
}

void Test0(string &deg_file_path, string &adj_file_path) {
    ui n, m;

    auto deg_arr = ReadDegree(deg_file_path, n, m);
    auto offset_out_edges = vector<ui>(n + 1);

    offset_out_edges[0] = 0;
    cout << deg_arr.back() << endl;
    for (auto i = 0; i < n; i++) { offset_out_edges[i + 1] = offset_out_edges[i] + deg_arr[i]; }

    auto idx = 1612104523u;
    auto v = 52665228;
    cout << offset_out_edges.size() << endl;
    cout << offset_out_edges[v] << endl;
    cout << offset_out_edges[v + 1] << endl;

    int u;
    for (auto i = 0; i < n; i++) {
        if (offset_out_edges[i] <= idx && offset_out_edges[i + 1] > idx) {
            u = i;
            cout << "u:" << u << endl;
            break;
        }
    }

    auto adj_of_v = ReadAdjacencyList(adj_file_path, 4 * offset_out_edges[v],
                                      offset_out_edges[v + 1] - offset_out_edges[v]);
    cout << adj_of_v << endl;
    sort(adj_of_v.begin(), adj_of_v.end());
    cout << adj_of_v << endl;

    v = 38964727;
    cout << offset_out_edges[v] << "," << offset_out_edges[v + 1] << endl;
    adj_of_v = ReadAdjacencyList(adj_file_path, 4 * offset_out_edges[v],
                                 offset_out_edges[v + 1] - offset_out_edges[v]);
    cout << adj_of_v << endl;
    sort(adj_of_v.begin(), adj_of_v.end());
    cout << adj_of_v << endl;

    v = 21789429;
    cout << offset_out_edges[v] << "," << offset_out_edges[v + 1] << endl;
    adj_of_v = ReadAdjacencyList(adj_file_path, 4 * offset_out_edges[v],
                                 offset_out_edges[v + 1] - offset_out_edges[v]);
    cout << adj_of_v << endl;
    sort(adj_of_v.begin(), adj_of_v.end());
    cout << adj_of_v << endl;
}

int main(int argc, char *argv[]) {
//    string deg_file_path(argv[1]);
//    string adj_file_path(argv[2]);
//    Test0(deg_file_path, adj_file_path);

    string dir(argv[1]);
    auto io_helper_ptr = yche::make_unique<InputOutput>(dir);

    using namespace std::chrono;

    auto start = high_resolution_clock::now();
#ifdef WITHGPERFTOOLS
    cout << "with google perf start------------\n";
    ProfilerStart("pscanProfile.log");
#endif
    io_helper_ptr->ReadGraph();

    auto end = high_resolution_clock::now();
    cout << "total time:" << duration_cast<milliseconds>(end - start).count() << " ms\n";

    auto out_edge_start = std::move(io_helper_ptr->offset_out_edges);
    auto out_edges = std::move(io_helper_ptr->out_edges);

    for (auto i = 0; i < io_helper_ptr->n; i++) {
        for (auto j = out_edge_start[i]; j < out_edge_start[i + 1]; j++) {
            auto v = out_edges[j];
            auto offset_v_i = BinarySearch(out_edges, out_edge_start[v], out_edge_start[v + 1], i);
            assert(i == out_edges[offset_v_i]);
        }
    }

    auto end2 = high_resolution_clock::now();
    cout << "check time:" << duration_cast<milliseconds>(end2 - end).count() << " ms\n";

#ifdef WITHGPERFTOOLS
    cout << "with google perf end--------------\n";
    ProfilerStop();
#endif
}