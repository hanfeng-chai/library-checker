#pragma GCC target("popcnt")
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <algorithm>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;

struct Blk { uint64_t bits; uint32_t cnt; uint32_t _pad; };

// ---------------- table-based fast I/O over mmap'd stdin ----------------
static uint8_t PT[1 << 16];      // two ASCII bytes -> 2-digit value, else >99
static int     OT[10000];        // d in [0,9999] -> 4 packed ASCII chars (little-endian)
static char*   ip;               // input cursor (mmap)
static char*   obuf; static char* op;

static void io_init() {
    struct stat st; char* base = (char*)MAP_FAILED; size_t sz = 0;
    if (fstat(0, &st) == 0 && S_ISREG(st.st_mode) && st.st_size > 0) {        // mmap path (judge: file redirect)
        sz = (size_t)st.st_size;
        base = (char*)mmap(nullptr, sz + 8, PROT_READ, MAP_PRIVATE, 0, 0);
        if (base != (char*)MAP_FAILED) madvise(base, sz + 8, MADV_SEQUENTIAL);
    }
    if (base == (char*)MAP_FAILED) {                                          // fallback: slurp (pipe/tty)
        size_t cap = 1 << 22; char* buf = (char*)malloc(cap); sz = 0;
        for (;;) { if (sz + (1<<20) > cap) { cap <<= 1; buf = (char*)realloc(buf, cap); }
            size_t g = fread(buf + sz, 1, 1<<20, stdin); sz += g; if (g < (size_t)(1<<20)) break; }
        buf = (char*)realloc(buf, sz + 8); memset(buf + sz, 0, 8); base = buf;
    }
    ip = base;
    memset(PT, 0xFF, sizeof(PT));
    for (int i = 0; i < 10; i++) for (int j = 0; j < 10; j++)
        PT[('0' + i) | (('0' + j) << 8)] = (uint8_t)(i * 10 + j);   // low=first char, high=second
    for (int d = 0; d < 10000; d++)
        OT[d] = ('0'+d/1000) | (('0'+(d/100)%10)<<8) | (('0'+(d/10)%10)<<16) | (('0'+d%10)<<24);
    obuf = (char*)malloc(1 << 23); op = obuf;
}
static inline unsigned ru() {
    while (*ip <= ' ') ++ip;
    unsigned x = (unsigned)(*ip++ - '0');
    for (;;) { unsigned y = PT[*(const uint16_t*)ip]; if (y > 99) break; x = x * 100 + y; ip += 2; }
    if (*ip > ' ') x = x * 10 + (unsigned)(*ip++ & 15);
    return x;
}
static inline void pb(unsigned d) { *(int*)op = OT[d]; op += 4; }            // 4 padded digits
static inline void p4(unsigned x) {                                          // x<10000, no leading zeros
    if (x > 99) { if (x > 999) pb(x); else { *(int*)op = OT[x * 10]; op += 3; } }
    else if (x > 9) { *(int*)op = OT[x * 100]; op += 2; }
    else *op++ = char('0' + x);
}
static inline void wu(unsigned x) {                                          // up to 1e9
    if (x >= 100000000u) { unsigned hi = x / 100000000u; x %= 100000000u; p4(hi); pb(x / 10000); pb(x % 10000); }
    else if (x >= 10000u) { p4(x / 10000); pb(x % 10000); }
    else p4(x);
}

int main() {
    io_init();
    int N = (int)ru(), Q = (int)ru();
    int* a = (int*)malloc(4*(size_t)N);
    for (int i = 0; i < N; i++) a[i] = (int)ru();

    // ---- coordinate compress via 2-pass 16-bit radix on packed (value<<18 | index) ----
    uint64_t* key = (uint64_t*)malloc(8*(size_t)N);
    for (int i = 0; i < N; i++) key[i] = ((uint64_t)(unsigned)a[i] << 18) | (unsigned)i;
    uint64_t* tmp = (uint64_t*)malloc(8*(size_t)N);
    static int cnt[65537];
    uint64_t *src = key, *dst = tmp;
    for (int pass = 0; pass < 2; pass++) {
        int shift = 18 + pass * 16;
        memset(cnt, 0, sizeof(cnt));
        for (int i = 0; i < N; i++) cnt[(src[i] >> shift) & 0xFFFF]++;
        int sum = 0; for (int b = 0; b < 65536; b++) { int c = cnt[b]; cnt[b] = sum; sum += c; }
        for (int i = 0; i < N; i++) { int b = (int)((src[i] >> shift) & 0xFFFF); dst[cnt[b]++] = src[i]; }
        uint64_t* t = src; src = dst; dst = t;
    }
    key = src; free(dst);
    int* vals = (int*)malloc(4*(size_t)N); int m = 0; int prevv = -1;
    for (int i = 0; i < N; i++) { int v = (int)(key[i] >> 18), idx = (int)(key[i] & ((1u<<18)-1));
        if (v != prevv) { vals[m++] = v; prevv = v; } a[idx] = m - 1; }
    free(key);

    int H = 1; while ((1 << H) < m) H++;
    const int W = (N + 63) >> 6, S = W + 1;
    Blk* blk = (Blk*)malloc(sizeof(Blk)*(size_t)H*S);
    int* zeros = (int*)malloc(4*(size_t)H); int* bitval = (int*)malloc(4*(size_t)H);
    for (int l = 0; l < H; l++) bitval[l] = 1 << (H - 1 - l);

    int* cur = a; int* nxt = (int*)malloc(4*(size_t)N); int* onb = (int*)malloc(4*(size_t)N);
    for (int l = 0; l < H; l++) {
        const int bit = H - 1 - l;
        Blk* L = blk + (size_t)l * S;
        uint64_t word = 0; int wbit = 0, w = 0; uint32_t ones = 0; int zc = 0, oc = 0;
        for (int i = 0; i < N; i++) {
            int b = (cur[i] >> bit) & 1;
            word |= (uint64_t)b << wbit;
            nxt[zc] = cur[i]; onb[oc] = cur[i]; zc += b ^ 1; oc += b;
            if (++wbit == 64) { L[w].cnt = ones; L[w].bits = word; ones += (uint32_t)__builtin_popcountll(word); w++; word = 0; wbit = 0; }
        }
        if (wbit) { L[w].cnt = ones; L[w].bits = word; ones += (uint32_t)__builtin_popcountll(word); w++; }
        L[W].cnt = ones; L[W].bits = 0;
        zeros[l] = zc;
        memcpy(nxt + zc, onb, (size_t)oc * 4);
        int* t = cur; cur = nxt; nxt = t;
    }

    int* QL = (int*)malloc(4*(size_t)Q); int* QR = (int*)malloc(4*(size_t)Q);
    int* QK = (int*)malloc(4*(size_t)Q); int* RS = (int*)malloc(4*(size_t)Q);
    for (int i = 0; i < Q; i++) { QL[i] = (int)ru(); QR[i] = (int)ru(); QK[i] = (int)ru(); RS[i] = 0; }

    for (int lev = 0; lev < H; lev++) {
        const Blk* base = blk + (size_t)lev * S;
        const int z = zeros[lev], bv = bitval[lev];
        for (int i = 0; i < Q; i++) {
            int l = QL[i], r = QR[i];
            const Blk& Bl = base[l >> 6];
            const Blk& Br = base[r >> 6];
            int r1l = (int)Bl.cnt + __builtin_popcountll(Bl.bits & ((1ULL << (l & 63)) - 1));
            int r1r = (int)Br.cnt + __builtin_popcountll(Br.bits & ((1ULL << (r & 63)) - 1));
            int l0 = l - r1l, r0 = r - r1r, zr = r0 - l0;
            int ge = -(int)(QK[i] >= zr);
            QL[i] = (l0 & ~ge) | ((z + r1l) & ge);
            QR[i] = (r0 & ~ge) | ((z + r1r) & ge);
            QK[i] -= zr & ge;
            RS[i] |= bv & ge;
        }
    }

    for (int i = 0; i < Q; i++) { wu((unsigned)vals[RS[i]]); *op++ = '\n'; }
    fwrite_unlocked(obuf, 1, op - obuf, stdout);
    return 0;
}