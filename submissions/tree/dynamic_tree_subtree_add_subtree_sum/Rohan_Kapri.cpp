#include <bits/stdc++.h>
using namespace std;

struct Hash {
    template <typename T>
    static inline void combine(size_t &h, const T &v) {
        h ^= Hash{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }

    template <typename T>
    size_t operator()(const T &v) const {
        if constexpr (requires { tuple_size<T>::value; })
            return apply([](const auto &...e) {
                size_t h = 0;
                (combine(h, e), ...);
                return h;
            }, v);
        else if constexpr (requires { declval<T>().begin(); declval<T>().end(); } && !is_same_v<T, string>) {
            size_t h = 0;
            for (const auto &e : v) combine(h, e);
            return h;
        } else return hash<T>{}(v);
    }
};

using int64 = long long;

namespace FastIO {
    class Scanner {
        static constexpr int buf_size = (1 << 19);
        static constexpr int integer_size = 20;
        static constexpr int string_size = (int)5e5 + 5; // default
        char buf[buf_size] = {};
        char *cur = buf, *ed = buf;

    public:
        Scanner() {}

        template <class T>
        inline Scanner& operator>>(T& val) {
            read(val);
            return *this;
        }

    private:
        inline void reload() {
            size_t len = ed - cur;
            memmove(buf, cur, len);
            char* tmp = buf + len;
            ed = tmp + fread(tmp, 1, buf_size - len, stdin);
            *ed = 0;
            cur = buf;
        }

        inline void skip_space() {
            while (true) {
                if (cur == ed) reload();
                while (*cur == ' ' || *cur == '\n') ++cur;
                if (__builtin_expect(cur != ed, 1)) return;
            }
        }

        template <class T, std::enable_if_t<std::is_same<T, int>::value, int> = 0>
        inline void read(T& num) {
            skip_space();
            if (cur + integer_size >= ed) reload();
            bool neg = false;
            num = 0;
            if (*cur == '-') neg = true, ++cur;
            while (*cur >= '0') num = num * 10 + (*cur ^ 48), ++cur;
            if (neg) num = -num;
        }

        template <class T, std::enable_if_t<std::is_same<T, int64>::value, int> = 0>
        inline void read(T& num) {
            skip_space();
            if (cur + integer_size >= ed) reload();
            bool neg = false;
            num = 0;
            if (*cur == '-') neg = true, ++cur;
            while (*cur >= '0') num = num * 10 + (*cur ^ 48), ++cur;
            if (neg) num = -num;
        }

        template <class T,
                std::enable_if_t<std::is_same<T, std::string>::value, int> = 0>
        inline void read(T& str) {
            skip_space();
            if (cur + str.size() >= ed) reload();
            auto it = cur;
            while (!(*cur == ' ' || *cur == '\n')) ++cur;
            str = std::string(it, cur);
        }

        template <class T, std::enable_if_t<std::is_same<T, char>::value, int> = 0>
        inline void read(T& c) {
            skip_space();
            if (cur + 1 >= ed) reload();
            c = *cur, ++cur;
        }

        template <class T, std::enable_if_t<std::is_same<T, double>::value, int> = 0>
        inline void read(T& num) {
            skip_space();
            if (cur + integer_size >= ed) reload();
            bool neg = false;
            num = 0;
            if (*cur == '-') neg = true, ++cur;
            while (*cur >= '0' && *cur <= '9') num = num * 10 + (*cur ^ 48), ++cur;
            if (*cur != '.') return;
            ++cur;
            T base = 0.1;
            while (*cur >= '0' && *cur <= '9') {
                num += base * (*cur ^ 48);
                ++cur;
                base *= 0.1;
            }
            if (neg) num = -num;
        }

        template <class T,
                std::enable_if_t<std::is_same<T, long double>::value, int> = 0>
        inline void read(T& num) {
            skip_space();
            if (cur + integer_size >= ed) reload();
            bool neg = false;
            num = 0;
            if (*cur == '-') neg = true, ++cur;
            while (*cur >= '0' && *cur <= '9') num = num * 10 + (*cur ^ 48), ++cur;
            if (*cur != '.') return;
            ++cur;
            T base = 0.1;
            while (*cur >= '0' && *cur <= '9') {
                num += base * (*cur ^ 48);
                ++cur;
                base *= 0.1;
            }
            if (neg) num = -num;
        }

        template <class T>
        inline void read(std::vector<T>& vec) {
            for (T& e : vec) read(e);
        }

        template <class T, class U>
        inline void read(std::pair<T, U>& p) {
            read(p.first, p.second);
        }

        template <class Tuple, std::size_t... Is>
        inline void tuple_scan(Tuple& tp, std::index_sequence<Is...>) {
            (read(std::get<Is>(tp)), ...);
        }

        template <class... Args>
        inline void read(std::tuple<Args...>& tp) {
            tuple_scan(tp, std::index_sequence_for<Args...>{});
        }

        inline void read() {}

        template <class Head, class... Tail>
        inline void read(Head&& head, Tail&&... tail) {
            read(head);
            read(std::forward<Tail>(tail)...);
        }
    };

    class Printer {
        static constexpr int buf_size = (1 << 18);
        static constexpr int integer_size = 20;
        static constexpr int string_size = (1 << 6);
        static constexpr int margin = 1;
        static constexpr int n = 10000;
        char buf[buf_size + margin] = {};
        char table[n * 4] = {};
        char* cur = buf;

    public:
        constexpr Printer() { build(); }

        ~Printer() { flush(); }

        template <class T>
        inline Printer& operator<<(T val) {
            write(val);
            return *this;
        }

        template<class T>
        inline void println(T val) {
            write(val);
            write('\n');
        }

    private:
        constexpr void build() {
            for (int i = 0; i < 10000; ++i) {
                int tmp = i;
                for (int j = 3; j >= 0; --j) {
                    table[i * 4 + j] = tmp % 10 + '0';
                    tmp /= 10;
                }
            }
        }

        inline void flush() {
            fwrite(buf, 1, cur - buf, stdout);
            cur = buf;
        }

        template <class T, std::enable_if_t<std::is_same<T, int>::value, int> = 0>
        inline int get_digit(T n) {
            if (n >= (int)1e5) {
                if (n >= (int)1e8) return 9;
                if (n >= (int)1e7) return 8;
                if (n >= (int)1e6) return 7;
                return 6;
            } else {
                if (n >= (int)1e4) return 5;
                if (n >= (int)1e3) return 4;
                if (n >= (int)1e2) return 3;
                if (n >= (int)1e1) return 2;
                return 1;
            }
        }

        template <class T, std::enable_if_t<std::is_same<T, int64>::value, int> = 0>
        inline int get_digit(T n) {
            if (n >= (int64)1e10) {
                if (n >= (int64)1e14) {
                    if (n >= (int64)1e18) return 19;
                    if (n >= (int64)1e17) return 18;
                    if (n >= (int64)1e16) return 17;
                    if (n >= (int64)1e15) return 16;
                    return 15;
                } else {
                    if (n >= (int64)1e14) return 15;
                    if (n >= (int64)1e13) return 14;
                    if (n >= (int64)1e12) return 13;
                    if (n >= (int64)1e11) return 12;
                    return 11;
                }
            } else {
                if (n >= (int64)1e5) {
                    if (n >= (int64)1e9) return 10;
                    if (n >= (int64)1e8) return 9;
                    if (n >= (int64)1e7) return 8;
                    if (n >= (int64)1e6) return 7;
                    return 6;
                } else {
                    if (n >= (int64)1e4) return 5;
                    if (n >= (int64)1e3) return 4;
                    if (n >= (int64)1e2) return 3;
                    if (n >= (int64)1e1) return 2;
                    return 1;
                }
            }
        }

        template <class T, std::enable_if_t<std::is_same<T, int>::value, int> = 0>
        inline void write(T num) {
            if (__builtin_expect(cur + integer_size >= buf + buf_size, 0)) flush();
            if (num == 0) {
                write('0');
                return;
            }
            if (num < 0) {
                write('-');
                num = -num;
            }
            int len = get_digit(num);
            int digits = len;
            while (num >= 10000) {
                memcpy(cur + len - 4, table + (num % 10000) * 4, 4);
                num /= 10000;
                len -= 4;
            }
            memcpy(cur, table + num * 4 + (4 - len), len);
            cur += digits;
        }

        template <class T, std::enable_if_t<std::is_same<T, int64>::value, int> = 0>
        inline void write(T num) {
            if (__builtin_expect(cur + integer_size >= buf + buf_size, 0)) flush();
            if (num == 0) {
                write('0');
                return;
            }
            if (num < 0) {
                write('-');
                num = -num;
            }
            int len = get_digit(num);
            int digits = len;
            while (num >= 10000) {
                memcpy(cur + len - 4, table + (num % 10000) * 4, 4);
                num /= 10000;
                len -= 4;
            }
            memcpy(cur, table + num * 4 + (4 - len), len);
            cur += digits;
        }

        template <class T, std::enable_if_t<std::is_same<T, char>::value, int> = 0>
        inline void write(T c) {
            if (__builtin_expect(cur + 1 >= buf + buf_size, 0)) flush();
            *cur = c;
            ++cur;
        }

        template <class T,
                std::enable_if_t<std::is_same<T, std::string>::value, int> = 0>
        inline void write(T str) {
            if (__builtin_expect(cur + str.size() >= buf + buf_size, 0)) flush();
            for (char c : str) write(c);
        }

        template <class T,
                std::enable_if_t<std::is_same<T, const char*>::value, int> = 0>
        inline void write(T str) {
            if (__builtin_expect(cur + string_size >= buf + buf_size, 0)) flush();
            for (int i = 0; str[i]; ++i) write(str[i]);
        }
    };
}  // namespace FastIO

FastIO::Scanner fin;
FastIO::Printer fout;

struct FoldableAntiMonopolyTree {
    vector<int> parent, size, weight;
    vector<long long> sum, lazy;

    FoldableAntiMonopolyTree(const vector<long long> &a)
        : parent(a.size(), -1), size(a.size(), 1), weight(a.size(), INT_MAX),
          sum(a), lazy(a.size(), 0) {}

    void promote(int v) {
        int p = parent[v];
        int len = size[v];
        long long x = lazy[p];

        size[p] -= len;
        sum[p] -= sum[v] + x * len;

        parent[v] = parent[p];
        lazy[v] += x;
        sum[v] += x * len;

        if (weight[v] < weight[p]) {
            x = lazy[v];

            lazy[p] -= x;
            sum[p] -= x * size[p];

            size[v] += size[p];
            sum[v] += sum[p] + x * size[p];

            swap(weight[v], weight[p]);
            parent[p] = v;
        }
    }

    void upward_maintain(int v) {
        while (~parent[v]) {
            int p = parent[v];
            if (3LL * size[v] <= 2LL * size[p]) {
                v = p;
                continue;
            }
            promote(v);
        }
    }

    int root(int v) const {
        while (~parent[v]) v = parent[v];
        return v;
    }

    long long potential(int v) const {
        long long x = 0;
        for (; ~v; v = parent[v]) x += lazy[v];
        return x;
    }

    pair<int, int> path_max(int u, int v) {
        upward_maintain(u);
        upward_maintain(v);

        int max_w = INT_MIN, t = -1;
        while (u != v) {
            if (size[u] > size[v]) swap(u, v);
            if (!~parent[u]) return {INT_MAX, -1};

            if (weight[u] > max_w) {
                max_w = weight[u];
                t = u;
            }
            u = parent[u];
        }
        return {max_w, t};
    }

    void cut(int v) {
        int len = size[v];

        long long x = 0;
        for (int p = parent[v]; ~p; p = parent[p]) x += lazy[p];

        int p = parent[v];
        if (~p) {
            int dsize = -len;
            long long dsum = -(sum[v] + lazy[p] * len);

            for (;;) {
                size[p] += dsize;
                sum[p] += dsum;

                int q = parent[p];
                if (!~q) break;

                dsum += lazy[q] * dsize;
                p = q;
            }
        }

        lazy[v] += x;
        sum[v] += x * len;

        parent[v] = -1;
        weight[v] = INT_MAX;
    }

    bool add(int u, int v, int w) {
        if (u == v) return false;

        auto [max_w, t] = path_max(u, v);
        bool merged = max_w == INT_MAX;

        if (!merged) {
            if (w >= max_w) return false;
            cut(t);
        }

        long long pu = potential(u), pv = potential(v);
        int du = 0, dv = 0;
        long long su = 0, sv = 0;

        for (;;) {
            if (w >= weight[u]) {
                int p = parent[u];

                su += lazy[p] * du;
                size[p] += du;
                sum[p] += su;

                pu -= lazy[u];
                u = p;
            } else if (w >= weight[v]) {
                int p = parent[v];

                sv += lazy[p] * dv;
                size[p] += dv;
                sum[p] += sv;

                pv -= lazy[v];
                v = p;
            } else {
                if (size[u] > size[v]) {
                    swap(u, v);
                    swap(du, dv);
                    swap(su, sv);
                    swap(pu, pv);
                }

                int len = size[u];
                long long value = sum[u];
                long long old_lazy = lazy[u];
                int old_parent = parent[u];
                int old_weight = weight[u];

                if (~old_parent) {
                    long long next_sum = su + lazy[old_parent] * du;

                    du -= len;
                    su = next_sum - value - lazy[old_parent] * len;

                    size[old_parent] += du;
                    sum[old_parent] += su;
                }

                long long delta = pu - old_lazy - pv;
                lazy[u] += delta;
                sum[u] += delta * len;

                long long value_v = sum[u] + lazy[v] * len;

                dv += len;
                sv += value_v;
                size[v] += len;
                sum[v] += value_v;

                parent[u] = v;
                weight[u] = w;
                w = old_weight;

                if (!~old_parent) {
                    for (; ~parent[v]; v = parent[v]) {
                        int p = parent[v];

                        sv += lazy[p] * dv;
                        size[p] += dv;
                        sum[p] += sv;
                    }
                    return merged;
                }

                pu -= old_lazy;
                u = old_parent;
            }
        }
    }

    bool erase(int u, int v, int w) {
        auto [max_w, t] = path_max(u, v);
        if (!~t || w >= max_w) return false;

        cut(t);
        return true;
    }

    void add_vertex(int v, long long x) {
        upward_maintain(v);

        for (; ~v; v = parent[v]) sum[v] += x;
    }

    void add_all(int v, long long x) {
        upward_maintain(v);

        int r = root(v);
        lazy[r] += x;
        sum[r] += x * size[r];
    }

    long long fold(int v) {
        upward_maintain(v);
        return sum[root(v)];
    }
};

struct DynamicConnectivity {
    struct Event {
        int u, v, w;
    };

    int n;
    vector<Event> events;
    unordered_map<pair<int, int>, int, Hash> active;
    vector<pair<int, function<void(FoldableAntiMonopolyTree &)>>> queries;

    DynamicConnectivity(int n) : n(n) {}

    void link(int u, int v) {
        if (u > v) swap(u, v);

        active[{u, v}] = events.size();
        events.push_back({u, v, INT_MIN});
    }

    void cut(int u, int v) {
        if (u > v) swap(u, v);

        int i = active[{u, v}];
        active.erase({u, v});

        events[i].w = -(int) events.size();
        events.push_back({u, v, 1});
    }

    template <typename F>
    void query(F f) {
        queries.emplace_back(events.size(), f);
    }

    void solve(FoldableAntiMonopolyTree &amt) {
        int k = 0;

        for (int i = 0; i < (int) events.size(); i++) {
            while (k < (int) queries.size() && queries[k].first == i)
                queries[k++].second(amt);

            auto [u, v, w] = events[i];

            if (w == 1) amt.erase(u, v, -(i + 1));
            else amt.add(u, v, w);
        }

        while (k < (int) queries.size())
            queries[k++].second(amt);
    }
};

int main() {
    ios::sync_with_stdio(false);

    int n, q;
    fin >> n >> q;

    vector<long long> a(n);
    for (auto &x : a) fin >> x;

    DynamicConnectivity dc(n);

    for (int i = 1; i < n; i++) {
        int u, v;
        fin >> u >> v;
        dc.link(u, v);
    }

    while (q--) {
        int type;
        fin >> type;

        if (type == 0) {
            int u, v, w, x;
            fin >> u >> v >> w >> x;

            dc.cut(u, v);
            dc.link(w, x);
        } else if (type == 1) {
            int v, p;
            long long x;
            fin >> v >> p >> x;

            dc.cut(v, p);
            dc.query([v, x](auto &amt) {
                amt.add_all(v, x);
            });
            dc.link(v, p);
        } else {
            int v, p;
            fin >> v >> p;

            dc.cut(v, p);
            dc.query([v](auto &amt) {
                fout << amt.fold(v) << '\n';
            });
            dc.link(v, p);
        }
    }

    FoldableAntiMonopolyTree amt(a);
    dc.solve(amt);
}