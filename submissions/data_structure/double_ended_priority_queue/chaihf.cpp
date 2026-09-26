#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <array>
#include <climits>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <utility>
using u64 = uint64_t;
using u32 = uint32_t;
using i64 = int64_t;

static constexpr auto LUT0 = [] {
    std::array<std::array<char, 4>, 10000> res;
    for (int i = 0; i < 10000; ++i) {
        res[i][0] = '0' + i / 1000;
        res[i][1] = '0' + i / 100 % 10;
        res[i][2] = '0' + i / 10 % 10;
        res[i][3] = '0' + i % 10;
        if (i < 1000) res[i][0] = ' ';
        if (i < 100) res[i][1] = ' ';
        if (i < 10) res[i][2] = ' ';
        if (100 <= i && i < 1000) res[i][0] = '-';
        if (10 <= i && i < 100) res[i][1] = '-';
        if (1 <= i && i < 10) res[i][2] = '-';
        if (i == 0) res[i][3] = '-';
    }
    return res;
}();

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

int a[1000000];

bool try_swap(int& x, int& y) {
    if (x + y >= 0) {
        int t = x;
        x = ~y;
        y = ~t;
        return true;
    }
    return false;
}

bool pushup_once(int i) {
    [[assume(i > 0)]];
    return try_swap(a[i / 2], a[i]);
}

void pushup(int i) {
    if (i <= 1) return;
    if (pushup_once(i)) i /= 2;
    while (a[i / 4] > a[i]) std::swap(a[i / 4], a[i]), i /= 4;
}

void pushdown_once(int i) {
    [[assume(i > 0)]];
    int j = a[i * 2] > a[i * 2 + 1] ? i * 2 : i * 2 + 1;
    pushup_once(j);
}

void pushdown(int i, int n) {
    int value = a[i];
    while (i * 4 + 3 <= n) {
        u64 x = i * 4;
        u64 y = i * 4 + 1;
        u64 z = i * 4 + 2;
        u64 w = i * 4 + 3;
        x = (u64)a[x] << 32 | x;
        y = (u64)a[y] << 32 | y;
        z = (u64)a[z] << 32 | z;
        w = (u64)a[w] << 32 | w;
        u64 min = std::min(std::min(x, y), std::min(z, w));
        int j = min & INT_MAX;
        int value_j = min >> 32;
        if (value_j >= value) {
            a[i] = value;
            return;
        }
        try_swap(a[j / 2], value);
        a[i] = a[j];
        i = j;
    }
    a[i] = value;
    if (i <= n / 4) {
        a[n + 1] = a[n + 2] = a[n + 3] = INT_MAX;
        i64 x = i * 4;
        i64 y = i * 4 + 1;
        i64 z = i * 4 + 2;
        i64 w = i * 4 + 3;
        x = (i64)a[x] << 32 | x;
        y = (i64)a[y] << 32 | y;
        z = (i64)a[z] << 32 | z;
        w = (i64)a[w] << 32 | w;
        i64 min = std::min(std::min(x, y), std::min(z, w));
        int j = min & INT_MAX;
        int value_j = min >> 32;
        if (value_j < a[i]) {
            std::swap(a[i], a[j]);
            pushup_once(j);
        }
    }
    if (i * 2 <= n) {
        a[n + 1] = INT_MIN;
        pushdown_once(i);
    }
}

void initialize(int n) {
    a[0] = INT_MIN;
    if (n <= 1) return;
    pushup_once(n);
    for (int i = (n - 1) / 2; i > n / 4; --i) pushdown_once(i);
    for (int i = n / 4; i > 0; --i) pushdown(i, n);
}

int main() {
    struct stat st;
    fstat(0, &st);
    char* I = (char*)mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
    const auto i1 = [&]() {
        char value = *I - 48;
        I += 2;
        return value;
    };
    const auto ii = [&]() {
        bool neg{};
        if (*I == '-') neg = 1, ++I;
        u64 a;
        u32 x{};
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
        return neg ? -x : x;
    };
    char bufO[1 << 19], *ptrO = bufO, *endO = bufO + sizeof(bufO);
    auto flush = [&]() {
        write(1, bufO, ptrO - bufO);
        ptrO = bufO;
    };
    const auto print0 = [&](u32 x) { memcpy(ptrO, &LUT0[x], 4), ptrO += 4; };
    const auto print1 = [&](u32 x) { memcpy(ptrO, &LUT1[x], 4), ptrO += 4; };
    const auto print2 = [&](u32 x) { memcpy(ptrO, &LUT2[x], 4), ptrO += 4; };
    const auto oop = [&](u32 x) {
        if (x > 9999'9999) {
            print1(x / 10000 / 10000);
            print2(x / 10000 % 10000);
            print2(x % 10000);
        } else if (x > 9999) {
            print1(x / 10000);
            print2(x % 10000);
        } else {
            print1(x);
        }
    };
    const auto oon = [&](u32 x) {
        if (x > 999'9999) {
            print0(x / 10000 / 10000);
            print2(x / 10000 % 10000);
            print2(x % 10000);
        } else if (x > 999) {
            print0(x / 10000);
            print2(x % 10000);
        } else {
            print0(x);
        }
    };
    const auto oo = [&](int x) {
        if (endO - ptrO < 40) flush();
        if (x < 0)
            oon(-x);
        else
            oop(x);
        *ptrO = '\n', ptrO++;
    };
    int n = ii();
    int q = ii();
    if (n == 0) ++I;
    constexpr int N = 1e9;
    for (int i = 1; i <= n; ++i) {
        int x = ii() + N;
        a[i] = (__builtin_clz(i) & 1) ? x : ~x;
    }
    initialize(n);
    for (int i = 1; i <= q; ++i) {
        switch (i1()) {
            case 0: {
                int x = ii() + N;
                ++n;
                a[n] = (__builtin_clz(n) & 1) ? x : ~x;
                pushup(n);
                break;
            }
            case 1: {
                oo(a[1] - N);
                a[1] = (__builtin_clz(n) & 1) ? a[n] : ~a[n];
                pushdown(1, --n);
                break;
            }
            case 2: {
                if (n == 1) {
                    oo(a[n--] - N);
                    continue;
                }
                if (n == 2) {
                    oo(~a[n--] - N);
                    continue;
                }
                int k = a[2] < a[3] ? 2 : 3;
                oo(~a[k] - N);
                a[k] = (__builtin_clz(n) & 1) ? ~a[n] : a[n];
                pushdown(k, --n);
                break;
            }
            default:
                std::unreachable();
        }
    }
    flush();
}
