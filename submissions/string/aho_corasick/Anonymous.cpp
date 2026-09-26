#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt,sse4.2")

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <immintrin.h>


#define FASTIO_ALWAYS_INLINE static inline __attribute__((always_inline))
#define FASTIO_OUT_BUF_SIZE (1u << 20)
#define FASTIO_MAX_INT_DIGITS 32

typedef struct FastInput {
    const char *base;
    const char *ptr;
    const char *end;
    size_t map_len;
    int is_mapped;
} FastInput;

typedef struct FastOutput {
    int fd;
    char *buf;
    size_t cap;
    size_t len;
} FastOutput;

typedef struct FastIO {
    FastInput in;
    FastOutput out;
} FastIO;

static inline void fastio__write_all(int fd, const char *__restrict__ buf, size_t len) {
    size_t off = 0;
    while (off < len) {
        ssize_t w = write(fd, buf + off, len - off);
        if (w < 0) {
            if (errno == EINTR) continue;
            _exit(1);
        }
        off += (size_t)w;
    }
}

static inline void fastio__read_all_fallback(FastInput *in, int fd) {
    size_t cap = 1u << 20;
    size_t len = 0;
    char  *buf = (char *)malloc(cap);
    for (;;) {
        if (len == cap) {
            cap <<= 1;
            buf = (char *)realloc(buf, cap);
        }
        ssize_t r = read(fd, buf + len, cap - len);
        if (r < 0) {
            if (errno == EINTR) continue;
            _exit(1);
        }
        if (r == 0) break;
        len += (size_t)r;
    }
    in->base = buf;
    in->ptr = buf;
    in->end = buf + len;
    in->map_len = 0;
    in->is_mapped = 0;
}

static inline void fastio_init(FastIO *io, int in_fd, int out_fd) {
    struct stat st;
    if (fstat(in_fd, &st) == 0 && S_ISREG(st.st_mode) && st.st_size > 0) {
        void *p = mmap(NULL, (size_t)st.st_size, PROT_READ, MAP_PRIVATE, in_fd, 0);
        if (p != MAP_FAILED) {
            io->in.base = (const char *)p;
            io->in.ptr = io->in.base;
            io->in.end = io->in.base + (size_t)st.st_size;
            io->in.map_len = (size_t)st.st_size;
            io->in.is_mapped = 1;
        } else {
            fastio__read_all_fallback(&io->in, in_fd);
        }
    } else if (fstat(in_fd, &st) == 0 && S_ISREG(st.st_mode) && st.st_size == 0) {
        io->in.base = NULL;
        io->in.ptr = NULL;
        io->in.end = NULL;
        io->in.map_len = 0;
        io->in.is_mapped = 0;
    } else {
        fastio__read_all_fallback(&io->in, in_fd);
    }

    io->out.fd = out_fd;
    io->out.cap = FASTIO_OUT_BUF_SIZE;
    io->out.buf = (char *)aligned_alloc(64, io->out.cap);
    io->out.len = 0;
}

static inline void fastio_flush(FastIO *io) {
    if (io->out.len > 0) {
        fastio__write_all(io->out.fd, io->out.buf, io->out.len);
        io->out.len = 0;
    }
}

static inline void fastio_destroy(FastIO *io) {
    fastio_flush(io);
    free(io->out.buf);
    io->out.buf = NULL;
    if (io->in.base) {
        if (io->in.is_mapped) {
            munmap((void *)io->in.base, io->in.map_len);
        } else {
            free((void *)io->in.base);
        }
    }
    io->in.base = io->in.ptr = io->in.end = NULL;
}

static inline void fastio__skip_ws(FastInput *__restrict__ in) {
    const char *p = in->ptr;
    const char *end = in->end;

    const __m256i sp = _mm256_set1_epi8(' ');
    const __m256i nl = _mm256_set1_epi8('\n');
    const __m256i cr = _mm256_set1_epi8('\r');
    const __m256i tb = _mm256_set1_epi8('\t');

    while (p + 32 <= end) {
        __m256i v = _mm256_loadu_si256((const __m256i *)p);
        __m256i is_ws = _mm256_or_si256(
            _mm256_or_si256(_mm256_cmpeq_epi8(v, sp), _mm256_cmpeq_epi8(v, nl)),
            _mm256_or_si256(_mm256_cmpeq_epi8(v, cr), _mm256_cmpeq_epi8(v, tb)));
        unsigned mask = (unsigned)_mm256_movemask_epi8(is_ws);
        if (mask != 0xFFFFFFFFu) {
            unsigned non_ws = ~mask;
            p += __builtin_ctz(non_ws);
            in->ptr = p;
            return;
        }
        p += 32;
    }
    while (p < end) {
        char c = *p;
        if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
            ++p;
        } else {
            break;
        }
    }
    in->ptr = p;
}

static inline uint32_t fastio__parse8(uint64_t val) {
    const uint64_t mask = 0x000000FF000000FFULL;
    const uint64_t mul1 = 0x000F424000000064ULL;
    const uint64_t mul2 = 0x0000271000000001ULL;
    val -= 0x3030303030303030ULL;
    val = (val * 10) + (val >> 8);
    val = (((val & mask) * mul1) + (((val >> 16) & mask) * mul2)) >> 32;
    return (uint32_t)val;
}

static inline uint64_t fastio__parse_udigits(FastInput *__restrict__ in) {
    const char *p = in->ptr;
    const char *end = in->end;
    uint64_t acc = 0;

    const __m256i lo_bound = _mm256_set1_epi8('0' - 1);
    const __m256i hi_bound = _mm256_set1_epi8('9' + 1);

    while (p + 32 <= end) {
        __m256i v = _mm256_loadu_si256((const __m256i *)p);
        __m256i ge = _mm256_cmpgt_epi8(v, lo_bound);
        __m256i le = _mm256_cmpgt_epi8(hi_bound, v);
        __m256i is_digit = _mm256_and_si256(ge, le);
        unsigned mask = (unsigned)_mm256_movemask_epi8(is_digit);

        if (mask == 0xFFFFFFFFu) {
            for (int k = 0; k < 32; k += 8) {
                uint64_t chunk;
                memcpy(&chunk, p + k, 8);
                acc = acc * 100000000ULL + fastio__parse8(chunk);
            }
            p += 32;
            continue;
        }

        unsigned non_digit = ~mask;
        int run = __builtin_ctz(non_digit);
        int k = 0;
        for (; k + 8 <= run; k += 8) {
            uint64_t chunk;
            memcpy(&chunk, p + k, 8);
            acc = acc * 100000000ULL + fastio__parse8(chunk);
        }
        for (; k < run; ++k) {
            acc = acc * 10 + (uint64_t)(unsigned char)(p[k] - '0');
        }
        p += run;
        in->ptr = p;
        return acc;
    }

    while (p < end) {
        unsigned char c = (unsigned char)*p;
        if (c < '0' || c > '9') break;
        acc = acc * 10 + (uint64_t)(c - '0');
        ++p;
    }
    in->ptr = p;
    return acc;
}

static inline size_t fastio__scan_token_len(const char *__restrict__ p, const char *__restrict__ end) {
    const char *start = p;
    const __m256i sp = _mm256_set1_epi8(' ');
    const __m256i nl = _mm256_set1_epi8('\n');
    const __m256i cr = _mm256_set1_epi8('\r');
    const __m256i tb = _mm256_set1_epi8('\t');

    while (p + 32 <= end) {
        __m256i v = _mm256_loadu_si256((const __m256i *)p);
        __m256i is_ws = _mm256_or_si256(
            _mm256_or_si256(_mm256_cmpeq_epi8(v, sp), _mm256_cmpeq_epi8(v, nl)),
            _mm256_or_si256(_mm256_cmpeq_epi8(v, cr), _mm256_cmpeq_epi8(v, tb)));
        unsigned mask = (unsigned)_mm256_movemask_epi8(is_ws);
        if (mask != 0) {
            return (size_t)(p - start) + (size_t)__builtin_ctz(mask);
        }
        p += 32;
    }
    while (p < end) {
        char c = *p;
        if (c == ' ' || c == '\n' || c == '\r' || c == '\t') break;
        ++p;
    }
    return (size_t)(p - start);
}

static inline int64_t fastio_read_i64(FastIO *io) {
    fastio__skip_ws(&io->in);
    int neg = 0;
    if (io->in.ptr < io->in.end && *io->in.ptr == '-') {
        neg = 1;
        ++io->in.ptr;
    }
    uint64_t u = fastio__parse_udigits(&io->in);
    if (!neg) return (int64_t)u;
    return -(int64_t)(u - 1) - 1;
}

static inline int32_t fastio_read_i32(FastIO *io) {
    return (int32_t)fastio_read_i64(io);
}

static inline uint64_t fastio_read_u64(FastIO *io) {
    fastio__skip_ws(&io->in);
    return fastio__parse_udigits(&io->in);
}

static inline uint32_t fastio_read_u32(FastIO *io) {
    return (uint32_t)fastio_read_u64(io);
}

static inline char fastio_read_char(FastIO *io) {
    fastio__skip_ws(&io->in);
    if (io->in.ptr >= io->in.end) return '\0';
    return *io->in.ptr++;
}

static inline bool fastio_read_bool(FastIO *io) {
    fastio__skip_ws(&io->in);
    if (io->in.ptr >= io->in.end) return false;
    char c = *io->in.ptr++;
    return c != '0';
}

static inline size_t fastio_read_str(FastIO *io, char *dst, size_t dst_cap) {
    fastio__skip_ws(&io->in);
    size_t len = fastio__scan_token_len(io->in.ptr, io->in.end);
    size_t copy_len = (dst_cap > 0 && len >= dst_cap) ? dst_cap - 1 : len;
    if (copy_len > 0) memcpy(dst, io->in.ptr, copy_len);
    if (dst_cap > 0) dst[copy_len] = '\0';
    io->in.ptr += len;
    return copy_len;
}

static inline size_t fastio_read_str_view(FastIO *io, const char **out_ptr) {
    fastio__skip_ws(&io->in);
    size_t len = fastio__scan_token_len(io->in.ptr, io->in.end);
    *out_ptr = io->in.ptr;
    io->in.ptr += len;
    return len;
}

static inline void fastio__ensure(FastIO *__restrict__ io, size_t need) {
    if (__builtin_expect(io->out.len + need > io->out.cap, 0)) {
        fastio_flush(io);
    }
}

static inline void fastio_write_char(FastIO *__restrict__ io, char c) {
    fastio__ensure(io, 1);
    io->out.buf[io->out.len++] = c;
}

static inline void fastio_write_newline(FastIO *io) {
    fastio_write_char(io, '\n');
}

static inline void fastio_write_bool(FastIO *io, bool b) {
    fastio_write_char(io, b ? '1' : '0');
}

FASTIO_ALWAYS_INLINE void fastio__simd_copy(char *__restrict__ dst, const char *__restrict__ src, size_t len) {
    size_t i = 0;
    for (; i + 32 <= len; i += 32) {
        __m256i v = _mm256_loadu_si256((const __m256i *)(src + i));
        _mm256_storeu_si256((__m256i *)(dst + i), v);
    }
    if (i < len) memcpy(dst + i, src + i, len - i);
}

static inline void fastio_write_str(FastIO *__restrict__ io, const char *__restrict__ s, size_t len) {
    if (len >= io->out.cap) {
        fastio_flush(io);
        fastio__write_all(io->out.fd, s, len);
        return;
    }
    fastio__ensure(io, len);
    if (len >= 32) {
        fastio__simd_copy(io->out.buf + io->out.len, s, len);
    } else {
        memcpy(io->out.buf + io->out.len, s, len);
    }
    io->out.len += len;
}

static inline void fastio_write_cstr(FastIO *io, const char *s) {
    fastio_write_str(io, s, strlen(s));
}

static uint32_t *fastio__lut1 = NULL;
static uint32_t *fastio__lut2 = NULL;

__attribute__((constructor))
static void fastio__build_luts(void) {
    fastio__lut1 = (uint32_t *)malloc(10000 * sizeof(uint32_t));
    fastio__lut2 = (uint32_t *)malloc(10000 * sizeof(uint32_t));
    for (int i = 0; i < 10000; ++i) {
        char b1[4];
        b1[0] = (i >= 1000) ? (char)('0' + (i / 1000) % 10) : ' ';
        b1[1] = (i >= 100)  ? (char)('0' + (i / 100) % 10)  : ' ';
        b1[2] = (i >= 10)   ? (char)('0' + (i / 10) % 10)   : ' ';
        b1[3] = (char)('0' + i % 10);
        memcpy(&fastio__lut1[i], b1, 4);

        char b2[4];
        b2[0] = (char)('0' + (i / 1000) % 10);
        b2[1] = (char)('0' + (i / 100) % 10);
        b2[2] = (char)('0' + (i / 10) % 10);
        b2[3] = (char)('0' + i % 10);
        memcpy(&fastio__lut2[i], b2, 4);
    }
}

__attribute__((destructor))
static void fastio__free_luts(void) {
    free(fastio__lut1);
    free(fastio__lut2);
    fastio__lut1 = NULL;
    fastio__lut2 = NULL;
}

FASTIO_ALWAYS_INLINE int fastio__sig4(uint32_t hi) {
    if (hi < 10) return 1;
    if (hi < 100) return 2;
    if (hi < 1000) return 3;
    return 4;
}

FASTIO_ALWAYS_INLINE int fastio__emit_lead_fwd(char *__restrict__ dst, uint32_t hi) {
    int sig = fastio__sig4(hi);
    uint32_t sv = fastio__lut1[hi] >> ((4 - sig) * 8);
    memcpy(dst, &sv, 4);
    return sig;
}

static inline void fastio_write_u32(FastIO *io, uint32_t val) {
    int nblocks;
    if (val < 10000u) nblocks = 1;
    else if (val < 100000000u) nblocks = 2;
    else nblocks = 3;

    fastio__ensure(io, (size_t)nblocks * 4);
    char *dst = io->out.buf + io->out.len;
    int total_len;

    if (nblocks == 1) {
        total_len = fastio__emit_lead_fwd(dst, val);
    } else if (nblocks == 2) {
        uint32_t hi = val / 10000u;
        uint32_t lo = val % 10000u;
        int sig = fastio__emit_lead_fwd(dst, hi);
        memcpy(dst + sig, &fastio__lut2[lo], 4);
        total_len = sig + 4;
    } else {
        uint32_t hi = val / 100000000u;
        uint32_t rem = val % 100000000u;
        uint32_t b1 = rem / 10000u;
        uint32_t b2 = rem % 10000u;
        int sig = fastio__emit_lead_fwd(dst, hi);
        memcpy(dst + sig, &fastio__lut2[b1], 4);
        memcpy(dst + sig + 4, &fastio__lut2[b2], 4);
        total_len = sig + 8;
    }
    io->out.len += (size_t)total_len;
}

static inline void fastio_write_u64(FastIO *io, uint64_t val) {
    int nblocks;
    if (val < 10000ULL) nblocks = 1;
    else if (val < 100000000ULL) nblocks = 2;
    else if (val < 1000000000000ULL) nblocks = 3;
    else if (val < 10000000000000000ULL) nblocks = 4;
    else nblocks = 5;

    fastio__ensure(io, (size_t)nblocks * 4);
    char *dst = io->out.buf + io->out.len;
    int total_len;

    switch (nblocks) {
    case 1: {
        total_len = fastio__emit_lead_fwd(dst, (uint32_t)val);
        break;
    }
    case 2: {
        uint32_t hi = (uint32_t)(val / 10000ULL);
        uint32_t lo = (uint32_t)(val % 10000ULL);
        int sig = fastio__emit_lead_fwd(dst, hi);
        memcpy(dst + sig, &fastio__lut2[lo], 4);
        total_len = sig + 4;
        break;
    }
    case 3: {
        uint64_t hi64 = val / 100000000ULL;
        uint64_t rem = val % 100000000ULL;
        uint32_t b1 = (uint32_t)(rem / 10000ULL);
        uint32_t b2 = (uint32_t)(rem % 10000ULL);
        int sig = fastio__emit_lead_fwd(dst, (uint32_t)hi64);
        memcpy(dst + sig, &fastio__lut2[b1], 4);
        memcpy(dst + sig + 4, &fastio__lut2[b2], 4);
        total_len = sig + 8;
        break;
    }
    case 4: {
        uint64_t hi64 = val / 1000000000000ULL;
        uint64_t rem = val % 1000000000000ULL;
        uint32_t b1 = (uint32_t)(rem / 100000000ULL);
        rem %= 100000000ULL;
        uint32_t b2 = (uint32_t)(rem / 10000ULL);
        uint32_t b3 = (uint32_t)(rem % 10000ULL);
        int sig = fastio__emit_lead_fwd(dst, (uint32_t)hi64);
        memcpy(dst + sig, &fastio__lut2[b1], 4);
        memcpy(dst + sig + 4, &fastio__lut2[b2], 4);
        memcpy(dst + sig + 8, &fastio__lut2[b3], 4);
        total_len = sig + 12;
        break;
    }
    default: {
        uint64_t hi64 = val / 10000000000000000ULL;
        uint64_t rem = val % 10000000000000000ULL;
        uint32_t b1 = (uint32_t)(rem / 1000000000000ULL);
        rem %= 1000000000000ULL;
        uint32_t b2 = (uint32_t)(rem / 100000000ULL);
        rem %= 100000000ULL;
        uint32_t b3 = (uint32_t)(rem / 10000ULL);
        uint32_t b4 = (uint32_t)(rem % 10000ULL);
        int sig = fastio__emit_lead_fwd(dst, (uint32_t)hi64);
        memcpy(dst + sig, &fastio__lut2[b1], 4);
        memcpy(dst + sig + 4, &fastio__lut2[b2], 4);
        memcpy(dst + sig + 8, &fastio__lut2[b3], 4);
        memcpy(dst + sig + 12, &fastio__lut2[b4], 4);
        total_len = sig + 16;
        break;
    }
    }
    io->out.len += (size_t)total_len;
}

static inline void fastio_write_i64(FastIO *io, int64_t val) {
    if (val < 0) {
        fastio_write_char(io, '-');
        uint64_t u = (uint64_t)(-(val + 1)) + 1ULL;
        fastio_write_u64(io, u);
    } else {
        fastio_write_u64(io, (uint64_t)val);
    }
}

static inline void fastio_write_i32(FastIO *io, int32_t val) {
    if (val < 0) {
        fastio_write_char(io, '-');
        uint32_t u = (uint32_t)(-(val + 1)) + 1u;
        fastio_write_u32(io, u);
    } else {
        fastio_write_u32(io, (uint32_t)val);
    }
}

static inline unsigned __int128 fastio__parse_udigits128(FastInput *__restrict__ in) {
    const char *p = in->ptr;
    const char *end = in->end;
    unsigned __int128 acc = 0;

    const __m256i lo_bound = _mm256_set1_epi8('0' - 1);
    const __m256i hi_bound = _mm256_set1_epi8('9' + 1);

    while (p + 32 <= end) {
        __m256i v = _mm256_loadu_si256((const __m256i *)p);
        __m256i ge = _mm256_cmpgt_epi8(v, lo_bound);
        __m256i le = _mm256_cmpgt_epi8(hi_bound, v);
        __m256i is_digit = _mm256_and_si256(ge, le);
        unsigned mask = (unsigned)_mm256_movemask_epi8(is_digit);

        if (mask == 0xFFFFFFFFu) {
            for (int k = 0; k < 32; k += 8) {
                uint64_t chunk;
                memcpy(&chunk, p + k, 8);
                acc = acc * 100000000ULL + fastio__parse8(chunk);
            }
            p += 32;
            continue;
        }

        unsigned non_digit = ~mask;
        int run = __builtin_ctz(non_digit);
        int k = 0;
        for (; k + 8 <= run; k += 8) {
            uint64_t chunk;
            memcpy(&chunk, p + k, 8);
            acc = acc * 100000000ULL + fastio__parse8(chunk);
        }
        for (; k < run; ++k) {
            acc = acc * 10 + (unsigned)(p[k] - '0');
        }
        p += run;
        in->ptr = p;
        return acc;
    }

    while (p < end) {
        unsigned char c = (unsigned char)*p;
        if (c < '0' || c > '9') break;
        acc = acc * 10 + (unsigned)(c - '0');
        ++p;
    }
    in->ptr = p;
    return acc;
}

static inline unsigned __int128 fastio_read_u128(FastIO *io) {
    fastio__skip_ws(&io->in);
    return fastio__parse_udigits128(&io->in);
}

static inline __int128 fastio_read_i128(FastIO *io) {
    fastio__skip_ws(&io->in);
    int neg = 0;
    if (io->in.ptr < io->in.end && *io->in.ptr == '-') {
        neg = 1;
        ++io->in.ptr;
    }
    unsigned __int128 u = fastio__parse_udigits128(&io->in);
    if (!neg) return (__int128)u;
    return -(__int128)(u - 1) - 1;
}

static inline int fastio__u128_to_buf(unsigned __int128 val, char *tmp_end) {
    char *p = tmp_end;
    while (val >= 10000) {
        unsigned idx = (unsigned)(val % 10000);
        val /= 10000;
        p -= 4;
        memcpy(p, &fastio__lut2[idx], 4);
    }
    uint32_t hi = (uint32_t)val;
    int sig = fastio__sig4(hi);
    uint32_t sv = fastio__lut1[hi] >> ((4 - sig) * 8);
    p -= sig;
    memcpy(p, &sv, (size_t)sig);
    return (int)(tmp_end - p);
}

static inline void fastio_write_u128(FastIO *io, unsigned __int128 val) {
    char tmp[48];
    int len = fastio__u128_to_buf(val, tmp + 48);
    fastio_write_str(io, tmp + 48 - len, (size_t)len);
}

static inline void fastio_write_i128(FastIO *io, __int128 val) {
    if (val < 0) {
        fastio_write_char(io, '-');
        unsigned __int128 u = (unsigned __int128)(-(val + 1)) + 1;
        fastio_write_u128(io, u);
    } else {
        fastio_write_u128(io, (unsigned __int128)val);
    }
}

#define MAXN 1000042
#define SIGMA 26

int link_node[MAXN];
int par_node[MAXN];
signed char to_node[MAXN][SIGMA];

int tr_edges[MAXN][SIGMA];
int tr_size[MAXN];

int sz = 1;

static inline bool has_to(int v, int c) {
    return to_node[v][c] != -1;
}

static inline int get_to(int v, int c) {
    if (!has_to(v, c)) return 0;
    int edge_idx = to_node[v][c];
    return tr_edges[v][edge_idx] >> 8;
}

static inline int get_to_simd(int v, int c) {
    int n_edges = tr_size[v];
    if (n_edges == 0) return 0;

    __m256i target_c = _mm256_set1_epi8((char)c);

    int i = 0;
    for (; i + 7 < n_edges; i += 8) {
        __m256i edges = _mm256_loadu_si256((const __m256i*)&tr_edges[v][i]);
        
        __m256i cmp = _mm256_cmpeq_epi8(edges, target_c);
        
        int mask = _mm256_movemask_epi8(cmp);
        if (mask != 0) {
            int byte_idx = __builtin_ctz(mask);
            int matched_offset = byte_idx / 4;
            return tr_edges[v][i + matched_offset] >> 8;
        }
    }

    for (; i < n_edges; i++) {
        if ((tr_edges[v][i] & 0xFF) == c) {
            return tr_edges[v][i] >> 8;
        }
    }

    return 0;
}

int ac_add(const char *s) {
    int v = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        int x = s[i] - 'a';
        if (!has_to(v, x)) {
            par_node[sz] = v;
            
            int current_tr_len = tr_size[v];
            to_node[v][x] = (signed char)current_tr_len;
            
            tr_edges[v][current_tr_len] = x | (sz << 8);
            tr_size[v]++;
            
            v = sz++;
        } else {
            v = get_to(v, x);
        }
    }
    return v;
}

typedef struct {
    int data[MAXN];
    int head;
    int tail;
} Queue;

static inline void queue_init(Queue *q) { q->head = 0; q->tail = 0; }
static inline void queue_push(Queue *q, int val) { q->data[q->tail++] = val; }
static inline int queue_pop(Queue *q) { return q->data[q->head++]; }
static inline bool queue_empty(const Queue *q) { return q->head == q->tail; }

void ac_build(void) {
    Queue q;
    queue_init(&q);
    queue_push(&q, 0);

    while (!queue_empty(&q)) {
        int v = queue_pop(&q);
        int num_edges = tr_size[v];

        for (int i = 0; i < num_edges; i++) {
            int cw = tr_edges[v][i];
            char c = (char)(cw & 0xFF);
            int w = cw >> 8;

            link_node[w] = link_node[v];
            while (link_node[w] && !has_to(link_node[w], c)) {
                link_node[w] = link_node[link_node[w]];
            }

            link_node[w] = v ? get_to(link_node[w], c) : 0;
            queue_push(&q, w);
        }
    }
}

void ac_init(void) {
    memset(to_node, -1, sizeof(to_node));
    memset(tr_size, 0, sizeof(tr_size));
    sz = 1;
}

int main(void) {

    FastIO io;
    fastio_init(&io, 0, 1);
    char str[1000001];

    int32_t n = fastio_read_i32(&io);
    ac_init();
    int32_t *v = (int32_t *)malloc(sizeof(int32_t) * n);
    for (int32_t i = 0; i < n; i++) {
        fastio_read_str(&io, str, 1000001);
        v[i] = ac_add(str);
    }
    ac_build();

    fastio_write_i32(&io, sz);
    fastio_write_char(&io, '\n');

    for (int32_t i = 1; i < sz; i++) {
        fastio_write_i32(&io, par_node[i]);
        fastio_write_char(&io, ' ');
        fastio_write_i32(&io, link_node[i]);
        fastio_write_char(&io, '\n');
    }

    for (int32_t i = 0; i < n; i++) {
        fastio_write_i32(&io, v[i]);
        if (i < n - 1) {
            fastio_write_char(&io, ' ');
        }
    }
    fastio_write_char(&io, '\n');

    free(v);
    fastio_destroy(&io);

    return 0;
}