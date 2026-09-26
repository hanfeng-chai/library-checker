#pragma GCC optimize("O3,unroll-loops,rename-registers")

#include <immintrin.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;

#define N         (4096)
#define NWORDS    (N / 256)
#define BLOCKSIZE (32)

u16 firsts[N];
__m256i zeroes[NWORDS];
__m256i vecs[N][NWORDS];

void parse_vector(__m256i *dst, const char *src, size_t n) {
    u32 *d32 = (u32 *)dst;
    __m256i one = _mm256_set1_epi8('1');
    size_t i = 0;
    for (; i + 32 <= n; i += 32) {
        __m256i str = _mm256_loadu_si256((__m256i *)&src[i]);
        d32[i / 32] = _mm256_movemask_epi8(_mm256_cmpeq_epi8(str, one));
    }
    for (; i < n; i++) {
        d32[i / 32] |= (u32)(src[i] == '1') << i % 32;
    }
}

void read_input(size_t n) {
    struct stat fs;
    fstat(0, &fs);
    char *stdin = (char *)mmap(NULL, fs.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
    size_t off = snprintf(NULL, 0, "%lu", n) + 1;
    for (size_t i = 0; i < n; i++) {
        parse_vector(vecs[i], stdin + off + (n + 1) * i, n);
    }
    munmap(stdin, fs.st_size);
}

void xor_from(__m256i *dst, const __m256i *src, size_t start) {
    for (size_t i = start / 256; i < NWORDS; i++) {
        dst[i] ^= src[i];
    }
}

size_t find_first(__m256i *vec) {
    u64 *v64 = (u64 *)vec;
    for (size_t i = 0; i < N / 64; i++) {
        if (v64[i] != 0) {
            return i * 64 + _tzcnt_u64(v64[i]);
        }
    }
    return ~(size_t)0;
}

int get_ith(const __m256i *vec, size_t idx) {
    return (((const u64 *)(vec))[idx / 64] >> idx % 64) & 1;
}

void set_ith(__m256i *vec, size_t idx) {
    ((u64 *)(vec))[idx / 64] |= (u64)1 << idx % 64;
}

int main() {
    size_t n;
    scanf("%lu", &n);
    read_input(n);

    // i'm too lazy to deal with weird sizes.
    // this doesn't change the answer or worst runtime
    for (; n % BLOCKSIZE != 0; n++) {
        set_ith(vecs[n], n);
    }

#ifdef BENCHMARK_LOOPS
    for (size_t _ = 0; _ < BENCHMARK_LOOPS; _++)
#endif
        for (size_t il = 0; il < n; il += BLOCKSIZE) {
            for (size_t jl = 0; jl < il; jl += BLOCKSIZE) {
                for (size_t j = jl; j < jl + BLOCKSIZE; j++) {
                    for (size_t i = il; i < il + BLOCKSIZE; i++) {
                        __m256i *src =
                            get_ith(vecs[i], firsts[j]) ? vecs[j] : zeroes;
                        xor_from(vecs[i], src, firsts[j]);
                    }
                }
            }
            for (size_t i = il; i < il + BLOCKSIZE; i++) {
                for (size_t j = il; j < i; j++) {
                    __m256i *src =
                        get_ith(vecs[i], firsts[j]) ? vecs[j] : zeroes;
                    xor_from(vecs[i], src, firsts[j]);
                }
                size_t first_bit = find_first(vecs[i]);
                if (first_bit == ~(size_t)0) {
                    puts("0");
                    return 0;
                }
                firsts[i] = first_bit;
            }
        }
    puts("1");
}
