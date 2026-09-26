#define NDEBUG
#pragma GCC optimize("O3")
#line 2 "/home/chai/workspaces/solutions/include/io.h"

#include <sys/mman.h>
#include <sys/stat.h>
#line 2 "/home/chai/workspaces/solutions/include/common.h"

#include <bits/extc++.h>

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using i128 = __int128;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using u128 = unsigned __int128;

using f32 = float;
using f64 = double;
using f80 = long double;

#define fun auto
#define def auto
#define let const auto
#line 6 "/home/chai/workspaces/solutions/include/io.h"

struct rd {
    static constexpr fun all_digit(u32 x) {
        x ^= 0x30303030;
        x &= 0xf0f0f0f0;
        return !x;
    }
    static constexpr fun all_digit(u64 x) {
        x ^= 0x3030303030303030;
        x &= 0xf0f0f0f0f0f0f0f0;
        return !x;
    }

    char* p;
    rd() {
        struct stat st;
        fstat(0, &st);
        p = (char*)mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
    }

    fun u1() -> u32 {
        u32 x = *p++ - '0';
        return ++p, x;
    }

    fun ub() -> u32 {
        u32 x = *p++ - '0';
        for (; *p >= '0'; ++p) x = x * 10 + *p - '0';
        return ++p, x;
    }

    fun uh() -> u32 {
        u32 x{};
        u32 a;
        memcpy(&a, p, sizeof(a));
        if (all_digit(a)) {
            a ^= 0x30303030;
            a = (a * 10 + (a >> 8)) & 0x00ff00ff;
            a = (a * 100 + (a >> 16)) & 0x0000ffff;
            x = a, p += sizeof(a);
        }
        for (; *p >= '0'; ++p) x = x * 10 + *p - '0';
        return ++p, x;
    }

    fun uw() -> u32 {
        u32 x{};
        u64 a;
        memcpy(&a, p, sizeof(a));
        if (all_digit(a)) {
            a ^= 0x3030303030303030;
            a = (a * 10 + (a >> 8)) & 0x00ff00ff00ff00ff;
            a = (a * 100 + (a >> 16)) & 0x0000ffff0000ffff;
            a = (a * 10000 + (a >> 32)) & 0x00000000ffffffff;
            x = a, p += sizeof(a);
        }
        for (; *p >= '0'; ++p) x = x * 10 + *p - '0';
        return ++p, x;
    }

    fun ud() -> u64 {
        u64 x{};
        union {
            char ch[16];
            u64 d[2];
        };
        memcpy(ch, p, sizeof(ch));
        u64 a = d[0], b = d[1];
        if (all_digit(a) && all_digit(b)) {
            a ^= 0x3030303030303030;
            b ^= 0x3030303030303030;
            a = (a * 10 + (a >> 8)) & 0x00ff00ff00ff00ff;
            b = (b * 10 + (b >> 8)) & 0x00ff00ff00ff00ff;
            a = (a * 100 + (a >> 16)) & 0x0000ffff0000ffff;
            b = (b * 100 + (b >> 16)) & 0x0000ffff0000ffff;
            a = (a * 10000 + (a >> 32)) & 0x00000000ffffffff;
            b = (b * 10000 + (b >> 32)) & 0x00000000ffffffff;
            x = a * 100000000 + b, p += sizeof(ch);
        }
        for (; *p >= '0'; ++p) x = x * 10 + *p - '0';
        return ++p, x;
    }

    fun i1() -> i32 { return *p == '-' ? ++p, -u1() : u1(); }
    fun ib() -> i32 { return *p == '-' ? ++p, -ub() : ub(); }
    fun ih() -> i32 { return *p == '-' ? ++p, -uh() : uh(); }
    fun iw() -> i32 { return *p == '-' ? ++p, -uw() : uw(); }
    fun id() -> i64 { return *p == '-' ? ++p, -ud() : ud(); }
};

struct wt {
    static constexpr def low = [] {
        std::array<std::array<char, 4>, 10000> res;
        for (int i = 0; i < 10000; ++i) {
            res[i][0] = '0' + i / 1000;
            res[i][1] = '0' + i / 100 % 10;
            res[i][2] = '0' + i / 10 % 10;
            res[i][3] = '0' + i % 10;
        }
        return res;
    }();
    static constexpr def pos = [] {
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

    inline static char buf[1 << 20];
    static constexpr char* sentinel = buf + sizeof(buf) - 50;
    char* p = buf;
    ~wt() { flush(); }
    void flush() { write(1, buf, p - buf), p = buf; }
    void print_low(u64 x) { memcpy(p, &low[x], 4), p += 4; }
    void print_pos(u64 x) { memcpy(p, &pos[x], 4), p += 4; }

    void puts(const char* src, int n) {
        if (sentinel < p + n) flush();
        memcpy(p, src, n), p += n;
    }

    void uw(u32 x) {
        if (sentinel < p) flush();
        *p++ = ' ';
        if (x > 9999'9999) {
            print_pos(x / 10000 / 10000);
            print_low(x / 10000 % 10000);
            print_low(x % 10000);
        } else if (x > 9999) {
            print_pos(x / 10000);
            print_low(x % 10000);
        } else {
            print_pos(x);
        }
    }

    void ud(u64 x) {
        if (sentinel < p) flush();
        *p++ = ' ';
        if (x > 9999'9999'9999'9999) {
            print_pos(x / 10000 / 10000 / 10000 / 10000);
            print_low(x / 10000 / 10000 / 10000 % 10000);
            print_low(x / 10000 / 10000 % 10000);
            print_low(x / 10000 % 10000);
            print_low(x % 10000);
        } else if (x > 9999'9999'9999) {
            print_pos(x / 10000 / 10000 / 10000);
            print_low(x / 10000 / 10000 % 10000);
            print_low(x / 10000 % 10000);
            print_low(x % 10000);
        } else if (x > 9999'9999) {
            print_pos(x / 10000 / 10000);
            print_low(x / 10000 % 10000);
            print_low(x % 10000);
        } else if (x > 9999) {
            print_pos(x / 10000);
            print_low(x % 10000);
        } else {
            print_pos(x);
        }
    }
};
#line 2 "/home/chai/workspaces/solutions/sol/tree/vertex_add_path_sum/main.cpp"

namespace {

constexpr int N = 5e5;
int head[N];
struct {
    int to;
    int next;
} edge[N * 2];
int p[N];
int l[N];
int r[N];
int a[N];
u64 b[N + 1];
u64 c[N + 2];
int id;

void dfs(int u, int w) {
    p[u] = w;
    l[u] = ++id;
    c[id] += b[id] = a[u];
    for (int e = head[u]; e; e = edge[e].next) {
        int v = edge[e].to;
        if (v != w) {
            dfs(v, u);
        }
    }
    c[r[u] = id + 1] -= a[u];
}

}  // namespace

int main() {
    rd rd;
    wt wt;
    int n = rd.uh();
    int q = rd.uh();
    for (int i = 0; i < n; ++i) a[i] = rd.uw();
    for (int i = 1; i < n; ++i) {
        int u = rd.uh();
        int v = rd.uh();
        edge[i * 2 | 0] = {v, head[u]}, head[u] = i * 2 | 0;
        edge[i * 2 | 1] = {u, head[v]}, head[v] = i * 2 | 1;
    }
    dfs(0, -1);
    u64 sum = 0;
    for (int i = 1; i <= n; ++i) c[i] = sum += c[i];
    for (int i = n; i >= 1; --i) c[i] -= c[i - (i & -i)];
    int val[n + 1];
    for (int i = 1; i < n; ++i) val[l[i]] = l[p[i]];
    ++n;
    int m = n / 64 + 1;
    int len = std::__lg(m) + 1;
    int table[len][m];
    int suf[n];
    int pre[n];
    for (int min, i = 0; i < n; ++i) {
        int id = i & 63;
        int value = pre[i] = val[i];
        suf[i] = min = id ? std::min(min, value) : value;
        if (id == 63) table[0][i / 64] = suf[i];
    }
    for (int min, i = n - 2; i >= 0; --i) {
        int id = ~i & 63;
        int value = pre[i];
        pre[i] = min = id ? std::min(min, value) : value;
    }
    for (int i = 1; i < len; ++i) {
        for (int j = 0, k = 1 << (i - 1); k < m; ++j, ++k) {
            table[i][j] = std::min(table[i - 1][j], table[i - 1][k]);
        }
    }
    for (int i = 1; i <= q; ++i) {
        switch (rd.u1()) {
            case 0: {
                int k = rd.uh();
                int x = rd.uw();
                int u = l[k];
                int v = r[k];
                b[u] += x;
                for (; u <= n; u += u & -u) c[u] += x;
                for (; v <= n; v += v & -v) c[v] -= x;
                break;
            }
            case 1: {
                int u = l[rd.uh()];
                int v = l[rd.uh()];
                if (u == v) {
                    wt.ud(b[u]);
                    break;
                }
                int l = std::min(u, v) + 1;
                int r = std::max(u, v);
                int L = l / 64;
                int R = r / 64;
                int w;
                if (L < R - 1) {
                    int p = pre[l];
                    int s = suf[r];
                    w = std::min(p, s);
                    int k = std::__lg(R - L - 1);
                    int a = table[k][L + 1];
                    int b = table[k][R - (1 << k)];
                    int tmp = std::min(a, b);
                    w = std::min(w, tmp);
                } else if (L == R - 1) {
                    int p = pre[l];
                    int s = suf[r];
                    w = std::min(p, s);
                } else {
                    w = val[l];
                    for (int i = l + 1; i <= r; ++i) w = std::min(w, val[i]);
                }
                u64 sum = b[w];
                if (--l == w) {
                    for (; l; l -= l & -l) sum -= c[l];
                    for (; r; r -= r & -r) sum += c[r];
                } else {
                    for (; l; l -= l & -l) sum += c[l];
                    for (; r; r -= r & -r) sum += c[r];
                    for (; w; w -= w & -w) sum -= c[w] * 2;
                }
                wt.ud(sum);
                break;
            }
            default:
                std::unreachable();
        }
    }
}
