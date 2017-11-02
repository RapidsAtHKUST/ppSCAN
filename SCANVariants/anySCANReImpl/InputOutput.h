//
// Created by yche on 8/4/17.
//

#ifndef PPSCAN_INPUTOUTPUT_H
#define PPSCAN_INPUTOUTPUT_H

#include <fstream>
#include <vector>
#include <atomic>
#include <unordered_set>
#include <memory>

#include "DisjointSet.h"

using ui=unsigned int;
using namespace std;

namespace anySCAN {
    constexpr char UN_TOUCHED = 0;

    // eps-neighborhood unknown
    constexpr char UNPROCESSED_CORE = 1;
    // eps-neighborhood known
    constexpr char PROCESSED_CORE = 2;

    constexpr char UNPROCESSED_BORDER = 3;
    constexpr char PROCESSED_BORDER = 4;
    constexpr char UNPROCESSED_NOISE = 5;
    constexpr char PROCESSED_NOISE = 6;
}

namespace yche {
    constexpr int NOT_SIMILAR = -2;
    constexpr int SIMILAR = -1;

    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args &&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}

class InputOutput {
private:
    string dir;

    void ReadDegree();

    void CheckInputGraph();

    void ReadAdjacencyList();

public:
    explicit InputOutput(const string &dir);

    ui n;
    ui m;

    // csr
    vector<ui> offset_out_edges;
    vector<int> out_edges;

    // vertex property
    vector<int> degree;

    void ReadGraph();

    pair<int, int> ParseEps(const char *eps_s);

    // anySCAN
    void OutputAnySCAN(const char *eps_s, const char *min_u, vector<pair<int, int>> &noncore_cluster,
                       vector<char> &core_status, vector<int> &cid, vector<int> &parent);
};

#endif //PPSCAN_INPUTOUTPUT_H