// https://judge.yosupo.jp/problem/unionfind_with_potential
// sigma@iki.fi
#pragma GCC optimize("Ofast,inline,unroll-loops")
#include <bits/stdc++.h>

#pragma GCC target("avx2,bmi,bmi2,popcnt")
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define FAST_FWRITE fwrite_unlocked

struct FastInput {
    alignas(64) const unsigned char *buf, *cur;
    size_t map_size;

    FastInput() noexcept {
        struct stat st;
        fstat(fileno(stdin), &st);
        map_size = st.st_size + 32;
        buf = cur = static_cast<const unsigned char*>(mmap(nullptr, map_size,
            PROT_READ, MAP_PRIVATE/* | MAP_POPULATE*/, fileno(stdin), 0));
//        madvise(const_cast<unsigned char*>(buf), map_size, MADV_SEQUENTIAL);
//        madvise(const_cast<unsigned char*>(buf), map_size, MADV_WILLNEED);
    }
    ~FastInput() noexcept { munmap(const_cast<unsigned char*>(buf), map_size); }

    uint32_t read0() noexcept {
        const unsigned char *c = cur;
        uint32_t d;
        memcpy(&d, c, 4);
        d ^= 0x30303030u;
        const uint32_t e = d & 0xf0f0f0f0u;
        if (e) {
            if ((uint16_t)e) { cur = c += 2; return (uint8_t)d; } 
            uint32_t x = (uint8_t)d * 10 + (uint8_t)(d >> 8);
            if ((uint8_t)(e >> 16)) { cur = c += 3; return x; }
            cur = c += 4; return x * 10 + (uint8_t)(d >> 16);
        }
        c += 4;
        d = (d * 10 +  (d >> 8))  & 0x00ff00ffu;
        d = (d * 100 + (d >> 16)) & 0x0000ffffu;
        unsigned char b;
        while ((b = *c++) >= '0') d = d * 10 + b - '0';
        cur = c;
        return d;
    }

    inline uint32_t read1() noexcept {
        const unsigned char *c = cur;
        uint64_t d;
        memcpy(&d, c, 8);
        d ^= 0x3030303030303030ull;
        const uint64_t f = d & 0xf0f0f0f0f0f0f0f0ull;
        if (f) {
            uint32_t e = (uint32_t)f;
            if (e) {
                if ((uint16_t)e) { cur = c += 2; return (uint8_t)d; }
                uint32_t x = (uint8_t)d * 10 + (uint8_t)(d >> 8);
                if ((uint8_t)(e >> 16)) { cur = c += 3; return x; }
                cur = c += 4; return x * 10 + (uint8_t)(d >> 16);
            }
            e = (uint32_t)(f >> 32);
            const size_t len = ((uint16_t)e) ?
                (((uint8_t)e) ? 0 : 1) : ((((uint8_t)(e >> 16)) ? 0 : 1) + 2);
            d <<= ((4 - len) * 8);
            c += len + 5;
        } else c += 8;
        d = (d * 10 +    (d >> 8))  & 0x00ff00ff00ff00ffull;
        d = (d * 100 +   (d >> 16)) & 0x0000ffff0000ffffull;
        d = (d * 10000 + (d >> 32)) & 0x00000000ffffffffull;
        uint32_t x = (uint32_t)d;
        unsigned char b;
        if (!f) { while ((b = *c++) >= '0') x = x * 10 + b - '0'; }
        cur = c;
        return x;
    }
};

struct FastOutput {
    alignas(64) char *buf, *cur, *end;

    FastOutput(char* p, size_t n) : buf(p), cur(p), end(p + n) {}
    ~FastOutput() noexcept { flush(); }

    void flush() noexcept {
        size_t n = size_t(cur - buf);
        if (n) { FAST_FWRITE(buf, 1, n, stdout); cur = buf; }
    }

    void ensure(size_t need) noexcept {
        if (size_t(end - cur) >= need) [[likely]] return;
        flush();
    }

    inline void write(char c) noexcept { /*ensure(1);*/ *cur++ = c; }

    template <class T>
    void write(T u) noexcept {
        static_assert(std::is_integral_v<T>);
        // ensure(26);
        char *c = cur;
        __builtin_prefetch(c + 16, 1, 1); // prefetch new write target
        constexpr uint64_t B = 1000000ull;
        if (u < B) [[likely]] { writeUpTo6((uint32_t)u);
        } else if (u < B * B) [[likely]] {
            uint64_t v = (uint64_t)u;
            uint32_t q = ((__uint128_t)v * 18446744073710ull) >> 64; // q=v/B
            uint32_t r = (uint32_t)(v - q * B);                      // r=v%B
            writeUpTo6(q); writePad6(r);
        } else if (u < B * B * B) [[likely]] {
            uint64_t v = (uint64_t)u;
            uint64_t q0 = ((__uint128_t)v * 4835703278458516699ull) >> 82;
            uint32_t r0 = (uint32_t)(v - q0 * B);
            uint32_t q1 = ((__uint128_t)q0 * 18446744073710ull) >> 64;
            uint32_t r1 = (uint32_t)(q0 - q1 * B);
            writeUpTo6(q1); writePad6(r1); writePad6(r0);
        } else {
            write('E');
        }
    }

private:
    struct alignas(4) Entry { char p[3]; uint8_t len; };
    alignas(64) static constexpr auto T = [] {
        std::array<Entry, 1000> t{};
        for (int i = 0; i < 1000; ++i) {
            t[i].p[0]='0'+i/100; t[i].p[1]='0'+(i/10)%10; t[i].p[2]='0'+i%10;
            t[i].len = (i>99)+(i>9)+1;
        }
        return t;
    }();

    void writeUpTo6(const uint32_t x) noexcept {
        const uint32_t hi = ((uint64_t)x * 4294968ull) >> 32; // hi = x / 1000
        const uint32_t lo = x - hi * 1000;                    // lo = x % 1000
        const Entry& e = T[hi ? hi : x]; // hi + (((hi ? 1 : 0) - 1) & x)
        const size_t len = e.len;
        char *c = cur;
        memcpy(c, &e.p[3 - len], 4); c += len;
        if (hi) { memcpy(c, T[lo].p, 4); c += 3; }
        cur = c;
    }

    void writePad6(const uint32_t x) noexcept {
        constexpr uint32_t mask = 0x00ffffffu;
        const uint32_t hi = ((uint64_t)x * 4294968ull) >> 32; // hi = x / 1000
        const uint32_t lo = x - hi * 1000;                    // lo = x % 1000
        const uint32_t a = *(uint32_t*)T[hi].p & mask;
        const uint32_t b = *(uint32_t*)T[lo].p & mask;
        const uint64_t qword = (uint64_t)a | ((uint64_t)b << 24);
        char *c = cur; memcpy(c, &qword, 8); cur = c += 6;
    }
};

using namespace std;

static constexpr uint32_t MOD = 998244353u;
#if 0
// multiply with MOD's reciprocal:
static inline constexpr int mulmodrec(int v) noexcept {
    __int128 prod = (__int128)v * 155014655926305585ll;
    int q = prod >> 87;
    q -= v >> 31;
    return q;
}

static inline uint32_t reduce(uint32_t v) noexcept {
    return v - (MOD & -(v >= MOD));
}
#endif

static constexpr int MAXN = 200001;
struct alignas(8) Node { uint32_t pot; int parent; };
alignas(64) static char outbuf[1 << 21];    // zeroed during compile
alignas(64) static int rnk[MAXN];           // zeroed during compile
alignas(64) static constinit auto node = [] {
    std::array<Node, MAXN> t{};
    for (int i = 0; i < MAXN; ++i) { t[i].parent = i; }
    return t;
}();
static inline int find(int i) noexcept {
    const int p = node[i].parent;
    if (p == i) return i;
    const int root = find(p);
    const uint32_t z = node[i].pot + node[p].pot;
    node[i].pot = z >= MOD ? z - MOD : z;
    node[i].parent = root;
    return root;
}

static inline bool unite(int u, int v, uint32_t x) noexcept {
    __builtin_prefetch(&node[u], 0, 1);
    __builtin_prefetch(&node[v], 0, 1);
    const int ru = find(u);
    const int rv = find(v);
    const uint32_t pu = node[u].pot;
    const uint32_t pv = node[v].pot;

    if (ru == rv) { const uint32_t z = pu - pv + MOD;
        return (z >= MOD ? z - MOD : z) == x; }
    int32_t d = (int32_t)(x + pv) - (int32_t)pu;
    if (d < 0) d += (int32_t)MOD;
    const uint32_t z = (uint32_t)d;
    uint32_t diff = z >= MOD ? z - MOD : z;

    if (rnk[ru] >= rnk[rv]) {
        if (rnk[ru] == rnk[rv]) rnk[ru]++;
        node[rv].pot = diff ? MOD - diff : 0u;
        node[rv].parent = ru;
        return true;
    } else {
        node[ru].pot = diff;
        node[ru].parent = rv;
        return true;
    }
}

static inline int query(int u, int v) noexcept {
    __builtin_prefetch(&node[u], 0, 1);
    __builtin_prefetch(&node[v], 0, 1);
    const int ru = find(u);
    const int rv = find(v);
    if (ru != rv) return -1;
    const uint32_t z = node[u].pot - node[v].pot + MOD;
    return z >= MOD ? z - MOD : z;
}

int main() noexcept {
    FastInput in;
    const uint32_t N = in.read0(); uint32_t Q = in.read0();
    FastOutput out(outbuf, sizeof outbuf);
    while (Q--) {
        const uint8_t t = *in.cur - '0';
        in.cur += 2;
        const int32_t u = in.read0();
        const int32_t v = in.read0();
        if (t) {
            const int32_t w = query(u, v);
            if (w < 0) {
                out.write('-'); out.write('1');
            } else out.write(w);
        } else out.write(unite(u, v, in.read1()) ? '1' : '0');
        out.write('\n');
    }
    return 0;
}
