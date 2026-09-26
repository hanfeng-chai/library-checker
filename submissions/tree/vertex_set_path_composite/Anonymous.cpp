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
#line 2 "/home/chai/workspaces/solutions/sol/tree/vertex_set_path_composite/main.cpp"

namespace {

const int N = 2e5;
const u32 P = 998244353;
struct Node {
    u32 a, b, c;
    Node operator+(const Node& t) const {
        u32 x = u64(a) * t.a % P;
        u32 y = (u64(a) * t.b + b) % P;
        u32 z = (u64(t.a) * c + t.c) % P;
        return {x, y, z};
    }
} node[N * 2];
u32 operator+(const Node& t, u32 x) { return (u64(t.a) * x + t.b) % P; }
u32 operator+(u32 x, const Node& t) { return (u64(t.a) * x + t.c) % P; }
u32 a[N];
u32 b[N];
int head[N];
int size[N];
int depth[N];
int heavy[N];
int ances[N];
int parent[N];
int node2id[N];
int id;
struct {
    int to;
    int next;
} edge[N * 2];

fun build_step_1(int u, int p) -> void {
    size[u] = 1;
    for (int e = head[u]; e; e = edge[e].next) {
        int v = edge[e].to;
        if (v != p) {
            build_step_1(v, u);
            size[u] += size[v];
            if (heavy[u] == 0 || size[v] > size[heavy[u]]) {
                heavy[u] = v;
            }
        }
    }
}

fun build_step_2(int u, int w, int p, int d) -> void {
    int i = id++;
    node2id[u] = i;
    node[i] = {a[u], b[u], b[u]};
    depth[i] = d;
    ances[i] = node2id[w];
    parent[i] = node2id[p];
    if (int v = heavy[u]; v) {
        build_step_2(v, w, u, d + 1);
    }
    for (int e = head[u]; e; e = edge[e].next) {
        int v = edge[e].to;
        if (v != p && v != heavy[u]) {
            build_step_2(v, v, u, d + 1);
        }
    }
}

}  // namespace

int main() {
    rd rd;
    wt wt;
    let n = rd.uh();
    let q = rd.uh();
    for (int i = 0; i < n; ++i) a[i] = rd.uw(), b[i] = rd.uw();
    for (int i = 1; i < n; ++i) {
        int u = rd.uh();
        int v = rd.uh();
        edge[i * 2 | 0] = {v, head[u]}, head[u] = i * 2 | 0;
        edge[i * 2 | 1] = {u, head[v]}, head[v] = i * 2 | 1;
    }
    build_step_1(0, 0);
    build_step_2(0, 0, 0, 0);
    memcpy(node + n, node, sizeof(Node) * n);
    for (int i = n - 1; i > 0; --i) node[i] = node[i * 2] + node[i * 2 + 1];
    let apply_1 = [&](int l, int r, u32 x) -> u32 {
        l += n - 1;
        r += n + 1;
        int k = std::__lg(l ^ r);
        int R = r >> k;
        for (r = r >> __builtin_ctz(r) ^ 1; r > R;
             r = r >> __builtin_ctz(r) ^ 1)
            x = node[r] + x;
        for (int t = ~l & ~(-1 << k), i; t > 0; t -= 1 << i) {
            i = std::__lg(t);
            x = node[l >> i ^ 1] + x;
        }
        return x;
    };
    let apply_2 = [&](int l, int r, u32 x) -> u32 {
        l += n - 1;
        r += n + 1;
        int k = std::__lg(l ^ r);
        int R = r >> k;
        for (l = l >> __builtin_ctz(~l) ^ 1; l > R;
             l = l >> __builtin_ctz(~l) ^ 1)
            x = x + node[l];
        for (int t = r & ~(-1 << k), i; t > 0; t -= 1 << i) {
            i = std::__lg(t);
            x = x + node[r >> i ^ 1];
        }
        return x;
    };
    for (int i = 0; i < q; ++i) {
        switch (rd.u1()) {
            case 0: {
                int k = n + node2id[rd.uh()];
                u32 c = rd.uw();
                u32 d = rd.uw();
                node[k] = {c, d, d};
                for (k /= 2; k > 0; k /= 2) {
                    node[k] = node[k * 2] + node[k * 2 + 1];
                }
                break;
            }
            case 1: {
                int u = node2id[rd.uh()];
                int v = node2id[rd.uh()];
                u32 x = rd.uw();
                if (depth[u] < 36 & depth[v] < 36) {
                    Node vec[36];
                    int c = 0;
                    while (depth[u] > depth[v]) {
                        x = node[n + u] + x;
                        u = parent[u];
                    }
                    while (depth[u] < depth[v]) {
                        vec[++c] = node[n + v];
                        v = parent[v];
                    }
                    while (u != v) {
                        x = node[n + u] + x;
                        u = parent[u];
                        vec[++c] = node[n + v];
                        v = parent[v];
                    }
                    x = node[n + u] + x;
                    for (; c > 0; --c) {
                        x = x + vec[c];
                    }
                    wt.uw(x);
                    break;
                }
                std::pair<int, int> vec[20];
                int c = 0;
                while (ances[u] != ances[v]) {
                    if (depth[ances[u]] > depth[ances[v]]) {
                        x = apply_1(ances[u], u, x);
                        u = parent[ances[u]];
                    } else {
                        vec[++c] = {ances[v], v};
                        v = parent[ances[v]];
                    }
                }
                if (u > v) {
                    x = apply_1(v, u, x);
                } else {
                    x = apply_2(u, v, x);
                }
                for (; c > 0; --c) {
                    def[l, r] = vec[c];
                    x = apply_2(l, r, x);
                }
                wt.uw(x);
                break;
            }
            default:
                std::unreachable();
        }
    }
}
