//
// Created by yche on 10/31/17.
//

#include "csr_set_intersection.h"

#include <immintrin.h>  //AVX

uint32_t BinarySearchForGallopingSearch(int *array, uint32_t offset_beg, uint32_t offset_end, int val) {
    auto mid = static_cast<uint32_t>((static_cast<unsigned long>(offset_beg) + offset_end) / 2);
    if (array[mid] == val) {
        return mid;
    }
    if (array[mid] < val) {
        return mid + 1 == offset_end ? mid : BinarySearchForGallopingSearch(array, mid + 1, offset_end, val);
    }
    if (mid == offset_beg) {
        return offset_beg;
    }

    return BinarySearchForGallopingSearch(array, offset_beg, mid, val);
}

uint32_t GallopingSearch(int *array, uint32_t offset_beg, uint32_t offset_end, int val) {
    if (array[offset_end - 1] < val) {
        return offset_end;
    }
    // galloping
    if (array[offset_beg] >= val) {
        return offset_beg;
    }
    if (array[offset_beg + 1] >= val) {
        return offset_beg + 1;
    }
    if (array[offset_beg + 2] >= val) {
        return offset_beg + 2;
    }

    auto jump_idx = 4u;
    bool is_working = true;
    while (is_working) {
        auto peek_idx = offset_beg + jump_idx;
        if (peek_idx >= offset_end) {
            peek_idx = offset_end - 1;
            is_working = false;
        }
        if (array[peek_idx] == val) {
            return peek_idx;
        }
        if (array[peek_idx] > val) {
//            return GallopingSearch(array, jump_idx / 2 + offset_beg + 1, peek_idx + 1, val);
            return BinarySearchForGallopingSearch(array, jump_idx / 2 + offset_beg + 1, peek_idx + 1, val);
        }
        jump_idx <<= 1;
    }
}

int ComputeCNNaive(Graph *g, uint32_t edge_idx) {
    auto u = g->edge_src[edge_idx];
    auto v = g->edge_dst[edge_idx];
    auto cn_count = 0;
    auto offset_nei_u = g->node_off[u], offset_nei_v = g->node_off[v];
    auto off_u_end = g->node_off[u + 1], off_v_end = g->node_off[v + 1];
    while (true) {
        while (g->edge_dst[offset_nei_u] < g->edge_dst[offset_nei_v]) {
            ++offset_nei_u;
            if (offset_nei_u >= off_u_end) {
                return cn_count;
            }
        }

        while (g->edge_dst[offset_nei_u] > g->edge_dst[offset_nei_v]) {
            ++offset_nei_v;
            if (offset_nei_v >= off_v_end) {
                return cn_count;
            }
        }

        if (g->edge_dst[offset_nei_u] == g->edge_dst[offset_nei_v]) {
            cn_count++;
            ++offset_nei_u;
            ++offset_nei_v;
            if (offset_nei_u >= off_u_end || offset_nei_v >= off_v_end) {
                return cn_count;
            }
        }
    }
}

int ComputeCNGallopingDoubleDir(Graph *g, uint32_t edge_idx) {
    auto u = g->edge_src[edge_idx];
    auto v = g->edge_dst[edge_idx];
    auto cn_count = 0;
    auto offset_nei_u = g->node_off[u], offset_nei_v = g->node_off[v];
    auto off_u_end = g->node_off[u + 1], off_v_end = g->node_off[v + 1];
    while (true) {
        offset_nei_u = GallopingSearch(g->edge_dst, offset_nei_u, off_u_end, g->edge_dst[offset_nei_v]);
        if (offset_nei_u >= off_u_end) {
            return cn_count;
        }

        offset_nei_v = GallopingSearch(g->edge_dst, offset_nei_v, off_v_end, g->edge_dst[offset_nei_u]);
        if (offset_nei_v >= off_v_end) {
            return cn_count;
        }

        if (g->edge_dst[offset_nei_u] == g->edge_dst[offset_nei_v]) {
            cn_count++;
            ++offset_nei_u;
            ++offset_nei_v;
            if (offset_nei_u >= off_u_end || offset_nei_v >= off_v_end) {
                return cn_count;
            }
        }
    }
}

int ComputeCNGallopingSingleDir(Graph *g, uint32_t edge_idx) {
    auto u = g->edge_src[edge_idx];
    auto v = g->edge_dst[edge_idx];
    auto cn_count = 0;
    auto offset_nei_u = g->node_off[u], offset_nei_v = g->node_off[v];
    auto off_u_end = g->node_off[u + 1], off_v_end = g->node_off[v + 1];
    auto is_first_galloping = off_u_end - offset_nei_u > off_v_end - offset_nei_v;

    if (is_first_galloping) {
        while (true) {
            offset_nei_u = GallopingSearch(g->edge_dst, offset_nei_u, off_u_end, g->edge_dst[offset_nei_v]);
            if (offset_nei_u >= off_u_end) {
                return cn_count;
            }

            while (g->edge_dst[offset_nei_u] > g->edge_dst[offset_nei_v]) {
                ++offset_nei_v;
                if (offset_nei_v >= off_v_end) {
                    return cn_count;
                }
            }

            if (g->edge_dst[offset_nei_u] == g->edge_dst[offset_nei_v]) {
                cn_count++;
                ++offset_nei_u;
                ++offset_nei_v;
                if (offset_nei_u >= off_u_end || offset_nei_v >= off_v_end) {
                    return cn_count;
                }
            }
        }
    } else {
        while (true) {
            while (g->edge_dst[offset_nei_u] < g->edge_dst[offset_nei_v]) {
                ++offset_nei_u;
                if (offset_nei_u >= off_u_end) {
                    return cn_count;
                }
            }

            offset_nei_v = GallopingSearch(g->edge_dst, offset_nei_v, off_v_end, g->edge_dst[offset_nei_u]);
            if (offset_nei_v >= off_v_end) {
                return cn_count;
            }

            if (g->edge_dst[offset_nei_u] == g->edge_dst[offset_nei_v]) {
                cn_count++;
                ++offset_nei_u;
                ++offset_nei_v;
                if (offset_nei_u >= off_u_end || offset_nei_v >= off_v_end) {
                    return cn_count;
                }
            }
        }
    }
}

int ComputeCNAVX2(Graph *g, uint32_t edge_idx) {
    constexpr int PARA = 8;
    auto cn_count = 0;
    int countplus[PARA] = {1, 1, 1, 1, 1, 1, 1, 1};
    __m256i sse_countplus = _mm256_load_si256((__m256i *) (countplus));
    __m256i sj = _mm256_set_epi32(1, 1, 1, 1, 0, 0, 0, 0);
    __m256i st = _mm256_set_epi32(3, 2, 1, 0, 3, 2, 1, 0);
    int cnv[PARA] = {0, 0, 0, 0, 0, 0, 0, 0};

    __m256i sse_cnv = _mm256_load_si256((__m256i *) (cnv));
    int j, t, j2, t2;

    if (g->node_off[g->edge_src[edge_idx] + 1] - g->node_off[g->edge_src[edge_idx]] <
        g->node_off[g->edge_dst[edge_idx] + 1] - g->node_off[g->edge_dst[edge_idx]]) {
        j = g->node_off[g->edge_src[edge_idx]];
        t = g->node_off[g->edge_dst[edge_idx]];
        j2 = g->node_off[g->edge_src[edge_idx] + 1];
        t2 = g->node_off[g->edge_dst[edge_idx] + 1];
    } else {
        t = g->node_off[g->edge_src[edge_idx]];
        j = g->node_off[g->edge_dst[edge_idx]];
        t2 = g->node_off[g->edge_src[edge_idx] + 1];
        j2 = g->node_off[g->edge_dst[edge_idx] + 1];
    }

    int size_ratio;
    size_ratio = (t2 - t) / (j2 - j);
    if (size_ratio > 2) {
        __m256i jnode = _mm256_set1_epi32(g->edge_dst[j]);
        __m256i tnode = _mm256_loadu_si256((__m256i *) (g->edge_dst + t));
        __m256i ssecnv = _mm256_load_si256((__m256i *) (cnv));

        int vsize = ((t2 - t) / PARA) * PARA;
        int to = t;

        while (j < j2 && t < to + vsize) {
            __m256i mask = _mm256_cmpeq_epi32(jnode, tnode);
            mask = _mm256_and_si256(sse_countplus, mask);
            ssecnv = _mm256_add_epi32(ssecnv, mask);

            if (g->edge_dst[j] > g->edge_dst[t + 7]) {
                t += PARA;
                tnode = _mm256_loadu_si256((__m256i *) (g->edge_dst + t));
            } else {
                j++;
                jnode = _mm256_set1_epi32(g->edge_dst[j]);
            }
        }
        _mm256_store_si256((__m256i *) cnv, ssecnv);

        for (int cnvplus : cnv) {
            cn_count += cnvplus;
        }

        t = (to + vsize);

        while (j < j2 && t < t2) {
            if (g->edge_dst[j] == g->edge_dst[t]) {
                cn_count++;
                j++;
                t++;
            } else if (g->edge_dst[j] > g->edge_dst[t]) {
                t++;
            } else {
                j++;
            }
        }
    } else {
        if (t2 - t < j2 - j) {
            j = g->node_off[g->edge_dst[edge_idx]];
            t = g->node_off[g->edge_src[edge_idx]];
            j2 = g->node_off[g->edge_dst[edge_idx] + 1];
            t2 = g->node_off[g->edge_src[edge_idx] + 1];
        }
        int jsize = ((j2 - j) / 2) * 2 + j;
        int tsize = ((t2 - t) / 4) * 4 + t;

        __m256i jnode, tnode;

        jnode = _mm256_loadu_si256((__m256i *) (g->edge_dst + j));
        tnode = _mm256_loadu_si256((__m256i *) (g->edge_dst + t));

        while (j < jsize && t < tsize) {
            __m256i jnodeA = _mm256_permutevar8x32_epi32(jnode, sj);
            __m256i tnodeA = _mm256_permutevar8x32_epi32(tnode, st);
            __m256i mask = _mm256_cmpeq_epi32(jnodeA, tnodeA);
            mask = _mm256_and_si256(sse_countplus, mask);
            sse_cnv = _mm256_add_epi32(sse_cnv, mask);

            if (g->edge_dst[j + 1] == g->edge_dst[t + 3]) {
                j += 2;
                t += 4;
                jnode = _mm256_loadu_si256((__m256i *) (g->edge_dst + j));
                tnode = _mm256_loadu_si256((__m256i *) (g->edge_dst + t));
            } else if (g->edge_dst[j + 1] > g->edge_dst[t + 3]) {
                t += 4;

                tnode = _mm256_loadu_si256((__m256i *) (g->edge_dst + t));
                _mm256_permutevar8x32_epi32(tnode, st);
            } else {
                j += 2;
                jnode = _mm256_loadu_si256((__m256i *) (g->edge_dst + j));
            }
        }

        _mm256_store_si256((__m256i *) cnv, sse_cnv);

        for (int cnvplus : cnv) {
            cn_count += cnvplus;
        }

        if (j >= jsize) {
            j = jsize;
        } else {
            t = tsize;
        }

        while (j < j2 && t < t2) {
            if (g->edge_dst[j] == g->edge_dst[t]) {
                cn_count++;
                j++;
                t++;
            } else if (g->edge_dst[j] > g->edge_dst[t]) {
                t++;

            } else {
                j++;
            }
        }
    }
    return cn_count;
}