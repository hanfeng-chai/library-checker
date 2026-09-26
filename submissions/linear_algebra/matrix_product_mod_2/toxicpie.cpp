// I might have found the most cursed optimization technique in all of
// competitive programming.
//                                               -- toxicpie, 2024-10-16

#pragma GCC optimize("O3,unroll-loops,rename-registers")

#include <immintrin.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

constexpr size_t N = 4096;
constexpr size_t LEN = N / 256;
constexpr size_t BLOCK_AR = 12;
constexpr size_t BLOCK_AC = 256;

__m256i A[N + BLOCK_AR][LEN + 1];
[[gnu::aligned(64)]] __m256i B[LEN][N + 74];
__m256i C[N + BLOCK_AR][LEN + 1];

void read_input(size_t n, size_t m, size_t k) {
    struct stat fs;
    fstat(0, &fs);
    char *stdin = (char *)mmap(NULL, fs.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
    const char *cur = stdin + snprintf(NULL, 0, "%lu %lu %lu\n", n, m, k);
    __m256i one = _mm256_set1_epi8('1');
    for (size_t i = 0, j; i < n; i++, cur += m + 1) {
        for (j = 0; j + 32 <= m; j += 32) {
            __m256i str = _mm256_loadu_si256((__m256i *)&cur[j]);
            ((u32 *)A[i])[j / 32] =
                _mm256_movemask_epi8(_mm256_cmpeq_epi8(str, one));
        }
        for (; j < m; j++) {
            ((u32 *)A[i])[j / 32] |= (u32)(cur[j] == '1') << j % 32;
        }
    }
    for (size_t i = 0, j; i < m; i++, cur += k + 1) {
        for (j = 0; j + 32 <= k; j += 32) {
            __m256i str = _mm256_loadu_si256((__m256i *)&cur[j]);
            ((u32 *)B[j / 256])[i * 8 + j / 32 % 8] =
                _mm256_movemask_epi8(_mm256_cmpeq_epi8(str, one));
        }
        for (; j < k; j++) {
            ((u32 *)B[j / 256])[i * 8 + j / 32 % 8] |= (u32)(cur[j] == '1')
                                                       << j % 32;
        }
    }
}

[[gnu::always_inline]] inline __m256i _mm256_inv_movemask_epi8(u32 mask) {
    __m256i shuffle_select =
        _mm256_setr_epi64x(0x0000000000000000, 0x0101010101010101,
                           0x0202020202020202, 0x0303030303030303);
    __m256i or_mask = _mm256_set1_epi64x(0x7fbfdfeff7fbfdfe);
    __m256i ones = _mm256_set1_epi64x(-1);
    __m256i result = _mm256_set1_epi32(mask);
    result = _mm256_shuffle_epi8(result, shuffle_select) | or_mask;
    return _mm256_cmpeq_epi8(result, ones);
}

void write_output(size_t n, size_t k) {
    char *buf = (char *)aligned_alloc(32, k + 1);
    buf[k] = '\n';
    __m256i *str256 = (__m256i *)buf;
    __m256i zeroes = _mm256_set1_epi8('0');
    __m256i ones = _mm256_set1_epi8('1');
    for (size_t i = 0, j; i < n; i++) {
        const u32 *src32 = (u32 *)C[i];
        for (j = 0; j + 32 <= k; j += 32) {
            __m256i mask = _mm256_inv_movemask_epi8(src32[j / 32]);
            _mm256_store_si256(&str256[j / 32],
                               _mm256_blendv_epi8(zeroes, ones, mask));
        }
        for (; j < k; j++) {
            buf[j] = (src32[j / 32] >> j % 32) & 1 ? '1' : '0';
        }
        write(1, buf, k + 1);
    }
    free(buf);
}

typedef void (*code_ptr)(void *, void *);

constexpr size_t KERNEL_1_LEN = 128;
constexpr size_t KERNEL_2_LEN = 64;
constexpr size_t KERNEL_3_LEN = 93;

[[gnu::aligned(64)]] __m256i kernel_2_cache[KERNEL_2_LEN / 32];

[[gnu::naked]] void kernel_1() {
    __asm__(R"(
.intel_syntax noprefix
    vmovdqa ymm4,   YMMWORD PTR [rdi + 0x0000]
    vmovdqa ymm5,   YMMWORD PTR [rdi + 0x0220]
    vmovdqa ymm6,   YMMWORD PTR [rdi + 0x0440]
    vmovdqa ymm7,   YMMWORD PTR [rdi + 0x0660]
    vmovdqa ymm8,   YMMWORD PTR [rdi + 0x0880]
    vmovdqa ymm9,   YMMWORD PTR [rdi + 0x0aa0]
    vmovdqa ymm10,  YMMWORD PTR [rdi + 0x0cc0]
    vmovdqa ymm11,  YMMWORD PTR [rdi + 0x0ee0]
    vmovdqa ymm12,  YMMWORD PTR [rdi + 0x1100]
    vmovdqa ymm13,  YMMWORD PTR [rdi + 0x1320]
    vmovdqa ymm14,  YMMWORD PTR [rdi + 0x1540]
    vmovdqa ymm15,  YMMWORD PTR [rdi + 0x1760]
    mov     rdx,    0x20
    mov     rcx,    0x40
    vpxor   ymm0,   ymm0,   ymm0
    .nops 18
.att_syntax
    )");
}

[[gnu::naked]] void kernel_2() {
    __asm__(R"(
.intel_syntax noprefix
    vmovdqa ymm1,   YMMWORD PTR [rsi]
    vmovdqa ymm2,   YMMWORD PTR [rsi + rdx]
    vpxor   ymm3,   ymm1,   ymm2
    add     rsi,    rcx
    vpxor   ymm4,   ymm4,   ymm0
    vpxor   ymm5,   ymm5,   ymm0
    vpxor   ymm6,   ymm6,   ymm0
    vpxor   ymm7,   ymm7,   ymm0
    vpxor   ymm8,   ymm8,   ymm0
    vpxor   ymm9,   ymm9,   ymm0
    vpxor   ymm10,  ymm10,  ymm0
    vpxor   ymm11,  ymm11,  ymm0
    vpxor   ymm12,  ymm12,  ymm0
    vpxor   ymm13,  ymm13,  ymm0
    vpxor   ymm14,  ymm14,  ymm0
    vpxor   ymm15,  ymm15,  ymm0
.att_syntax
    )");
}

[[gnu::naked]] void kernel_3() {
    __asm__(R"(
.intel_syntax noprefix
    vmovdqa YMMWORD PTR [rdi + 0x0000], ymm4
    vmovdqa YMMWORD PTR [rdi + 0x0220], ymm5
    vmovdqa YMMWORD PTR [rdi + 0x0440], ymm6
    vmovdqa YMMWORD PTR [rdi + 0x0660], ymm7
    vmovdqa YMMWORD PTR [rdi + 0x0880], ymm8
    vmovdqa YMMWORD PTR [rdi + 0x0aa0], ymm9
    vmovdqa YMMWORD PTR [rdi + 0x0cc0], ymm10
    vmovdqa YMMWORD PTR [rdi + 0x0ee0], ymm11
    vmovdqa YMMWORD PTR [rdi + 0x1100], ymm12
    vmovdqa YMMWORD PTR [rdi + 0x1320], ymm13
    vmovdqa YMMWORD PTR [rdi + 0x1540], ymm14
    vmovdqa YMMWORD PTR [rdi + 0x1760], ymm15
    ret
.att_syntax
    )");
}

code_ptr jit_init() {
    code_ptr code =
        (code_ptr)mmap(NULL, 0x80000, PROT_READ | PROT_WRITE | PROT_EXEC,
                       MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    u8 *write_ptr = (u8 *)code;
    memcpy((void *)write_ptr, (void *)kernel_1, KERNEL_1_LEN);
    memcpy(kernel_2_cache, (void *)kernel_2, KERNEL_2_LEN);
    memcpy((void *)(write_ptr + KERNEL_1_LEN + KERNEL_2_LEN * BLOCK_AC / 2),
           (void *)kernel_3, KERNEL_3_LEN);
    return code;
}

[[gnu::always_inline]] inline __m256i _mm256_expandmask1_2x16(u16 mask) {
    __m256i result = _mm256_set1_epi16(mask);
    __m256i shifts = _mm256_setr_epi32(0, 2, 4, 6, 8, 10, 12, 14);
    return _mm256_srlv_epi32(result, shifts);
}

[[gnu::always_inline]] inline __m256i _mm256_expandmask2_2x16(u16 mask) {
    __m256i result = _mm256_set1_epi16(mask);
    __m256i shifts = _mm256_setr_epi32(24, 22, 20, 18, 16, 14, 12, 10);
    __m256i select = _mm256_set1_epi32(0x03000000);
    return _mm256_sllv_epi32(result, shifts) & select;
}

void jit_compile_kernel(code_ptr code, size_t r, size_t c) {
    [[gnu::aligned(64)]] u32 masks[BLOCK_AC / 2] = {};
    for (size_t i = 0; i < BLOCK_AR; i++) {
        __m256i bits = _mm256_set1_epi32(0x03 << (i * 2 + 8));
        for (size_t j = 0; j < BLOCK_AC; j += 16) {
            u16 num = ((u16 *)A[r + i])[(c + j) / 16];
            ((__m256i *)masks)[j / 16] |=
                bits &
                _mm256_slli_epi32(_mm256_expandmask1_2x16(num), i * 2 + 8);
        }
    }
    for (size_t j = 0; j < BLOCK_AC / 2; j++) {
        __m256i *block_ptr =
            (__m256i *)((u8 *)code + KERNEL_1_LEN + KERNEL_2_LEN * j);
        for (size_t k = 0; k < 2; k++) {
            __m256i bits = _mm256_expandmask2_2x16(masks[j] >> (k * 16));
            __m256i vpxor = kernel_2_cache[k];
            _mm256_store_si256(&block_ptr[k], vpxor | bits);
        }
    }
}

size_t min(size_t x, size_t y) {
    return x < y ? x : y;
}

int main() {
    size_t n, m, k;
    scanf("%lu %lu %lu", &n, &m, &k);
    read_input(n, m, k);

    code_ptr code = jit_init();

#ifdef BENCHMARK_LOOPS
    for (size_t _ = 0; _ < BENCHMARK_LOOPS; _++)
#endif
        for (size_t c = 0; c < N; c += BLOCK_AC) {
            for (size_t r = 0; r < N; r += BLOCK_AR) {
                jit_compile_kernel(code, r, c);
                for (size_t i = 0; i < LEN; i += 1) {
                    code(&C[r][i], &B[i][c]);
                }
            }
        }

    write_output(n, k);
}
