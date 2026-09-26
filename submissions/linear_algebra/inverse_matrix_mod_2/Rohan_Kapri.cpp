#pragma GCC optimize("O3,unroll-loops,rename-registers")

#include <immintrin.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

#define N         (4096)
#define LEN       (N / 256 * 2)
#define BLOCKSIZE (16)

u16 firsts[N], invfirsts[N];
__m256i zeroes[LEN];
__m256i vecs[N][LEN];

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

void print_bitset(char *dst, const __m256i *src, size_t n) {
    const u8 *src8 = (u8 *)src;
    u64 *str64 = (u64 *)dst;
    size_t i = 0;
    for (; i + 8 <= n; i += 8) {
        str64[i / 8] = 0x3030303030303030ULL |
                       _pdep_u64(src8[i / 8], 0x0101010101010101ULL);
    }
    for (; i < n; i++) {
        dst[i] = (src8[i / 8] >> i % 8) & 1 ? '1' : '0';
    }
}

void xor_from(__m256i *dst, const __m256i *src, size_t start) {
    for (size_t i = start / 256; i < LEN; i++) {
        dst[i] ^= src[i];
    }
}

size_t find_first_before(__m256i *vec, size_t n) {
    u64 *v64 = (u64 *)vec;
    for (size_t i = 0; i < (n + 63) / 64; i++) {
        if (v64[i] != 0) {
            size_t ans = i * 64 + _tzcnt_u64(v64[i]);
            return ans < n ? ans : ~(size_t)0;
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
    size_t orig_n = n;

    // i'm too lazy to deal with weird sizes.
    // this doesn't change the answer or worst runtime
    for (; n % BLOCKSIZE != 0; n++) {
        set_ith(vecs[n], n);
    }

    for (size_t i = 0; i < n; i++) {
        set_ith(vecs[i], i + n);
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
                size_t first_bit = find_first_before(vecs[i], n);
                if (first_bit == ~(size_t)0) {
                    puts("-1");
                    return 0;
                }
                firsts[i] = first_bit;
            }
        }

#ifdef BENCHMARK_LOOPS
    for (size_t _ = 0; _ < BENCHMARK_LOOPS; _++)
#endif
        for (size_t jl = 0; jl < n; jl += BLOCKSIZE) {
            for (size_t il = jl + BLOCKSIZE; il < n; il += BLOCKSIZE) {
                for (size_t i = il; i < il + BLOCKSIZE; i++) {
                    for (size_t j = jl; j < jl + BLOCKSIZE; j++) {
                        __m256i *src =
                            get_ith(vecs[j], firsts[i]) ? vecs[i] : zeroes;
                        xor_from(vecs[j], src, firsts[i]);
                    }
                }
            }
            for (size_t j = jl; j < jl + BLOCKSIZE; j++) {
                for (size_t i = j + 1; i < jl + BLOCKSIZE; i++) {
                    __m256i *src =
                        get_ith(vecs[j], firsts[i]) ? vecs[i] : zeroes;
                    xor_from(vecs[j], src, firsts[i]);
                }
            }
        }

    for (size_t i = 0; i < n; i++) {
        invfirsts[firsts[i]] = i;
    }

    char *ans = (char *)aligned_alloc(64, n + orig_n + 1);
    ans[n + orig_n] = '\0';
    for (size_t i = 0; i < orig_n; i++) {
        print_bitset(ans + n / 256 * 256, vecs[invfirsts[i]] + n / 256,
                     orig_n + n % 256);
        puts(ans + n);
    }
    free(ans);
}
