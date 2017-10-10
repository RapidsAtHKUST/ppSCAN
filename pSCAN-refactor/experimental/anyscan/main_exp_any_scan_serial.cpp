//
// Created by yche on 10/3/17.
//
#include <cstring>

#include <chrono>

#include <iostream>
#include "AnySCANGraphSerial.h"

#ifdef WITHGPERFTOOLS

#include <gperftools/profiler.h>

#endif

void Usage() {
    cout << "Usage: [1]exe [2]graph-dir [3]similarity-threshold [4]density-threshold [5 optional]output\n";
}

int main(int argc, char *argv[]) {
    using namespace chrono;
    if (argc < 4) {
        Usage();
        return 0;
    }
    cout << "**** original anySCAN with easy-comp: " << argv[1] << ", " << argv[2] << ", " << argv[3]
         << " *** \n";

    // load graph
    auto start = high_resolution_clock::now();
    auto graph = new AnySCANGraph(argv[1], argv[2], atoi(argv[3]));
    auto end = high_resolution_clock::now();
    cout << "Total input cost:" << duration_cast<milliseconds>(end - start).count() << " ms\n\n";

    // computation
#ifdef WITHGPERFTOOLS
    cout << "with google perf start------------\n";
    ProfilerStart("pscan_algo.log");
#endif
    graph->anySCAN();
    auto end2 = high_resolution_clock::now();
    cout << "Total time without IO:" << duration_cast<milliseconds>(end2 - end).count() << " ms\n";
#ifdef WITHGPERFTOOLS
    cout << "with google perf end--------------\n";
    ProfilerStop();
#endif

    // output
    if (argc >= 5 && strcmp(argv[4], "output") == 0) graph->Output(argv[2], argv[3]);
    auto end3 = high_resolution_clock::now();
    cout << "\nTotal output cost:" << duration_cast<milliseconds>(end3 - end2).count() << " ms\n";
    return 0;
}