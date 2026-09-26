// https://judge.yosupo.jp/problem/directedmst
#pragma GCC optimize("fast-math")

#include <bits/stdc++.h>
#ifdef LOCAL
#  include <dbg.h>
#else
#  define dbg(...) do {} while (0)
#endif

#define cp_lib_4th(_1, _2, _3, x, ...)  x
#define cp_lib_rep(i, l, r)             for (int i = (l); (i) < (r); ++(i))
#define cp_lib_rep0(i, r)               cp_lib_rep(i, 0, r)
#define rep(...)                        cp_lib_4th(__VA_ARGS__, cp_lib_rep, cp_lib_rep0, _)(__VA_ARGS__)
#define cp_lib_repr(i, r, l, ...)       for (int i = (r); (i) >= (l); --(i))
#define repr(...)                       cp_lib_repr(__VA_ARGS__, 0)
#define all(a)                          ::begin(a),::end(a)
#define trav(a, b)                      for (auto&& a : (b))

using namespace std;
using ll = long long;
using ld = long double;
[[maybe_unused]] static constexpr int INF = int(1e9 + 5);
[[maybe_unused]] static constexpr ll INFL = ll(INF) * INF;
template <class C> int sz(const C& c) { return int(::size(c)); }


#ifdef CP_LIB_DEBUG
#define cp_lib_assert(expr) \
    do { if (!(expr)) { \
        ::cerr << "assertion failed: " << #expr << " (" << __FILE__ << ':' << __LINE__ << ")\n"; \
        ::abort(); \
    } } while (0)
#else
#define cp_lib_assert(expr)
#endif


#if __cplusplus < 202002L
struct identity { template <class T> constexpr T&& operator()(T&& t) const noexcept { return forward<T>(t); }; };
#endif
#if __cpp_lib_remove_cvref < 201711L
template <class T> using remove_cvref_t = remove_cv_t<remove_reference_t<T>>;
#endif

namespace cp_lib_type_meta {
    template <class T, class = void> constexpr bool is_tuple_like = false;
    template <class T> constexpr bool is_tuple_like<T, void_t<tuple_element_t<0, T>>> = true;
}


namespace cp_lib_modint { struct ModIntTag {}; }
#include <unistd.h>

namespace cp_lib_io {
    constexpr int BUF_SIZE = 1 << 20;
    constexpr array<array<char, 4>, 10'000> DIGITS = []{
        array<array<char, 4>, 10'000> digits{};
        for (int i = 3, d = 1; i >= 0; --i, d *= 10)
            rep(j, 10'000)
                digits[j][i] = char('0' + j / d % 10);
        return digits;
    }();
    array<char, BUF_SIZE> ibuf, obuf;
    char *iptr = data(ibuf), *iend = iptr, *optr = data(obuf);

    template <class T> constexpr bool is_std_array = false;
    template <class T, size_t I> constexpr bool is_std_array<array<T, I>> = true;

    void flush() {
        for (auto* p = begin(obuf); p != optr; p += write(STDOUT_FILENO, p, optr - p));
        optr = begin(obuf);
    }
    int _flush_atexit = []{ atexit(flush); return 0; }();

    void refill() {
        memmove(begin(ibuf), iptr, iend - iptr);
        iend -= iptr - begin(ibuf);
        iptr = begin(ibuf);
        iend += read(STDIN_FILENO, iend, end(ibuf) - 1 - iend);
        *iend = '\0';
    }

    template <class T, class T2 = remove_cvref_t<T>>
    void print(T&& val) {
        if (end(obuf) - optr < 64)
            flush();

        if constexpr (is_same_v<T2, char>)
            *optr++ = val;
        else if constexpr (is_same_v<T2, bool> || is_same_v<T2, vector<bool>::reference>)
            return print(int(val));
        else if constexpr (is_integral_v<T2> && is_signed_v<T2>) {
            if (val < 0)
                *optr++ = '-';
            using U = make_unsigned_t<T2>;
            return print(U(val < 0 ? -U(val) : U(val)));
        } else if constexpr (is_integral_v<T2> && is_unsigned_v<T2>) {
            T2 val2 = val;
            array<char, 64> tmp;
            char* tptr = end(tmp);
            while (val2 >= 10'000)
                tptr -= 4, memcpy(tptr, &DIGITS[val2 % 10'000][0], 4), val2 /= T2(10'000);
            int d = (val2 >= 100 ? (val2 >= 1000 ? 4 : 3) : (val2 >= 10 ? 2 : 1));
            memcpy(optr, &DIGITS[val2][4 - d], d);
            memcpy(optr + d, tptr, end(tmp) - tptr);
            optr += d + int(end(tmp) - tptr);
        } else if constexpr (is_floating_point_v<T2>)
            optr += sprintf(optr, "%.30Lf", (long double)val);
        else if constexpr (is_convertible_v<T, string_view>) {
            string_view sv(val);
            if (sz(sv) + 1 <= end(obuf) - optr)
                memcpy(optr, data(sv), sz(sv)), optr += sz(sv);
            else {
                flush();
                for (auto *p = data(sv), *pe = p + sz(sv); p != pe; p += write(STDOUT_FILENO, p, pe - p));
            }
        } else if constexpr (is_base_of_v<cp_lib_modint::ModIntTag, T2>)
            return print(decltype(T2::mod())(val));
        else if constexpr (cp_lib_type_meta::is_tuple_like<T2> && !is_std_array<T2>)
            return apply([](auto&&... items) { (print(items), ...); }, forward<T>(val));
        else {
            trav(item, val)
                print(item);
            return;
        }
        *optr++ = ' ';
    }

    template <class T>
    void read(T& val) {
        auto skip_ws = [] {
            do {
                for (; iptr != iend && *iptr <= ' '; ++iptr);
                if (iend - iptr < 64)
                    refill();
            } while (*iptr <= ' ');
        };
        auto read_other = [&](auto other) {
            read(other);
            return other;
        };

        if constexpr (is_same_v<T, char>)
            skip_ws(), val = *iptr++;
        else if constexpr (is_same_v<T, bool> || is_same_v<T, vector<bool>::reference>) {
            val = bool(read_other(uint8_t()));
        } else if constexpr (is_base_of_v<cp_lib_modint::ModIntTag, T>) {
            val = T(read_other(ll()));
        } else if constexpr (is_integral_v<T>) {
            skip_ws();
            if (is_signed_v<T> && *iptr == '-')
                ++iptr, val = T(-read_other(make_unsigned_t<T>()));
            else
                for (val = 0; iptr != iend && *iptr > ' '; val = T(10 * val + (*iptr++ & 15)));
        } else if constexpr (is_floating_point_v<T>)
            skip_ws(), val = T(strtold(iptr, &iptr));
        else if constexpr (is_same_v<T, string>) {
            skip_ws();
            val.clear();
            do {
                auto* after = find_if(iptr, iend, [](char c) { return c <= ' '; });
                val.append(iptr, after);
                if ((iptr = after) != iend)
                    break;
                refill();
            } while (iptr != iend);
        } else if constexpr (cp_lib_type_meta::is_tuple_like<T> && !is_std_array<T>)
            apply([](auto&... items) { (read(items), ...); }, val);
        else
            trav(item, val)
                read(item);
    }
}

using cp_lib_io::flush;

template <class... Args>
void print(Args&&... args) { (cp_lib_io::print(forward<Args>(args)), ...); }

template <class... Args>
void println(Args&&... args) {
    if (sizeof...(Args))
        (cp_lib_io::print(forward<Args>(args)), ...), *(cp_lib_io::optr - 1) = '\n';
    else
        print('\n'), --cp_lib_io::optr;
}

template <class... Args>
void read(Args&... args) { (cp_lib_io::read(args), ...); }



struct Dsu {
    vector<int> p;

    explicit Dsu(int n) : p(n, -1) {}

    int find(int i) { return p[i] < 0 ? i : p[i] = find(p[i]); }
    bool same(int i, int j) { return find(i) == find(j); }
    int size(int i) { return -p[find(i)]; }
    bool join(int i, int j) {
        i = find(i), j = find(j);
        if (i == j) return false;
        if (p[i] > p[j]) swap(i, j);
        p[i] += p[j], p[j] = i;
        return true;
    }
};


struct RollbackDsu {
    vector<int> p;
    vector<pair<int, int>> joins;

    explicit RollbackDsu(int n) : p(n, -1) {}

    int find(int i) const { return p[i] < 0 ? i : find(p[i]); }
    bool same(int i, int j) const { return find(i) == find(j); }
    int size(int i) const { return -p[find(i)]; }
    int time() const { return sz(joins); }

    bool join(int i, int j) {
        i = find(i), j = find(j);
        if (i == j) return false;
        if (p[i] > p[j]) swap(i, j);
        joins.emplace_back(j, p[j]);
        p[i] += p[j], p[j] = i;
        return true;
    }

    void rollback(int t) {
        while (sz(joins) > t) {
            auto [i, pi] = joins.back(); joins.pop_back();
            cp_lib_assert(p[p[i]] < 0);
            p[p[i]] -= pi;
            p[i] = pi;
        }
    }
};

template <class W>
struct MinCostArborescenceGraph {
 private:
    struct SkewHeapNode { int l, r, from, to; W weight, lz; };
    vector<SkewHeapNode> nodes;
    vector<int> heap;

    void apply(int i, W upd) { nodes[i].weight -= upd; nodes[i].lz += upd; }
    void push(int i) {
        if (nodes[i].l != -1) apply(nodes[i].l, nodes[i].lz);
        if (nodes[i].r != -1) apply(nodes[i].r, nodes[i].lz);
        nodes[i].lz = W(0);
    }
    int merge(int u, int v) {
        if (u == -1 || v == -1) return (u == -1 ? v : u);
        if (nodes[v].weight < nodes[u].weight) swap(u, v);
        push(u);
        nodes[u].r = merge(nodes[u].r, v);
        swap(nodes[u].l, nodes[u].r);
        return u;
    }
    void pop(int v) {
        push(heap[v]);
        heap[v] = merge(nodes[heap[v]].l, nodes[heap[v]].r);
    }

 public:
    explicit MinCostArborescenceGraph(int n, int m = 0) : heap(n, -1) { nodes.reserve(m); }

    void add_edge(int from, int to, W weight) {
        cp_lib_assert(0 <= from && from < sz(heap) && 0 <= to && to < sz(heap));
        nodes.push_back(SkewHeapNode{-1, -1, from, to, weight, W(0)});
        heap[to] = merge(heap[to], sz(nodes) - 1);
    }

    template <class WSum = W>
    pair<WSum, vector<int>> solve(int root) {
        cp_lib_assert(0 <= root && root < sz(heap));
        auto ans = WSum(0);
        vector edge(sz(heap), -1);
        vector<pair<int, int>> cycles;
        Dsu dsu_cyc(sz(heap));
        RollbackDsu dsu_contract(sz(heap));
        rep(i, sz(heap)) {
            if (i == root) continue;
            int v = i;
            while (true) {
                if (heap[v] == -1) return {W(0), {}};
                edge[v] = heap[v];
                ans += nodes[edge[v]].weight;
                apply(edge[v], nodes[edge[v]].weight);
                if (dsu_cyc.join(v, dsu_contract.find(nodes[edge[v]].from)))
                    break;

                int vnext = dsu_contract.find(nodes[edge[v]].from), t = dsu_contract.time();
                while (dsu_contract.join(v, vnext)) {
                    heap[dsu_contract.find(v)] = merge(heap[v], heap[vnext]);
                    v = dsu_contract.find(v);
                    vnext = dsu_contract.find(nodes[edge[vnext]].from);
                }
                cycles.emplace_back(edge[v], t);

                while (heap[v] != -1 && dsu_contract.same(nodes[heap[v]].from, v))
                    pop(v);
            }
        }

        for (auto it = rbegin(cycles); it != rend(cycles); ++it) {
            int vrepr = dsu_contract.find(nodes[it->first].to);
            dsu_contract.rollback(it->second);
            int vinc = dsu_contract.find(nodes[edge[vrepr]].to);
            edge[vinc] = exchange(edge[vrepr], it->first);
        }

        rep(i, sz(heap))
            edge[i] = (i == root ? -1 : nodes[edge[i]].from);
        return {ans, edge};
    }
};

int main() {
    int n, m, vr; read(n, m, vr);
    MinCostArborescenceGraph<int> g(n, m);
    rep(_, m) {
        int u, v, w; read(u, v, w);
        g.add_edge(u, v, w);
    }

    auto [cost, edges] = g.solve<ll>(vr);
    println(cost);
    rep(i, n)
        print(i == vr ? vr : edges[i]);
    println();
}
