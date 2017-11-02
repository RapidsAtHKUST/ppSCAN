//
// Created by yche on 10/31/17.
//
#include "scanxp.h"
#include <chrono>

void Usage() {
    cout << "Usage: [1]exe [2]graph-dir [3]similarity-threshold "
            "[4]density-threshold [5 thread_num] [6 optional]output\n";
}

int main(int argc, char *argv[]) {
    using namespace chrono;
    if (argc < 4) {
        Usage();
        return 0;
    }
    cout << "****scan-xp on cpu: " << argv[1] << ", " << argv[2] << ", " << argv[3] << " *** \n";
    // parse parameters
    cout << "graph dir" << argv[1] << endl;
    auto EPSILON = strtod(argv[2], nullptr);
    auto MY_U = atoi(argv[3]);
    auto NUMT = atoi(argv[4]);

    SCAN_XP scanxp(NUMT, MY_U, EPSILON, argv[1]);
    scanxp.Execute();
}
