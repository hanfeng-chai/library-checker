// https://judge.yosupo.jp/problem/shortest_path
// sigma@iki.fi
#pragma GCC optimize("Ofast,inline,unroll-loops")
#include <bits/stdc++.h>

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

    inline void write(char c) noexcept { *cur++ = c; }

    template <class T>
    void write(T u) noexcept {
        static_assert(std::is_integral_v<T>);
        char *c = cur;
        __builtin_prefetch(c + 16, 1, 1);
        constexpr uint64_t B = 1000000ull;
        if (u < B) [[likely]] { writeUpTo6((uint32_t)u);
        } else if (u < B * B) [[likely]] {
            uint64_t v = (uint64_t)u;
            uint32_t q = ((__uint128_t)v * 18446744073710ull) >> 64;
            uint32_t r = (uint32_t)(v - q * B);
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

    inline void write2(uint32_t u) noexcept {
        ensure(20);
        char *const c1 = cur;
        write(u);
        char *const c2 = cur;
        write('\n');
        memcpy(cur, c1, c2 - c1);
        cur += c2 - c1;
        write(' ');
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
        const uint32_t hi = ((uint64_t)x * 4294968ull) >> 32;
        const uint32_t lo = x - hi * 1000;
        const Entry& e = T[hi ? hi : x];
        const size_t len = e.len;
        char *c = cur;
        memcpy(c, &e.p[3 - len], 4); c += len;
        if (hi) { memcpy(c, T[lo].p, 4); c += 3; }
        cur = c;
    }

    void writePad6(const uint32_t x) noexcept {
        constexpr uint32_t mask = 0x00ffffffu;
        const uint32_t hi = ((uint64_t)x * 4294968ull) >> 32;
        const uint32_t lo = x - hi * 1000;
        const uint32_t a = *(uint32_t*)T[hi].p & mask;
        const uint32_t b = *(uint32_t*)T[lo].p & mask;
        const uint64_t qword = (uint64_t)a | ((uint64_t)b << 24);
        char *c = cur; memcpy(c, &qword, 8); cur = c += 6;
    }
};

using namespace std;

constexpr uint32_t MAXN = 500000;
constexpr uint32_t MAXM = 500000;

alignas(64) uint64_t dist[MAXN];
alignas(64) uint32_t parent[MAXN];
static uint32_t edge_cnt = 0;
static uint32_t heap_size = 0;
static uint32_t path_len = 0;

// Optimized heap with hole-based percolation bubbling avoids unnecessary swaps
alignas(64) struct HeapNode {
    int64_t k;
    uint32_t v;
} heap[MAXM];

static inline void heap_push(const int64_t k, const uint32_t v) noexcept {
    size_t i = ++heap_size;
    while (k < heap[i >> 1].k) {    // parent is greater than v, move it down
        heap[i] = heap[i >> 1];
        i >>= 1;
    }
    heap[i] = (HeapNode){k, v};
}

static inline HeapNode heap_pop() noexcept {
    const HeapNode res = heap[1];               // min value
    const HeapNode& last = heap[heap_size--];   // take the last element
    size_t c, i = 1;
    while (true) {                              // Percolate the hole down
        c = i << 1;
        if (c > heap_size) break;               // no children
        if (c < heap_size && heap[c + 1].k < heap[c].k)
            c++;                                // choose the smaller child
        if (last.k <= heap[c].k) break;         // correct position found
        heap[i] = heap[c];                      // move smaller child up
        i = c;
    }
    heap[i] = last;
    return res;
}

template<typename T, auto init, size_t sz, size_t Offset, size_t Count>
constexpr void fill_chunk(std::array<T, sz>& a) noexcept {
    for (size_t i = 0; i < Count; ++i)
        a[Offset + i] = init;
}

template<typename T, auto init, size_t sz>
constexpr std::array<T, sz> make() noexcept {
    std::array<T, sz> a{};
    fill_chunk<T, UINT32_MAX, MAXN,      0, sz / 2>(a);
    fill_chunk<T, UINT32_MAX, MAXN, sz / 2, sz / 2>(a);
    return a;
}

alignas(64) struct Edge {
    uint64_t w;
    uint32_t next;
    uint32_t to;
} edges[MAXM];
alignas(64) constinit static auto head = make<uint32_t, UINT32_MAX, MAXN>();

static inline void add_edge(const uint64_t w, const uint32_t v, const uint32_t u)
    noexcept {
    edges[edge_cnt] = (Edge){w, head[v], u};
    head[v] = edge_cnt++;
}

alignas(64) bool processed[MAXN]; // zero i.e. false default by compiler

int main() {
    FastInput in;
    const uint32_t N = in.read0(), M = in.read0(),
        s = in.read0(), t = in.read0();
    for (uint32_t i = 0; i < M; i++)
        add_edge(in.read1(), in.read0(), in.read0());
    heap[0].v = 0; // heap sentinel
    heap[0].k = -1;
    dist[t] = 1; // ULLONG_MAX trick
    heap_push(1, t);
    while (heap_size > 0) {
        const HeapNode h = heap_pop();
        const uint64_t d = h.k; 
        const uint32_t v = h.v;
        if (d > dist[v]) continue;
        if (v == s) break;
        processed[v] = true;
        uint32_t c = head[v];
        while (c != UINT32_MAX) {
            const Edge& e = edges[c];
            uint32_t u = e.to;
            if (!processed[u]) {
                uint64_t new_dist = d + e.w;
                if (uint64_t(dist[u] - 1) >= new_dist) { // ULLONG_MAX trick
                    dist[u] = new_dist;
                    parent[u] = v;
                    heap_push(new_dist, u);
                }
            }
            c = e.next;
        }
    }
    FastOutput out((char*)dist, sizeof(dist));
    if (dist[s] == 0) { // ULLONG_MAX trick
        out.write('-'); out.write('1'); out.write('\n');
        return 0;
    }
    uint32_t curr = s;
    while (curr != t) {
        head[path_len++] = curr = parent[curr];
    } // ULLONG_MAX trick:
    out.write(dist[s] - 1); out.write(' '); out.write(path_len); out.write('\n');
    out.write(s); out.write(' ');
    for (int i = 0; i < path_len - 1; i++) {
        out.write2(head[i]);
    }    
    out.write(t); out.write('\n');
    return 0;
}
