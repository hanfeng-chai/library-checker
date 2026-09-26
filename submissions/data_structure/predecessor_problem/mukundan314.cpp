// https://judge.yosupo.jp/submission/267790
// + pragma
// + initialization of lv12 and lv18 with avx2
// + different integer input

#pragma GCC optimize("O3,unroll-loops")

/***
TODO:
  1. improve string input speed (use mmap?)
  1. improve data structure (speed up initialization!)
  1. improve data structure (maintain min and max?)
 */

#include <bits/stdc++.h>
#include <immintrin.h>
#include <sys/mman.h>
#include <sys/stat.h>

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;

const int N = 1e7;

static constexpr auto LUT1 = [] {
    std::array<std::array<char, 4>, 10000> res;
    for (int i = 0; i < 10000; ++i) {
        res[i][0] = '0' + i / 1000;
        res[i][1] = '0' + i / 100 % 10;
        res[i][2] = '0' + i / 10 % 10;
        res[i][3] = '0' + i % 10;
        if (i < 1000) res[i][0] = ' ';
        if (i < 100) res[i][1] = ' ';
        if (i < 10) res[i][2] = ' ';
    }
    return res;
}();

static constexpr auto LUT2 = [] {
    std::array<std::array<char, 4>, 10000> res;
    for (int i = 0; i < 10000; ++i) {
        res[i][0] = '0' + i / 1000;
        res[i][1] = '0' + i / 100 % 10;
        res[i][2] = '0' + i / 10 % 10;
        res[i][3] = '0' + i % 10;
    }
    return res;
}();

u64 root;
u64 lv18[39 + 4];      // 312B
u64 lv12[2442 + 4];    // 19KB
u64 leaf[156250 + 4];  // 1.2MB

void set(u32 x) {
    leaf[x >> 6] |= 1ull << (x & 63);
    lv12[x >> 12] |= 1ull << ((x >> 6) & 63);
    lv18[x >> 18] |= 1ull << ((x >> 12) & 63);
    root |= 1ull << (x >> 18);
}

void unset(u32 x) {
    if (!(leaf[x >> 6] &= ~(1ull << (x & 63))))
        if (!(lv12[x >> 12] &= ~(1ull << ((x >> 6) & 63))))
            if (!(lv18[x >> 18] &= ~(1ull << ((x >> 12) & 63))))
                root &= ~(1ull << (x >> 18));
}

bool get(u32 x) { return leaf[x >> 6] & (1ull << (x & 63)); }

u32 nxt(u32 x) {
    if (u64 tmp = leaf[x >> 6] & (-1ull << (x & 63)); !tmp) {
        if (u64 tmp = lv12[x >> 12] & (-2ull << ((x >> 6) & 63)); !tmp) {
            if (u64 tmp = lv18[x >> 18] & (-2ull << ((x >> 12) & 63)); !tmp) {
                if (u64 tmp = root & (-2ull << (x >> 18)); !tmp) {
                    return -1;
                } else {
                    u32 ans = __builtin_ctzll(tmp);
                    ans = ans << 6 | __builtin_ctzll(lv18[ans]);
                    ans = ans << 6 | __builtin_ctzll(lv12[ans]);
                    ans = ans << 6 | __builtin_ctzll(leaf[ans]);
                    return ans;
                }
            } else {
                u32 ans = (x >> 18) << 6 | __builtin_ctzll(tmp);
                ans = ans << 6 | __builtin_ctzll(lv12[ans]);
                ans = ans << 6 | __builtin_ctzll(leaf[ans]);
                return ans;
            }
        } else {
            u32 ans = (x >> 12) << 6 | __builtin_ctzll(tmp);
            ans = ans << 6 | __builtin_ctzll(leaf[ans]);
            return ans;
        }
    } else {
        u32 ans = (x >> 6) << 6 | __builtin_ctzll(tmp);
        return ans;
    }
}

u32 pre(u32 x) {
    if (u64 tmp = leaf[x >> 6] & ~(-2ull << (x & 63)); !tmp) {
        if (u64 tmp = lv12[x >> 12] & ~(-1ull << ((x >> 6) & 63)); !tmp) {
            if (u64 tmp = lv18[x >> 18] & ~(-1ull << ((x >> 12) & 63)); !tmp) {
                if (u64 tmp = root & ~(-1ull << (x >> 18)); !tmp) {
                    return -1;
                } else {
                    u32 ans = 63 - __builtin_clzll(tmp);
                    ans = ans << 6 | 63 - __builtin_clzll(lv18[ans]);
                    ans = ans << 6 | 63 - __builtin_clzll(lv12[ans]);
                    ans = ans << 6 | 63 - __builtin_clzll(leaf[ans]);
                    return ans;
                }
            } else {
                u32 ans = (x >> 18) << 6 | 63 - __builtin_clzll(tmp);
                ans = ans << 6 | 63 - __builtin_clzll(lv12[ans]);
                ans = ans << 6 | 63 - __builtin_clzll(leaf[ans]);
                return ans;
            }
        } else {
            u32 ans = (x >> 12) << 6 | 63 - __builtin_clzll(tmp);
            ans = ans << 6 | 63 - __builtin_clzll(leaf[ans]);
            return ans;
        }
    } else {
        u32 ans = (x >> 6) << 6 | 63 - __builtin_clzll(tmp);
        return ans;
    }
}

int main() {
    struct stat st;
    fstat(0, &st);
    char* I = (char*)mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
    const auto ii = [&]() {
        u64 raw;
        memcpy(&raw, I, 8);

        u32 bits = std::countr_zero((~raw >> 4) & 0x0101010101010101);
        raw <<= 64 - bits;
        raw &= 0x0f0f0f0f0f0f0f0f;
        raw = (raw * 10 + (raw >> 8)) & 0x00ff00ff00ff00ff;
        raw = (raw * 100 + (raw >> 16)) & 0x0000ffff0000ffff;
        raw = (raw * 10000 + (raw >> 32)) & 0x00000000ffffffff;
        I += (bits >> 3) + 1;

        return raw;
    };
    const auto i1 = [&]() {
        char value = *I;
        I += 2;
        return value - '0';
    };
    char bufO[1 << 19], *ptrO = bufO, *endO = bufO + sizeof(bufO);
    auto flush = [&]() {
        write(1, bufO, ptrO - bufO);
        ptrO = bufO;
    };
    const auto print1 = [&](u64 x) { memcpy(ptrO, &LUT1[x], 4), ptrO += 4; };
    const auto print2 = [&](u64 x) { memcpy(ptrO, &LUT2[x], 4), ptrO += 4; };
    const auto oo = [&](u32 x) {
        if (endO - ptrO < 40) flush();
        if ((int)x > 9999) {
            print1(x / 10000);
            print2(x % 10000);
        } else if (x == -1) {
            *ptrO = '-', ptrO++;
            *ptrO = '1', ptrO++;
        } else {
            print1(x);
        }
        *ptrO = ' ', ptrO++;
    };
    const auto oc = [&](char c) {
        if (endO - ptrO < 40) flush();
        *ptrO = c, ptrO++;
        *ptrO = ' ', ptrO++;
    };
    int n = ii();
    int q = ii();
    int i = 0;
    for (; i + 64 < n; i += 64) {
        u32 a = _mm256_movemask_epi8(_mm256_cmpeq_epi8(
            _mm256_loadu_si256(reinterpret_cast<const __m256i_u*>(I + i)),
            _mm256_set1_epi8('1')));
        u32 b = _mm256_movemask_epi8(_mm256_cmpeq_epi8(
            _mm256_loadu_si256(reinterpret_cast<const __m256i_u*>(I + i + 32)),
            _mm256_set1_epi8('1')));
        leaf[i >> 6] = a | u64(b) << 32;
    }
    for (; i < n; ++i) leaf[i >> 6] |= u64(I[i] - '0') << (i & 63);

    auto zero = _mm256_setzero_si256();

    for (int i = 0; i <= (N - 1) / 64; i += 4) {
      auto reg = _mm256_load_si256(reinterpret_cast<__m256i *>(&leaf[i]));
      u64 mask = _mm256_movemask_pd(__m256d(_mm256_cmpeq_epi64(reg, zero))) ^ 0b1111;
      lv12[i >> 6] |= mask << (i & 63);
    }

    for (int i = 0; i <= (N - 1) / 64 / 64; i += 4) {
      auto reg = _mm256_load_si256(reinterpret_cast<__m256i *>(&lv12[i]));
      u64 mask = _mm256_movemask_pd(__m256d(_mm256_cmpeq_epi64(reg, zero))) ^ 0b1111;
      lv18[i >> 6] |= mask << (i & 63);
    }

    for (int i = 0; i <= (N - 1) / 64 / 64 / 64; i += 4) {
      auto reg = _mm256_load_si256(reinterpret_cast<__m256i *>(&lv18[i]));
      u64 mask = _mm256_movemask_pd(__m256d(_mm256_cmpeq_epi64(reg, zero))) ^ 0b1111;
      root |= mask << (i & 63);
    }

    I += n + 1;
    for (int i = 0; i < q; ++i) {
        switch (i1()) {
            case 0: {
                set(ii());
                break;
            }
            case 1: {
                unset(ii());
                break;
            }
            case 2: {
                oc('0' + get(ii()));
                break;
            }
            case 3: {
                oo(nxt(ii()));
                break;
            }
            case 4: {
                oo(pre(ii()));
                break;
            }
            default:
                std::unreachable();
        }
    }
    flush();
}