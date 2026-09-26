// https://judge.yosupo.jp/problem/minimum_spanning_tree
// Kruskal implementation - sigma@iki.fi
#pragma GCC optimize("Ofast,inline,unroll-loops")
#include <bits/stdc++.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define FAST_FWRITE fwrite_unlocked

struct FastInput {
    char* buf;
    char* cur;
    size_t map_size;

    FastInput() {
        struct stat st;
        const int fd = fileno(stdin);
        fstat(fd, &st);
        map_size = st.st_size;
        buf = cur = static_cast<char*>(
            mmap(nullptr, map_size, PROT_READ, MAP_PRIVATE, fd, 0));
//        madvise(const_cast<char*>(buf), map_size, MADV_SEQUENTIAL);
//        madvise(const_cast<char*>(buf), map_size, MADV_WILLNEED);
    }
    ~FastInput() { munmap(buf, map_size); }

    uint32_t read0() {
        uint32_t d;
        uint32_t x = *cur++;
        memcpy(&d, cur, 4);
        x -= '0';
        d ^= 0x30303030;
        if (!(d & 0xf0f0f0f0)) {
            cur += 4;
            d = (d * 10 +  (d >> 8))  & 0xff00ff;
            d = (d * 100 + (d >> 16)) & 0xffff;
            x = x * 10000 + d;
        }
        unsigned char c;
        for (int i = 0; ((c = *cur++) >= '0') && (i < 3); i++)
            x = x * 10 + c - '0';
        return x;
    }

    uint32_t read1() {
        uint64_t d;
        uint32_t x = *cur++;
        memcpy(&d, cur, 8);
        x -= '0';
        d ^= 0x3030303030303030ull;
        if (!(d & 0xf0f0f0f0f0f0f0f0ull)) {
            cur += 8;
            d = (d * 10 +    (d >> 8))  & 0xff00ff00ff00ffull;
            d = (d * 100 +   (d >> 16)) & 0xffff0000ffffull;
            d = (d * 10000 + (d >> 32)) & 0xffffffffull;
            x = x * 100000000 + d; 
        } else if (!(d & 0xf0f0f0f0)) {
            cur += 4;
            d = (d * 10 +  (d >> 8))  & 0xff00ff;
            d = (d * 100 + (d >> 16)) & 0xffff;
            x = x * 10000 + d;
        }
        unsigned char c;
        for (int i = 0; ((c = *cur++) >= '0') && (i < 3); i++)
            x = x * 10 + c - '0';
        return x;
    }
};

struct FastOutput {
    char* buf;
    char* cur;
    char* end;

    FastOutput(char* p, size_t n) : buf(p), cur(p), end(p + n) {}
    ~FastOutput() { flush(); }

    void ensure(size_t need) {
        if (size_t(end - cur) >= need) [[likely]] return;
        flush();
    }

    void flush() {
        size_t n = size_t(cur - buf);
        if (n) { FAST_FWRITE(buf, 1, n, stdout); cur = buf; }
    }

    inline void write(char c) { /*ensure(1);*/ *cur++ = c; }

    template <class T>
    void write(T u) {
        __builtin_prefetch(cur + 16, 1, 1); // prefetch write target
        static_assert(std::is_integral_v<T>);
        ensure(25);

        constexpr uint64_t B = 1000000ull;
        if (u >= B)
        {
            if (u >= B * B) {
                uint64_t v = (uint64_t)u;
                // Faster than 64/64 division on most x86-64; won't reach 0
                uint64_t q0 = __builtin_expect(v >= B * B, 0) ? v / B : 0;
                uint32_t r0 = (uint32_t)(v - q0 * B);
                uint64_t q1 = q0 / B;
                uint32_t r1 = (uint32_t)(q0 - q1 * B);
                writeUIntUpTo6((uint32_t)q1);
                writePadded6(r1);
                writePadded6(r0);
            } else [[likely]] {
                uint64_t q = (uint64_t)u / B;
                uint32_t r = (uint32_t)((uint64_t)u - q * B);
                writeUIntUpTo6((uint32_t)q);
                writePadded6(r);
            }
        } else [[likely]] writeUIntUpTo6((uint32_t)u);
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
            t[i].len = (i > 99) + (i > 9) + 1;
        }
        return t;
    }();

    void writeUIntUpTo6(const uint32_t x) {
        const uint32_t y = x / 1000;
        const Entry& e = table[y ? y : x];
        const size_t len = e.len;
        memcpy(cur, &e.padded[3 - len], len); cur += len;
        if (y) {
            memcpy(cur, table[x % 1000].padded, 3); cur += 3;
        }
    }

    void writePadded6(const uint32_t x) {
        memcpy(cur,     table[x / 1000].padded, 3);
        memcpy(cur + 3, table[x % 1000].padded, 3);
        cur += 6;
    }
};

struct DSU {
    int* p = nullptr;
    int n = 0;

    DSU() = default;
    DSU(int n_, int* storage) { init(n_, storage); }
    
    inline constexpr void init(int n_, int* storage) {
        n = n_;
        p = storage;
        memset(p, 0xFF, size_t(n) * sizeof(int));
    }

    inline constexpr int find(int x) {
        int* pp = p;
        while (pp[x] >= 0) {
            int px = pp[x];
            int gx = pp[px];
            if (gx >= 0) pp[x] = gx;
            x = px;
        }
        return x;
    }

    inline constexpr bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;

        int* pp = p;
        int pa = pp[a];
        int pb = pp[b];

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

    template <typename T, uint32_t N = 16, typename It, typename F>
    void radix_sort_u32(It f, int n, uint64_t* tmp, F&& func) {
        static constexpr uint32_t Mask = (1u << N) - 1;

        if (static_cast<uint32_t>(n) < Mask / 8) {
            std::stable_sort(f, f + n, [&](const T& x, const T& y) {
                return func(x) < func(y);
            });
            return;
        } 
        std::array<uint32_t, Mask + 1> cnt1{}, cnt2{};
        for (int i = 0; i < n; ++i) {
            uint32_t val = func(f[i]);
            ++cnt1[val & Mask];
            ++cnt2[val >> N];
        }
        for (uint32_t i = 0; i < Mask; ++i) {
            cnt1[i + 1] += cnt1[i];
            cnt2[i + 1] += cnt2[i];
        }
        for (int i = n - 1; i >= 0; --i) {
            tmp[--cnt1[func(f[i]) & Mask]] = std::move(f[i]);
        }
        for (int i = n - 1; i >= 0; --i) {
            f[--cnt2[func(tmp[i]) >> N]] = std::move(tmp[i]);
    }
}

using namespace std;

int main() {
    FastInput in;
    uint32_t N, M;
    N = in.read0();
    M = in.read0();
    const int n = N, m = M;
    vector<uint64_t> id(m);
    const size_t edge_u32_count = 5 * m;
    const size_t bytes_edges = edge_u32_count * sizeof(uint32_t);
    const size_t bytes_dsu = n * sizeof(int);
    const size_t total_bytes = bytes_edges + bytes_dsu;
    alignas(64) char raw[total_bytes];
    uint64_t* tmp = reinterpret_cast<uint64_t*>(raw);
    int* dsu_storage = reinterpret_cast<int*>(tmp + m);
    uint32_t* eu  = reinterpret_cast<uint32_t*>(dsu_storage + n);
    uint32_t* ev  = eu  + m;
    uint32_t* mst = reinterpret_cast<uint32_t*>(ev + m);

    for (int i = 0; i < m; ++i) {
        eu[i] = in.read0();
        ev[i] = in.read0();
        uint32_t c;
        c = in.read1();
        id[i] = static_cast<uint64_t>(i) << 32 | c;
    }
    radix_sort_u32<uint64_t, 15>(id.data(), m, tmp, [&](auto x) {
        return static_cast<uint32_t>(x);
    });

    DSU dsu(n, dsu_storage);
    int cnt = 0;
    uint64_t ans = 0;
    for (auto x : id) {
        uint32_t i = static_cast<uint32_t>(x >> 32);
        if (dsu.unite(eu[i], ev[i])) {
            ans += static_cast<uint32_t>(x);
            mst[cnt++] = i;
        }
        if (cnt == n - 1) break;
    }
    if(cnt > 0) {
        size_t out_bufsize = mst - (uint32_t*)tmp;
        if (out_bufsize > (1 << 20))
            out_bufsize = 1 << 20;
        FastOutput out(raw, out_bufsize);
        out.write(ans);
        out.write('\n');
        out.write(mst[0]);
        for (int i = 1; i < cnt; i++) {
            out.write(' ');
            out.write(mst[i]);
        }
        out.write('\n');
    }
    return 0;
}
