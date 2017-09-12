//
// Created by yche on 8/18/17.
//

#include <cstring>

#include <iostream>
#include <chrono>

#ifdef WITHGPERFTOOLS

#include <gperftools/profiler.h>

#endif

#include "GraphParallelExp.h"

void Usage() {
    cout << "Usage: [1]exe [2]graph-dir [3]similarity-threshold "
            "[4]density-threshold [5 thread_num] [6 optional]output\n";
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        Usage();
    } else {
        // input
        using namespace std::chrono;
        auto io_start = high_resolution_clock::now();
        auto *graph = new GraphParallelExp(argv[1], argv[2], atoi(argv[3]), static_cast<ui>(atoi(argv[4])));
        auto io_end = high_resolution_clock::now();
        cout << "\nTotal input cost:" << duration_cast<milliseconds>(io_end - io_start).count() << " ms\n";

        // compute
        auto start = high_resolution_clock::now();
#ifdef WITHGPERFTOOLS
        cout << "with google perf start------------\n";
        ProfilerStart("pscanProfile.log");
#endif
        graph->pSCAN();
#ifdef WITHGPERFTOOLS
        cout << "with google perf end--------------\n";
        ProfilerStop();
#endif
        auto end = high_resolution_clock::now();
        cout << "Total time without IO:" << duration_cast<milliseconds>(end - start).count() << " ms\n";

        // Output
        io_start = high_resolution_clock::now();
        if (argc >= 6 && strcmp(argv[5], "output") == 0) { graph->Output(argv[2], argv[3]); }
        io_end = high_resolution_clock::now();
        cout << "Total output cost:" << duration_cast<milliseconds>(io_end - io_start).count() << " ms\n\n";
    }
    return 0;
}

