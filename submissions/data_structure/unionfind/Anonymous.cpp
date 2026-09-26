// https://judge.yosupo.jp/problem/unionfind
// sigma@iki.fi
#pragma GCC optimize("Ofast,inline,unroll-loops")
#include <bits/stdc++.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define FAST_FWRITE fwrite_unlocked

struct FastInput {
    alignas(64) const unsigned char* buf;
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

    uint32_t read() noexcept {
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
        }
        c += 4;
        d = (d * 10 +  (d >> 8))  & 0x00ff00ffu;
        d = (d * 100 + (d >> 16)) & 0x0000ffffu;
        unsigned char b;
        while ((b = *c++) >= '0')
            d = d * 10 + b - '0';
        cur = c;
        return (uint32_t)d;
    }
};

struct FastOutput {
    alignas(64) char* buf;
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
};

struct DSU {
    alignas(64) int32_t* p = nullptr;
    int32_t n = 0;

    DSU() = default;
    DSU(int32_t n_, int32_t* storage) noexcept { init(n_, storage); }
    
    inline constexpr void init(int32_t n_, int32_t* storage) noexcept {
        n = n_;
        p = storage;
        for (int i = 0; i < n_; i += sizeof(uint64_t)/ sizeof(int32_t))
            *(int64_t*) &p[i] = -1;
    }

    inline int32_t find(int32_t x) noexcept {
        int32_t* const __restrict__ pp = p;
        return pp[x] < 0 ? x : pp[x] = find(pp[x]);
    }

    inline constexpr void unite(int32_t a, int32_t b) noexcept {
        a = find(a); b = find(b);
        if (a == b) return;
        int32_t* const pp = p;
        const int32_t pa = pp[a];
        const int32_t pb = pp[b];
        if (pa > pb) {
            pp[b] = pb + pa;
            pp[a] = b;
        } else {
            pp[a] = pa + pb;
            pp[b] = a;
        }
    }

    inline constexpr bool same(int32_t a, int32_t b) noexcept {
        return find(a) == find(b);
    }
};

using namespace std;

int main() noexcept {
    FastInput in;
    uint32_t N, Q;
    N = in.read();
    Q = in.read();
    int32_t dsu_storage[250000];
    char outbuf[250000];
    alignas(64) DSU dsu(N, dsu_storage);
    alignas(64) FastOutput out(outbuf, sizeof outbuf);
    for (int i = 0; i < sizeof outbuf; i += sizeof(uint64_t))
        *(uint64_t*) &outbuf[i] = 0x0a300a300a300a30ull;
    int q = 0;
    while (Q--) {
        char c = *in.cur - '0';
        in.cur += 2;
        int32_t a = in.read();
        int32_t b = in.read();
        if (c) {
            q += 2;
            if (dsu.same(a, b))
                outbuf[q - 2] = '1';
        }
        else
            dsu.unite(a, b);
    }
    out.cur += q;
    return 0;
}
