//
// Created by yche on 8/7/17.
//

#ifndef PSCAN_MAXPRIORITYQUEUE_H
#define PSCAN_MAXPRIORITYQUEUE_H

#include <vector>

using namespace std;
constexpr int INVALID_VERTEX_IDX = -1;

class MaxPriorityQueueWithLazyUpdate {
private:
    vector<int> bin_next;
    vector<int> bin_head;

    vector<int> *effective_deg_ptr_;
    int cur_max_ed;
    int min_u_;
public:
    MaxPriorityQueueWithLazyUpdate(unsigned int vertex_num, vector<int> *effective_deg_ptr, int min_u);

    int pop();
};


#endif //PSCAN_MAXPRIORITYQUEUE_H
