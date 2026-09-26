#include <bits/stdc++.h>
using ll = long long;
using namespace std;

struct LinkCutTree {
    struct Node;
    using Ptr = Node*;
    struct Node {
        Ptr p, l, r;
        bool rev;
        ll key, acc;
        Node() : p(), l(), r(), rev(), key(), acc() {}
    };

    void toggle(Ptr u) {
        swap(u->l, u->r);
        u->rev ^= 1;
    }
    void flush(Ptr u) {
        if (u->rev) {
            if (u->l) toggle(u->l);
            if (u->r) toggle(u->r);
            u->rev = 0;
        }
    }
    void fetch(Ptr u) { u->acc = (u->l ? u->l->acc : 0) + u->key + (u->r ? u->r->acc : 0); }

    int dir(Ptr u) {
        Ptr p = u->p;
        if (p && p->l == u) return -1;
        if (p && p->r == u) return +1;
        return 0;
    }
    void rot(Ptr t) {
        Ptr x = t->p, y = x->p;
        if (dir(t) == -1) {
            if ((x->l = t->r)) t->r->p = x;
            t->r = x, x->p = t;
        } else {
            if ((x->r = t->l)) t->l->p = x;
            t->l = x, x->p = t;
        }
        fetch(x), fetch(t);
        if ((t->p = y)) {
            if (y->l == x) y->l = t;
            if (y->r == x) y->r = t;
        }
    }
    void splay(Ptr t) {
        flush(t);
        while (dir(t)) {
            Ptr q = t->p;
            if (!dir(q)) {
                flush(q), flush(t);
                rot(t);
            } else {
                Ptr r = q->p;
                flush(r), flush(q), flush(t);
                if (dir(q) == dir(t)) rot(q), rot(t);
                else rot(t), rot(t);
            }
        }
    }

    Ptr expose(Ptr t) {
        for (Ptr x = t, y = nullptr; x; x = x->p) {
            splay(x);
            x->r = y;
            fetch(y = x);
        }
        splay(t);
        return t;
    }

    vector<Node> pool;
    LinkCutTree(int n) : pool(n) {}
    Ptr get(int u) { return expose(&pool[u]); }

    void evert(int u) { toggle(get(u)); }            // reroot at u
    void link(int u, int v) { get(u)->p = get(v); }  // add edge from u to v
    void cut(int u) {                                // cut u and its parent
        Ptr x = get(u), y = x->l;
        x->l = y->p = nullptr;
        fetch(x);
    }

    void set_key(int u, ll key) {
        Ptr x = get(u);
        x->key = key;
        fetch(x);
    }
    ll fold(int u, int v) {
        evert(u);
        return get(v)->acc;
    }
};

namespace fast_io {

using u64 = uint64_t;
template <u64 N> constexpr u64 TEN = 10 * TEN<N - 1>;
template <> constexpr u64 TEN<0> = 1;

constexpr int BUF_SIZE = 1 << 17;

inline constexpr int integer_digits(const u64 n) {
    if (n >= TEN<10>) {
        if (n >= TEN<15>) {
            if (n >= TEN<19>) return 20;
            if (n >= TEN<18>) return 19;
            if (n >= TEN<17>) return 18;
            if (n >= TEN<16>) return 17;
            return 16;
        } else {
            if (n >= TEN<14>) return 15;
            if (n >= TEN<13>) return 14;
            if (n >= TEN<12>) return 13;
            if (n >= TEN<11>) return 12;
            return 11;
        }
    } else {
        if (n >= TEN<5>) {
            if (n >= TEN<9>) return 10;
            if (n >= TEN<8>) return 9;
            if (n >= TEN<7>) return 8;
            if (n >= TEN<6>) return 7;
            return 6;
        } else {
            if (n >= TEN<4>) return 5;
            if (n >= TEN<3>) return 4;
            if (n >= TEN<2>) return 3;
            if (n >= TEN<1>) return 2;
            return 1;
        }
    }
}

struct NumBlock {
    char NUM[40000];
    constexpr NumBlock() : NUM() {
        for (int i = 0; i < 10000; ++i) {
            int n = i;
            for (int j = 3; j >= 0; --j) {
                NUM[i * 4 + j] = n % 10 + '0';
                n /= 10;
            }
        }
    }
} constexpr num_block;

class Scanner {
    char buf[BUF_SIZE];
    int left, right;

    inline void load() {
        const int len = right - left;
        std::memcpy(buf, buf + left, len);
        right = len + std::fread(buf + len, 1, BUF_SIZE - len, stdin);
        left = 0;
    }

    inline void ignore_spaces() {
        while (buf[left] <= ' ') {
            if (__builtin_expect(++left == right, 0)) load();
        }
    }

  public:
    Scanner() : buf(), left(0), right(0) { load(); }

    void scan(char& c) {
        ignore_spaces();
        c = buf[left++];
    }

    template <typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr> inline void scan(T& x) {
        ignore_spaces();
        if (__builtin_expect(left + 32 > right, 0)) load();
        char c = buf[left++];
        bool minus = false;
        if constexpr (std::is_signed_v<T>) {
            if (c == '-') {
                minus = 1;
                c = buf[left++];
            }
        }
        x = 0;
        while (c >= '0') {
            x = x * 10 + (c & 15);
            c = buf[left++];
        }
        if constexpr (std::is_signed_v<T>) {
            if (minus) x = -x;
        }
    }

    template <class T, class... Args> inline void scan(T& x, Args&... args) {
        scan(x);
        scan(args...);
    }
};

class Printer {
    char buf[BUF_SIZE];
    int pos;

    inline void flush() {
        std::fwrite(buf, 1, pos, stdout);
        pos = 0;
    }

  public:
    Printer() : buf(), pos(0) {}
    ~Printer() { flush(); }

    void print(const char c) {
        buf[pos] = c;
        if (__builtin_expect(++pos == BUF_SIZE, 0)) flush();
    }

    template <typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr> inline void print(T x) {
        if (__builtin_expect(pos + 32 > BUF_SIZE, 0)) flush();
        if (x == 0) {
            buf[pos++] = '0';
            return;
        }
        if constexpr (std::is_signed_v<T>) {
            if (x < 0) {
                buf[pos++] = '-';
                x = -x;
            }
        }
        const int digit = integer_digits(x);
        int len = digit;
        while (len >= 4) {
            len -= 4;
            std::memcpy(buf + pos + len, num_block.NUM + (x % 10000) * 4, 4);
            x /= 10000;
        }
        std::memcpy(buf + pos, num_block.NUM + x * 4 + 4 - len, len);
        pos += digit;
    }

    template <class T, class... Args> inline void print(T x, Args&&... args) {
        print(x);
        print(' ');
        print(std::forward<Args>(args)...);
    }

    template <class... Args> void println(Args&&... args) {
        print(std::forward<Args>(args)...);
        print('\n');
    }
};

};  // namespace fast_io

int main() {
    fast_io::Scanner rd;
    fast_io::Printer wt;

    int N, Q;
    rd.scan(N, Q);
    LinkCutTree lct(N);
    vector<ll> a(N);
    for (int i = 0; i < N; ++i) {
        rd.scan(a[i]);
        lct.set_key(i, a[i]);
    }
    for (int i = 0; i < N - 1; ++i) {
        int u, v;
        rd.scan(u, v);
        lct.evert(u);
        lct.evert(v);
        lct.link(v, u);
    }

    while (Q--) {
        int t;
        rd.scan(t);
        if (t == 0) {
            int u, v, w, x;
            rd.scan(u, v, w, x);
            lct.evert(u);
            lct.cut(v);
            lct.evert(w);
            lct.link(w, x);
        } else if (t == 1) {
            int u, x;
            rd.scan(u, x);
            a[u] += x;
            lct.set_key(u, a[u]);
        } else {
            int u, v;
            rd.scan(u, v);
            wt.println(lct.fold(u, v));
        }
    }
}