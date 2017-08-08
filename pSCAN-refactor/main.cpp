#include <iostream>
#include <cstring>
#include <chrono>

#include "Graph.h"

#ifdef WITHGPERFTOOLS

#include <gperftools/profiler.h>

#endif

void Usage() {
    cout << "Usage: [1]exe [2]graph-dir [3]similarity-threshold [4]density-threshold [5 optional]Output\n";
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        Usage();
    } else {
        // input
        auto *graph = new Graph(argv[1], argv[2], atoi(argv[3]));

        // compute
        using namespace std::chrono;
        auto start = high_resolution_clock::now();
#ifdef WITHGPERFTOOLS
        cout << "with google perf start" << endl;
        ProfilerStart("pscanProfile.log");
#endif
        graph->pSCAN();
#ifdef WITHGPERFTOOLS
        cout << "with google perf end" << endl;
        ProfilerStop();
#endif
        auto end = high_resolution_clock::now();
        cout << "Total time without IO:" << duration_cast<milliseconds>(end - start).count() << " ms\n";

        // Output
        if (argc >= 5 && strcmp(argv[4], "output") == 0) { graph->Output(argv[2], argv[3]); }
    }
    return 0;
}

