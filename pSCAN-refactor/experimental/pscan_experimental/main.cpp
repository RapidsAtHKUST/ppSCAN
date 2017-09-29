#include <chrono>

#include "Utility.h"
#include "pSCANGraph.h"

#ifdef WITHGPERFTOOLS

#include <gperftools/profiler.h>

#endif

void Usage() {
    cout << "Usage: [1]exe [2]graph-dir [3]similarity-threshold [4]density-threshold [5 optional]output\n";
}

int main(int argc, char *argv[]) {
    using namespace std;
    using namespace chrono;
    if (argc < 4) {
        Usage();
        return 0;
    }
    cout << "**** Graph Clustering (Release): " << argv[1] << ", " << argv[2] << ", " << argv[3] << " *** \n";

    // load graph
    auto start = high_resolution_clock::now();
    auto graph = new pSCANGraph(argv[1]);
    graph->read_graph();
    auto end = high_resolution_clock::now();
    cout << "Total input cost:" << duration_cast<milliseconds>(end - start).count() << " ms\n";

    // computation
#ifdef WITHGPERFTOOLS
    cout << "with google perf start------------\n";
    ProfilerStart("pscan_algo.log");
#endif
    graph->pSCAN(argv[2], atoi(argv[3]));
    auto end2 = high_resolution_clock::now();
    cout << "Total time without IO:" << duration_cast<milliseconds>(end2 - end).count() << " ms\n";
#ifdef WITHGPERFTOOLS
    cout << "with google perf end--------------\n";
    ProfilerStop();
#endif

    // output
    if (argc >= 5 && strcmp(argv[4], "output") == 0) graph->output(argv[2], argv[3]);
    auto end3 = high_resolution_clock::now();
    cout << "Total output cost:" << duration_cast<milliseconds>(end3 - end2).count() << " ms\n";
    return 0;
}