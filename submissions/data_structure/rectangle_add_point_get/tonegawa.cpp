

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <tuple>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <bitset>
#include <cmath>
#include <functional>
#include <cassert>
#include <climits>
#include <iomanip>
#include <numeric>
#include <memory>
#include <random>
#include <thread>
#include <chrono>
#define allof(obj) (obj).begin(), (obj).end()
#define range(i, l, r) for(int i=l;i<r;i++)
#define unique_elem(obj) obj.erase(std::unique(allof(obj)), obj.end())
#define bit_subset(i, S) for(int i=S, zero_cnt=0;(zero_cnt+=i==S)<2;i=(i-1)&S)
#define bit_kpop(i, n, k) for(int i=(1<<k)-1,x_bit,y_bit;i<(1<<n);x_bit=(i&-i),y_bit=i+x_bit,i=(!i?(1<<n):((i&~y_bit)/x_bit>>1)|y_bit))
#define bit_kth(i, k) ((i >> k)&1)
#define bit_highest(i) (i?63-__builtin_clzll(i):-1)
#define bit_lowest(i) (i?__builtin_ctzll(i):-1)
#define sleepms(t) std::this_thread::sleep_for(std::chrono::milliseconds(t))
using ll = long long;
using ld = long double;
using ul = uint64_t;
using pi = std::pair<int, int>;
using pl = std::pair<ll, ll>;
using namespace std;

template<typename F, typename S>
std::ostream &operator << (std::ostream &dest, const std::pair<F, S> &p) {
    dest << p.first << ' ' << p.second;
    return dest;
}

template<typename A, typename B>
std::ostream &operator << (std::ostream &dest, const std::tuple<A, B> &t) {
    dest << std::get<0>(t) << ' ' << std::get<1>(t);
    return dest;
}

template<typename A, typename B, typename C>
std::ostream &operator << (std::ostream &dest, const std::tuple<A, B, C> &t) {
    dest << std::get<0>(t) << ' ' << std::get<1>(t) << ' ' << std::get<2>(t);
    return dest;
}

template<typename A, typename B, typename C, typename D>
std::ostream &operator << (std::ostream &dest, const std::tuple<A, B, C, D> &t) {
    dest << std::get<0>(t) << ' ' << std::get<1>(t) << ' ' << std::get<2>(t) << ' ' << std::get<3>(t);
    return dest;
}

template<typename T>
std::ostream &operator << (std::ostream &dest, const std::vector<std::vector<T>> &v) {
    int sz = v.size();
    if (!sz) return dest;
    for (int i = 0; i < sz; i++) {
        int m = v[i].size();
        for (int j = 0; j < m; j++) dest << v[i][j] << (i != sz - 1 && j == m - 1 ? '\n' : ' ');
    }
    return dest;
}

template<typename T>
std::ostream &operator << (std::ostream &dest, const std::vector<T> &v) {
    int sz = v.size();
    if (!sz) return dest;
    for (int i = 0; i < sz - 1; i++) dest << v[i] << ' ';
    dest << v[sz - 1];
    return dest;
}

template<typename T, size_t sz>
std::ostream &operator << (std::ostream &dest, const std::array<T, sz> &v) {
    if (!sz) return dest;
    for (int i = 0; i < sz - 1; i++) dest << v[i] << ' ';
    dest << v[sz - 1];
    return dest;
}

template<typename T>
std::ostream &operator << (std::ostream &dest, const std::set<T> &v) {
    for (auto itr = v.begin(); itr != v.end();) {
        dest << *itr;
        itr++;
        if (itr != v.end()) dest << ' ';
    }
    return dest;
}

template<typename T, typename E>
std::ostream &operator << (std::ostream &dest, const std::map<T, E> &v) {
    for (auto itr = v.begin(); itr != v.end(); ) {
        dest << '(' << itr->first << ", " << itr->second << ')';
        itr++;
        if (itr != v.end()) dest << '\n';
    }
    return dest;
}

template<typename T>
vector<T> make_vec(size_t sz, T val) { return std::vector<T>(sz, val); }

template<typename T, typename... Tail>
auto make_vec(size_t sz, Tail ...tail) {
    return std::vector<decltype(make_vec<T>(tail...))>(sz, make_vec<T>(tail...));
}

template<typename T>
vector<T> read_vec(size_t sz) {
    std::vector<T> v(sz);
    for (int i = 0; i < (int)sz; i++) std::cin >> v[i];
    return v;
}

template<typename T, typename... Tail>
auto read_vec(size_t sz, Tail ...tail) {
    auto v = std::vector<decltype(read_vec<T>(tail...))>(sz);
    for (int i = 0; i < (int)sz; i++) v[i] = read_vec<T>(tail...);
    return v;
}

long long max(long long a, int b) { return std::max(a, (long long)b); }
long long max(int a, long long b) { return std::max((long long)a, b); }
long long min(long long a, int b) { return std::min(a, (long long)b); }
long long min(int a, long long b) { return std::min((long long)a, b); }
long long modulo(long long a, long long m) { a %= m; return a < 0 ? a + m : a; }

// x / y以上の最小の整数
ll ceil_div(ll x, ll y) {
    assert(y > 0);
    return (x + (x > 0 ? y - 1 : 0)) / y;
}

// x / y以下の最大の整数
ll floor_div(ll x, ll y) {
    assert(y > 0);
    return (x + (x > 0 ? 0 : -y + 1)) / y;
}

void io_init() {
    std::cin.tie(nullptr);
    std::ios::sync_with_stdio(false);
}



#include <unistd.h>

// サイズは空白や改行も含めた文字数
template<int size_in = 1 << 25, int size_out = 1 << 25>
struct fast_io {
    char ibuf[size_in], obuf[size_out];
    char *ip, *op;

    fast_io() : ip(ibuf), op(obuf) {
        int t = 0, k = 0;
        while ((k = read(STDIN_FILENO, ibuf + t, sizeof(ibuf) - t)) > 0) {
            t += k;
        }
    }
    
    ~fast_io() {
        int t = 0, k = 0;
        while ((k = write(STDOUT_FILENO, obuf + t, op - obuf - t)) > 0) {
            t += k;
        }
    }
  
    long long in() {
        long long x = 0;
        bool neg = false;
        for (; *ip < '+'; ip++) ;
        if (*ip == '-'){ neg = true; ip++;}
        else if (*ip == '+') ip++;
        for (; *ip >= '0'; ip++) x = 10 * x + *ip - '0';
        if (neg) x = -x;
        return x;
    }

    unsigned long long inu64() {
        unsigned long long x = 0;
        for (; *ip < '+'; ip++) ;
        if (*ip == '+') ip++;
        for (; *ip >= '0'; ip++) x = 10 * x + *ip - '0';
        return x;
    }

    char in_char() {
        for (; *ip < '!'; ip++) ;
        return *ip++;
    }
  
    void out(long long x, char c = 0) {
        static char tmp[20];
        if (!x) {
            *op++ = '0';
        } else {
            int i;
            if (x < 0) {
                *op++ = '-';
                x = -x;
            }
            for (i = 0; x; i++) {
                tmp[i] = x % 10;
                x /= 10;
            }
            for (i--; i >= 0; i--) *op++ = tmp[i] + '0';
        }
        if (c) *op++ = c;
    }

    void outu64(unsigned long long x, char c = 0) {
        static char tmp[20];
        if (!x) {
            *op++ = '0';
        } else {
            int i;
            for (i = 0; x; i++) {
                tmp[i] = x % 10;
                x /= 10;
            }
            for (i--; i >= 0; i--) *op++ = tmp[i] + '0';
        }
        if (c) *op++ = c;
    }

    void out_char(char x, char c = 0){
        *op++ = x;
        if (c) *op++ = c;
    }

    long long memory_size() {
        return (long long)(size_in + size_out) * sizeof(char);
    }
};


/*
#include ".lib/data_structure/rectangle/point_add_rectangle_sum.hpp"

int main() {
    fast_io<1 << 24, 1 << 22> io;
    int N = io.in();
    int Q = io.in();

    point_add_rectangle_sum<int, long long> rect;
    for (int i = 0; i < N; i++) {
        int a = io.in();
        int b = io.in();
        int c = io.in();
        rect.apply(a, b, c);
    }

    for (int i = 0; i < Q; i++) {
        int t = io.in();
        if (t == 0) {
            int a = io.in();
            int b = io.in();
            int c = io.in();
            rect.apply(a, b, c);
        } else {
            int a = io.in();
            int b = io.in();
            int c = io.in();
            int d = io.in();
            rect.prod(a, c, b, d);
        }
    }
    for (long long ans : rect.solve()) {
        io.out(ans, '\n');
    }
}
*/










unsigned int bit_ceil(unsigned int n) {
    unsigned int x = 1;
    while (x < (unsigned int)(n)) x *= 2;
    return x;
}


// 可換でないと壊れる
// prefixだけなら逆元がいらない
// 区間クエリは逆元が必要
template<typename T, T (*id)(), T (*op)(T, T), T (*inv)(T)>
struct fenwick_tree_abstract {
  private:
    int N, W;
    std::vector<T> V;
  
  public:
    fenwick_tree_abstract() {}
    fenwick_tree_abstract(int _N) : N(_N), W(bit_ceil(N)), V(N + 1 , id()) {}
    fenwick_tree_abstract(const std::vector<T> &v): N(v.size()), W(bit_ceil(N)), V(1, id()) {
        V.insert(V.begin() + 1, v.begin(), v.end());
        for (int i = 1; i <= N; i++) {
            int nxt = i + (i & (-i));
            if (nxt <= N) V[nxt] += V[i];
        }
    }

    // V[k] <- op(V[k], x)
    void apply(int k, T x) {
        for (int i = k + 1; i <= N; i += (i & (-i))) {
            V[i] = op(V[i], x);
        }
    }

    // prod[0, r)
    T prod(int r) {
        T res = id();
        for (int k = r; k > 0; k -= (k & (-k))) {
            res = op(V[k], res);
        }
        return res;
    }

    // 逆元がある必要がある
    T prod(int l, int r) {
        return op(inv(prod(l)), prod(r));
    }

    // f(op(a[0], a[1], ..., a[r - 1])) = trueとなる最大のr
    // f(id) = true
    // 0 <= r <= N
    template<bool (*f)(T)>
    int max_right() {
        assert(f(id()));
        int r = 0, w = W;
        T s = 0, tmp;
        while (w) {
            if (r + w <= N && f((tmp = op(s, V[r + w])))) {
                s = tmp;
                r += w;
            }
            w >>= 1;
        }
        return r;
    }
};

template<typename T>
struct _fenwick_add_sum {
    using Val = T;
    static Val id() { return 0; }
    static Val inv(Val a) { return -a; }
    static Val merge(Val a, Val b) { return a + b; }
};

template<typename T>
using fenwick_tree = fenwick_tree_abstract<T, _fenwick_add_sum<T>::id, _fenwick_add_sum<T>::merge, _fenwick_add_sum<T>::inv>;


template<typename Idx, typename Val>
struct rectangle_add_point_get {
  private:
    static constexpr int qlim = 1e8;
    struct Query {
        Idx x, y;
        int id;
    };
    struct Update {
        Idx lx, rx, ly, ry;
        Val z;
    };
    struct Event {
        Idx x;
        int lyc, ryc;
        Val z;
    };
    std::vector<Update> U;
    std::vector<Query> Q;
    std::vector<std::pair<bool, int>> T;
    
    void solve(int l, int r, std::vector<Val> &ans) {
        static constexpr int DO_NAIVE = 20;
        if (r - l < 2) return;
        if (r - l <= DO_NAIVE) {
            for (int i = l; i < r; i++) {
                if (T[i].first == 1) continue;
                Idx x = Q[T[i].second].x;
                Idx y = Q[T[i].second].y;
                for (int j = l; j < i; j++) {
                    int k = T[j].second;
                    if (T[j].first == 1 && U[k].lx <= x && x < U[k].rx && U[k].ly <= y && y < U[k].ry) {
                       ans[T[i].second] += U[k].z;
                    }
                }
            }
            return;
        }
        
        int mid = (l + r) / 2;
        solve(l, mid, ans);
        solve(mid, r, ans);
        std::vector<Idx> Y;
        for (int i = mid; i < r; i++) {
            if (T[i].first) continue;
            int id = T[i].second;
            Y.push_back(Q[id].y);
        }

        if (Y.empty()) return;
        std::sort(Y.begin(), Y.end());
        Y.erase(std::unique(Y.begin(), Y.end()), Y.end());
        std::vector<Event> E;
        for (int i = l; i < mid; i++) {
            if (!T[i].first) continue;
            int id = T[i].second;
            int lyc = std::lower_bound(Y.begin(), Y.end(), U[id].ly) - Y.begin();
            int ryc = std::lower_bound(Y.begin(), Y.end(), U[id].ry) - Y.begin();
            E.push_back(Event{U[id].lx, lyc, ryc, U[id].z});
            E.push_back(Event{U[id].rx, lyc, ryc, -U[id].z});
        }

        for (int i = mid; i < r; i++) {
            if (T[i].first) continue;
            int id = T[i].second;
            int y = std::lower_bound(Y.begin(), Y.end(), Q[id].y) - Y.begin();
            E.push_back(Event{Q[id].x, y, Q[id].id + qlim, 0});
        }

        std::sort(E.begin(), E.end(), [](const Event &a, const Event &b) {
            if (a.x == b.x) return a.ryc < b.ryc;
            return a.x < b.x;
        });

        fenwick_tree<Val> ft(Y.size());
        for (const Event &e : E) {
            if (e.ryc < qlim) {
                if (e.lyc < Y.size()) ft.apply(e.lyc, e.z);
                if (e.ryc < Y.size()) ft.apply(e.ryc, -e.z);
            } else {
                int id = e.ryc - qlim;
                ans[id] += ft.prod(e.lyc + 1);
            }
        }
    }

  public:
    // [lx, rx) × [ly, ry)にzを足す
    void apply(Idx lx, Idx rx, Idx ly, Idx ry, Val z) {
        T.push_back({1, U.size()});
        U.push_back(Update{lx, rx, ly, ry, z});
    }

    // get(x, y)
    void prod(Idx x, Idx y) {
        T.push_back({0, Q.size()});
        Q.push_back(Query{x, y, (int)Q.size()});
    }

    std::vector<Val> solve() {
        std::vector<Val> ans(Q.size(), 0);
        solve(0, T.size(), ans);
        return ans;
    }
};

int main() {
    fast_io<1 << 24, 1 << 23> io;
    int N = io.in();
    int Q = io.in();
    rectangle_add_point_get<int, ll> rect;
    range(i, 0, N) {
        int lx, ly, rx, ry, z;
        lx = io.in();
        ly = io.in();
        rx = io.in();
        ry = io.in();
        z = io.in();
        rect.apply(lx, rx, ly, ry, z);
    }

    range(i, 0, Q) {
        int t = io.in();
        if (!t) {
            int lx, ly, rx, ry, z;
            lx = io.in();
            ly = io.in();
            rx = io.in();
            ry = io.in();
            z = io.in();
            rect.apply(lx, rx, ly, ry, z);
        } else {
            int x, y;
            x = io.in();
            y = io.in();
            rect.prod(x, y);
        }
    }
    for(ll x : rect.solve()) io.out(x, '\n');
}
