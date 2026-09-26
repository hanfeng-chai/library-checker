#line 1 "test/oj/min_cost_b_flow.test.cpp"
// verification-helper: PROBLEM https://judge.yosupo.jp/problem/min_cost_b_flow
#line 2 "src/yosupo/fastio.hpp"

#include <unistd.h>
#include <algorithm>
#include <array>
#include <bit>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstring>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

#line 2 "src/yosupo/internal_type_traits.hpp"

#line 5 "src/yosupo/internal_type_traits.hpp"

namespace yosupo {

namespace internal {

template <class T>
using is_signed_int128 =
    typename std::conditional<std::is_same<T, __int128_t>::value ||
                                  std::is_same<T, __int128>::value,
                              std::true_type,
                              std::false_type>::type;

template <class T>
using is_unsigned_int128 =
    typename std::conditional<std::is_same<T, __uint128_t>::value ||
                                  std::is_same<T, unsigned __int128>::value,
                              std::true_type,
                              std::false_type>::type;

template <class T>
using make_unsigned_int128 =
    typename std::conditional<std::is_same<T, __int128_t>::value,
                              __uint128_t,
                              unsigned __int128>;

template <class T>
using is_integral =
    typename std::conditional<std::is_integral<T>::value ||
                                  internal::is_signed_int128<T>::value ||
                                  internal::is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <class T>
using is_signed_int = typename std::conditional<(is_integral<T>::value &&
                                                 std::is_signed<T>::value) ||
                                                    is_signed_int128<T>::value,
                                                std::true_type,
                                                std::false_type>::type;

template <class T>
using is_unsigned_int =
    typename std::conditional<(is_integral<T>::value &&
                               std::is_unsigned<T>::value) ||
                                  is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <class T>
using to_unsigned = typename std::conditional<
    is_signed_int128<T>::value,
    make_unsigned_int128<T>,
    typename std::conditional<std::is_signed<T>::value,
                              std::make_unsigned<T>,
                              std::common_type<T>>::type>::type;

template <class T>
using is_integral_t = std::enable_if_t<is_integral<T>::value>;

template <class T>
using is_signed_int_t = std::enable_if_t<is_signed_int<T>::value>;

template <class T>
using is_unsigned_int_t = std::enable_if_t<is_unsigned_int<T>::value>;

template <class T> using to_unsigned_t = typename to_unsigned<T>::type;

}  // namespace internal

}  // namespace yosupo
#line 17 "src/yosupo/fastio.hpp"

namespace yosupo {

struct Scanner {
  public:
    Scanner(const Scanner&) = delete;
    Scanner& operator=(const Scanner&) = delete;

    Scanner(FILE* fp) : fd(fileno(fp)) { line[0] = 127; }

    void read() {}
    template <class H, class... T> void read(H& h, T&... t) {
        bool f = read_single(h);
        assert(f);
        read(t...);
    }

    int read_unsafe() { return 0; }
    template <class H, class... T> int read_unsafe(H& h, T&... t) {
        bool f = read_single(h);
        if (!f) return 0;
        return 1 + read_unsafe(t...);
    }

    int close() { return ::close(fd); }

  private:
    static constexpr int SIZE = 1 << 15;

    int fd = -1;
    std::array<char, SIZE + 1> line;
    int st = 0, ed = 0;
    bool eof = false;

    bool read_single(std::string& ref) {
        if (!skip_space()) return false;
        ref = "";
        while (true) {
            char c = top();
            if (c <= ' ') break;
            ref += c;
            st++;
        }
        return true;
    }
    bool read_single(double& ref) {
        std::string s;
        if (!read_single(s)) return false;
        ref = std::stod(s);
        return true;
    }

    template <class T,
              std::enable_if_t<std::is_same<T, char>::value>* = nullptr>
    bool read_single(T& ref) {
        if (!skip_space<50>()) return false;
        ref = top();
        st++;
        return true;
    }

    template <class T,
              internal::is_signed_int_t<T>* = nullptr,
              std::enable_if_t<!std::is_same<T, char>::value>* = nullptr>
    bool read_single(T& sref) {
        using U = internal::to_unsigned_t<T>;
        if (!skip_space<50>()) return false;
        bool neg = false;
        if (line[st] == '-') {
            neg = true;
            st++;
        }
        U ref = 0;
        do {
            ref = 10 * ref + (line[st++] & 0x0f);
        } while (line[st] >= '0');
        sref = neg ? -ref : ref;
        return true;
    }
    template <class U,
              internal::is_unsigned_int_t<U>* = nullptr,
              std::enable_if_t<!std::is_same<U, char>::value>* = nullptr>
    bool read_single(U& ref) {
        if (!skip_space<50>()) return false;
        ref = 0;
        do {
            ref = 10 * ref + (line[st++] & 0x0f);
        } while (line[st] >= '0');
        return true;
    }

    bool reread() {
        if (ed - st >= 50) return true;
        if (st > SIZE / 2) {
            std::memmove(line.data(), line.data() + st, ed - st);
            ed -= st;
            st = 0;
        }
        if (eof) return false;
        auto u = ::read(fd, line.data() + ed, SIZE - ed);
        if (u == 0) {
            eof = true;
            line[ed] = '\0';
            u = 1;
        }
        ed += int(u);
        line[ed] = char(127);
        return true;
    }

    char top() {
        if (st == ed) {
            bool f = reread();
            assert(f);
        }
        return line[st];
    }

    template <int TOKEN_LEN = 0> bool skip_space() {
        while (true) {
            while (line[st] <= ' ') st++;
            if (ed - st > TOKEN_LEN) return true;
            if (st > ed) st = ed;
            for (auto i = st; i < ed; i++) {
                if (line[i] <= ' ') return true;
            }
            if (!reread()) return false;
        }
    }
};

struct Printer {
  public:
    template <char sep = ' ', bool F = false> void write() {}
    template <char sep = ' ', bool F = false, class H, class... T>
    void write(const H& h, const T&... t) {
        if (F) write_single(sep);
        write_single(h);
        write<true>(t...);
    }
    template <char sep = ' ', class... T> void writeln(const T&... t) {
        write<sep>(t...);
        write_single('\n');
    }

    Printer(FILE* _fp) : fd(fileno(_fp)) {}
    ~Printer() { flush(); }

    int close() {
        flush();
        return ::close(fd);
    }

    void flush() {
        if (pos) {
            auto res = ::write(fd, line.data(), pos);
            assert(res != -1);
            pos = 0;
        }
    }

  private:
    static std::array<std::array<char, 2>, 100> small;
    static std::array<unsigned long long, 20> tens;

    static constexpr size_t SIZE = 1 << 15;
    int fd;
    std::array<char, SIZE> line;
    size_t pos = 0;
    std::stringstream ss;

    template <class T,
              std::enable_if_t<std::is_same<char, T>::value>* = nullptr>
    void write_single(const T& val) {
        if (pos == SIZE) flush();
        line[pos++] = val;
    }

    template <class T,
              internal::is_signed_int_t<T>* = nullptr,
              std::enable_if_t<!std::is_same<char, T>::value>* = nullptr>
    void write_single(const T& val) {
        using U = internal::to_unsigned_t<T>;
        if (val == 0) {
            write_single('0');
            return;
        }
        if (pos > SIZE - 50) flush();
        U uval = val;
        if (val < 0) {
            write_single('-');
            uval = -uval;
        }
        write_unsigned(uval);
    }

    template <class U,
              internal::is_unsigned_int_t<U>* = nullptr,
              std::enable_if_t<!std::is_same<char, U>::value>* = nullptr>
    void write_single(U uval) {
        if (uval == 0) {
            write_single('0');
            return;
        }
        if (pos > SIZE - 50) flush();

        write_unsigned(uval);
    }

    static int calc_len(uint64_t x) {
        int i = ((63 - std::countl_zero(x)) * 3 + 3) / 10;
        if (x < tens[i])
            return i;
        else
            return i + 1;
    }

    template <class U,
              internal::is_unsigned_int_t<U>* = nullptr,
              std::enable_if_t<2 >= sizeof(U)>* = nullptr>
    void write_unsigned(U uval) {
        size_t len = calc_len(uval);
        pos += len;

        char* ptr = line.data() + pos;
        while (uval >= 100) {
            ptr -= 2;
            memcpy(ptr, small[uval % 100].data(), 2);
            uval /= 100;
        }
        if (uval >= 10) {
            memcpy(ptr - 2, small[uval].data(), 2);
        } else {
            *(ptr - 1) = char('0' + uval);
        }
    }

    template <class U,
              internal::is_unsigned_int_t<U>* = nullptr,
              std::enable_if_t<4 == sizeof(U)>* = nullptr>
    void write_unsigned(U uval) {
        std::array<char, 8> buf;
        memcpy(buf.data() + 6, small[uval % 100].data(), 2);
        memcpy(buf.data() + 4, small[uval / 100 % 100].data(), 2);
        memcpy(buf.data() + 2, small[uval / 10000 % 100].data(), 2);
        memcpy(buf.data() + 0, small[uval / 1000000 % 100].data(), 2);

        if (uval >= 100000000) {
            if (uval >= 1000000000) {
                memcpy(line.data() + pos, small[uval / 100000000 % 100].data(),
                       2);
                pos += 2;
            } else {
                line[pos] = char('0' + uval / 100000000);
                pos++;
            }
            memcpy(line.data() + pos, buf.data(), 8);
            pos += 8;
        } else {
            size_t len = calc_len(uval);
            memcpy(line.data() + pos, buf.data() + (8 - len), len);
            pos += len;
        }
    }

    template <class U,
              internal::is_unsigned_int_t<U>* = nullptr,
              std::enable_if_t<8 == sizeof(U)>* = nullptr>
    void write_unsigned(U uval) {
        size_t len = calc_len(uval);
        pos += len;

        char* ptr = line.data() + pos;
        while (uval >= 100) {
            ptr -= 2;
            memcpy(ptr, small[uval % 100].data(), 2);
            uval /= 100;
        }
        if (uval >= 10) {
            memcpy(ptr - 2, small[uval].data(), 2);
        } else {
            *(ptr - 1) = char('0' + uval);
        }
    }

    template <
        class U,
        std::enable_if_t<internal::is_unsigned_int128<U>::value>* = nullptr>
    void write_unsigned(U uval) {
        static std::array<char, 50> buf;
        size_t len = 0;
        while (uval > 0) {
            buf[len++] = char((uval % 10) + '0');
            uval /= 10;
        }
        std::reverse(buf.begin(), buf.begin() + len);
        memcpy(line.data() + pos, buf.data(), len);
        pos += len;
    }

    void write_single(const std::string& s) {
        for (char c : s) write_single(c);
    }
    void write_single(const char* s) {
        size_t len = strlen(s);
        for (size_t i = 0; i < len; i++) write_single(s[i]);
    }
    template <class T> void write_single(const std::vector<T>& val) {
        auto n = val.size();
        for (size_t i = 0; i < n; i++) {
            if (i) write_single(' ');
            write_single(val[i]);
        }
    }
};

inline std::array<std::array<char, 2>, 100> Printer::small = [] {
    std::array<std::array<char, 2>, 100> table;
    for (int i = 0; i <= 99; i++) {
        table[i][1] = char('0' + (i % 10));
        table[i][0] = char('0' + (i / 10 % 10));
    }
    return table;
}();
inline std::array<unsigned long long, 20> Printer::tens = [] {
    std::array<unsigned long long, 20> table;
    for (int i = 0; i < 20; i++) {
        table[i] = 1;
        for (int j = 0; j < i; j++) {
            table[i] *= 10;
        }
    }
    return table;
}();

}  // namespace yosupo
#line 2 "src/yosupo/networksimplex.hpp"

#line 4 "src/yosupo/networksimplex.hpp"
#include <cmath>
#include <ranges>
#line 7 "src/yosupo/networksimplex.hpp"

namespace yosupo {

template <class Cap, class Cost> struct NetworkSimplexGraph {
    struct Edge {
        int to;
        Cap cap;
        Cost cost;
    };
    std::vector<Edge> edges;
    std::vector<Cap> lowers;

    int n;
    std::vector<Cap> excess;

    explicit NetworkSimplexGraph(int _n = 0) : n(_n), excess(n) {}

    int add_vertex() {
        n++;
        return n - 1;
    }

    void add_excess(int i, Cap ex) { excess[i] += ex; }

    void add_edge(int u, int v, Cap lower, Cap upper, Cost cost) {
        edges.push_back({v, upper - lower, cost});
        edges.push_back({u, 0, -cost});
        excess[u] -= lower;
        excess[v] += lower;
        lowers.push_back(lower);
    }
};

template <class TotalCost, class Cap, class Cost> struct FlowResult {
    bool feasible;
    TotalCost cost;
    std::vector<Cap> flow;
    std::vector<Cost> potential;
};

template <class TotalCost, class Cap, class Cost>
FlowResult<TotalCost, Cap, Cost> solve(NetworkSimplexGraph<Cap, Cost> g) {
    auto edges = std::move(g.edges);
    auto excess = std::move(g.excess);
    int n = g.n;
    int m = int(edges.size());

    // potential
    std::vector<Cost> p(n + 1);

    Cost art_cost = 1;
    for (int i = 0; i < m; i += 2) {
        art_cost += std::abs(edges[i].cost);
    }

    struct Parent {
        int p;
        int e;  // edge id of (i -> p)
        Cap up, down;
    };
    std::vector<Parent> parents(n);

    edges.reserve(m + 2 * n);
    for (int i = 0; i < n; i++) {
        if (excess[i] >= 0) {
            edges.push_back({n, 0, art_cost});
            edges.push_back({i, excess[i], -art_cost});
            p[i] = -art_cost;
        } else {
            edges.push_back({n, -excess[i], -art_cost});
            edges.push_back({i, 0, art_cost});
            p[i] = art_cost;
        }
        parents[i].p = n;
        parents[i].e = int(edges.size()) - 2;
        parents[i].up = edges[parents[i].e].cap;
        parents[i].down = edges[parents[i].e ^ 1].cap;
    }

    std::vector<int> depth(n + 1, 1);
    depth[n] = 0;

    std::vector<int> next(2 * (n + 1)), prev(2 * (n + 1));
    auto conn = [&](int a, int b) {
        next[a] = b;
        prev[b] = a;
    };
    for (int i : std::views::iota(0, n + 1)) {
        conn(2 * i, 2 * i + 1);
    }
    for (int i : std::views::iota(0, n)) {
        conn(2 * i + 1, next[2 * n]);
        conn(2 * n, 2 * i);
    }

    auto push_flow = [&](int ei0) {
        const int u0 = edges[ei0 ^ 1].to, v0 = edges[ei0].to;
        const Cost cycle_len = edges[ei0].cost + p[u0] - p[v0];

        Cap flow = edges[ei0].cap;
        bool del_u_side = true;
        int del_u = v0;

        int lca = -1;
        {
            int u = u0, v = v0;
            auto up_u = [&]() {
                if (parents[u].down < flow) {
                    flow = parents[u].down;
                    del_u = u;
                    del_u_side = true;
                }
                u = parents[u].p;
            };
            auto up_v = [&]() {
                if (parents[v].up <= flow) {
                    flow = parents[v].up;
                    del_u = v;
                    del_u_side = false;
                }
                v = parents[v].p;
            };

            if (depth[u] >= depth[v]) {
                for (int _ : std::views::iota(0, depth[u] - depth[v])) {
                    up_u();
                }
            } else {
                for (int _ : std::views::iota(0, depth[v] - depth[u])) {
                    up_v();
                }
            }
            while (u != v) {
                up_u();
                up_v();
            }
            lca = u;
        }

        if (flow) {
            int u = u0;
            while (u != lca) {
                parents[u].up += flow;
                parents[u].down -= flow;
                u = parents[u].p;
            }
            int v = v0;
            while (v != lca) {
                parents[v].up -= flow;
                parents[v].down += flow;
                v = parents[v].p;
            }
        }

        int u = u0, par = v0;
        Cost p_diff = -cycle_len;
        std::pair<Cap, Cap> p_caps = {edges[ei0].cap - flow,
                                      edges[ei0 ^ 1].cap + flow};
        if (!del_u_side) {
            std::swap(u, par);
            std::swap(p_caps.first, p_caps.second);
            p_diff *= -1;
        }
        int par_e = ei0 ^ (del_u_side ? 0 : 1);

        while (par != del_u) {
            int d = depth[par];
            int idx = 2 * u;
            while (idx != 2 * u + 1) {
                if (idx % 2 == 0) {
                    d++;
                    p[idx / 2] += p_diff;
                    depth[idx / 2] = d;
                } else {
                    d--;
                }
                idx = next[idx];
            }

            conn(prev[2 * u], next[2 * u + 1]);
            conn(2 * u + 1, next[2 * par]);
            conn(2 * par, 2 * u);

            std::swap(parents[u].e, par_e);
            par_e ^= 1;

            std::swap(parents[u].up, p_caps.first);
            std::swap(parents[u].down, p_caps.second);
            std::swap(p_caps.first, p_caps.second);

            int next_u = parents[u].p;
            parents[u].p = par;
            par = u;
            u = next_u;
        }
        edges[par_e].cap = p_caps.first;
        edges[par_e ^ 1].cap = p_caps.second;
    };

    // pivot-rule: candidate-list
    const int LIST_LENGTH = std::max(int(0.2 * std::sqrt(double(m))), 10);
    const int MINOR_LIMIT = std::max(int(0.1 * LIST_LENGTH), 3);
    std::vector<int> candidates;
    candidates.reserve(LIST_LENGTH);

    auto minor = [&]() {
        if (candidates.empty()) return false;
        Cost best = 0;
        int best_ei = -1;

        int i = 0;
        while (i < int(candidates.size())) {
            int ei = candidates[i];
            if (!edges[ei].cap) {
                std::swap(candidates[i], candidates.back());
                candidates.pop_back();
                continue;
            }
            Cost c_len = edges[ei].cost + p[edges[ei ^ 1].to] - p[edges[ei].to];
            if (c_len >= 0) {
                std::swap(candidates[i], candidates.back());
                candidates.pop_back();
                continue;
            }
            if (c_len < best) {
                best = c_len;
                best_ei = ei;
            }
            i++;
        }

        if (best_ei == -1) return false;
        push_flow(best_ei);
        return true;
    };

    int ei = 0;
    while (true) {
        for (int i = 0; i < MINOR_LIMIT; i++) {
            if (!minor()) break;
        }

        Cost best = 0;
        int best_ei = -1;

        candidates.clear();
        for (int i = 0; i < int(edges.size()); i++) {
            if (edges[ei].cap) {
                Cost clen =
                    edges[ei].cost + p[edges[ei ^ 1].to] - p[edges[ei].to];
                if (clen < 0) {
                    if (clen < best) {
                        best = clen;
                        best_ei = ei;
                    }
                    candidates.push_back(ei);
                    if (int(candidates.size()) == LIST_LENGTH) break;
                }
            }

            ei++;
            if (ei == int(edges.size())) ei = 0;
        }
        if (candidates.empty()) break;
        push_flow(best_ei);
    }

    for (int i : std::views::iota(0, n)) {
        edges[parents[i].e].cap = parents[i].up;
        edges[parents[i].e ^ 1].cap = parents[i].down;
    }

    bool feasible = true;
    for (int i : std::views::iota(0, n)) {
        if (excess[i] >= 0) {
            if (edges[m + 2 * i + 1].cap) {
                feasible = false;
                break;
            }
        } else {
            if (edges[m + 2 * i].cap) {
                feasible = false;
                break;
            }
        }
    }
    if (!feasible) {
        return {false, {}, {}, {}};
    }

    TotalCost cost = 0;
    std::vector<Cap> flow;
    for (int i = 0; i < m; i += 2) {
        flow.push_back(g.lowers[i / 2] + edges[i ^ 1].cap);
        cost += TotalCost(1) * flow.back() * edges[i].cost;
    }
    p.pop_back();
    return {true, cost, flow, p};
}

}  // namespace yosupo
#line 4 "test/oj/min_cost_b_flow.test.cpp"

yosupo::Scanner sc(stdin);
yosupo::Printer pr(stdout);

using ll = long long;
int main() {
    int n, m;
    sc.read(n, m);
    yosupo::NetworkSimplexGraph<ll, ll> g(n);
    for (int i : std::views::iota(0, n)) {
        ll b;
        sc.read(b);
        g.add_excess(i, b);
    }
    for (int _ : std::views::iota(0, m)) {
        int s, t;
        ll l, u, c;
        sc.read(s, t, l, u, c);
        g.add_edge(s, t, l, u, c);
    }
    auto r = solve<__int128>(std::move(g));

    if (!r.feasible) {
        pr.writeln("infeasible");
        return 0;
    }
    pr.writeln(r.cost);
    for (int i : std::views::iota(0, n)) {
        pr.writeln(r.potential[i]);
    }
    for (int i : std::views::iota(0, m)) {
        pr.writeln(r.flow[i]);
    }
}
