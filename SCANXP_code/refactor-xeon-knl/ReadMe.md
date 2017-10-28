## AVX Instructions

Set packed 32-bit integers in dst with the supplied values.

```c
__m256i _mm256_set_epi32 (int e7, int e6, int e5, int e4, int e3, int e2, int e1, int e0)
```

Load 256-bits of integer data from memory into dst. mem_addr does not need to be aligned on any particular boundary.

```c
__m256i _mm256_loadu_si256 (__m256i const * mem_addr)
```

Load 256-bits of integer data from memory into dst. mem_addr must be aligned on a 32-byte boundary or a general-protection exception may be generated.

```c
__m256i _mm256_load_si256 (__m256i const * mem_addr)
```

Store 256-bits of integer data from a into memory. mem_addr must be aligned on a 32-byte boundary or a general-protection exception may be generated.

```c
void _mm256_store_si256 (__m256i * mem_addr, __m256i a)
```

## AVX2 Instructions

Compare packed 32-bit integers in a and b for equality, and store the results in dst.

```c
__m256i _mm256_cmpeq_epi32 (__m256i a, __m256i b)
```

Compute the bitwise AND of 256 bits (representing integer data) in a and b, and store the result in dst.

```c
__m256i _mm256_and_si256 (__m256i a, __m256i b)
```

Add packed 32-bit integers in a and b, and store the results in dst.

```c
__m256i _mm256_add_epi32 (__m256i a, __m256i b)
```

Shuffle 32-bit integers in a across lanes using the corresponding index in idx, and store the results in dst.

```c
__m256i _mm256_permutevar8x32_epi32 (__m256i a, __m256i idx)
```