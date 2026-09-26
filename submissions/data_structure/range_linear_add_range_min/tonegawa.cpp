

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




#include <limits>



constexpr unsigned int bit_ceil(unsigned int n) {
    unsigned int x = 1;
    while (x < (unsigned int)(n)) x *= 2;
    return x;
}

constexpr int bit_ceil_log(unsigned int n) {
    int x = 0;
    while ((1 << x) < (unsigned int)(n)) x++;
    return x;
}


// T : 2 * 値が収まる型
// TN : 2 * (Tの最大値) * (要素数) が収まる型
// TN2 : 2 * (Tの最大値) * (要素数)^2 が収まる型
template <typename T, typename TN, typename TN2>
struct linear_add_range_min {
    struct point {
        int x;
        T y;
        static TN cross(const point &a, const point &b, const point &c) {
            return (TN)(b.y - a.y) * (c.x - a.x) - (TN)(c.y - a.y) * (b.x - a.x);
        }
    };
    struct node {
        point lbr, rbr;
        T lza, lzb;
        // 葉
        node(int x, T y): lbr{x, y}, rbr{x, y}, lza(0), lzb(0){}
        // 葉以外
        node(): lza(0), lzb(0){}
    };
  public:
    static constexpr T inf = std::numeric_limits<T>::max();

    linear_add_range_min() : linear_add_range_min(0) {}
    explicit linear_add_range_min(int n) : linear_add_range_min(std::vector<T>(n, 0)) {}
    explicit linear_add_range_min(const std::vector<T>& v) : _n(int(v.size())) {
        size = bit_ceil((unsigned int)(_n));
        log = bit_ceil_log((unsigned int)size);
        nd = std::vector<node>(2 * size);
        correct = std::vector<bool>(2 * size, true);
        for (int i = 0; i < size; i++) nd[size + i] = node(i, (i < _n ? v[i] : 0));
        for (int i = size - 1; i >= 1; i--) pull(i);
    }
    /*
    void set(int p, T x) {
        assert(0 <= p && p < _n);
        int P = p;
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        if(nd[p].lbr.y == x) return;
        bool is_decrease = nd[p].lbr.y > x;
        nd[p] = node(P, x);
        // nd[p]の値が増加した場合、元々の橋がpだった場合のみpull
        //           減少した場合、常にpull
        for (int i = 1; i <= log; i++) {
            if(is_decrease || nd[p >> i].lbr.x == P || nd[p >> i].rbr.x == P) {
                pull(p >> i);
            }
        }
    }
    */
    T get(int p) {
        assert(0 <= p && p < _n);
        p += size;
        T a = 0, b = 0;
        for (int i = log; i >= 1; i--) a += nd[p].lza, b += nd[p].lzb;
        return nd[p].lbr.y + (p - size) * a + b;
    }
    // [l, r)のmin
    T prod(int l, int r) {
        assert(0 <= l && l <= r && r <= _n);
        if (l == r) return inf;

        l += size;
        r += size;

        for (int i = log; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }

        T res = inf;
        while (l < r) {
            if (l & 1) res = std::min(res, min_subtree(l++));
            if (r & 1) res = std::min(res, min_subtree(--r));
            l >>= 1;
            r >>= 1;
        }

        return res;
    }
    // [l, r)にax+bを足したと仮定してのmin
    T prod_assume_add(int l, int r, T a, T b) {
        assert(0 <= l && l <= r && r <= _n);
        if (l == r) return inf;

        l += size;
        r += size;

        for (int i = log; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }

        T res = inf;
        while (l < r) {
            if (l & 1) res = std::min(res, min_subtree(l++, a, b));
            if (r & 1) res = std::min(res, min_subtree(--r, a, b));
            l >>= 1;
            r >>= 1;
        }

        return res;
    }
    void apply(int l, int r, T a, T b) {
        assert(0 <= l && l <= r && r <= _n);
        if (l == r) return;

        l += size;
        r += size;
        {
            int l2 = l, r2 = r;
            while (l < r) {
                if (l & 1) all_apply(l++, a, b);
                if (r & 1) all_apply(--r, a, b);
                l >>= 1;
                r >>= 1;
            }
            l = l2;
            r = r2;
        }

        for (int i = 1; i <= log; i++) {
            if (((l >> i) << i) != l) correct[l >> i] = false;
            if (((r >> i) << i) != r) correct[(r - 1) >> i] = false;
        }
    }
  private:
    int _n, size, log;
    std::vector<node> nd;
    std::vector<bool> correct;
    void all_apply(int k, T a, T b) {
        nd[k].lbr.y += a * nd[k].lbr.x + b;
        nd[k].rbr.y += a * nd[k].rbr.x + b;
        if (k < size) nd[k].lza += a, nd[k].lzb += b;
    }
    void push(int k) {
        all_apply(2 * k, nd[k].lza, nd[k].lzb);
        all_apply(2 * k + 1, nd[k].lza, nd[k].lzb);
        nd[k].lza = nd[k].lzb = 0;
    }
    int leftmost(int k) {
        int msb = 31 - __builtin_clz(k);
        return (k - (1 << msb)) << (log - msb);
    }
    void pull(int k) {
        assert(k < size);
        int l = k * 2, r = k * 2 + 1;
        push(k);
        if (!correct[l]) pull(l);
        if (!correct[r]) pull(r);
        int splitx = leftmost(r);
        T lza = 0, lzb = 0, lzA = 0, lzB = 0;
        point a = nd[l].lbr, b = nd[l].rbr, c = nd[r].lbr, d = nd[r].rbr;

        #define movel(f){\
            lza += nd[l].lza, lzb += nd[l].lzb;\
            l = l * 2 + f;\
            a = nd[l].lbr, b = nd[l].rbr;\
            a.y += lza * a.x + lzb;\
            b.y += lza * b.x + lzb;\
        }
        #define mover(f){\
            lzA += nd[r].lza, lzB += nd[r].lzb;\
            r = r * 2 + f;\
            c = nd[r].lbr, d = nd[r].rbr;\
            c.y += lzA * c.x + lzB;\
            d.y += lzA * d.x + lzB;\
        }
        while ((l < size) || (r < size)) {
            TN s1 = point::cross(a, b, c);
            if (l < size && s1 > 0) {
                movel(0);
            } else if (r < size && point::cross(b, c, d) > 0) {
                mover(1);
            } else if (l >= size) {
                mover(0);
            } else if (r >= size) {
                movel(1);
            } else {
                TN2 s2 = point::cross(b, a, d);
                if (s1 + s2 == 0 || (TN2)s1 * (d.x - splitx) < s2 * (splitx - c.x)) {
                    movel(1);
                } else {
                    mover(0);
                }
            }
        }
        nd[k].lbr = a;
        nd[k].rbr = c;
        correct[k] = true;
        #undef movel
        #undef mover
    }
    T min_subtree(int k, T a = 0, T b = 0) {
        if (!correct[k]) pull(k);
        while (k < size) {
            bool f = (nd[k].lbr.y - nd[k].rbr.y) > a * (nd[k].rbr.x - nd[k].lbr.x);
            a += nd[k].lza;
            b += nd[k].lzb;
            k = k * 2 + f;
        }
        return nd[k].lbr.y + a * nd[k].lbr.x + b;
    }
};


int main() {
    fast_io<1 << 25, 1 << 24> io;
    int N = io.in();
    int Q = io.in();
    std::vector<long long> A(N);
    for (int i = 0; i < N; i++) A[i] = io.in();
    linear_add_range_min<long long, long long, __int128_t> seg(A);

    for (int i = 0; i < Q; i++) {
        int t = io.in();
        int l = io.in();
        int r = io.in();
        if (t == 0) {
            int b = io.in();
            int c = io.in();
            seg.apply(l, r, b, c);
        } else {
            io.out(seg.prod(l, r), '\n');
        }
    }
}
