#pragma GCC optimize("Ofast,inline,unroll-loops")
#include <bits/stdc++.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define FAST_FWRITE fwrite_unlocked

struct FastInput {
    const unsigned char* buf;
    const unsigned char* cur;
    size_t map_size;

    FastInput() noexcept {
        struct stat st;
        const int fd = fileno(stdin);
        fstat(fd, &st);
        map_size = st.st_size + 32;
        buf = cur = static_cast<const unsigned char*>(mmap(nullptr, map_size,
            PROT_READ, MAP_PRIVATE/* | MAP_POPULATE*/, fd, 0));
//        madvise(const_cast<unsigned char*>(buf), map_size, MADV_SEQUENTIAL);
//        madvise(const_cast<unsigned char*>(buf), map_size, MADV_WILLNEED);
    }
    ~FastInput() noexcept { munmap(const_cast<unsigned char*>(buf), map_size); }

    uint32_t read0() noexcept {
        const unsigned char* c = cur;
        uint32_t d;
        memcpy(&d, c, 4);
        d ^= 0x30303030u;
        const uint32_t e = d & 0xf0f0f0f0u;
        if (e) {
            if ((uint16_t)e) {
                cur = c += 2; return (uint8_t)d;
            } 
            uint32_t x = (uint8_t)d * 10 + (uint8_t)(d >> 8);
            if ((uint8_t)(e >> 16)) {
                cur = c += 3; return x;
            }
            cur = c += 4; return x * 10 + (uint8_t)(d >> 16);
        } else c += 4;
        d = (d * 10 +  (d >> 8))  & 0x00ff00ffu;
        d = (d * 100 + (d >> 16)) & 0x0000ffffu;
        if(!e) {
            unsigned char b;
            for (int i = 0; ((b = *c++) >= '0') && (i < 2); i++)
                d = d * 10 + b - '0';
        }
        cur = c;
        return (uint32_t)d;
    }

    inline uint32_t read1() noexcept {
        const unsigned char* c = cur;
        uint64_t d;
        memcpy(&d, c, 8);
        d ^= 0x3030303030303030ull;
        const uint64_t f = d & 0xf0f0f0f0f0f0f0f0ull;
        if (f) {
            uint32_t e = (uint32_t)f;
            if (e) {
                if ((uint16_t)e) {
                    cur = c += 2; return (uint8_t)d;
                }
                uint32_t x = (uint8_t)d * 10 + (uint8_t)(d >> 8);
                if ((uint8_t)(e >> 16)) {
                    cur = c += 3; return x;
                }
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
        if(!f) {
            unsigned char b;
            for (int i = 0; ((b = *c++) >= '0') && (i < 2); i++)
                x = x * 10 + b - '0';
        }
        cur = c;
        return x;
    }
};

struct FastOutput {
    char* buf;
    char* cur;
    char* end;

    FastOutput(char* p, size_t n) : buf(p), cur(p), end(p + n) {}
    ~FastOutput() noexcept { flush(); }

    void ensure(size_t need) noexcept {
        if (size_t(end - cur) >= need) [[likely]] return;
        flush();
    }

    void flush() noexcept {
        size_t n = size_t(cur - buf);
        if (n) { FAST_FWRITE(buf, 1, n, stdout); cur = buf; }
    }

    inline void write(char c) noexcept { /*ensure(1);*/ *cur++ = c; }

    template <class T>
    void write(T u) noexcept {
        static_assert(std::is_integral_v<T>);
        ensure(25);
        char* c = cur;
        __builtin_prefetch(c + 16, 1, 1); // prefetch new write target

        constexpr uint64_t B = 1000000ull;
        if (u < B) [[likely]] writeUIntUpTo6((uint32_t)u);
        else if (u < B * B) [[likely]] {
            uint64_t v = (uint64_t)u;
            uint32_t r = (uint32_t)(v % B);
            writeUIntUpTo6((uint32_t)(v / B));
            writePadded6(r);
        } else {
            uint64_t v = (uint64_t)u;
            uint64_t q0 = v / B;
            uint32_t r0 = (uint32_t)(v - q0 * B);
            uint32_t r1 = (uint32_t)(q0 % B);
            writeUIntUpTo6((uint32_t)(q0 / B));
            writePadded6(r1);
            writePadded6(r0);
        }
    }

private:
    struct Entry {
        char padded[3];
        uint8_t len;
    };

    static constexpr auto table = [] {
        std::array<Entry, 1000> t{};
        for (int i = 0; i < 1000; ++i) {
            t[i].padded[0] = '0' + i / 100;
            t[i].padded[1] = '0' + (i / 10) % 10;
            t[i].padded[2] = '0' + i % 10;
            t[i].len = ((i > 99) ? 1 : 0) + ((i > 9) ? 1 : 0) + 1;
        }
        return t;
    }();

    void writeUIntUpTo6(const uint32_t x) noexcept {
        constexpr uint32_t div = 1000;
        const uint32_t hi = x / div;
        const uint32_t lo = x % div;
        // branchless trick if the compiler does not emit cmove
        // hi + (((hi ? 1 : 0) - 1) & x)
        const Entry& e = table[hi ? hi : x];
        const size_t len = e.len;
        char* c = cur;
        memcpy(c, &e.padded[3 - len], 4); c += len;
        if (hi) {
            memcpy(c, table[lo].padded, 4); c += 3;
        }
        cur = c;
    }

    void writePadded6(const uint32_t x) noexcept {
        constexpr uint32_t mask = 0x00ffffffu;
        constexpr uint32_t div = 1000;
        const uint32_t hi = x / div;
        const uint32_t lo = x % div;
        const uint32_t a = *(uint32_t*)table[hi].padded & mask;
        const uint32_t b = *(uint32_t*)table[lo].padded & mask;
        const uint64_t qword = (uint64_t)a | ((uint64_t)b << 24);
        char* c = cur;
        memcpy(c, &qword, 8);
        cur = c += 6;
    }
};

struct DSU {
    int* p = nullptr;
    int n = 0;

    DSU() = default;
    DSU(int n_, int* storage) noexcept { init(n_, storage); }
    
    inline constexpr void init(int n_, int* storage) noexcept {
        n = n_;
        p = storage;
        memset(p, 0xFF, n * sizeof(int));
    }

    inline constexpr int find(int x) noexcept {
        int* pp = p;
        while (pp[x] >= 0) {
            int ppx = pp[x];
            int pppx = pp[ppx];
            pp[x] = pppx >= 0 ? pppx : ppx;
            x = ppx;
        }
        return x;
    }

    inline constexpr bool unite(int a, int b) noexcept {
        a = find(a); b = find(b);
        if (a == b) return false;
        int* pp = p;
        const int pa = pp[a];
        const int pb = pp[b];
        if (pa > pb) {
            pp[b] = pb + pa;
            pp[a] = b;
        } else {
            pp[a] = pa + pb;
            pp[b] = a;
        }
        return true;
    }
};

struct Edge {
    uint32_t u, v, w, i;
};
alignas(64) static Edge E[500000];
alignas(64) static Edge T[500000];
alignas(64) static uint32_t H[1024]; // cleared to zero default by compiler
static uint32_t N, M;

inline void radix_sort_edges() noexcept {
    if (M < 8192) {
        std::stable_sort(E, E + M, [](const Edge& x, const Edge& y) {
            return x.w < y.w;
        });
        return;
    }
    // memset(H, 0, sizeof(H)); // if you need to sort repeatingly...
    uint32_t* const h0 = H;         uint32_t* const h1 = H + 256;
    uint32_t* const h2 = H + 512;   uint32_t* const h3 = H + 768;
    int i = 0;
    const int M4 = M & ~3;
    for (; i < M4; i += 4) {
        __builtin_prefetch(&E[i + 8], 0, 1);
        h0[((uint8_t*)&E[i + 0].w)[0]]++; h1[((uint8_t*)&E[i + 0].w)[1]]++;
        h2[((uint8_t*)&E[i + 0].w)[2]]++; h3[((uint8_t*)&E[i + 0].w)[3]]++;
        h0[((uint8_t*)&E[i + 1].w)[0]]++; h1[((uint8_t*)&E[i + 1].w)[1]]++;
        h2[((uint8_t*)&E[i + 1].w)[2]]++; h3[((uint8_t*)&E[i + 1].w)[3]]++;
        h0[((uint8_t*)&E[i + 2].w)[0]]++; h1[((uint8_t*)&E[i + 2].w)[1]]++;
        h2[((uint8_t*)&E[i + 2].w)[2]]++; h3[((uint8_t*)&E[i + 2].w)[3]]++;
        h0[((uint8_t*)&E[i + 3].w)[0]]++; h1[((uint8_t*)&E[i + 3].w)[1]]++;
        h2[((uint8_t*)&E[i + 3].w)[2]]++; h3[((uint8_t*)&E[i + 3].w)[3]]++;
    }
    for (; i < M; i++) {
        h0[((uint8_t*)&E[i].w)[0]]++; h1[((uint8_t*)&E[i].w)[1]]++;
        h2[((uint8_t*)&E[i].w)[2]]++; h3[((uint8_t*)&E[i].w)[3]]++;
    }
    uint32_t s0 = 0, s1 = 0, s2 = 0, s3 = 0;
    for (int j = 0; j < 256; j++) {
        const uint32_t c0 = h0[j], c1 = h1[j], c2 = h2[j], c3 = h3[j];
        h0[j] = s0;  s0 += c0;  h1[j] = s1;  s1 += c1;
        h2[j] = s2;  s2 += c2;  h3[j] = s3;  s3 += c3;
    }
    for (int i = 0; i < M; i++) {
        const uint8_t key = ((uint8_t*)&E[i].w)[0];
        __builtin_prefetch(&T[h0[key] + 16], 1, 1);
        T[h0[key]++] = E[i];
    }
    for (int i = 0; i < M; i++) {
        const uint8_t key = ((uint8_t*)&T[i].w)[1];
        __builtin_prefetch(&E[h1[key] + 16], 1, 1);
        E[h1[key]++] = T[i];
    }
    for (int i = 0; i < M; i++) {
        const uint8_t key = ((uint8_t*)&E[i].w)[2];
        __builtin_prefetch(&T[h2[key] + 16], 1, 1);
        T[h2[key]++] = E[i];
    }
    for (int i = 0; i < M; i++) {
        const uint8_t key = ((uint8_t*)&T[i].w)[3];
        __builtin_prefetch(&E[h3[key] + 16], 1, 1);
        E[h3[key]++] = T[i];
    }
}

using namespace std;

int main() noexcept {
    FastInput in;
    N = in.read0();
    M = in.read0();
    for (int i = 0; i < M; i++) {
        Edge &e = E[i];
        e.u = in.read0();
        e.v = in.read0();
        e.w = in.read1();
        e.i = (uint32_t)i;
    }
    radix_sort_edges();
    int* dsu_storage = reinterpret_cast<int*>(T); // re-use the temp memory!
    uint32_t* mst = reinterpret_cast<uint32_t*>(dsu_storage + N);
    DSU dsu(N, dsu_storage);
    int cnt = 0;
    uint64_t ans = 0;
    for (int i = 0; i < M; i++) {
        const Edge &e = E[i];
        __builtin_prefetch(&E[i+16], 0, 1);
        if (dsu.unite(e.u, e.v)) [[likely]] {
            ans += e.w;
            mst[cnt++] = e.i;
            if (cnt == N - 1) [[unlikely]] break;
        }
    }
    if(cnt > 0) {
        size_t out_bufsize = sizeof(E);
//        if (out_bufsize > (1 << 20))
//            out_bufsize = 1 << 20;
        FastOutput out(reinterpret_cast<char*>(E), out_bufsize);
        out.write(ans);
        out.write('\n');
        out.write(mst[0]);
        for (int i = 1; i < cnt; i++) {
            out.write(' ');
            out.write(mst[i]);
        }
        out.write('\n');
//        out.flush();
    }
    return 0;
}
