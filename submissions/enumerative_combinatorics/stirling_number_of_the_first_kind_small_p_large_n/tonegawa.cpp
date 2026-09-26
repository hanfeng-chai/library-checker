

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











#include <cstdint>

struct barrett{
  unsigned int _m;
  unsigned long long im;
  explicit barrett(unsigned int m) : _m(m), im((unsigned long long)(-1) / m + 1){}
  unsigned int umod()const{return _m;}
  unsigned int mul(unsigned int a, unsigned int b)const{
    unsigned long long z = a;
    z *= b;
#ifdef _MSC_VER
    unsigned long long x;
    _umul128(z, im, &x);
#else
    unsigned long long x = (unsigned long long)(((unsigned __int128)(z) * im) >> 64);
#endif
    unsigned long long y = x * _m;
    return (unsigned int)(z - y + (z < y ? _m : 0));
  }
};





// @param m `1 <= m`
constexpr long long safe_mod(long long x, long long m){
  x %= m;
  if (x < 0) x += m;
  return x;
}


// -10^18 <= _a, _b <= 10^18
long long gcd(long long _a, long long _b) {
    long long a = abs(_a), b = abs(_b);
    if (a == 0) return b;
    if (b == 0) return a;
    int shift = __builtin_ctzll(a | b);
    a >>= __builtin_ctzll(a);
    do{
        b >>= __builtin_ctzll(b);
        if(a > b) std::swap(a, b);
        b -= a;
    } while (b);
    return a << shift;
}

// 最大でa*b
// -10^18 <= a, b <= 10^18
// a, bは負でもいいが非負の値を返す
__int128_t lcm(long long a, long long b) {
    a = abs(a), b = abs(b);
    long long g = gcd(a, b);
    if (!g) return 0;
    return __int128_t(a) * b / g;
}

// {x, y, gcd(a, b)} s.t. ax + by = gcd(a, b)
// g >= 0
std::tuple<long long, long long, long long> extgcd(long long a, long long b) {
    long long x, y;
    for (long long u = y = 1, v = x = 0; a;) {
        long long q = b / a;
        std::swap(x -= q * u, u);
        std::swap(y -= q * v, v);
        std::swap(b -= q * a, a);
    }
    // x + k * (b / g), y - k * (a / g) も条件を満たす(kは任意の整数)
    return {x, y, b};
}

// @param b `1 <= b`
// @return pair(g, x) s.t. g = gcd(a, b), xa = g (mod b), 0 <= x < b/g
constexpr std::pair<long long, long long> inv_gcd(long long a, long long b) {
    a = safe_mod(a, b);
    if (a == 0) return {b, 0};
    long long s = b, t = a;
    long long m0 = 0, m1 = 1;
    while (t) {
        long long u = s / t;
        s -= t * u;
        m0 -= m1 * u;
        auto tmp = s;
        s = t;
        t = tmp;
        tmp = m0;
        m0 = m1;
        m1 = tmp;
    }
    if (m0 < 0) m0 += b / s;
    return {s, m0};
}



template<int id> 
struct modint32_dynamic {
    using mint = modint32_dynamic;
  public:
    
    static int mod() { return (int)(bt.umod()); }
    static void set_mod(int m) {
        assert(1 <= m);
        bt = barrett(m);
    }
    static mint raw(int v) {
        mint x;
        x._v = v;
        return x;
    }
  
    modint32_dynamic(): _v(0) {}
    
    template <class T>
    modint32_dynamic(T v) {
        long long x = v % (long long)(mod());
        if (x < 0) x += mod();
        _v = x;
    }
    unsigned int val() const { return _v; }
  
    mint& operator ++ () {
        _v++;
        if (_v == umod()) _v = 0;
        return *this;
    }
    mint& operator -- () {
        if (_v == 0) _v = umod();
        _v--;
        return *this;
    }
    mint operator ++ (int) {
        mint result = *this;
        ++*this;
        return result;
    }
    mint operator -- (int) {
        mint result = *this;
        --*this;
        return result;
    }
    mint& operator += (const mint& rhs) {
        _v += rhs._v;
        if (_v >= umod()) _v -= umod();
        return *this;
    }
    mint& operator -= (const mint& rhs) {
        _v += mod() - rhs._v;
        if (_v >= umod()) _v -= umod();
        return *this;
    }
    mint& operator *= (const mint& rhs) {
        _v = bt.mul(_v, rhs._v);
        return *this;
    }
    mint& operator /= (const mint& rhs) { return *this = *this * rhs.inv(); }
    mint operator + () const { return *this; }
    mint operator - () const { return mint() - *this; }
    mint pow(long long n) const {
        assert(0 <= n);
        mint x = *this, r = 1;
        while (n) {
            if (n & 1) r *= x;
            x *= x;
            n >>= 1;
        }
        return r;
    }
    mint inv() const {
        auto eg = inv_gcd(_v, mod());
        assert(eg.first == 1);
        return eg.second;
    }
    friend mint operator + (const mint& lhs, const mint& rhs) { return mint(lhs) += rhs; }
    friend mint operator - (const mint& lhs, const mint& rhs) { return mint(lhs) -= rhs; }
    friend mint operator * (const mint& lhs, const mint& rhs) { return mint(lhs) *= rhs; }
    friend mint operator / (const mint& lhs, const mint& rhs) { return mint(lhs) /= rhs; }
    friend bool operator == (const mint& lhs, const mint& rhs) { return lhs._v == rhs._v; }
    friend bool operator != (const mint& lhs, const mint& rhs) { return lhs._v != rhs._v; }
  private:
    unsigned int _v;
    static barrett bt;
    static unsigned int umod() { return bt.umod(); }
};

template <int id>
barrett modint32_dynamic<id>::bt(998244353);

template<int id>
std::ostream &operator<<(std::ostream &dest, const modint32_dynamic<id> &a) {
    dest << a.val();
    return dest;
}

// 関数の内部などで使う用
using temporary_modint32 = modint32_dynamic<std::numeric_limits<int>::min()>;




template<typename mint>
struct combination_mod {
  private:
    static int N;
    static std::vector<mint> F, FI, I;

  public:
    static bool built() { return !F.empty(); }

    static void clear() { N = 0; }

    // [0, N]を扱えるようにする
    // dynamic modint 等でmodを変えて再びbuildするときはclearを呼んでおく
    // O(logMOD + 増えた分)
    static void build(int _N) {
        _N++;
        assert(0 < _N && _N <= mint::mod());
        if (N >= _N) return;
        
        int preN = N;
        N = _N;
        F.resize(N);
        FI.resize(N);
        I.resize(N);
    
        F[0] = 1;
        for (int i = std::max(1, preN); i < N; i++) {
            F[i] = F[i - 1] * i;
        }
        FI[N - 1] = mint(F[N - 1]).inv();
        
        for (int i = N - 1; i >= std::max(1, preN); i--) {
            FI[i - 1] = FI[i] * i;
            I[i] = FI[i] * F[i - 1];
        }
    }

    static mint inv(int k) {
        return I[k];
    }

    using TypeMod = typename std::invoke_result<decltype(&mint::mod)>::type; // modintの内部的な整数型
    
    static mint inv_large(TypeMod k) {
        if constexpr (std::is_same<TypeMod, int>::value) {
            long long res = 1;
            while (k >= N) {
                int q = -(mint::mod() / k);
                res *= q;
                res %= mint::mod();
                k = mint::mod() + q * k;
            }
            return mint(res) * I[k];
        } else {
            mint res = 1;
            while (k >= N) {
                TypeMod q = -(mint::mod() / k);
                res *= q;
                k = mint::mod() + q * k;
            }
            return res * I[k];
        }
    }

    static mint fac(int k) {
        return F[k];
    }

    static mint ifac(int k) {
        return FI[k];
    }

    static mint comb(int a, int b) {
        if (a < b || b < 0) return 0;
        return F[a] * FI[a - b] * FI[b];
    }

    static mint icomb(int a, int b) {
        assert(a >= b && b >= 0);
        return FI[a] * F[a - b] * F[b];
    }
    
    // O(b)
    static mint comb_small(int a, int b) {
        assert(b < mint::mod());
        if (a < b) return 0;
        mint res = 1;
        for (int i = 0; i < b; i++) res *= a - i;
        return res * FI[b];
    }

    // O(|b|) sum(b) = a
    static mint comb_multi(int a, const std::vector<int> &b) {
        mint res = 1;
        for (int r : b) {
            res *= comb(a, r);
            a -= r;
        }
        if (a == 0) return res;
        return 0;
    }

    static mint perm(int a, int b) {
        if (a < b || b < 0) return 0;
        return F[a] * FI[a - b];
    }

    static mint iperm(int a, int b) {
        assert(a >= b && b >= 0);
        return FI[a] * F[a - b];
    }

    // O(b)
    static mint perm_small(int a, int b) {
        assert(b < mint::mod());
        if (a < b) return 0;
        mint res = 1;
        for (int i = 0; i < b; i++) res *= a - i;
        return res;
    }
};

template<typename mint>
int combination_mod<mint>::N = 0;
template<typename mint>
std::vector<mint> combination_mod<mint>::F;
template<typename mint>
std::vector<mint> combination_mod<mint>::FI;
template<typename mint>
std::vector<mint> combination_mod<mint>::I;


// mod == 2: 定数時間
// modが素数: 初期化 O(mod), クエリ O(log(n))
template<int id>
struct lucas_prime {
    using mint = modint32_dynamic<id>;
    using cmb = combination_mod<mint>;
    
    static void set_mod(int mod) {
        assert(0 < mod && mod < 1e7);
        mint::set_mod(mod);
        cmb::build(mint::mod() - 1);
    }

    static int comb(long long n, long long r) {
        if (mint::mod() == 1 || n < 0 || r < 0 || n < r) return 0;
        if (mint::mod() == 2) return (n & r) == r;
        mint res = 1;
        while (n) {
            int x = n % mint::mod(), y = r % mint::mod();
            n /= mint::mod(), r /= mint::mod();
            res *= cmb::comb(x, y);
        }
        return res.val();
    }

    // sum(r) = n
    static int comb_multi(long long n, const std::vector<long long> &r) {
        if (mint::mod() == 1 || n < 0) return 0;

        {
            long long S = 0, O = 0;
            for (long long x : r) {
                if (n < x || x < 0) return 0;
                S += x;
                O |= x;
            }
            if (S != n) return 0;
            if (mint::mod() == 2) {
                return n == O;
            }
        }

        mint res = 1;
        std::vector<int> tmp(r.size());
        while (n) {
            int x = n % mint::mod();
            n /= mint::mod();
            for (int i = 0; i < r.size(); i++) {
                tmp[i] = r[i] % mint::mod();
                r[i] /= mint::mod();
            }
            res *= cmb::comb_multi(x, tmp);
        }
        return res.val();
    }
};

template<int id>
struct lucas_prime_power {
  private:
    static int p, q, P;
    static std::vector<int> F, FI;
    static barrett br;
    static int _kth_comb(int a, int b, int c) {
        return br.mul(F[a], br.mul(FI[b], FI[c]));
    }

    static int _comb_p2q1(long long n, long long r) {
        assert(p == 2 && q == 1);
        return (n & r) == r;
    }

    static int _comb_q1(long long n, long long r) {
        assert(q == 1);
        int res = 1;
        while (n) {
            int x = n % p, y = r % p;
            if (x < y) return 0;
            n /= p, r /= p;
            res = br.mul(res, br.mul(F[x], br.mul(FI[y], FI[x - y])));
        }
        return res;
    }
 public:

    // mod p^q
    static void set_mod(int _p, int _q) {
        p = _p;
        q = _q;
        P = 1;
        for (int i = 0; i < q; i++) P *= p;
        F.resize(P);
        FI.resize(P);
        br = barrett(P);

        F[0] = 1;
        for (int i = 1, j = p; i < P; i++) {
            if (i == j) {
                F[i] = F[i - 1];
                j += p;
            } else {
                F[i] = br.mul(F[i - 1], i);
            }
        }
        FI[P - 1] = inv_gcd(F.back(), P).second;
        for (int i = P - 1, j = P - p; i > 0; i--) {
            if (i == j) {
                FI[i - 1] = FI[i];
                j -= p;
            } else {
                FI[i - 1] = br.mul(FI[i], i);
            }
        }
    }

    static int comb(long long n, long long r) {
        if (n < r || r < 0) return 0;
        if (n == 0) return 1;
        
        if (p == 2 && q == 1) return _comb_p2q1(n, r);
        if (q == 1) return _comb_q1(n, r);

        int x = 1;
        long long m = n - r;
        std::vector<int> k;

        while (n) {
            x = br.mul(x, _kth_comb(n % P, m % P, r % P));
            if (x == 0) return 0;
            bool eps = k.empty() ? 0 : k.back();
            k.push_back(m % p + r % p + eps >= p);
            n /= p, m /= p, r /= p;
        }
        
        for (int i = (int)k.size() - 2; i >= 0; i--) k[i] += k[i + 1];
        
        int y = 1;
        for (int i = 0; i < k[0]; i++) y = br.mul(y, p);
        if ((p != 2 || q <= 2) && k.size() > q - 1 && k[q - 1] % 2 == 1) {
            x = (x == 0 ? 0 : P - x);
        }
        return br.mul(x, y);
    }
};

template<int id>
int lucas_prime_power<id>::p = 0;
template<int id>
int lucas_prime_power<id>::q = 0;
template<int id>
int lucas_prime_power<id>::P = 0;
template<int id>
std::vector<int> lucas_prime_power<id>::F;
template<int id>
std::vector<int> lucas_prime_power<id>::FI;
template<int id>
barrett lucas_prime_power<id>::br(998244353);



template<int id>
struct stirling_number_small_prime {
    using mint = modint32_dynamic<id>;
    using cmb = combination_mod<mint>;
    using luc = lucas_prime<id>;
    
    static void set_mod(int mod) {
        assert(0 < mod && mod < 1e7);
        mint::set_mod(mod);
        cmb::build(mod - 1);
        luc::set_mod(mod);
    }

    static mint first(long long n, long long k) {
        static std::vector<std::vector<mint>> table;
        if (table.empty()) {
            table.resize(mint::mod(), std::vector<mint>(mint::mod(), 0));
            table[0][0] = 1;
            for (int i = 1; i < mint::mod(); i++) {
                table[i][0] = 0;
                for (int j = 1; j <= i; j++) {
                    table[i][j] = table[i - 1][j - 1] + table[i - 1][j] * (-i + 1);
                }
            }
        }
        if (n < k) return 0;
        if (n < mint::mod()) return table[n][k];
        if (k == 0) return n == 0;
        long long i = n / mint::mod();
        long long j = n - i * mint::mod();
        if (i > k) return 0;
        long long t = k - i;
        long long a = t / (mint::mod() - 1);
        long long b = t - a * (mint::mod() - 1);
        mint res = 0;
        if (a <= i && b <= j) {
            res += ((i - a) % 2 != 0 ? -1 : 1) * luc::comb(i, a) * first(j, b);
        }
        if (a > 0 && a - 1 <= i && b == 0 && j == mint::mod() - 1) {
            a--;
            b = mint::mod() - 1;
            res += ((i - a) % 2 != 0 ? -1 : 1) * luc::comb(i, a) * first(j, b);
        }
        return res;
    }
    
    static mint second(long long n, long long k) {

    }
};




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



int main() {
    fast_io<1 << 25, 1 << 25> io;
    int T, p;
    T = io.in();
    p = io.in();
    using st = stirling_number_small_prime<0>;
    st::set_mod(p);
    for (int i = 0; i < T; i++) {
        long long n, k;
        n = io.in();
        k = io.in();
        io.out(st::first(n, k).val(), '\n');
    }
}
