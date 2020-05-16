//
// Created by CheYulin on 2020/5/16.
//
#include "Graph.h"
#include <x86intrin.h>
#include <immintrin.h>

#include "Util.h"

using namespace yche;

int Graph::IntersectNeighborSetsSSE(int u, int v, int min_cn_num) {
    int cn = 2; // count for self and v, count for self and u
    int du = out_edge_start[u + 1] - out_edge_start[u] + 2, dv =
            out_edge_start[v + 1] - out_edge_start[v] + 2; // count for self and v, count for self and u

    auto offset_nei_u = out_edge_start[u], offset_nei_v = out_edge_start[v];

    // correctness guaranteed by two pruning previously in computing min_cn
    constexpr int parallelism = 4;
    while (true) {
        // sse4.2, out_edges[offset_nei_v] as the pivot
        __m128i pivot_u = _mm_set1_epi32(out_edges[offset_nei_v]);
        while (offset_nei_u + parallelism < out_edge_start[u + 1]) {
            __m128i inspected_ele = _mm_loadu_si128(reinterpret_cast<const __m128i *>(&out_edges[offset_nei_u]));
            __m128i cmp_res = _mm_cmpgt_epi32(pivot_u, inspected_ele);
            auto mask = _mm_movemask_epi8(cmp_res);
            auto count = mask == 0xffff ? parallelism : _popcnt32(mask) >> 2;
            // update offset_nei_u and du
            offset_nei_u += count;
            du -= count;
            if (du < min_cn_num) {
                return NOT_SIMILAR;
            }
            if (count < parallelism) {
                break;
            }
        }
        if (offset_nei_u + parallelism >= out_edge_start[u + 1]) {
            break;
        }

        // sse4.2, out_edges[offset_nei_u] as the pivot
        __m128i pivot_v = _mm_set1_epi32(out_edges[offset_nei_u]);
        while (offset_nei_v + parallelism < out_edge_start[v + 1]) {
            __m128i inspected_ele = _mm_loadu_si128(reinterpret_cast<const __m128i *>(&out_edges[offset_nei_v]));
            __m128i cmp_res = _mm_cmpgt_epi32(pivot_v, inspected_ele);
            auto mask = _mm_movemask_epi8(cmp_res);
            auto count = mask == 0xffff ? parallelism : _popcnt32(mask) >> 2;

            // update offset_nei_u and du
            offset_nei_v += count;
            dv -= count;
            if (dv < min_cn_num) {
                return NOT_SIMILAR;
            }
            if (count < parallelism) {
                break;
            }
        }
        if (offset_nei_v + parallelism >= out_edge_start[v + 1]) {
            break;
        }

        // find possible equality
        if (out_edges[offset_nei_u] == out_edges[offset_nei_v]) {
            ++cn;
            if (cn >= min_cn_num) {
                return SIMILAR;
            }
            ++offset_nei_u;
            ++offset_nei_v;
        }
    }

    while (true) {
        // left ones
        while (out_edges[offset_nei_u] < out_edges[offset_nei_v]) {
            --du;
            if (du < min_cn_num) { return NOT_SIMILAR; }
            ++offset_nei_u;
        }
        while (out_edges[offset_nei_u] > out_edges[offset_nei_v]) {
            --dv;
            if (dv < min_cn_num) { return NOT_SIMILAR; }
            ++offset_nei_v;
        }
        if (out_edges[offset_nei_u] == out_edges[offset_nei_v]) {
            ++cn;
            if (cn >= min_cn_num) {
                return SIMILAR;
            }
            ++offset_nei_u;
            ++offset_nei_v;
        }
    }
}

#if defined(ENABLE_AVX2)
int Graph::IntersectNeighborSetsAVX2(int u, int v, int min_cn_num) {
    int cn = 2; // count for self and v, count for self and u
    int du = out_edge_start[u + 1] - out_edge_start[u] + 2, dv =
            out_edge_start[v + 1] - out_edge_start[v] + 2; // count for self and v, count for self and u

    auto offset_nei_u = out_edge_start[u], offset_nei_v = out_edge_start[v];

    // correctness guaranteed by two pruning previously in computing min_cn
    constexpr int parallelism = 8;
    while (true) {
        // avx2, out_edges[offset_nei_v] as the pivot
        __m256i pivot_u = _mm256_set1_epi32(out_edges[offset_nei_v]);
        while (offset_nei_u + parallelism < out_edge_start[u + 1]) {
            __m256i inspected_ele = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(&out_edges[offset_nei_u]));
            __m256i cmp_res = _mm256_cmpgt_epi32(pivot_u, inspected_ele);
            auto mask = _mm256_movemask_epi8(cmp_res);
            auto count = mask == 0xffffffff ? parallelism : _popcnt32(mask) >> 2;
            // update offset_nei_u and du
            offset_nei_u += count;
            du -= count;
            if (du < min_cn_num) {
                return NOT_SIMILAR;
            }
            if (count < parallelism) {
                break;
            }
        }
        if (offset_nei_u + parallelism >= out_edge_start[u + 1]) {
            break;
        }

        // avx2, out_edges[offset_nei_u] as the pivot
        __m256i pivot_v = _mm256_set1_epi32(out_edges[offset_nei_u]);
        while (offset_nei_v + parallelism < out_edge_start[v + 1]) {
            __m256i inspected_ele = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(&out_edges[offset_nei_v]));
            __m256i cmp_res = _mm256_cmpgt_epi32(pivot_v, inspected_ele);
            auto mask = _mm256_movemask_epi8(cmp_res);
            auto count = mask == 0xffffffff ? parallelism : _popcnt32(mask) >> 2;

            // update offset_nei_u and du
            offset_nei_v += count;
            dv -= count;
            if (dv < min_cn_num) {
                return NOT_SIMILAR;
            }
            if (count < parallelism) {
                break;
            }
        }
        if (offset_nei_v + parallelism >= out_edge_start[v + 1]) {
            break;
        }

        // find possible equality
        if (out_edges[offset_nei_u] == out_edges[offset_nei_v]) {
            ++cn;
            if (cn >= min_cn_num) {
                return SIMILAR;
            }
            ++offset_nei_u;
            ++offset_nei_v;
        }
    }

    while (true) {
        // left ones
        while (out_edges[offset_nei_u] < out_edges[offset_nei_v]) {
            --du;
            if (du < min_cn_num) { return NOT_SIMILAR; }
            ++offset_nei_u;
        }
        while (out_edges[offset_nei_u] > out_edges[offset_nei_v]) {
            --dv;
            if (dv < min_cn_num) { return NOT_SIMILAR; }
            ++offset_nei_v;
        }
        if (out_edges[offset_nei_u] == out_edges[offset_nei_v]) {
            ++cn;
            if (cn >= min_cn_num) {
                return SIMILAR;
            }
            ++offset_nei_u;
            ++offset_nei_v;
        }
    }
}
#endif

#if defined(ENABLE_AVX2_MERGE)
int Graph::IntersectNeighborSetsAVX2MergePopCnt(int u, int v, int min_cn_num) {
    if (degree[u] > degree[v]) {
        auto tmp = u;
        u = v;
        v = tmp;
    }
    auto off_nei_u = out_edge_start[u], off_nei_v = out_edge_start[v];
    auto off_u_end = out_edge_start[u + 1], off_v_end = out_edge_start[v + 1];

    auto cn_count = 2;
    __m256i two_per_rule = _mm256_set_epi32(1, 1, 1, 1, 0, 0, 0, 0);
    __m256i four_per_rule = _mm256_set_epi32(3, 2, 1, 0, 3, 2, 1, 0);

    auto size1 = (off_v_end - off_nei_v) / (off_u_end - off_nei_u);
    if (size1 > 2) {
        if (off_nei_u < off_u_end && off_nei_v + 7 < off_v_end) {
            __m256i u_elements = _mm256_set1_epi32(out_edges[off_nei_u]);
            __m256i v_elements = _mm256_loadu_si256((__m256i *) (&out_edges[off_nei_v]));

            while (true) {
                __m256i tmp_res = _mm256_cmpeq_epi32(u_elements, v_elements);
                int mask = _mm256_movemask_epi8(tmp_res);
                cn_count += _popcnt32(mask) >> 2;

                if (out_edges[off_nei_u] > out_edges[off_nei_v + 7]) {
                    off_nei_v += 8;
                    if (off_nei_v + 7 >= off_v_end) {
                        break;
                    }
                    v_elements = _mm256_loadu_si256((__m256i *) (&out_edges[off_nei_v]));
                } else {
                    off_nei_u++;
                    if (off_nei_u >= off_u_end) {
                        break;
                    }
                    u_elements = _mm256_set1_epi32(out_edges[off_nei_u]);
                }
            }
        }
    } else {
        if (off_nei_u + 1 < off_u_end && off_nei_v + 3 < off_v_end) {
            __m256i u_elements = _mm256_loadu_si256((__m256i *) (&out_edges[off_nei_u]));
            __m256i u_elements_per = _mm256_permutevar8x32_epi32(u_elements, two_per_rule);
            __m256i v_elements = _mm256_loadu_si256((__m256i *) (&out_edges[off_nei_v]));
            __m256i v_elements_per = _mm256_permutevar8x32_epi32(v_elements, four_per_rule);

            while (true) {
                __m256i tmp_res = _mm256_cmpeq_epi32(u_elements_per, v_elements_per);
                int mask = _mm256_movemask_epi8(tmp_res);
                cn_count += _popcnt32(mask) >> 2;

                if (out_edges[off_nei_u + 1] > out_edges[off_nei_v + 3]) {
                    off_nei_v += 4;
                    if (off_nei_v + 3 >= off_v_end) {
                        break;
                    }
                    v_elements = _mm256_loadu_si256((__m256i *) (&out_edges[off_nei_v]));
                    v_elements_per = _mm256_permutevar8x32_epi32(v_elements, four_per_rule);
                } else if (out_edges[off_nei_u + 1] < out_edges[off_nei_v + 3]) {
                    off_nei_u += 2;
                    if (off_nei_u + 1 >= off_u_end) {
                        break;
                    }
                    u_elements = _mm256_loadu_si256((__m256i *) (&out_edges[off_nei_u]));
                    u_elements_per = _mm256_permutevar8x32_epi32(u_elements, two_per_rule);
                } else {
                    off_nei_u += 2;
                    off_nei_v += 4;
                    if (off_nei_u + 1 >= off_u_end || off_nei_v + 3 >= off_v_end) {
                        break;
                    }
                    u_elements = _mm256_loadu_si256((__m256i *) (&out_edges[off_nei_u]));
                    u_elements_per = _mm256_permutevar8x32_epi32(u_elements, two_per_rule);
                    v_elements = _mm256_loadu_si256((__m256i *) (&out_edges[off_nei_v]));
                    v_elements_per = _mm256_permutevar8x32_epi32(v_elements, four_per_rule);
                }
            }
        }
    }
    if (off_nei_u < off_u_end && off_nei_v < off_v_end) {
        while (true) {
            while (out_edges[off_nei_u] < out_edges[off_nei_v]) {
                ++off_nei_u;
                if (off_nei_u >= off_u_end) {
                    return cn_count >= min_cn_num ? SIMILAR : NOT_SIMILAR;
                }
            }
            while (out_edges[off_nei_u] > out_edges[off_nei_v]) {
                ++off_nei_v;
                if (off_nei_v >= off_v_end) {
                    return cn_count >= min_cn_num ? SIMILAR : NOT_SIMILAR;
                }
            }
            if (out_edges[off_nei_u] == out_edges[off_nei_v]) {
                cn_count++;
                ++off_nei_u;
                ++off_nei_v;
                if (off_nei_u >= off_u_end || off_nei_v >= off_v_end) {
                    return cn_count >= min_cn_num ? SIMILAR : NOT_SIMILAR;
                }
            }
        }
    }
    return cn_count >= min_cn_num ? SIMILAR : NOT_SIMILAR;
}
#endif

#if defined(ENABLE_AVX512)
int Graph::IntersectNeighborSetsAVX512(int u, int v, int min_cn_num) {
    int cn = 2; // count for self and v, count for self and u
    int du = out_edge_start[u + 1] - out_edge_start[u] + 2, dv =
            out_edge_start[v + 1] - out_edge_start[v] + 2; // count for self and v, count for self and u

    auto offset_nei_u = out_edge_start[u], offset_nei_v = out_edge_start[v];

    // correctness guaranteed by two pruning previously in computing min_cn
    constexpr int parallelism = 16;
    while (true) {
        // avx512(knl), out_edges[offset_nei_v] as the pivot
        while (offset_nei_u + parallelism < out_edge_start[u + 1]) {
            __m512i pivot = _mm512_set1_epi32(out_edges[offset_nei_v]);
            __m512i inspected_ele = _mm512_loadu_si512(reinterpret_cast<const __m512i *>(&out_edges[offset_nei_u]));
            __mmask16 cmp_res = _mm512_cmpgt_epi32_mask(pivot, inspected_ele);
            auto count = _mm_popcnt_u32(cmp_res);

            // update offset_nei_u and du
            offset_nei_u += count;
            du -= count;
            if (du < min_cn_num) {
                return NOT_SIMILAR;
            }
            if (count < parallelism) {
                break;
            }
        }
        if (offset_nei_u + parallelism >= out_edge_start[u + 1]) {
            break;
        }

        // avx512(knl), out_edges[offset_nei_u] as the pivot
        while (offset_nei_v + parallelism < out_edge_start[v + 1]) {
            __m512i pivot = _mm512_set1_epi32(out_edges[offset_nei_u]);
            __m512i inspected_ele = _mm512_loadu_si512(reinterpret_cast<const __m512i *>(&out_edges[offset_nei_v]));
            __mmask16 cmp_res = _mm512_cmpgt_epi32_mask(pivot, inspected_ele);
            auto count = _mm_popcnt_u32(cmp_res);

            // update offset_nei_u and du
            offset_nei_v += count;
            dv -= count;
            if (dv < min_cn_num) {
                return NOT_SIMILAR;
            }
            if (count < parallelism) {
                break;
            }
        }
        if (offset_nei_v + parallelism >= out_edge_start[v + 1]) {
            break;
        }

        // find possible equality
        if (out_edges[offset_nei_u] == out_edges[offset_nei_v]) {
            ++cn;
            if (cn >= min_cn_num) {
                return SIMILAR;
            }
            ++offset_nei_u;
            ++offset_nei_v;
        }
    }

    while (true) {
        // left ones
        while (out_edges[offset_nei_u] < out_edges[offset_nei_v]) {
            --du;
            if (du < min_cn_num) { return NOT_SIMILAR; }
            ++offset_nei_u;
        }
        while (out_edges[offset_nei_u] > out_edges[offset_nei_v]) {
            --dv;
            if (dv < min_cn_num) { return NOT_SIMILAR; }
            ++offset_nei_v;
        }
        if (out_edges[offset_nei_u] == out_edges[offset_nei_v]) {
            ++cn;
            if (cn >= min_cn_num) {
                return SIMILAR;
            }
            ++offset_nei_u;
            ++offset_nei_v;
        }
    }
}
#endif

#if defined(ENABLE_AVX512_NO_DU_DV)
int Graph::IntersectNeighborSetsAVX512NoDuDv(int u, int v, int min_cn_num) {
    int cn_count = 2; // count for self and v, count for self and u

    auto off_nei_u = out_edge_start[u], off_nei_v = out_edge_start[v];
    auto off_u_end = out_edge_start[u + 1], off_v_end = out_edge_start[v + 1];
    // correctness guaranteed by two pruning previously in computing min_cn
    constexpr int parallelism = 16;
    while (true) {
        // avx512(knl), out_edges[offset_nei_v] as the pivot
        while (off_nei_u + parallelism < off_u_end) {
            __m512i pivot = _mm512_set1_epi32(out_edges[off_nei_v]);
            __m512i inspected_ele = _mm512_loadu_si512(reinterpret_cast<const __m512i *>(&out_edges[off_nei_u]));
            __mmask16 cmp_res = _mm512_cmpgt_epi32_mask(pivot, inspected_ele);
            auto count = _mm_popcnt_u32(cmp_res);

            // update offset_nei_u and du
            off_nei_u += count;
            if (count < parallelism) {
                break;
            }
        }
        if (off_nei_u + parallelism >= out_edge_start[u + 1]) {
            break;
        }

        // avx512(knl), out_edges[offset_nei_u] as the pivot
        while (off_nei_v + parallelism < off_v_end) {
            __m512i pivot = _mm512_set1_epi32(out_edges[off_nei_u]);
            __m512i inspected_ele = _mm512_loadu_si512(reinterpret_cast<const __m512i *>(&out_edges[off_nei_v]));
            __mmask16 cmp_res = _mm512_cmpgt_epi32_mask(pivot, inspected_ele);
            auto count = _mm_popcnt_u32(cmp_res);

            // update offset_nei_u and du
            off_nei_v += count;
            if (count < parallelism) {
                break;
            }
        }
        if (off_nei_v + parallelism >= out_edge_start[v + 1]) {
            break;
        }

        // find possible equality
        if (out_edges[off_nei_u] == out_edges[off_nei_v]) {
            ++cn_count;
            if (cn_count >= min_cn_num) {
                return SIMILAR;
            }
            ++off_nei_u;
            ++off_nei_v;
        }
    }

    if (off_nei_u < off_u_end && off_nei_v < off_v_end) {
        while (true) {
            while (out_edges[off_nei_u] < out_edges[off_nei_v]) {
                ++off_nei_u;
                if (off_nei_u >= off_u_end) {
                    return cn_count >= min_cn_num ? SIMILAR : NOT_SIMILAR;
                }
            }
            while (out_edges[off_nei_u] > out_edges[off_nei_v]) {
                ++off_nei_v;
                if (off_nei_v >= off_v_end) {
                    return cn_count >= min_cn_num ? SIMILAR : NOT_SIMILAR;
                }
            }
            if (out_edges[off_nei_u] == out_edges[off_nei_v]) {
                cn_count++;
                ++off_nei_u;
                ++off_nei_v;
                if (off_nei_u >= off_u_end || off_nei_v >= off_v_end) {
                    return cn_count >= min_cn_num ? SIMILAR : NOT_SIMILAR;
                }
            }
        }
    }
}
#endif

#if defined(ENABLE_AVX512_MERGE)
int Graph::IntersectNeighborSetsAVX512MergePopCnt(int u, int v, int min_cn_num) {
    if (degree[u] > degree[v]) {
        auto tmp = u;
        u = v;
        v = tmp;
    }
    auto off_nei_u = out_edge_start[u], off_nei_v = out_edge_start[v];
    auto off_u_end = out_edge_start[u + 1], off_v_end = out_edge_start[v + 1];

    auto cn_count = 2;
    constexpr int parallelism = 16;
    __m512i st = _mm512_set_epi32(3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0);

    auto size1 = (off_v_end - off_nei_v) / (off_u_end - off_nei_u);
    if (size1 > 2) {
        if (off_nei_u < off_u_end && off_nei_v + 15 < off_v_end) {
            __m512i u_elements = _mm512_set1_epi32(out_edges[off_nei_u]);
            __m512i v_elements = _mm512_loadu_si512((__m512i *) (&out_edges[off_nei_v]));

            while (true) {
                __mmask16 mask = _mm512_cmpeq_epi32_mask(u_elements, v_elements);
                cn_count += _mm_popcnt_u32(mask);

                if (out_edges[off_nei_u] > out_edges[off_nei_v + 15]) {
                    off_nei_v += 16;
                    if (off_nei_v + 15 >= off_v_end) {
                        break;
                    }
                    v_elements = _mm512_loadu_si512((__m512i *) (&out_edges[off_nei_v]));
                } else {
                    off_nei_u++;
                    if (off_nei_u >= off_u_end) {
                        break;
                    }
                    u_elements = _mm512_set1_epi32(out_edges[off_nei_u]);
                }
            }
        }
    } else {
        if (off_nei_u + 3 < off_u_end && off_nei_v + 3 < off_v_end) {
            __m512i u_elements = _mm512_loadu_si512((__m512i *) (&out_edges[off_nei_u]));
            __m512i u_elements_per = _mm512_permutevar_epi32(st, u_elements);
            __m512i v_elements = _mm512_loadu_si512((__m512i *) (&out_edges[off_nei_v]));
            __m512i v_elements_per = _mm512_permute4f128_epi32(v_elements, 0b00000000);

            while (true) {
                __mmask16 mask = _mm512_cmpeq_epi32_mask(u_elements_per, v_elements_per);
                cn_count += _mm_popcnt_u32(mask);

                if (out_edges[off_nei_u + 3] > out_edges[off_nei_v + 3]) {
                    off_nei_v += 4;
                    if (off_nei_v + 3 >= off_v_end) {
                        break;
                    }
                    v_elements = _mm512_loadu_si512((__m512i *) (&out_edges[off_nei_v]));
                    v_elements_per = _mm512_permute4f128_epi32(v_elements, 0b00000000);
                } else if (out_edges[off_nei_u + 3] < out_edges[off_nei_v + 3]) {
                    off_nei_u += 4;
                    if (off_nei_u + 3 >= off_u_end) {
                        break;
                    }
                    u_elements = _mm512_loadu_si512((__m512i *) (&out_edges[off_nei_u]));
                    u_elements_per = _mm512_permutevar_epi32(st, u_elements);
                } else {
                    off_nei_u += 4;
                    off_nei_v += 4;
                    if (off_nei_u + 3 >= off_u_end || off_nei_v + 3 >= off_v_end) {
                        break;
                    }
                    u_elements = _mm512_loadu_si512((__m512i *) (&out_edges[off_nei_u]));
                    u_elements_per = _mm512_permutevar_epi32(st, u_elements);
                    v_elements = _mm512_loadu_si512((__m512i *) (&out_edges[off_nei_v]));
                    v_elements_per = _mm512_permute4f128_epi32(v_elements, 0b00000000);
                }
            }
        }
    }
    if (off_nei_u < off_u_end && off_nei_v < off_v_end) {
        while (true) {
            while (out_edges[off_nei_u] < out_edges[off_nei_v]) {
                ++off_nei_u;
                if (off_nei_u >= off_u_end) {
                    return cn_count >= min_cn_num ? SIMILAR : NOT_SIMILAR;
                }
            }
            while (out_edges[off_nei_u] > out_edges[off_nei_v]) {
                ++off_nei_v;
                if (off_nei_v >= off_v_end) {
                    return cn_count >= min_cn_num ? SIMILAR : NOT_SIMILAR;
                }
            }
            if (out_edges[off_nei_u] == out_edges[off_nei_v]) {
                cn_count++;
                ++off_nei_u;
                ++off_nei_v;
                if (off_nei_u >= off_u_end || off_nei_v >= off_v_end) {
                    return cn_count >= min_cn_num ? SIMILAR : NOT_SIMILAR;
                }
            }
        }
    }
    return cn_count >= min_cn_num ? SIMILAR : NOT_SIMILAR;
}
#endif

int Graph::IntersectNeighborSets(int u, int v, int min_cn_num) {
    int cn = 2; // count for self and v, count for self and u
    int du = out_edge_start[u + 1] - out_edge_start[u] + 2, dv =
            out_edge_start[v + 1] - out_edge_start[v] + 2; // count for self and v, count for self and u

    auto offset_nei_u = out_edge_start[u], offset_nei_v = out_edge_start[v];

    // correctness guaranteed by two pruning previously in computing min_cn
    while (true) {
        while (out_edges[offset_nei_u] < out_edges[offset_nei_v]) {
            --du;
            if (du < min_cn_num) { return NOT_SIMILAR; }
            ++offset_nei_u;
        }
        while (out_edges[offset_nei_u] > out_edges[offset_nei_v]) {
            --dv;
            if (dv < min_cn_num) { return NOT_SIMILAR; }
            ++offset_nei_v;
        }
        if (out_edges[offset_nei_u] == out_edges[offset_nei_v]) {
            ++cn;
            if (cn >= min_cn_num) {
                return SIMILAR;
            }
            ++offset_nei_u;
            ++offset_nei_v;
        }
    }
}

int Graph::EvalSimilarity(int u, ui edge_idx) {
    int v = out_edges[edge_idx];
#if defined(ENABLE_AVX512)
    return IntersectNeighborSetsAVX512(u, v, min_cn[edge_idx]);
#elif defined(ENABLE_AVX512_NO_DU_DV)
    return IntersectNeighborSetsAVX512NoDuDv(u, v, min_cn[edge_idx]);
#elif defined(ENABLE_AVX512_MERGE)
    return IntersectNeighborSetsAVX512MergePopCnt(u, v, min_cn[edge_idx]);
#elif defined(ENABLE_AVX2)
    return IntersectNeighborSetsAVX2(u, v, min_cn[edge_idx]);
#elif defined(ENABLE_AVX2_MERGE)
    return IntersectNeighborSetsAVX2MergePopCnt(u, v, min_cn[edge_idx]);
#elif defined(ENABLE_SSE)
    return IntersectNeighborSetsSSE(u, v, min_cn[edge_idx]);
#else
    return IntersectNeighborSets(u, v, min_cn[edge_idx]);
#endif
}