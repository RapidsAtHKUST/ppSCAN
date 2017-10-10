//
// Created by yche on 10/10/17.
//
#include "../InputOutput.h"
#include "../playground/pretty_print.h"

using namespace std;

ui GallopingSearch(vector<int> &array, ui offset_beg, ui offset_end, int val) {
    if (array[offset_end - 1] < val) {
        return offset_end;
    }

    // galloping
    if (array[offset_beg] >= val) {
        return offset_beg;
    }
    if (array[offset_beg + 1] >= val) {
        return offset_beg + 1;
    }
    if (array[offset_beg + 2] >= val) {
        return offset_beg + 2;
    }

    auto jump_idx = 4u;
    bool is_working = true;
    while (is_working) {
        auto peek_idx = offset_beg + jump_idx;
        if (peek_idx >= offset_end) {
            peek_idx = offset_end - 1;
            is_working = false;
        }
        if (array[peek_idx] == val) {
            return peek_idx;
        }
        if (array[peek_idx] > val) {
            return GallopingSearch(array, jump_idx / 2 + offset_beg + 1, peek_idx + 1, val);
        }
        jump_idx <<= 1;
    }
}

int main(int argc, char **argv) {
    string dir(argv[1]);
    InputOutput inputOutput(dir);
    inputOutput.ReadGraph();

    ofstream ofs(dir + "/" + "anyscan_input.txt");
    ofs << inputOutput.n << "\n";
    for (auto u = 0; u < inputOutput.n; u++) {
        // inclusive degree
        ofs << (inputOutput.offset_out_edges[u + 1] - inputOutput.offset_out_edges[u] + 1) << " ";
        auto middle_end = GallopingSearch(inputOutput.out_edges, inputOutput.offset_out_edges[u],
                                          inputOutput.offset_out_edges[u + 1], u);
        for (auto i = inputOutput.offset_out_edges[u]; i < middle_end; i++) {
            ofs << inputOutput.out_edges[i] << " ";
        }
        ofs << u << " ";
        for (auto i = middle_end; i < inputOutput.offset_out_edges[u + 1]; i++) {
            ofs << inputOutput.out_edges[i] << " ";
        }
        ofs << "\n";
    }
}