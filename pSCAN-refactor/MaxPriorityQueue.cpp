//
// Created by yche on 8/7/17.
//

#include "MaxPriorityQueue.h"

MaxPriorityQueue::MaxPriorityQueue(unsigned int vertex_num, vector<int> *effective_deg_ptr, int min_u) {
    bin_head = vector<int>(vertex_num);
    bin_next = vector<int>(vertex_num);
    std::fill(bin_head.begin(), bin_head.end(), INVALID_VERTEX_IDX);
    effective_deg_ptr_ = effective_deg_ptr;
    cur_max_ed = 0;
    min_u_ = min_u;

    auto &effective_degree = *effective_deg_ptr_;
    for (auto i = 0; i < vertex_num; i++) {
        if (effective_degree[i] >= min_u) {
            int ed = effective_degree[i];
            if (ed > cur_max_ed) { cur_max_ed = ed; }
            bin_next[i] = bin_head[ed];
            bin_head[ed] = i;
        }
    }
}

int MaxPriorityQueue::pop() {
    int u = INVALID_VERTEX_IDX;
    auto &effective_degree = *effective_deg_ptr_;
    while (cur_max_ed >= min_u_ && u == INVALID_VERTEX_IDX) {
        for (int x = bin_head[cur_max_ed]; x != -1;) {
            int tmp = bin_next[x];
            int ed = effective_degree[x];
            // dynamically maintain the max-priority-queue(heap)
            if (ed == cur_max_ed) {
                u = x;
                bin_head[cur_max_ed] = bin_next[x];
                break;
            }
            // lazy update
            if (ed >= min_u_) {
                bin_next[x] = bin_head[ed];
                bin_head[ed] = x;
            }
            x = tmp;
        }
        // reach the tail of linked list, within underlying array
        if (u == INVALID_VERTEX_IDX) {
            bin_head[cur_max_ed] = -1;
            --cur_max_ed;
        }
    }
    return u;
}
