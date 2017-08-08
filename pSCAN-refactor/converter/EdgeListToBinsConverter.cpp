//
// Created by yche on 8/8/17.
//
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cassert>

#include "../play/pretty_print.h"

using namespace std;

vector<pair<int, int>> GetEdgeList(string &input_file_path, int &max_ele) {
    vector<pair<int, int>> lines;

    ifstream ifs(input_file_path);

    while (ifs.good()) {
        string tmp_str;
        stringstream ss;
        std::getline(ifs, tmp_str);
        if (!ifs.good())
            break;
        if (tmp_str[0] != '#') {
            ss.clear();
            ss << tmp_str;
            int first, second;
            ss >> first >> second;
            assert(first < INT32_MAX and second < INT32_MAX);
            max_ele = max(max_ele, second);
            lines.emplace_back(first, second);
        }
    }
    return lines;
};

bool IsAlreadyCSROrder(vector<pair<int, int>> &lines) {
    int cur_src_vertex = -1;
    int prev_dst_val = -1;
    for (const auto &line : lines) {
        int src, dst;
        std::tie(src, dst) = line;
        if (src == cur_src_vertex) {
            if (dst < prev_dst_val) { return false; }
            prev_dst_val = dst;
        } else {
            if (src < cur_src_vertex) { return false; }
            cur_src_vertex = src;
            prev_dst_val = dst;
        }
    }
    return true;
}

void WriteToOutputFiles(string &deg_output_file, string &adj_output_file, vector<pair<int, int>> &lines, int max_ele) {
    auto vertex_num = static_cast<unsigned long>(max_ele + 1);
    auto edge_num = lines.size() * 2;
    vector<int> degree_arr(vertex_num);
    std::fill(degree_arr.begin(), degree_arr.end(), 0);
    vector<vector<int>> matrix(vertex_num);

    ofstream deg_ofs(deg_output_file, ios::binary);
    for (const auto &line : lines) {
        int src, dst;
        std::tie(src, dst) = line;
        degree_arr[src]++;
        degree_arr[dst]++;
        matrix[src].emplace_back(dst);
        matrix[dst].emplace_back(src);
    }

    cout << "begin out" << endl;
    int int_size = sizeof(int);
    deg_ofs.write(reinterpret_cast<const char *>(&int_size), 4);
    deg_ofs.write(reinterpret_cast<const char *>(&vertex_num), 4);
    deg_ofs.write(reinterpret_cast<const char *>(&edge_num), 4);
    deg_ofs.write(reinterpret_cast<const char *>(&degree_arr.front()), degree_arr.size() * 4);

    ofstream adj_ofs(adj_output_file, ios::binary);
    cout << matrix.back() << endl;
    vector<int> flat_matrix;
    flat_matrix.reserve(edge_num);
    for (auto &adj_arr: matrix) {
        for (auto ele:adj_arr) {
            flat_matrix.emplace_back(ele);
        }
    }
    cout << "ready write..." << endl;
    adj_ofs.write(reinterpret_cast<const char *>(&flat_matrix.front()), flat_matrix.size() * 4);
    cout << "finish write.." << endl;
}

int main(int argc, char *argv[]) {
    string input_file_path(argv[1]);
    string deg_output_file_path(argv[2]);
    string adj_output_file_path(argv[3]);

    int max_ele = -1;
    auto lines = GetEdgeList(input_file_path, max_ele);
    cout << "number of edges:" << lines.size() << "\n";
    cout << "first element:" << lines.front() << "\n";
    cout << "last element:" << lines.back() << "\n";
    if (IsAlreadyCSROrder(lines)) {
        cout << "already csr" << "\n";
        WriteToOutputFiles(deg_output_file_path, adj_output_file_path, lines, max_ele);
    }
}
