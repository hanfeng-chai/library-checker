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

    fun skip(int n) -> void { p += n; }

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
#line 2 "/home/chai/workspaces/solutions/sol/tree/dynamic_tree_vertex_add_path_sum/main.cpp"

namespace {

constexpr int N = 2e5 + 1;
struct Node {
    u64 val;
    u64 sum;
    int ch[2];
    int p;
    i8 rev;
    i8 k = -1;
} node[N];

fun reverse(int x) -> void { node[x].rev ^= 1; }

fun pushdown(int x) -> void {
    assert(x);
    if (node[x].rev) {
        std::swap(node[x].ch[0], node[x].ch[1]);
        node[node[x].ch[0]].k = 0;
        node[node[x].ch[1]].k = 1;
        reverse(node[x].ch[0]);
        reverse(node[x].ch[1]);
        node[x].rev = 0;
    }
}

fun maintain(int x) -> void {
    assert(x);
    node[x].sum =
        node[node[x].ch[0]].sum + node[x].val + node[node[x].ch[1]].sum;
}

fun rotateup(int x) -> void {
    assert(x);
    int p = node[x].p;
    int g = node[p].p;
    int k = node[x].k;
    int t = node[p].k;
    assert(p);
    node[node[x].ch[k ^ 1]].p = p;
    node[node[x].ch[k ^ 1]].k = k;
    node[p].ch[k] = node[x].ch[k ^ 1];
    node[p].p = x;
    node[p].k = k ^ 1;
    node[x].ch[k ^ 1] = p;
    node[x].p = g;
    node[x].k = t;
    if (t != -1) {
        node[g].ch[t] = x;
    }
    maintain(p);
}

fun is_root(int x) -> bool {
    assert(x);
    return node[x].k == -1;
}

fun splay(int x) -> void {
    assert(x);
    pushdown(x);
    while (!is_root(x)) {
        if (int p = node[x].p; is_root(p)) {
            pushdown(p);
            pushdown(x);
            rotateup(x);
        } else {
            int g = node[p].p;
            pushdown(g);
            pushdown(p);
            pushdown(x);
            if (node[x].k == node[p].k) {
                rotateup(p);
                rotateup(x);
            } else {
                rotateup(x);
                rotateup(x);
            }
        }
    }
}

fun access(int x) -> void {
    splay(x);
    node[node[x].ch[1]].k = -1;
    node[x].ch[1] = 0;
    while (int p = node[x].p) {
        splay(p);
        node[node[p].ch[1]].k = -1;
        node[p].ch[1] = x;
        node[x].k = 1;
        rotateup(x);
    }
    maintain(x);
}

}  // namespace

int main() {
    rd rd;
    wt wt;
    int n = rd.uh();
    int q = rd.uh();
    for (int i = 1; i <= n; ++i) node[i].sum = node[i].val = rd.uw();
    for (int i = 2; i <= n; ++i) {
        int u = rd.uh() + 1;
        int v = rd.uh() + 1;
        access(u);
        reverse(u);
        node[u].p = v;
    }
    for (int i = 1; i <= q; ++i) {
        switch (rd.u1()) {
            case 0: {
                int u = rd.uh() + 1;
                int v = rd.uh() + 1;
                access(u);
                reverse(u);
                access(v);
                node[node[v].ch[0]].p = 0;
                node[node[v].ch[0]].k = -1;
                node[v].ch[0] = 0;
                maintain(v);
                int w = rd.uh() + 1;
                int x = rd.uh() + 1;
                access(w);
                reverse(w);
                node[w].p = x;
                break;
            }
            case 1: {
                int u = rd.uh() + 1;
                u32 x = rd.uw();
                splay(u);
                node[u].val += x;
                node[u].sum += x;
                break;
            }
            case 2: {
                int u = rd.uh() + 1;
                int v = rd.uh() + 1;
                access(u);
                reverse(u);
                access(v);
                wt.ud(node[node[v].ch[0]].sum + node[v].val);
                break;
            }
            default:
                std::unreachable();
        }
    }
}
