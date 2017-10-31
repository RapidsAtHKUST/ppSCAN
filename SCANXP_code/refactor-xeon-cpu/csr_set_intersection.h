//
// Created by yche on 10/31/17.
//

#ifndef REFACTOR_XEON_CPU_COUNT_SET_INTERSECTION_H
#define REFACTOR_XEON_CPU_COUNT_SET_INTERSECTION_H

#include "graph.h"

uint32_t BinarySearchForGallopingSearch(int *array, uint32_t offset_beg, uint32_t offset_end, int val);

uint32_t GallopingSearch(int *array, uint32_t offset_beg, uint32_t offset_end, int val);

int ComputeCNNaive(Graph *g, uint32_t edge_idx);

int ComputeCNGallopingSingleDir(Graph *g, uint32_t edge_idx);

int ComputeCNGallopingDoubleDir(Graph *g, uint32_t edge_idx);

int ComputeCNAVX2(Graph *g, uint32_t edge_idx);

#endif //REFACTOR_XEON_CPU_COUNT_SET_INTERSECTION_H
