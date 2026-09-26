

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





#include <type_traits>








// @param m `1 <= m`
constexpr long long safe_mod(long long x, long long m){
  x %= m;
  if (x < 0) x += m;
  return x;
}

// x^n mod m
// @param n `0 <= n`
// @param m `1 <= m`
constexpr long long pow_mod_constexpr(long long x, long long n, int m) {
    if (m == 1) return 0;
    unsigned int _m = (unsigned int)(m);
    unsigned long long r = 1;
    unsigned long long y = safe_mod(x, m);
    while (n) {
        if (n & 1) r = (r * y) % _m;
        y = (y * y) % _m;
        n >>= 1;
    }
    return r;
}

constexpr __uint128_t pow_mod64_constexpr(__int128_t x, __uint128_t n, unsigned long long m) {
    if (m == 1) return 0;
    __uint128_t r = 1;
    if (x >= m) x %= m;
    if (x < 0) x += m;
    while (n) {
        if (n & 1) r = (r * x) % m;
        x = (x * x) % m;
        n >>= 1;
    }
    return r;
}



constexpr bool miller_rabin32_constexpr(int n) {
    if (n <= 1) return false;
    if (n == 2 || n == 7 || n == 61) return true;
    if (n % 2 == 0) return false;
    long long d = n - 1;
    while (d % 2 == 0) d /= 2;
    constexpr long long bases[3] = {2, 7, 61};
    for (long long a : bases) {
        long long t = d;
        long long y = pow_mod_constexpr(a, t, n);
        while (t != n - 1 && y != 1 && y != n - 1) { 
            y = y * y % n;
            t <<= 1;
        }
        if (y != n - 1 && t % 2 == 0) {
            return false;
        }
    }
    return true;
}

template<int n>
constexpr bool miller_rabin32 = miller_rabin32_constexpr(n);




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



template <int m, std::enable_if_t<(1 <= m)>* = nullptr>
struct modint32_static {
    using mint = modint32_static;
  public:
    static constexpr int mod() { return m; }
    
    static mint raw(int v) {
        mint x;
        x._v = v;
        return x;
    }
  
    modint32_static(): _v(0) {}
    
    template <class T>
    modint32_static(T v) { 
        long long x = v % (long long)umod();
        if (x < 0) x += umod();
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
        _v -= rhs._v;
        if (_v >= umod()) _v += umod();
        return *this;
    }
    mint& operator *= (const mint& rhs) {
        unsigned long long z = _v;
        z *= rhs._v;
        _v = (unsigned int)(z % umod());
        return *this;
    }
    mint& operator /= (const mint& rhs) { return *this = *this * rhs.inv(); }
    mint operator + () const { return *this; }
    mint operator-() const { return mint() - *this; }
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
        if (prime) {
            assert(_v);
            return pow(umod() - 2);
        } else {
            auto eg = inv_gcd(_v, m);
            assert(eg.first == 1);
            return eg.second;
        }
    }
    friend mint operator + (const mint& lhs, const mint& rhs) { return mint(lhs) += rhs; }
    friend mint operator - (const mint& lhs, const mint& rhs) { return mint(lhs) -= rhs; }
    friend mint operator * (const mint& lhs, const mint& rhs) { return mint(lhs) *= rhs; }
    friend mint operator / (const mint& lhs, const mint& rhs) { return mint(lhs) /= rhs; }
    friend bool operator == (const mint& lhs, const mint& rhs) { return lhs._v == rhs._v; }
    friend bool operator != (const mint& lhs, const mint& rhs) { return lhs._v != rhs._v; }
  private:
    unsigned int _v;
    static constexpr unsigned int umod() { return m; }
    static constexpr bool prime = miller_rabin32<m>;
};

template<int m>
std::ostream &operator<<(std::ostream &dest, const modint32_static<m> &a) {
    dest << a.val();
    return dest;
}

using modint998244353 = modint32_static<998244353>;
using modint1000000007 = modint32_static<1000000007>;






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
        assert(0 < _N && _N < mint::mod());
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








constexpr int primitive_root32_constexpr(int m) {
    if (m == 2) return 1;
    if (m == 167772161) return 3;
    if (m == 469762049) return 3;
    if (m == 754974721) return 11;
    if (m == 998244353) return 3;
    int divs[20] = {};
    divs[0] = 2;
    int cnt = 1;
    int x = (m - 1) / 2;
    while (x % 2 == 0) x /= 2;
    for (int i = 3; (long long)(i)*i <= x; i += 2) {
        if (x % i == 0) {
            divs[cnt++] = i;
            while (x % i == 0) {
                x /= i;
            }
        }
    }
    if (x > 1) divs[cnt++] = x;
    for (int g = 2;; g++) {
        bool ok = true;
        for (int i = 0; i < cnt; i++) {
            if (pow_mod_constexpr(g, (m - 1) / divs[i], m) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
}

template <int m>
constexpr int primitive_root32 = primitive_root32_constexpr(m);





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


template <class mint, int g = primitive_root32<mint::mod()>>
struct fft_info {
    static constexpr int rank2 = __builtin_ctz(mint::mod() - 1);
    std::array<mint, rank2 + 1> root;   // root[i]^(2^i) == 1
    std::array<mint, rank2 + 1> iroot;  // root[i] * iroot[i] == 1

    std::array<mint, std::max(0, rank2 - 2 + 1)> rate2;
    std::array<mint, std::max(0, rank2 - 2 + 1)> irate2;

    std::array<mint, std::max(0, rank2 - 3 + 1)> rate3;
    std::array<mint, std::max(0, rank2 - 3 + 1)> irate3;

    fft_info() {
        root[rank2] = mint(g).pow((mint::mod() - 1) >> rank2);
        iroot[rank2] = root[rank2].inv();
        for (int i = rank2 - 1; i >= 0; i--) {
            root[i] = root[i + 1] * root[i + 1];
            iroot[i] = iroot[i + 1] * iroot[i + 1];
        }

        {
            mint prod = 1, iprod = 1;
            for (int i = 0; i <= rank2 - 2; i++) {
                rate2[i] = root[i + 2] * prod;
                irate2[i] = iroot[i + 2] * iprod;
                prod *= iroot[i + 2];
                iprod *= root[i + 2];
            }
        }
        {
            mint prod = 1, iprod = 1;
            for (int i = 0; i <= rank2 - 3; i++) {
                rate3[i] = root[i + 3] * prod;
                irate3[i] = iroot[i + 3] * iprod;
                prod *= iroot[i + 3];
                iprod *= root[i + 3];
            }
        }
    }
};

template <class mint>
void butterfly(std::vector<mint>& a) {
    int n = int(a.size());
    int h = __builtin_ctz((unsigned int)n);

    static const fft_info<mint> info;
    int len = 0;  // a[i, i+(n>>len), i+2*(n>>len), ..] is transformed
    while (len < h) {
        if (h - len == 1) {
            int p = 1 << (h - len - 1);
            mint rot = 1;
            for (int s = 0; s < (1 << len); s++) {
                int offset = s << (h - len);
                for (int i = 0; i < p; i++) {
                    auto l = a[i + offset];
                    auto r = a[i + offset + p] * rot;
                    a[i + offset] = l + r;
                    a[i + offset + p] = l - r;
                }
                if (s + 1 != (1 << len)) {
                    rot *= info.rate2[__builtin_ctz(~(unsigned int)(s))];
                }
            }
            len++;
        } else {
            // 4-base
            int p = 1 << (h - len - 2);
            mint rot = 1, imag = info.root[2];
            for (int s = 0; s < (1 << len); s++) {
                mint rot2 = rot * rot;
                mint rot3 = rot2 * rot;
                int offset = s << (h - len);
                for (int i = 0; i < p; i++) {
                    auto mod2 = 1ULL * mint::mod() * mint::mod();
                    auto a0 = 1ULL * a[i + offset].val();
                    auto a1 = 1ULL * a[i + offset + p].val() * rot.val();
                    auto a2 = 1ULL * a[i + offset + 2 * p].val() * rot2.val();
                    auto a3 = 1ULL * a[i + offset + 3 * p].val() * rot3.val();
                    auto a1na3imag = 1ULL * mint(a1 + mod2 - a3).val() * imag.val();
                    auto na2 = mod2 - a2;
                    a[i + offset] = a0 + a2 + a1 + a3;
                    a[i + offset + 1 * p] = a0 + a2 + (2 * mod2 - (a1 + a3));
                    a[i + offset + 2 * p] = a0 + na2 + a1na3imag;
                    a[i + offset + 3 * p] = a0 + na2 + (mod2 - a1na3imag);
                }
                if (s + 1 != (1 << len)) {
                    rot *= info.rate3[__builtin_ctz(~(unsigned int)(s))];
                }
            }
            len += 2;
        }
    }
}

template <class mint>
void butterfly_inv(std::vector<mint>& a) {
    int n = int(a.size());
    int h = __builtin_ctz((unsigned int)n);

    static const fft_info<mint> info;

    int len = h;  // a[i, i+(n>>len), i+2*(n>>len), ..] is transformed
    while (len) {
        if (len == 1) {
            int p = 1 << (h - len);
            mint irot = 1;
            for (int s = 0; s < (1 << (len - 1)); s++) {
                int offset = s << (h - len + 1);
                for (int i = 0; i < p; i++) {
                    auto l = a[i + offset];
                    auto r = a[i + offset + p];
                    a[i + offset] = l + r;
                    a[i + offset + p] = (unsigned long long)(mint::mod() + l.val() - r.val()) * irot.val();
                }
                if (s + 1 != (1 << (len - 1))) {
                    irot *= info.irate2[__builtin_ctz(~(unsigned int)(s))];
                }
            }
            len--;
        } else {
            // 4-base
            int p = 1 << (h - len);
            mint irot = 1, iimag = info.iroot[2];
            for (int s = 0; s < (1 << (len - 2)); s++) {
                mint irot2 = irot * irot;
                mint irot3 = irot2 * irot;
                int offset = s << (h - len + 2);
                for (int i = 0; i < p; i++) {
                    auto a0 = 1ULL * a[i + offset + 0 * p].val();
                    auto a1 = 1ULL * a[i + offset + 1 * p].val();
                    auto a2 = 1ULL * a[i + offset + 2 * p].val();
                    auto a3 = 1ULL * a[i + offset + 3 * p].val();
                    auto a2na3iimag = 1ULL * mint((mint::mod() + a2 - a3) * iimag.val()).val();

                    a[i + offset] = a0 + a1 + a2 + a3;
                    a[i + offset + 1 * p] = (a0 + (mint::mod() - a1) + a2na3iimag) * irot.val();
                    a[i + offset + 2 * p] = (a0 + a1 + (mint::mod() - a2) + (mint::mod() - a3)) * irot2.val();
                    a[i + offset + 3 * p] = (a0 + (mint::mod() - a1) + (mint::mod() - a2na3iimag)) * irot3.val();
                }
                if (s + 1 != (1 << (len - 2))) {
                    irot *= info.irate3[__builtin_ctz(~(unsigned int)(s))];
                }
            }
            len -= 2;
        }
    }
}

template <class mint>
std::vector<mint> convolution_naive(const std::vector<mint>& a, const std::vector<mint>& b) {
    int n = int(a.size()), m = int(b.size());
    std::vector<mint> ans(n + m - 1);
    if (n < m) {
        for (int j = 0; j < m; j++) {
            for (int i = 0; i < n; i++) {
                ans[i + j] += a[i] * b[j];
            }
        }
    } else {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                ans[i + j] += a[i] * b[j];
            }
        }
    }
    return ans;
}


template <class mint>
std::vector<mint> convolution_fft(std::vector<mint> a, std::vector<mint> b) {
    int n = int(a.size()), m = int(b.size());
    int z = (int)bit_ceil((unsigned int)(n + m - 1));
    a.resize(z);
    butterfly(a);
    b.resize(z);
    butterfly(b);
    for (int i = 0; i < z; i++) {
        a[i] *= b[i];
    }
    butterfly_inv(a);
    a.resize(n + m - 1);
    mint iz = mint(z).inv();
    for (int i = 0; i < n + m - 1; i++) a[i] *= iz;
    return a;
}

template <class mint>
std::vector<mint> convolution_mod(std::vector<mint>&& a, std::vector<mint>&& b) {
    int n = int(a.size()), m = int(b.size());
    if (!n || !m) return {};
    int z = (int)bit_ceil((unsigned int)(n + m - 1));
    assert((mint::mod() - 1) % z == 0);
    if (std::min(n, m) <= 60) return convolution_naive(std::move(a), std::move(b));
    return convolution_fft(std::move(a), std::move(b));
}
template <class mint>
std::vector<mint> convolution_mod(const std::vector<mint>& a, const std::vector<mint>& b) {
    int n = int(a.size()), m = int(b.size());
    if (!n || !m) return {};
    int z = (int)bit_ceil((unsigned int)(n + m - 1));
    assert((mint::mod() - 1) % z == 0);
    if (std::min(n, m) <= 60) return convolution_naive(a, b);
    return convolution_fft(a, b);
}

template <unsigned int mod = 998244353, class T>
std::vector<T> convolution_mod(const std::vector<T>& a, const std::vector<T>& b) {
    int n = int(a.size()), m = int(b.size());
    if (!n || !m) return {};
    using mint = modint32_static<mod>;
    int z = (int)bit_ceil((unsigned int)(n + m - 1));
    assert((mint::mod() - 1) % z == 0);

    std::vector<mint> a2(n), b2(m);
    for (int i = 0; i < n; i++) {
        a2[i] = mint(a[i]);
    }
    for (int i = 0; i < m; i++) {
        b2[i] = mint(b[i]);
    }
    auto c2 = convolution_mod(std::move(a2), std::move(b2));
    std::vector<T> c(n + m - 1);
    for (int i = 0; i < n + m - 1; i++) {
        c[i] = c2[i].val();
    }
    return c;
}




template<typename mint>
struct counting_path_on_grid {
    static std::vector<std::vector<mint>> F; // F[k] = ntt(0! + 1!x + 2!x^2 .... (2^k - 1)!x^(2^k - 1))
    // 畳み込む時に次数が超えるが問題ないはず

    // F[k]を作る
    static void make_f(int k) {
        combination_mod<mint>::build(1 << k);
        F[k].resize(1 << k, 0);
        for (int i = 0; i < (1 << k); i++) {
            F[k][i] = combination_mod<mint>::fac(i);
        }
        butterfly(F[k]);
    }

    static constexpr int naive_lim1 = 100;
    static constexpr int naive_lim2 = 10000;
    static std::vector<mint> solve_naive(int H, int W, const std::vector<mint> &w) {
        if (!H || !W) return {};
        static std::array<std::array<mint, naive_lim1>, naive_lim2> table;
        std::vector<mint> res(H + W - 1);
        assert(w.size() == H + W - 1);
        if (H < W && H <= naive_lim1) {
            for (int i = 0; i < W; i++) table[H - 1][i] = w[H - 1 + i];
            for (int i = H - 1; i >= 0; i--) {
                for (int j = 1; j < W; j++) table[i][j] += table[i][j - 1];
                if (i) {
                    table[i - 1][0] = table[i][0] + w[i - 1];
                    for (int j = 1; j < W; j++) table[i - 1][j] = table[i][j];
                }
            }
            for (int i = 0; i < W; i++) res[i] = table[0][i];
            for (int i = 1; i < H; i++) res[W - 1 + i] = table[i][W - 1];
        } else {
            for (int i = 0; i < H; i++) table[W - 1][i] = w[H - 1 - i];
            for (int i = W - 1; i >= 0; i--) {
                for (int j = 1; j < H; j++) table[i][j] += table[i][j - 1];
                if (i) {
                    table[i - 1][0] = table[i][0] + w[H + W - 1 - i];
                    for (int j = 1; j < H; j++) table[i - 1][j] = table[i][j];
                }
            }
            for (int i = 0; i < W; i++) res[i] = table[W - 1 - i][H - 1];
            for (int i = 1; i < H; i++) res[W - 1 + i] = table[0][H - 1 - i];
        }
        return res;
    }

    // |s/t0| t1 | t2 | t3 |
    // | s1 |    |    | t4 |
    // | s2 |    |    | t5 |
    // | s3 | s4 | s5 |s/t6|
    // s_0 ~ s_h+w-2からt_0 ~ t_h+w-2への重みつき経路数数え上げ
    // (右か上移動だけでs_i -> t_jへ移動できる時に(経路数) * w_iをres[j]に足す)  O((H + W) log(H + W))
    static std::vector<mint> solve(int H, int W, const std::vector<mint> &w) {
        assert(w.size() == H + W - 1);
        std::vector<mint> res(H + W - 1, 0);
        if (std::min(H, W) <= naive_lim1 && std::max(H, W) <= naive_lim2) return solve_naive(H, W, w);
        if (!H || !W) return {};
        combination_mod<mint>::build(H + W);
        int logHW = bit_ceil_log(H + W);
        
        std::vector<mint> A(1 << logHW, 0);
        mint iz = mint(1 << logHW).inv();

        bool use_sita = false;
        for (int i = 1; i < W; i++) {
            if (w[H - 1 + i] != 0) {
                use_sita = true;
                break;
            }
        }

        // 左 -> 上
        {
            for (int i = 0; i < H; i++) {
                A[i] = w[H - 1 - i] * combination_mod<mint>::ifac(H - 1 - i);
            }
            butterfly(A);
            if (F[logHW].empty()) make_f(logHW);
            for(int i = 0; i < (1 << logHW); i++) {
                A[i] *= F[logHW][i];
            }
            butterfly_inv(A);
            for (int i = 0, j = H - 1; i < W; i++, j++) {
                res[i] += A[j] * iz * combination_mod<mint>::ifac(i);
            }
        }

        // 下 -> 右
        if (use_sita) {
            std::fill(A.begin(), A.end(), 0);
            for (int i = 1; i < W; i++) {
                A[i] = w[H - 1 + i] * combination_mod<mint>::ifac(W - 1 - i);
            }
            butterfly(A);
            for(int i = 0; i < (1 << logHW); i++) {
                A[i] *= F[logHW][i];
            }
            butterfly_inv(A);
            for (int i = 0; i < H; i++) {
                res[H + W - 2 - i] += A[W - 1 + i] * iz * combination_mod<mint>::ifac(i);
            }
        }

        // 左 -> 右
        {
            std::vector<mint> B(w.begin(), w.begin() + H);
            std::vector<mint> C(H);
            for (int i = 0; i < H; i++) {
                C[i] = combination_mod<mint>::comb(H + W - 2 - i, W - 1);
            }
            C = convolution_mod<mint>(B, C);
            for (int i = 1; i < H; i++) {
                res[W - 1 + i] += C[H - 1 + i];
            }
        }

        // 下 -> 上
        if (use_sita) {
            std::vector<mint> B(W, 0);
            for (int i = 0; i < W - 1; i++) {
                B[i] = w[H + W - 2 - i];
            }
            std::vector<mint> C(W);
            for (int i = 0; i < W; i++) {
                C[i] = combination_mod<mint>::comb(H + W - 2 - i, H - 1);
            }
            C = convolution_mod<mint>(B, C);
            for (int i = 1; i < W; i++) {
                res[W - 1 - i] += C[W - 1 + i];
            }
        }
        return res;
    }
};
template<typename mint>
std::vector<std::vector<mint>> counting_path_on_grid<mint>::F(25);



// 0 <= Ai <= xi < Biを満たす広義単調増加列の数
// res[i] := A_j <= i < B_jである最右のjに対して末尾がiである長さj+1の列の数
template<typename mint>
std::vector<mint> number_of_increasing_sequence(std::vector<int> A, std::vector<int> B) {
    int N = A.size();
    assert(B.size() == N);
    if (N == 0) return {};

    std::vector<mint> D(N + 1, 0), L(B.back(), 0);
    L[A[0]] = 1;
    std::vector<int> C(N);

    auto dfs_inc = [&](auto &&dfs_inc, int l, int r, int d) -> void {
        if (d == C[r - 1]) return;
        if (r - l == 1) {
            L[d] += D[l];
            for (int i = d + 1; i < C[l]; i++) L[i] += L[i - 1];
            D[l] = L[C[l] - 1];
            return;
        }
        int m = (l + r) / 2;
        if (l < m) dfs_inc(dfs_inc, l, m, d);
        int y = C[m];
        int Hi = y - d, Wi = r - m;
        if (Hi && Wi) {
            std::vector<mint> w(Hi + Wi - 1, 0);
            for (int i = 0; i < Hi; i++) w[i] = L[y - 1 - i];
            for (int i = 0; i < Wi; i++) w[Hi - 1 + i] += D[m + i];
            w = counting_path_on_grid<mint>::solve(Hi, Wi, w);
            for (int i = 0; i < Wi; i++) D[m + i] = w[i];
            for (int i = 0; i < Hi; i++) L[y - 1 - i] = w[Wi - 1 + i];
        }
        if (m < r) dfs_inc(dfs_inc, m, r, y);
    };

    auto dfs_dec = [&](auto &&dfs_dec, int l, int r, int d) -> void {
        if (d == C[l]) return;
        if (r - l == 1) {
            L[C[l] - 1] += D[l];
            for (int i = C[l] - 2; i >= d; i--) L[i] += L[i + 1];
            D[l] = L[d];
            for (int i = d; i < C[l]; i++) D[l + 1] += L[i];
            return;
        }
        int m = (l + r) / 2;
        int y = C[m - 1];
        if (l < m) dfs_dec(dfs_dec, l, m, y);
        int Hi = y - d, Wi = m - l;
        if (Hi && Wi) {
            std::vector<mint> w(Hi + Wi - 1, 0);
            for (int i = 0; i < Hi; i++) w[i] = L[d + i];
            for (int i = 0; i < Wi; i++) w[Hi - 1 + i] += D[l + i];
            w = counting_path_on_grid<mint>::solve(Hi, Wi, w);
            for (int i = 0; i < Hi; i++) L[d + i] = w[Wi - 1 + i];
            for (int i = 0; i < Wi; i++) D[l + i] = w[i];
        }
        for (int i = C[m]; i < y; i++) D[m] += L[i];
        if (m < r) dfs_dec(dfs_dec, m, r, d);
    };

    mint low_sum = 0;
    int l = 0;
    while (l < N) {
        int r = l;
        do { r++; } while (r < N && A[r] < B[l]);
        D[l] = low_sum;
        low_sum = 0;
        for (int i = l; i < r; i++) C[i] = B[l] - A[i] + A[l];
        std::reverse(L.begin() + A[l], L.begin() + B[l]);
        dfs_dec(dfs_dec, l, r, A[l]);
        for (int i = A[l]; i < C[r - 1]; i++) low_sum += L[i];
        std::reverse(L.begin() + A[l], L.begin() + B[l]);
        for (int i = l; i < r; i++) C[i] = B[i];
        dfs_inc(dfs_inc, l, r, B[l]);
        for (int i = B[l]; i < (r < N ? A[r] : 0); i++) low_sum += L[i];
        l = r;
    }
    return L;
}

using mint = modint998244353;

int main() {
    fast_io<1 << 25, 1 << 10> io;
    int N = io.in();
    std::vector<int> A(N), B(N);
    A[0] = io.in();
    for (int i = 0; i < N; i++) A[i] = io.in();
    for (int i = 0; i < N; i++) B[i] = io.in();

    for (int i = 1; i < N; i++) A[i] = std::max(A[i], A[i - 1]);

    for (int i = N - 1; i >= 0; i--) {
        if (i < N - 1) B[i] = std::min(B[i], B[i + 1]);
        if (A[i] >= B[i]) {
            io.out(0, '\n');
            return 0;
        }
    }

    auto ans = number_of_increasing_sequence<mint>(A, B);
    mint s = 0;
    for (int i = A.back(); i < B.back(); i++) s += ans[i];
    io.out(s.val(), '\n');
}
