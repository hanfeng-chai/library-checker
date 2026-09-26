// https://judge.yosupo.jp/submission/267440

#include <immintrin.h>
#pragma GCC target("avx2")

#include <bits/stdc++.h>
#include <sys/mman.h>
#include <sys/stat.h>
using u64 = uint64_t;
using u32 = uint32_t;

static constexpr auto LUT1 = [] {
    std::array<int, 10000> res;
    for (int i = 0; i < 10000; ++i) {
        char ch[4];
        ch[0] = '0' + i / 1000;
        ch[1] = '0' + i / 100 % 10;
        ch[2] = '0' + i / 10 % 10;
        ch[3] = '0' + i % 10;
        if (i < 1000) ch[0] = ' ';
        if (i < 100) ch[1] = ' ';
        if (i < 10) ch[2] = ' ';
        if (i < 1) ch[3] = ' ';
        res[i] = ch[0] | ch[1] << 8 | ch[2] << 16 | ch[3] << 24;
    }
    return res;
}();

static constexpr auto LUT2 = [] {
    std::array<int, 10000> res;
    for (int i = 0; i < 10000; ++i) {
        char ch[4];
        ch[0] = '0' + i / 1000;
        ch[1] = '0' + i / 100 % 10;
        ch[2] = '0' + i / 10 % 10;
        ch[3] = '0' + i % 10;
        res[i] = ch[0] | ch[1] << 8 | ch[2] << 16 | ch[3] << 24;
    }
    return res;
}();

constexpr bool all_digit(u64 x) {
    x ^= 0x3030303030303030;
    x &= 0xf0f0f0f0f0f0f0f0;
    return !x;
}

constexpr bool all_digit(u32 x) {
    x ^= 0x30303030;
    x &= 0xf0f0f0f0;
    return !x;
}

alignas(16) u64 a[500000];

int main() {
    struct stat st;
    fstat(0, &st);
    char* I =
        (char*)mmap(nullptr, st.st_size + 64, PROT_READ, MAP_PRIVATE, 0, 0);
    const auto i1e9 = [&]() {
        u32 x{};
        u64 a;
        memcpy(&a, I, 8);
        if (all_digit(a)) {
            a ^= 0x3030303030303030;
            a = (a * 10 + (a >> 8)) & 0x00ff00ff00ff00ff;
            a = (a * 100 + (a >> 16)) & 0x0000ffff0000ffff;
            a = (a * 10000 + (a >> 32)) & 0x00000000ffffffff;
            x = a, I += 8;
        }
        for (; *I > 47; ++I) x = x * 10 + *I - 48;
        ++I;
        return x;
    };
    const auto i5e5 = [&]() {
        u32 x{};
        u32 a;
        memcpy(&a, I, 4);
        if (all_digit(a)) {
            a ^= 0x30303030;
            a = (a * 10 + (a >> 8)) & 0x00ff00ff;
            a = (a * 100 + (a >> 16)) & 0x0000ffff;
            x = a, I += 4;
        }
        for (; *I > 47; ++I) x = x * 10 + *I - 48;
        ++I;
        return x;
    };
    int bufO[1 << 17], *ptrO = bufO, *endO = bufO + sizeof(bufO) / sizeof(int);
    auto flush = [&]() {
        write(1, bufO, (ptrO - bufO) << 2);
        ptrO = bufO;
    };
    const auto print1 = [&](u64 x) { *ptrO = LUT1[x], ++ptrO; };
    const auto print2 = [&](u64 x) { *ptrO = LUT2[x], ++ptrO; };
    const auto oo = [&](u64 x) {
        if (endO - ptrO < 8) flush();
        if (x > 999'9999'9999) {
            print1(x / 10000 / 10000 / 10000);
            print2(x / 10000 / 10000 % 10000);
            print2(x / 10000 % 10000);
            print2(x % 10000);
        } else if (x > 999'9999) {
            print1(x / 10000 / 10000);
            print2(x / 10000 % 10000);
            print2(x % 10000);
        } else if (x > 999) {
            print1(x / 10000);
            print2(x % 10000);
        } else if (x > 0) {
            print1(x);
        } else {
            *ptrO = 0x30202020, ++ptrO;
        }
    };
    int n = i1e9();
    int q = i1e9();
    for (int i = 0; i < n; ++i) a[i] = i1e9();
    __m128i s = _mm_setzero_si128();
    for (int i = 0; i < n; i += 2) {
        __m128i v = _mm_load_si128((__m128i*)(a + i));
        v = _mm_add_epi64(v, _mm_slli_si128(v, 8));
        s = _mm_add_epi64(s, v);
        _mm_store_si128((__m128i*)(a + i), s);
        s = _mm_shuffle_epi32(s, 0b11101110);
    }
    while (q--) {
        int l = i5e5();
        int r = i5e5();
        oo(l ? a[--r] - a[--l] : a[--r]);
    }
    flush();
}
