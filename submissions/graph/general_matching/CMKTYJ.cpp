#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#pragma GCC target("avx,avx2,sse,sse2,sse3,sse4,popcnt")

// https://uoj.ac/submission/17577

#include <bits/stdc++.h>

namespace IO {
#ifndef DEBUG
    #define CHANGE_DEFAULT_STREAMS
    static struct FastInput {
        static constexpr int BUF_SIZE = 1 << 21;
        char buf[BUF_SIZE];
        size_t chars_read = 0;
        size_t buf_pos = 0;
        FILE* in = stdin;
        char cur = 0;
        inline __attribute__((always_inline)) char get_char() {
            if (buf_pos >= chars_read) {
                chars_read = fread(buf, 1, BUF_SIZE, in);
                buf_pos = 0;
                buf[0] = (chars_read == 0 ? -1 : buf[0]);
            }
            return cur = buf[buf_pos++];
        }
        inline __attribute__((always_inline)) FastInput* tie(int) {
            return this;
        }
        inline __attribute__((always_inline)) FastInput* tie(nullptr_t) {
            return this;
        }
        inline __attribute__((always_inline)) void sync_with_stdio(bool) {}
        inline __attribute__((always_inline)) explicit operator bool() {
            return cur != -1;
        }
        inline __attribute__((always_inline)) static bool is_blank(char c) {
            return c <= ' ';
        }
        inline __attribute__((always_inline)) bool skip_blanks() {
            while (is_blank(cur) && cur != -1) get_char();
            return cur != -1;
        }
        inline __attribute__((always_inline)) FastInput& operator>>(char& c) {
            skip_blanks();
            c = cur;
            return *this;
        }
        inline __attribute__((always_inline)) FastInput& operator>>(
            std::string& s) {
            if (skip_blanks()) {
                s.clear();
                do {
                    s += cur;
                } while (!is_blank(get_char()));
            }
            return *this;
        }
        template <typename T>
        inline __attribute__((always_inline)) FastInput& read_integer(T& n) {
            // unsafe, doesn't check that characters are actually digits
            n = 0;
            if (skip_blanks()) {
                int sign = +1;
                if (cur == '-') {
                    sign = -1;
                    get_char();
                }
                do {
                    n += n + (n << 3) + cur - '0';
                } while (!is_blank(get_char()));
                n *= sign;
            }
            return *this;
        }
        template <typename T>
        inline __attribute__((always_inline))
        typename std::enable_if<std::is_integral<T>::value, FastInput&>::type
        operator>>(T& n) {
            return read_integer(n);
        }
    #if !defined(_WIN32) || defined(_WIN64)
        inline __attribute__((always_inline)) FastInput& operator>>(
            __int128& n) {
            return read_integer(n);
        }
    #endif
        template <typename T>
        inline __attribute__((always_inline))
        typename std::enable_if<std::is_floating_point<T>::value,
                                FastInput&>::type
        operator>>(T& n) {
            // not sure if really fast, for compatibility only
            n = 0;
            if (skip_blanks()) {
                std::string s;
                (*this) >> s;
                sscanf(s.c_str(), "%lf", &n);
            }
            return *this;
        }
    } fast_input;
    #define cin IO::fast_input
    static struct FastOutput {
        static constexpr int BUF_SIZE = 1 << 21;
        char buf[BUF_SIZE];
        size_t buf_pos = 0;
        static constexpr int TMP_SIZE = 1 << 21;
        char tmp[TMP_SIZE];
        FILE* out = stdout;
        inline __attribute__((always_inline)) void put_char(char c) {
            buf[buf_pos++] = c;
            if (buf_pos == BUF_SIZE) {
                fwrite(buf, 1, buf_pos, out);
                buf_pos = 0;
            }
        }
        ~FastOutput() { fwrite(buf, 1, buf_pos, out); }
        inline __attribute__((always_inline)) FastOutput& operator<<(char c) {
            put_char(c);
            return *this;
        }
        inline __attribute__((always_inline)) FastOutput& operator<<(
            const char* s) {
            while (*s) put_char(*s++);
            return *this;
        }
        inline __attribute__((always_inline)) FastOutput& operator<<(
            const std::string& s) {
            for (auto x : s) put_char(x);
            return *this;
        }
        template <typename T>
        inline __attribute__((always_inline)) char* integer_to_string(T n) {
            // beware of TMP_SIZE
            char* p = tmp + TMP_SIZE - 1;
            if (n == 0)
                *--p = '0';
            else {
                bool is_negative = false;
                if (n < 0) {
                    is_negative = true;
                    n = -n;
                }
                while (n > 0) {
                    *--p = (char)('0' + n % 10);
                    n /= 10;
                }
                if (is_negative) *--p = '-';
            }
            return p;
        }
        template <typename T>
        inline __attribute__((always_inline))
        typename std::enable_if<std::is_integral<T>::value, char*>::type
        stringify(T n) {
            return integer_to_string(n);
        }
    #if !defined(_WIN32) || defined(_WIN64)
        inline __attribute__((always_inline)) char* stringify(__int128 n) {
            return integer_to_string(n);
        }
    #endif
        template <typename T>
        inline __attribute__((always_inline))
        typename std::enable_if<std::is_floating_point<T>::value, char*>::type
        stringify(T n) {
            sprintf(tmp, "%.17f", n);
            return tmp;
        }
        template <typename T>
        inline __attribute__((always_inline)) FastOutput& operator<<(
            const T& n) {
            auto p = stringify(n);
            for (; *p != 0; p++) put_char(*p);
            return *this;
        }
    } fast_output;
    #define cout IO::fast_output
#endif
}  // namespace IO

struct GraphEdgePointers {
    struct edge {
        int to, nxt;
        edge(int to, int nxt) : to(to), nxt(nxt) {}
    };
    std::vector<int> head;
    std::vector<edge> edges;
    int cur_edges;
    GraphEdgePointers(int n, int m) : head(n + 1, 0), cur_edges(1) {
        edges.reserve(m + 1);
        edges.emplace_back(0, 0);
    }
    inline __attribute__((always_inline)) void add_edge(int u, int v) {
        edges.emplace_back(v, head[u]);
        head[u] = cur_edges++;
    }
};

struct GeneralMatching {
    int n, m;
    GraphEdgePointers g;
    int n_matches, q_n, book_mark;
    std::vector<int> mate, q, book, type, fa, bel;

    GeneralMatching(int n, int m)
        : n(n),
          m(m),
          g(n, 2 * m),
          n_matches(0),
          q_n(0),
          book_mark(0),
          mate(n + 1),
          q(n + 1),
          book(n + 1),
          type(n + 1),
          fa(n + 1),
          bel(n + 1) {}

    void add_edge(int u, int v) {
        g.add_edge(u, v);
        g.add_edge(v, u);
    }

    int maximum_matching() {
        n_matches = 0;
        for (int u = 1; u <= n; ++u)
            if (!mate[u] && match(u)) ++n_matches;
        return n_matches;
    }

    std::vector<std::pair<int, int>> get_edges() {
        std::vector<std::pair<int, int>> ans;
        for (int u = 1; u <= n; ++u)
            if (mate[u] > u) ans.emplace_back(u, mate[u]);
        return ans;
    }

   private:
    inline __attribute__((always_inline)) void augment(int u) {
        while (u) {
            int nu = mate[fa[u]];
            mate[mate[u] = fa[u]] = u;
            u = nu;
        }
    }
    inline __attribute__((always_inline)) int get_lca(int u, int v) {
        ++book_mark;
        while (true) {
            if (u) {
                if (book[u] == book_mark) return u;
                book[u] = book_mark;
                u = bel[fa[mate[u]]];
            }
            std::swap(u, v);
        }
    }
    inline __attribute__((always_inline)) void go_up(int u, int v,
                                                     const int& mv) {
        while (bel[u] != mv) {
            fa[u] = v;
            v = mate[u];
            if (type[v] == 1) type[q[++q_n] = v] = 0;
            if (bel[u] == u) bel[u] = mv;
            if (bel[v] == v) bel[v] = mv;
            u = fa[v];
        }
    }
    inline __attribute__((always_inline)) void after_go_up() {
        for (int u = 1; u <= n; ++u) bel[u] = bel[bel[u]];
    }
    inline __attribute__((always_inline)) bool match(const int& sv) {
        for (int u = 1; u <= n; ++u) bel[u] = u, type[u] = -1;
        type[q[q_n = 1] = sv] = 0;
        for (int i = 1; i <= q_n; ++i) {
            int u = q[i];
            for (int e = g.head[u]; e; e = g.edges[e].nxt) {
                int v = g.edges[e].to;
                if (!~type[v]) {
                    fa[v] = u, type[v] = 1;
                    int nu = mate[v];
                    if (!nu) {
                        augment(v);
                        return true;
                    }
                    type[q[++q_n] = nu] = 0;
                } else if (!type[v] && bel[u] != bel[v]) {
                    int lca = get_lca(u, v);
                    go_up(u, v, lca);
                    go_up(v, u, lca);
                    after_go_up();
                }
            }
        }
        return false;
    }
};

using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    GeneralMatching M(n, m);
    while (m--) {
        int u, v;
        cin >> u >> v;
        ++u, ++v;
        M.add_edge(u, v);
    }
    cout << M.maximum_matching() << '\n';
    for (auto [x, y] : M.get_edges()) cout << x - 1 << ' ' << y - 1 << '\n';
}