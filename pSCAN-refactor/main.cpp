#include <iostream>
#include <cstring>
#include <chrono>

#include "Graph.h"

void usage() {
    cout << "Usage: [1]exe [2]graph-dir [3]similarity-threshold [4]density-threshold [5 optional]output\n";
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        usage();
        return 0;
    }
    // input
    auto *graph = new Graph(argv[1], argv[2], atoi(argv[3]));

    // compute
    using namespace std::chrono;
    auto start = high_resolution_clock::now();
    graph->pSCAN();
    auto end = high_resolution_clock::now();
    cout << "Total time without IO:" << duration_cast<milliseconds>(end - start).count() << " ms\n";

    // output
    if (argc >= 5 && strcmp(argv[4], "output") == 0) graph->output(argv[2], argv[3]);
    return 0;
}

