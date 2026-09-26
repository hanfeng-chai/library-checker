#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

using uint = unsigned int;

constexpr int MAXV = 1000005;
constexpr int IN_BUF = 1 << 20;
constexpr int OUT_BUF = 1 << 23;

alignas(64) uint nxt[MAXV][26];
uint par[MAXV];
uint link_node[MAXV];
uint mask_node[MAXV];
uint terminal_node[MAXV];
uint que[MAXV];

class FastInput {
    char buf[IN_BUF];
    int pos = 0;
    int len = 0;

public:
    inline char get() {
        if (__builtin_expect(pos == len, 0)) {
            len = fread(buf, 1, IN_BUF, stdin);
            pos = 0;
        }
        return buf[pos++];
    }

    inline int read_int() {
        char c = get();
        while (c < '0' || c > '9') c = get();

        int x = 0;
        do {
            x = x * 10 + c - '0';
            c = get();
        } while ('0' <= c && c <= '9');

        return x;
    }

    inline void insert_string(int id, uint& node_count) {
        char c = get();
        while (c < 'a' || c > 'z') c = get();

        uint v = 0;

        do {
            uint x = uint(c - 'a');
            uint u = nxt[v][x];

            if (__builtin_expect(u == 0, 0)) {
                u = node_count++;
                nxt[v][x] = u;
                par[u] = v;
                mask_node[v] |= 1u << x;
            }

            v = u;
            c = get();
        } while ('a' <= c && c <= 'z');

        terminal_node[id] = v;
    }
};

class FastOutput {
    char buf[OUT_BUF];
    int pos = 0;

public:
    ~FastOutput() {
        flush();
    }

    inline void flush() {
        fwrite(buf, 1, pos, stdout);
        pos = 0;
    }

    inline void write_uint(uint x, char end) {
        if (__builtin_expect(pos > OUT_BUF - 16, 0)) flush();

        char s[10];
        int n = 0;

        do {
            s[n++] = char('0' + x % 10);
            x /= 10;
        } while (x);

        while (n) buf[pos++] = s[--n];
        buf[pos++] = end;
    }
};

int main() {
    FastInput in;
    FastOutput out;

    int N = in.read_int();
    uint node_count = 1;

    for (int i = 0; i < N; ++i) {
        in.insert_string(i, node_count);
    }

    uint head = 0;
    uint tail = 0;

    uint root_mask = mask_node[0];

    while (root_mask) {
        uint c = __builtin_ctz(root_mask);
        root_mask &= root_mask - 1;
        que[tail++] = nxt[0][c];
    }

    while (head < tail) {
        uint v = que[head++];
        uint f = link_node[v];
        uint m = mask_node[v];

        uint child[26];
        uint x = m;

        while (x) {
            uint c = __builtin_ctz(x);
            x &= x - 1;

            uint u = nxt[v][c];
            child[c] = u;
            link_node[u] = nxt[f][c];
            que[tail++] = u;
        }

        memcpy(nxt[v], nxt[f], sizeof(nxt[v]));

        while (m) {
            uint c = __builtin_ctz(m);
            m &= m - 1;
            nxt[v][c] = child[c];
        }
    }

    out.write_uint(node_count, '\n');

    for (uint v = 1; v < node_count; ++v) {
        out.write_uint(par[v], ' ');
        out.write_uint(link_node[v], '\n');
    }

    for (int i = 0; i < N; ++i) {
        out.write_uint(terminal_node[i], i + 1 == N ? '\n' : ' ');
    }

    return 0;
}